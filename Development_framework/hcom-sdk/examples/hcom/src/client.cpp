/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Author: An
 * Description: TCP通信客户端，使用PostSend或PostSendRaw接口发送大量请求组并计算请求的延迟和吞吐量
 */
#include <unistd.h>
#include <getopt.h>

#include "hcom.h"

using namespace ock::hcom;

NetDriver *driver = nullptr;
NetEndpointPtr ep = nullptr;

int32_t pingCount = 1000000;
int32_t pingCount1 = 1000000;

std::string oobIp = "";
uint16_t oobPort = 5684;

std::string ipSeg = "127.0.0.1/24";

uint64_t startTime = 0;
uint64_t finishTime = 0;

int16_t asyncWorkerCpuId = -1;
int buffSizeKb = 4;
int dataSize = 1024;

sem_t sem;

uint32_t seqNo = 1;
bool blockingIo = true;
bool calledByWorker = false;
bool sendRaw = false;

bool enableTls = false;
std::string ENV_CART_PATH = "TLS_CERT_PATH";
std::string certPath;

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

void SendSingleRequest()
{
    static auto data = malloc(dataSize);
    if (pingCount-- == 0) {
        finishTime = MONOTONIC_TIME_NS();
        NN_LOG_INFO("Finished " << pingCount1 << " pingpong");
        printf("\tPerf summary\n");
        printf("\tPingpong times:\t\t%d\n", pingCount1);
        printf("\tTotal time(us):\t\t%f\n", (finishTime - startTime) / 1000.0);
        printf("\tTotal time(ms):\t\t%f\n", (finishTime - startTime) / 1000000.0);
        printf("\tTotal time(s):\t\t%f\n", (finishTime - startTime) / 1000000000.0);
        printf("\tLatency(us):\t\t%f\n", (finishTime - startTime) / pingCount1 / 1000.0);
        printf("\tOps:\t\t\t%f pp/s\n", (pingCount1 * 1000000000.0) / (finishTime - startTime));

        sem_post(&sem);

        return;
    }
    int result = 0;
    NetTransRequest req(data, dataSize, 0);
    if (sendRaw) {
        if ((result = ep->PostSendRaw(req, seqNo++)) != 0) {
            NN_LOG_INFO("failed to post message to data to server, result " << result);
            return;
        }
    } else {
        if ((result = ep->PostSend(1, req)) != 0) {
            NN_LOG_INFO("failed to post message to data to server, result " << result);
            return;
        }
    }
}

void EndPointBroken(const NetEndpointPtr &ep1)
{
    NN_LOG_INFO("end point " << ep1->Id() << " broken");
    if (ep != nullptr) {
        ep.Set(nullptr);
    }
}

int RequestReceived(const NetRequestContext &ctx)
{
    SendSingleRequest();
    return 0;
}

int RequestPosted(const NetRequestContext &ctx)
{
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
    if (driver != nullptr) {
        NN_LOG_ERROR("driver already created");
        return false;
    }

    driver = NetDriver::Instance(NetDriverProtocol::TCP, "sock_pp_client", false);
    if (driver == nullptr) {
        NN_LOG_ERROR("failed to create driver already created");
        return false;
    }

    atexit(exitFunc);
    NetDriverOptions options{};
    options.mode = NetDriverWorkingMode::NET_EVENT_POLLING;
    options.tcpSendBufSize = buffSizeKb;
    options.tcpReceiveBufSize = buffSizeKb;
    options.enableTls = enableTls;
    options.mrSendReceiveSegSize = dataSize + 32;
    options.mrSendReceiveSegCount = 10;
    options.SetNetDeviceIpMask(ipSeg);
    NN_LOG_INFO("set ip mask " << options.netDeviceIpMask);
    if (asyncWorkerCpuId != -1) {
        std::string str = std::to_string(asyncWorkerCpuId) + "-" + std::to_string(asyncWorkerCpuId);
        options.SetWorkerGroupsCpuSet(str);
        NN_LOG_INFO(" set cpuId: " << options.WorkerGroupCpus());
    }

    driver->RegisterEPBrokenHandler(std::bind(&EndPointBroken, std::placeholders::_1));
    driver->RegisterNewReqHandler(std::bind(&RequestReceived, std::placeholders::_1));
    driver->RegisterReqPostedHandler(std::bind(&RequestPosted, std::placeholders::_1));
    driver->RegisterOneSideDoneHandler(std::bind(&OneSideDone, std::placeholders::_1));

    if (enableTls) {
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

    sem_init(&sem, 0, 0);

    return true;
}

bool Connect()
{
    if (driver == nullptr) {
        NN_LOG_ERROR("driver is null");
        return false;
    }

    int result = 0;
    if ((result = driver->Connect("hello world", ep, 0)) != 0) {
        NN_LOG_ERROR("failed to connect to server, result " << result);
        return false;
    }

    EpOptions epOptions{};
    epOptions.tcpBlockingIo = blockingIo;
    epOptions.cbByWorkerInBlocking = calledByWorker;
    ep->SetEpOption(epOptions);

    return true;
}

void Run()
{
    if (!CreateDriver()) {
        return;
    }

    if (!Connect()) {
        return;
    }

    NN_LOG_INFO("Starting to ping pong " << pingCount1 << " times");
    startTime = MONOTONIC_TIME_NS();
    SendSingleRequest();
}

int main(int argc, char *argv[])
{
    struct option options[] = {
        {"ip", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {"pingpongtimes", required_argument, NULL, 't'},
        {"cpuId", required_argument, NULL, 'c'},
        {"blockio", required_argument, NULL, 'b'},
        {"callbyworker", required_argument, NULL, 'w'},
        {"enableTls", required_argument, NULL, 'e'},
        {"sendRaw", required_argument, NULL, 'r'},
        {"buffSize", required_argument, NULL, 's'},
        {"dataSize", required_argument, NULL, 'd'},
        {NULL, 0, NULL, 0},
    };

    std::string usage("usage\n"
        "        -i, --ip,                     server ip mask, e.g. 10.175.118.1\n"
        "        -p, --port,                   server port, by default 9981\n"
        "        -t, --pingpongtimes,          ping pong times\n"
        "        -c, --cpuId,                  async worker\n"
        "        -b, --blockio,                blocking io\n"
        "        -w, --callbyworker,           call by worker\n"
        "        -e, --enableTls,              enable tls\n"
        "        -r, --sendRaw,                send raw\n"
        "        -s, --buffSize,               buff size\n"
        "        -d, --dataSize,               data size\n");

    if (argc != 21) {
        std::cout << "invalid param, ";
        std::cout << usage.c_str() << std::endl;
        std::cout << "for example " << argv[0] << " -i rdma_nic_ip -p 9981 -t 1000000 -c 15 -b 0 -w 0 -e 0 -r 0 -s 4 -d 1024" <<
            std::endl;
        return -1;
    }

    int ret = 0;
    int index = 0;
    std::string str = "i:p:t:c:b:w:e:r:s:d:";
    while ((ret = getopt_long(argc, argv, str.c_str(), options, &index)) != -1) {
        switch (ret) {
            case 'i':
                oobIp = optarg;
                ipSeg = oobIp + "/24";
                break;
            case 'p':
                oobPort = strtoul(optarg, nullptr, 0);
                break;
            case 't':
                pingCount = strtoul(optarg, nullptr, 0);
                pingCount1 = pingCount;
                break;
            case 'c':
                asyncWorkerCpuId = strtoul(optarg, nullptr, 0);
                break;
            case 'b':
                blockingIo = strtoul(optarg, nullptr, 0);
                break;
            case 'w':
                calledByWorker = strtoul(optarg, nullptr, 0);
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
            case 'd':
                dataSize = strtoul(optarg, nullptr, 0);
                break;
        }
    }

    if (enableTls) {
        if (ValidateTlsCert() != 0) {
            return -1;
        }
    }
    Run();
    NN_LOG_INFO("Wait for finish");
    sem_wait(&sem);
    return 0;
}