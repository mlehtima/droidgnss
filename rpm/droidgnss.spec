%define strip /bin/true
%define __requires_exclude  ^.*$
%define __find_requires     %{nil}
%global debug_package       %{nil}
%define __provides_exclude_from ^.*$

%define _target_cpu %{device_rpm_architecture_string}

Name:          droidgnss
Summary:       Android gnss wrapper library
Version:       0.20180414.0
Release:       1
Group:         System/Libraries
License:       ASL 2.0
BuildRequires: ubu-trusty
BuildRequires: sudo-for-abuild
BuildRequires: droid-bin-src-full
Source0:       %{name}-%{version}.tgz
AutoReqProv:   no

%description
%{summary}

%package       devel
Summary:       droidgnss development headers
Requires:      droidgnss = %{version}-%{release}
BuildArch:     noarch

%description   devel
%{summary}

%prep

%if %{?device_rpm_architecture_string:0}%{!?device_rpm_architecture_string:1}
echo "device_rpm_architecture_string is not defined"
exit -1
%endif

%setup -T -c -n droidgnss
sudo chown -R abuild:abuild /home/abuild/src/droid/
mv /home/abuild/src/droid/* .
mkdir -p external
pushd external
tar -zxf %SOURCE0
mv droidgnss* droidgnss
popd

%build

droid-make %{?_smp_mflags} libdroidgnss

%install

mkdir -p $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/lib/
mkdir -p $RPM_BUILD_ROOT/%{_includedir}/droidgnss/
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/droidgnss/

cp out/target/product/*/system/lib/libdroidgnss.so \
    $RPM_BUILD_ROOT/%{_libexecdir}/droid-hybris/system/lib/

cp external/droidgnss/*.h $RPM_BUILD_ROOT/%{_includedir}/droidgnss/
cp external/droidgnss/hybris.c $RPM_BUILD_ROOT/%{_datadir}/droidgnss/

%files
%defattr(-,root,root,-)
%{_libexecdir}/droid-hybris/system/lib/libdroidgnss.so

%files devel
%defattr(-,root,root,-)
%{_includedir}/droidgnss/*.h
%{_datadir}/droidgnss/hybris.c
