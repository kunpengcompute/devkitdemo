#include <cstdio>
#include <vector>
#include <ksal/das.h>

std::vector<DasKvParam> prefetchInfoLists;

void das_req_prefetch(DasKvParam *params){
    DasKvParam prefetchInfo = {params->offset, params->len, params->timeStamp, params->lunID};
    prefetchInfoLists.push_back(prefetchInfo);
}

void* ProducerTask(const uint32_t ioNum)
{
    bool readyToExit = false;
    int32_t ret = 0;
    uint32_t counter = 0;

    for(uint32_t i = 0; i < ioNum; i++){
        DasKvParam params;
        params.offset = 4096 * i;
        params.len = 4096;
        params.timeStamp = 0 + i * 10000000;
        params.lunID = 1;

        ret = Rcache_PutDasInfo(&params);
    }
}

int main(int argc, char **argv)
{
    DasModuleParam *dasInstanceParam = new DasModuleParam();
    dasInstanceParam->cacheLineSize = 8192;
    dasInstanceParam->logger = nullptr;
    DasOPS *regOps = new DasOPS();
    regOps->SubmitDasPrefetch = das_req_prefetch;
    regOps->logFunc = nullptr;
    dasInstanceParam->ops = regOps;
    int32_t ret = Rcache_CeateDasModule(dasInstanceParam);

    uint32_t ioNum = 10000;
    ProducerTask(ioNum);

    for (int i = 0; i < prefetchInfoLists.size(); ++i) {
        printf("prefetchInfo: lunId[%u], offset[%lu], len[%lu]\n", prefetchInfoLists[i].lunID,
            prefetchInfoLists[i].offset, prefetchInfoLists[i].len);
    }

    Rcache_ExitDasModule();

    return 0;
}
