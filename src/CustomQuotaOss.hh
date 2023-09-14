/************************************************************************
 * EOS - the CERN Disk Storage System                                   *
 * Copyright © 2013 CERN/Switzerland                                    *
 *                                                                      *
 * Author: Joaquim Rocha <joaquim.rocha@cern.ch>                        *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#ifndef __lustre_OSS_HH__
#define __lustre_OSS_HH__

#include "qsStruct.h"
#include <XrdOss/XrdOss.hh>
#include <XrdOuc/XrdOucStream.hh>
#include <XrdOuc/XrdOucString.hh>
#include <XrdSys/XrdSysLogger.hh>
#include <XrdVersion.hh>
#include <chrono>
#include <stdio.h>
class CustomQuotaOss : public XrdOss {
  public:
    // We just use the base implementation via nativeOss for all functions defined in this header
    // see XrdOss/XrdOss.hh for more descriptive signatures regarding XrdOss
    CustomQuotaOss(XrdOss* native_oss, XrdSysLogger*, const char*);
    virtual ~CustomQuotaOss();
    XrdOssDF* newDir(const char* tident) { return nativeOss->newDir(tident); }
    XrdOssDF* newFile(const char* tident) { return nativeOss->newFile(tident); }
    int Chmod(const char* a, mode_t b, XrdOucEnv* c = 0) { return nativeOss->Chmod(a, b, c); }
    int Create(const char* a, const char* b, mode_t c, XrdOucEnv& d, int e) {
        return nativeOss->Create(a, b, c, d, e);
    }
    int Init(XrdSysLogger* a, const char* b) { return nativeOss->Init(a, b); }
    int Mkdir(const char* a, mode_t b, int c = 0, XrdOucEnv* d = 0) {
        return nativeOss->Mkdir(a, b, c, d);
    };
    const char* Lfn2Pfn(const char* a, char* b, int c, int& d) {
        return nativeOss->Lfn2Pfn(a, b, c, d);
    }
    int Lfn2Pfn(const char* a, char* b, int c) { return nativeOss->Lfn2Pfn(a, b, c); }
    int Reloc(const char* a, const char* b, const char* c, const char* d = 0) {
        return nativeOss->Reloc(a, b, c, d);
    }
    int Remdir(const char* a, int b = 0, XrdOucEnv* c = 0) { return nativeOss->Remdir(a, b, c); };
    int Rename(const char* a, const char* b, XrdOucEnv* c = 0, XrdOucEnv* d = 0) {
        return nativeOss->Rename(a, b, c, d);
    }
    int Stat(const char* a, struct stat* b, int c = 0, XrdOucEnv* d = 0) {
        return nativeOss->Stat(a, b, c, d);
    };
    int Truncate(const char* a, unsigned long long b, XrdOucEnv* c = 0) {
        return nativeOss->Truncate(a, b, c);
    }
    int Unlink(const char* a, int b = 0, XrdOucEnv* c = 0) { return nativeOss->Unlink(a, b, c); }
    void loadConfig(const char* filename);
    int StatXA(const char* a, char* b, int& c, XrdOucEnv* d = 0) {
        return nativeOss->StatXA(a, b, c, d);
    }
    virtual int FSctl(int a, int b, const char* c, char** d = 0) {
        return nativeOss->FSctl(a, b, c, d);
    }
    virtual int StatXP(const char* a, unsigned long long& b, XrdOucEnv* c = 0) {
        return nativeOss->StatXP(a, b, c);
    }
    // We only reimplement these methods in the source file
    virtual int StatVS(XrdOssVSInfo* sP, const char* sname = 0, int updt = 0);
    int StatFS(const char* path, char* buff, int& blen, XrdOucEnv* eP);
    int StatLS(XrdOucEnv& env, const char* path, char* buff, int& blen);

  private:
    std::string quota_read_filename;
    XrdOss* nativeOss;
    XrdSysLogger* log;
};

#endif /* __lustre_OSS_HH__ */
