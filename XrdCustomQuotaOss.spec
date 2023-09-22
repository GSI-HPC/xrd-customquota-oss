%define         __spec_install_post %{nil}
%define         debug_package %{nil}

Name:           XrdCustomQuotaOss
Version:        0.9.1
Release:        1%{?dist}
Summary:        An XRootD plugin that reads custom quota information from a file, caches its contents, and reports it to requesting clients.
Group:          XRootD/Plugins

License:        LGPLv3
URL:            https://github.com/jknedlik/XrdCustomQuotaOssWrapper
Source0:        %{name}-%{version}.tar.gz

Requires(pre):  xrootd-server
Requires:       python36
Requires:       python3-atomicwrites
Requires:       python3-pyyaml


%description
An XRootD plugin that reads custom quota information from a file, caches its contents, and reports it to requesting clients.
Contains a quota-file-writer which repeatedly writes the quota cache file and is started as a separate unit.

%prep
%setup -q

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/%{_libdir}
cp Lib%{name}.so %{buildroot}/%{_libdir}

mkdir -p %{buildroot}%{_bindir}
cp -av /root/src/quota-file-writer/quota-file-writer %{buildroot}%{_bindir}/
mkdir -p %{buildroot}%{_sysconfdir}
cp -av /root/src/quota-file-writer/quota-file-writer.conf.default %{buildroot}%{_sysconfdir}
mkdir -p %{buildroot}/usr/lib/systemd/system/
cp -av /root/src/quota-file-writer/quota-file-writer.service %{buildroot}/usr/lib/systemd/system/


%files
%defattr(-,root,root)
%{_libdir}/Lib%{name}.so

%attr(755,root,root)/usr/bin/quota-file-writer
%attr(644,root,root)/etc/quota-file-writer.conf.default
%attr(644,root,root)/usr/lib/systemd/system/quota-file-writer.service

%doc

