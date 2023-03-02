%define debug_package %{nil}

Name:           kunpeng-sc
Version:        1.3.0
Release:        1
Summary:        kunpeng sc
License:        Apache-2.0
Source0:        itrustee_client.tar.gz
Source1:        itrustee_sdk.tar.gz
Source2:        itrustee_tzdriver.tar.gz
Source3:        libboundscheck.tar.gz

BuildRequires:  gcc
Requires:       openssl-devel zlib-devel kernel-devel
Conflicts:      TEE_SDK TEE-devel_SDK

%description
Kunpeng sc for Secure Computing Application Development. Support os: openEuler 20.03 LTS SP1.

%prep
BuildDir=%{_builddir}/%{name}-%{version}

# copy itrustee_client
%setup -T -b 0 -c -n %{name}-%{version}
cp %{SOURCE0} ${BuildDir}/
tar -xf ${BuildDir}/itrustee_client.tar.gz

# copy itrustee_sdk
cp %{SOURCE1} ${BuildDir}/
tar -xf ${BuildDir}/itrustee_sdk.tar.gz

# copy itrustee_tzdriver
cp %{SOURCE2} ${BuildDir}/
tar -xf ${BuildDir}/itrustee_tzdriver.tar.gz

# copy libboundscheck
cp %{SOURCE3} ${BuildDir}/
tar -xf ${BuildDir}/libboundscheck.tar.gz

# copy libboundscheck to itrustee_client
cp -r ${BuildDir}/libboundscheck/ ${BuildDir}/itrustee_client/

# copy libboundscheck to itrustee_tzdriver
cp -r ${BuildDir}/libboundscheck/ ${BuildDir}/itrustee_tzdriver/

#%setup -q

%build
cd itrustee_client
make
cd ..

cd itrustee_sdk
make
cd ..

%install
BuildDir=%{_builddir}/%{name}-%{version}
UsrBin=$RPM_BUILD_ROOT/usr/bin
UsrLib64=$RPM_BUILD_ROOT/lib64
UsrLocal=$RPM_BUILD_ROOT/usr/local
TzDriverDir=${UsrLocal}/%{name}/driver


mkdir -p ${UsrBin}
mkdir -p ${UsrLib64}
mkdir -p ${TzDriverDir}

# install libboundscheck libteec
cp ${BuildDir}/itrustee_client/dist/*.so.1 ${UsrLib64}
cp ${BuildDir}/itrustee_sdk/libteec_adaptor.so.1 ${UsrLib64}

cp -d ${BuildDir}/itrustee_client/dist/*.so ${UsrLib64}
cp -d ${BuildDir}/itrustee_sdk/libteec_adaptor.so ${UsrLib64}

# install teecd tlogcat
cp ${BuildDir}/itrustee_client/dist/teecd ${UsrBin}/teecd
cp ${BuildDir}/itrustee_client/dist/tlogcat ${UsrBin}/tlogcat

# install driver
cp -r ${BuildDir}/itrustee_tzdriver/ ${TzDriverDir}

%files
#%doc

%defattr(-,root,root,-)
/lib64/libboundscheck.so
/lib64/libteec.so
/lib64/libteec_adaptor.so
%{_bindir}/teecd
%{_bindir}/tlogcat
/usr/local/%{name}

/lib64/libboundscheck.so.1
/lib64/libteec.so.1
/lib64/libteec_adaptor.so.1

%pre
checkDir=("/usr/local/%{name}")
for tmpDir in ${checkDir[*]}
do
    if [ -d ${tmpDir} ]; then
        echo "Failed to install the SDK because the ${tmpDir} directory already exists."
        echo "Clear the ${tmpDir} directory and install the SDK again."
        exit 1
    fi
done

rm -rf /lib64/libboundscheck.so
rm -rf /lib64/libteec.so
rm -rf /lib64/libteec_adaptor.so
rm -rf /usr/lib64/libboundscheck.so
rm -rf /usr/lib64/libteec.so
rm -rf /usr/lib64/libteec_adaptor.so
rm -rf %{_bindir}/teecd
rm -rf %{_bindir}/tlogcat

count=$(ps -ef | grep teecd | grep -v "grep" | wc -l)
if [ ${count} -ne 0 ]; then
    kill -9 $(pidof teecd)
fi

%post
# insert tzdriver
cnt1=$(lsmod | grep tzdriver | wc -l)
if [ ${cnt1} -eq 0 ]; then
    cd /usr/local/%{name}/driver/itrustee_tzdriver
    touch build_tzdriver.log
    make -C libboundscheck >> build_tzdriver.log  2>&1
    make >> build_tzdriver.log 2>&1
    
    # insmod should disable enforce
    cnt2=$(getenforce | grep -i "Enforcing" | wc -l)
    if [ ${cnt2} -ne 0 ]; then
        sudo setenforce 0
        sudo insmod tzdriver.ko
        sudo setenforce 1
    else
        sudo insmod tzdriver.ko
    fi
fi

cnt3=$(lsmod | grep tzdriver | wc -l)
if [ ${cnt3} -eq 0 ]; then
    echo "At present, you can only compile the code. If you want to run it, you need to ensure the tzdriver is loaded successfully. For details, please see build_driver.log in the kunpeng-sc/driver/itrustee_tzdriver directory."
fi

# lanuch teecd
cnt4=$(ps -ef|grep teecd | grep -v "grep" | wc -l)
if [ ${cnt4} -eq 0 ]; then
    /usr/bin/teecd &
fi

%preun
%postun
rm -rf /usr/local/%{name}

cnt2=$(ps -ef|grep teecd | grep -v "grep" | wc -l)
if [ ${cnt2} -ne 0 ]; then
    kill -9 $(pidof teecd)
fi

%changelog
