%define debug_package %{nil}

Name:           kunpeng-sc-devel
Version:        1.0.1
Release:        1
Summary:        kunpeng sc devel
License:        Apache-2.0
Source0:        itrustee_client.tar.gz
Source1:        itrustee_sdk.tar.gz
Source2:        libboundscheck.tar.gz
Source3:        demo.tar.gz

BuildRequires:  gcc
Requires:       openssl-devel zlib-devel kernel-devel kunpeng-sc = 1.0.1

Conflicts: TEE_SDK TEE-devel_SDK

%description
Kunpeng sc devel for Secure Computing Application Development.

%prep
BuildDir=%{_builddir}/%{name}-%{version}

# copy itrustee_client
%setup -T -b 0 -c -n %{name}-%{version}
cp %{SOURCE0} ${BuildDir}/
tar -xf ${BuildDir}/itrustee_client.tar.gz

# copy itrustee_sdk
cp %{SOURCE1} ${BuildDir}/
tar -xf ${BuildDir}/itrustee_sdk.tar.gz

# copy libboundscheck
cp %{SOURCE2} ${BuildDir}/
tar -xf ${BuildDir}/libboundscheck.tar.gz

# copy demo
cp %{SOURCE3} ${BuildDir}/
tar -xf ${BuildDir}/demo.tar.gz

#%setup -q

%build

%install
#rm -rf $RPM_BUILD_ROOT
# clean BUILDROOT
# rm -rf %{_buildrootdir}
BuildDir=%{_builddir}/%{name}-%{version}
SdkHome=$RPM_BUILD_ROOT/usr/local/%{name}
SdkSource=${SdkHome}/source
SdkUtil=${SdkHome}/utils
SdkExample=${SdkHome}/example
SdkInc=$RPM_BUILD_ROOT/usr/include/itrustee_sdk

mkdir -p ${SdkInc}/thirdparty
mkdir -p ${SdkSource}
mkdir -p ${SdkUtil}
mkdir -p ${SdkExample}

# install source
cp -r ${BuildDir}/itrustee_sdk/ ${SdkSource}
cp -r ${BuildDir}/itrustee_client/ ${SdkSource}
cp -r ${BuildDir}/libboundscheck/ ${SdkSource}

# install utils
cp -r ${BuildDir}/itrustee_sdk/build/signtools/ ${SdkUtil}
cp -r ${BuildDir}/itrustee_sdk/build/tools/ ${SdkUtil}

# install include
cp -r ${BuildDir}/itrustee_sdk/include/* ${SdkInc}
cp -r ${BuildDir}/itrustee_sdk/thirdparty/open_source/musl/ ${SdkInc}/thirdparty

# install example
cp -r ${BuildDir}/demo/* ${SdkExample}

%files
#%doc
%defattr(-,root,root,-)

/usr/local/%{name}
/usr/include/itrustee_sdk

%postun
rm -rf /usr/include/itrustee_sdk
rm -rf /usr/local/%{name}

%changelog
