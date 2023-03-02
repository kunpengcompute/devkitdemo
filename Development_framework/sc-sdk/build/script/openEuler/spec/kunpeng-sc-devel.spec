%define debug_package %{nil}

Name:           kunpeng-sc-devel
Version:        1.3.0
Release:        1
Summary:        kunpeng sc devel
License:        Apache-2.0
Source0:        itrustee_client.tar.gz
Source1:        itrustee_sdk.tar.gz
Source2:        libboundscheck.tar.gz
Source3:        demo.tar.gz

BuildRequires:  gcc
Requires:       openssl-devel zlib-devel kernel-devel kunpeng-sc = 1.3.0

Conflicts: TEE_SDK TEE-devel_SDK

%description
Kunpeng sc devel for Secure Computing Application Development. Support os: openEuler 20.03 LTS SP1.

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
BuildDir=%{_builddir}/%{name}-%{version}
SdkHome=$RPM_BUILD_ROOT/usr/local/%{name}
SdkSource=${SdkHome}/source
SdkUtil=${SdkHome}/utils
SdkExample=${SdkHome}/examples
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
cp -r ${BuildDir}/itrustee_sdk/build/pack-Config/ ${SdkUtil}
cp -r ${BuildDir}/itrustee_sdk/build/pack-TA/ ${SdkUtil}
mkdir -p ${SdkUtil}/pack-Config/xml2tlv_tools/csv
cp -r ${BuildDir}/itrustee_sdk/build/signtools/tag_parse_dict.csv ${SdkUtil}/pack-Config/xml2tlv_tools/csv

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

%pre
checkDir=("/usr/local/%{name}" "/usr/include/itrustee_sdk")
for tmpDir in ${checkDir[*]}
do
    if [ -d ${tmpDir} ]; then
        echo "Failed to install the SDK because the ${tmpDir} directory already exists."
        echo "Clear the ${tmpDir} directory and install the SDK again."
        exit 1
    fi
done

%postun
rm -rf /usr/include/itrustee_sdk
rm -rf /usr/local/%{name}

%changelog
