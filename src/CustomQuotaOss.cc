#include "CustomQuotaOss.hh"
#include <XrdOuc/XrdOucStream.hh>
#include <XrdOuc/XrdOucString.hh>
#include <XrdVersion.hh>
#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

extern "C" {
XrdOss* XrdOssGetStorageSystem(XrdOss* native_oss, XrdSysLogger* Logger, const char* config_fn,
                               const char* parms) {
    CustomQuotaOss* Oss = new CustomQuotaOss(native_oss, Logger, config_fn);
    return (XrdOss*)Oss;
}
}

CustomQuotaOss::CustomQuotaOss(XrdOss* native_oss, XrdSysLogger* logger, const char* config_fn)
  : nativeOss(native_oss), log(logger) {
    nativeOss->Init(log, config_fn);
    loadConfig(config_fn);
}

void CustomQuotaOss::loadConfig(const char* filename) {
    auto file2string = [](std::string filename) {
        std::ifstream file{ filename };
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    };
    auto lastMatch = [](std::string str, std::string rgx) {
        std::smatch matches;
        std::regex_search(str, matches, std::regex(rgx));
        return matches[matches.size() - 1];
    };

    std::string content = file2string(filename);
    sourceFilePath = lastMatch(content, "(?:customquota\\.sourcefile *)(\\S*).*\n");
}

int CustomQuotaOss::StatVS(XrdOssVSInfo* sP, const char* sname, int updt) {
    auto ifile = std::ifstream(sourceFilePath);
    auto getv = [line = std::string(), &ifile]() mutable {
        std::getline(ifile, line, ' ');
        return std::atoll(line.c_str());
    };

    sP->Total = getv();
    sP->Usage = getv();
    sP->Free = getv();
    sP->LFree = getv();
    return XrdOssOK;
};

int CustomQuotaOss::StatFS(const char* path, char* buff, int& blen, XrdOucEnv* eP) {
    XrdOssVSInfo sP;
    int rc = StatVS(&sP, 0, 0);
    if (rc)
        return rc;

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
