%define        __spec_install_post %{nil}
%define          debug_package %{nil}

Name:           XrdCustomQuotaOss
Version:        0.9
Release:        1%{?dist}
Summary:        A Lustre fs wrapper for xrd quota queries on lustre
Group:          XRootD/Plugins

License:        LGPLv3
URL:            https://github.com/jknedlik/XrdCustomQuotaOssWrapper
Source0:        %{name}-%{version}.tar.gz

Requires(pre): xrootd-server



%description
A Lustre fs wrapper for quota queries

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

