#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)
#define DEBUG(a) std::cerr << "[DEBUG]" << a << std::endl;
#include "CustomQuotaOss.hh"
#include <XrdOuc/XrdOucStream.hh>
#include <XrdOuc/XrdOucString.hh>
#include <XrdVersion.hh>
#include <chrono>
#include <fcntl.h>
#include <stdexcept>

extern "C" {
XrdOss* XrdOssGetStorageSystem(XrdOss* native_oss, XrdSysLogger* Logger, const char* config_fn,
                               const char* parms) {
    CustomQuotaOss* Oss = new CustomQuotaOss(native_oss, Logger, config_fn);
    return (XrdOss*)Oss;
}
}
extern "C" {
struct qsStruct getQuotaSpace(char* target);
};
CustomQuotaOss::CustomQuotaOss(XrdOss* native_oss, XrdSysLogger* logger, const char* config_fn)
  : nativeOss(native_oss), log(logger) {
    nativeOss->Init(log, config_fn);
    loadConfig(config_fn);
}

void CustomQuotaOss::loadConfig(const char* filename) {

    XrdOucStream Config;
    int cfgFD;
    if ((cfgFD = open(filename, O_RDONLY, 0)) < 0) {
        return;
    }
    const char* var;
    Config.Attach(cfgFD);
    while ((var = Config.GetMyFirstWord())) {
        if (strcmp(var, "customquota.sourcefile") == 0) {
            var += 21;
            sourceFilePath = std::string(Config.GetWord());
        }
    }
    Config.Close();
}

int CustomQuotaOss::StatVS(XrdOssVSInfo* sP, const char* sname, int updt) { return XrdOssOK; }

int CustomQuotaOss::StatFS(const char* path, char* buff, int& blen, XrdOucEnv* eP) {
    XrdOssVSInfo sP;
    int rc = StatVS(&sP, 0, 0);
    if (rc) {
        return rc;
    }
    int percentUsedSpace = (sP.Usage * 100) / sP.Total;
    blen = snprintf(buff, blen, "%d %lld %d %d %lld %d", 1, sP.Free, percentUsedSpace, 0, 0LL, 0);
    return XrdOssOK;
}
int CustomQuotaOss::StatLS(XrdOucEnv& env, const char* path, char* buff, int& blen) {
    static const char* Resp = "oss.cgroup=%s&oss.space=%lld&oss.free=%lld"
                              "&oss.maxf=%lld&oss.used=%lld&oss.quota=%lld";
    XrdOssVSInfo sP;
    int rc = StatVS(&sP, 0, 0);
    if (rc)
        return rc;

    blen = snprintf(buff, blen, Resp, "public", sP.Total, sP.Free, sP.LFree, sP.Usage, sP.Quota);
    return XrdOssOK;
}
CustomQuotaOss::~CustomQuotaOss() {}
XrdVERSIONINFO(XrdOssGetStorageSystem, CustomQuotaOss);
