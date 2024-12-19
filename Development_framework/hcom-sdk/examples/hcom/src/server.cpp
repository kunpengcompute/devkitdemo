/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Author: An
 * Description: TCP通信服务端，接收客户端大量请求并响应
 */
#include <unistd.h>
#include <getopt.h>

#include "hcom.h"

using namespace ock::hcom;

NetDriver *driver = nullptr;
NetEndpointPtr ep = nullptr;

std::string oobIp = "";
uint16_t oobPort = 6584;
int16_t asyncWorkerCpuId = -1;
int buffSizeKb = 4;
int dataSize = 1024;
bool blockingIo = true;
bool sendRaw = false;

std::string ipSeg = "127.0.0.1/24";

bool enableTls = false;
std::string ENV_CART_PATH = "TLS_CERT_PATH";   // 环境变量，TLS证书路径
std::string certPath;

static void Erase(void *pass, int len) {}
static int Verify(void *x509, const char *path)
{
    return 0;
}

static bool CertCallback(const std::string &name, std::string &value)
{
    value = certPath + "/server/cert.pem";
    return true;
}

static bool PrivateKeyCallback(const std::string &name, std::string &value, void *&keyPass, int &len,
    TLSEraseKeypass &erase)
{
    static char content[] = "huawei";
    keyPass = reinterpret_cast<void *>(content);
    len = sizeof(content);
    value = certPath + "/server/key.pem";
    erase = std::bind(&Erase, std::placeholders::_1, std::placeholders::_2);

    return true;
}

static bool CACallback(const std::string &name, std::string &caPath, std::string &crlPath,
    PeerCertVerifyType &peerCertVerifyType, TLSCertVerifyCallback &cb)
{
    caPath = certPath + "/CA/cacert.pem";
    cb = std::bind(&Verify, std::placeholders::_1, std::placeholders::_2);
    return true;
}

int ValidateTlsCert()
{
    char *envCertPath = ::getenv(ENV_CART_PATH.c_str());
    if (envCertPath == nullptr) {
        NN_LOG_ERROR("env for TLS cert is not set, set " << ENV_CART_PATH);
        return -1;
    }

    certPath = envCertPath;
    if (::access((certPath + "/CA/cacert.pem").c_str(), F_OK) != 0) {
        NN_LOG_ERROR("cacert.pem cannot be found under " << certPath);
        return -1;
    }

    if (::access((certPath + "/server/cert.pem").c_str(), F_OK) != 0) {
        NN_LOG_ERROR("cert.pem cannot be found under " << certPath);
        return -1;
    }

    if (::access((certPath + "/server/key.pem").c_str(), F_OK) != 0) {
        NN_LOG_ERROR("key.pem cannot be found under " << certPath);
        return -1;
    }

    return 0;
}

static void Log(int level, const char *msg)
{
    struct timeval tv {};
    char strTime[24];

    gettimeofday(&tv, nullptr);
    strftime(strTime, sizeof strTime, "%Y-%m-%d %H:%M:%S.", localtime(&tv.tv_sec));

    static  std::string levelInfo[4] = {"debug", "info", "warn", "error"};

    std::cout << strTime << tv.tv_usec << " " << levelInfo[level & 3] << " " << msg << std::endl;
}

int NewEndPoint(const std::string &ipPort, const NetEndpointPtr &newEP, const std::string &payload)
{
    NN_LOG_INFO("new endpoint from " << ipPort << " payload " << payload << " ep id " << newEP->Id());

    ep = newEP;
    EpOptions epOptions{};
    epOptions.tcpBlockingIo = blockingIo;
    ep->SetEpOption(epOptions);
    return 0;
}

void EndPointBroken(const NetEndpointPtr &ep)
{
    NN_LOG_INFO("end point " << ep->Id());
}

int RequestReceived(const NetRequestContext &ctx)
{
    static auto data = malloc(dataSize);

    int result = 0;
    NetTransRequest req(data, dataSize, 0);
    if (sendRaw) { // 发送无操作码和头部的请求， 执行成功返回0
        if ((result = ep->PostSendRaw(req, ctx.Header().seqNo)) != 0) {
            NN_LOG_ERROR("failed to post message to data to server, result " << result);
            return result;
        }
    } else { // 发送有操作吗和头部的请求
        if ((result = ep->PostSend(1, req)) != 0) {
            NN_LOG_ERROR("failed to post message to data to server, result " << result);
            return result;
        }
    }
    return 0;
}

int RequestPosted(const NetRequestContext &ctx)
{
    NN_LOG_TRACE_INFO("RequestPosted");
    return 0;
}

int OneSideDone(const NetRequestContext &ctx)
{
    NN_LOG_INFO("one side done");
    return 0;
}

void exitFunc()
{
    driver->Stop();
    driver->UnInitialize();
}

bool CreateDriver()
{
    NetLogger::Instance()->SetExternalLogFunction(Log);

    if (driver != nullptr) {
        NN_LOG_ERROR("driver already created");
        return false;
    }

    driver = NetDriver::Instance(NetDriverProtocol::TCP, "sock_pp_svr", true);
    if (driver == nullptr) {
        NN_LOG_ERROR("failed to create driver already created");
        return false;
    }

    atexit(exitFunc);  // 退出时触发
    NetDriverOptions options{}; // {}作用是初始化变量，为空表示默认初始化
    options.mode = NetDriverWorkingMode::NET_EVENT_POLLING; // 设置网络驱动模式为事件触发模式
    options.tcpSendBufSize = buffSizeKb;  // 设置tcp发送缓冲区容量
    options.tcpReceiveBufSize = buffSizeKb; // 设置接受缓冲区容量
    options.enableTls = enableTls;  // 是否支持TLS
    options.mrSendReceiveSegSize = dataSize + 32; // 数据分割段大小
    options.mrSendReceiveSegCount = 10; // 数据分割段数量
    if (asyncWorkerCpuId != -1) { // 设置cpu核心组
        std::string str = std::to_string(asyncWorkerCpuId) + "-" + std::to_string(asyncWorkerCpuId);
        options.SetWorkerGroupsCpuSet(str);
        NN_LOG_INFO(" set cpuId: " << options.WorkerGroupCpus());
    }
    options.SetNetDeviceIpMask(ipSeg);
    NN_LOG_INFO("set ip mask " << options.netDeviceIpMask);

    driver->RegisterNewEPHandler(
        std::bind(&NewEndPoint, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    driver->RegisterEPBrokenHandler(std::bind(&EndPointBroken, std::placeholders::_1));
    driver->RegisterNewReqHandler(std::bind(&RequestReceived, std::placeholders::_1));
    driver->RegisterReqPostedHandler(std::bind(&RequestPosted, std::placeholders::_1));
    driver->RegisterOneSideDoneHandler(std::bind(&OneSideDone, std::placeholders::_1));

    if (enableTls) { // 验证TLS证书有效性以及建立连接
        driver->RegisterTLSCertificationCallback(
            std::bind(&CertCallback, std::placeholders::_1, std::placeholders::_2));
        driver->RegisterTLSCaCallback(std::bind(&CACallback, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
        driver->RegisterTLSPrivateKeyCallback(std::bind(&PrivateKeyCallback, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    }

    driver->OobIpAndPort(oobIp, oobPort);

    int result = 0;
    if ((result = driver->Initialize(options)) != 0) {
        NN_LOG_ERROR("failed to initialize driver " << result);
        return false;
    }
    NN_LOG_INFO("driver initialized");

    if ((result = driver->Start()) != 0) {
        NN_LOG_ERROR("failed to start driver " << result);
        return false;
    }
    NN_LOG_INFO("driver started");

    return true;
}

void Run()
{
    if (!CreateDriver()) {
        return;
    }
}

int main(int argc, char *argv[])
{
    struct option options[] = {
        {"ip", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {"cpuId", required_argument, NULL, 'c'},
        {"enableTls", required_argument, NULL, 'e'},
        {"sendRaw", required_argument, NULL, 'r'},
        {"buffSize", required_argument, NULL, 's'},
        {"blockio", required_argument, NULL, 'b'},
        {"dataSize", required_argument, NULL, 'd'},
        {NULL, 0, NULL, 0},
    };

    std::string usage("usage\n"
        "        -i, --ip,                     server ip mask, e.g. 10.175.118.1\n"
        "        -p, --port,                   server port, by default 9981\n"
        "        -c, --cpuId,                  async worker\n"
        "        -e, --enableTls,              enable tls\n"
        "        -r, --sendRaw,                send raw\n"
        "        -s, --buffSize,               buff size\n"
        "        -b, --blockio,                blocking io\n"
        "        -d, --dataSize,               data size\n");

    if (argc != 17) {
        std::cout << "invalid param, ";
        std::cout << usage.c_str() << std::endl;
        std::cout << "for example " << argv[0] << " -i rdma_nic_ip -p 9981 -c asyncCpuId -e 0 -r 0 -s 4 -b 0 -d 1024" <<
            std::endl;
        return -1;
    }

    int ret = 0;
    int index = 0;
    std::string str = "i:p:c:e:r:s:b:d:";
    while ((ret = getopt_long(argc, argv, str.c_str(), options, &index)) != -1) {
        switch (ret) {
            case 'i':
                oobIp = optarg;
                ipSeg = oobIp + "/24";
                break;
            case 'p':
                oobPort = strtoul(optarg, nullptr, 0);
                break;
            case 'c':
                asyncWorkerCpuId = strtoul(optarg, nullptr, 0);
                break;
            case 'e':
                enableTls = strtoul(optarg, nullptr, 0);
                break;
            case 'r':
                sendRaw = strtoul(optarg, nullptr, 0);
                break;
            case 's':
                buffSizeKb = strtoul(optarg, nullptr, 0);
                break;
            case 'b':
                blockingIo = strtoul(optarg, nullptr, 0);
                break;
            case 'd':
                dataSize = strtoul(optarg, nullptr, 0);
                break;
        }
    }

    NN_LOG_INFO("oob ip " << oobIp << ", oob port " << oobPort << ", cpuId " << asyncWorkerCpuId);

    if (enableTls) {   // tls是提供安全通信的协议
        if (ValidateTlsCert() != 0) {
            return -1;
        }
    }

    Run();
    getchar();   // 等待用户按下Enter键
    return 0;
}