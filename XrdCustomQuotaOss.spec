%define        __spec_install_post %{nil}
%define          debug_package %{nil}

Name:           XrdCustomQuotaOss
Version:        0.9
Release:        1%{?dist}
Summary:        An XRootD plugin that reads custom quota information from a file, caches its contents, and reports it to requesting clients.
Group:          XRootD/Plugins

License:        LGPLv3
URL:            https://github.com/jknedlik/XrdCustomQuotaOssWrapper
Source0:        %{name}-%{version}.tar.gz

Requires(pre): xrootd-server



%description
An XRootD plugin that reads custom quota information from a file, caches its contents, and reports it to requesting clients.

%prep
%setup -q

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_libdir}
cp Lib%{name}.so $RPM_BUILD_ROOT/%{_libdir}


%files
%defattr(-,root,root)
%{_libdir}/Lib%{name}.so

%doc

