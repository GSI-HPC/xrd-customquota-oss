FROM rockylinux:8.6
MAINTAINER jknedlik <j.knedlik@gsi.de>
WORKDIR /root
RUN dnf update -y
RUN dnf install -y epel-release
RUN dnf install -y bash git make gcc gcc-c++ rpm-build libtool

# Install XRootD server + sources
RUN dnf install -y xrootd-server xrootd-server-devel 

# Build XrdCustomQuotaOss
COPY src /root/src
WORKDIR /root/src
RUN CPLUS_INCLUDE_PATH=/usr/include/xrootd make -j8

# package XrdCustomQuotaOss
WORKDIR /root/rpmbuild
RUN mkdir -p BUILD SOURCES SPECS RPMS SRPMS XrdCustomQuotaOss-0.9.1
RUN mv /root/src/LibXrdCustomQuotaOss.so XrdCustomQuotaOss-0.9.1
RUN tar -cf SOURCES/XrdCustomQuotaOss-0.9.1.tar.gz XrdCustomQuotaOss-0.9.1
WORKDIR /root/
COPY XrdCustomQuotaOss.spec /root/rpmbuild/SPECS/
RUN rpmbuild  -bb rpmbuild/SPECS/XrdCustomQuotaOss.spec

RUN mkdir /rpm
ENTRYPOINT ["cp","/root/rpmbuild/RPMS/x86_64/XrdCustomQuotaOss-0.9.1-1.el8.x86_64.rpm","/rpm"]
