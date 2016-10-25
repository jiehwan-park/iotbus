%define __usrdir /usr/lib/systemd/system

Name:       capi-iotbus
Summary:    Tizen IoT Bus library
Version:    0.0.1
Release:    0
Group:      Network & Connectivity
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    %{name}.manifest
Source2:    %{name}-server.manifest
Source3:    iotbus-server.service
Source10:   %{name}-test.manifest
BuildRequires:  cmake
BuildRequires:  boost-devel
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gio-2.0)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(capi-system-system-settings)
BuildRequires:  pkgconfig(cynara-client)
BuildRequires:  pkgconfig(cynara-session)
BuildRequires:  pkgconfig(cynara-creds-gdbus)
BuildRequires:  pkgconfig(mused)
Requires(post): /usr/bin/getent, /usr/sbin/useradd, /usr/sbin/groupadd
Requires(post): /sbin/ldconfig, /usr/bin/systemctl
Requires(postun): /sbin/ldconfig, /usr/bin/systemctl

%description
Tizen IoT Bus Library


%package devel
Summary:    TizenIoT Bus(devel)
Group:      Network & Connectivity/Development
Requires:   %{name} = %{version}

%description devel
IoT Bus development Kit

%package server
Summary:    TizenIoT Bus(server)
Group:      IoT Bus Server
Requires:   %{name} = %{version} 

%description server
Tizen IoT Bus server Programs

%package test
Summary:    TizenIoT Bus(test)
Group:      IoT Bus Testing
Requires:   %{name} = %{version}

%description test
Tizen IoT Bus Test Programs


%prep
%setup -q
chmod g-w %_sourcedir/*
cp %{SOURCE1} ./%{name}.manifest
cp %{SOURCE2} ./%{name}-server.manifest
cp %{SOURCE3} ./iotbus-server.service
cp %{SOURCE10} ./%{name}-test.manifest

%build
TZ_VER_3=1

export LDFLAGS+="-Wl,--as-needed"
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DMAJORVER=${MAJORVER} -DFULLVER=%{version} -DBIN_INSTALL_DIR:PATH=%{_bindir}

%install
%make_install
mkdir -p %{buildroot}/usr/lib/systemd/system/multi-user.target.wants
install -m 0644 %SOURCE3 %{buildroot}/usr/lib/systemd/system/iotbus-server.service
ln -s ../iotbus-server.service %{buildroot}/usr/lib/systemd/system/multi-user.target.wants/

%post
/sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so
%license LICENSE.APLv2

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/%{name}/*.h

%files server
%manifest %{name}-server.manifest
%defattr(-,root,root,-)
%{_bindir}/iotbus_server*
%license LICENSE.APLv2
/usr/lib/systemd/system/iotbus-server.service
/usr/lib/systemd/system/multi-user.target.wants/iotbus-server.service

%files test
%manifest %{name}-test.manifest
%defattr(-,root,root,-)
%{_bindir}/iotbus_test*
%license LICENSE.APLv2
