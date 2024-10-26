%define debug_package %{nil}

Name:           kunpeng-sc
Version:        %{PackageVersion}
Release:        1
Summary:        kunpeng sc
License:        Apache-2.0
Source0:        itrustee_client.tar.gz
Source1:        itrustee_sdk.tar.gz
Source2:        itrustee_tzdriver.tar.gz
Source3:        libboundscheck.tar.gz
Source4:        bisheng-jdk-8u372-tee-aarch64.tar.gz
Source5:        Python-3.6.15-tee-aarch64.tar.gz

BuildRequires:  gcc
Requires:       openssl-devel zlib-devel kernel-devel
Conflicts:      TEE_SDK TEE-devel_SDK

%description
Kunpeng sc for Secure Computing Application Development. Support os: Kylin V10.

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

cp %{SOURCE4} ${BuildDir}/
cp %{SOURCE5} ${BuildDir}/

# copy libboundscheck to itrustee_client
cp -r ${BuildDir}/libboundscheck/ ${BuildDir}/itrustee_client/

# copy libboundscheck to itrustee_tzdriver
cp -r ${BuildDir}/libboundscheck/ ${BuildDir}/itrustee_tzdriver/

# copy libboundscheck to tee_upgrade
cp -r ${BuildDir}/libboundscheck/ ${BuildDir}/itrustee_sdk/test/CA/tee_upgrade

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

# copy tee runtime
mkdir -p ${UsrLocal}/%{name}/java/
mkdir -p ${UsrLocal}/%{name}/python/
cp -rf ${BuildDir}/bisheng-jdk-8u372-tee-aarch64.tar.gz ${UsrLocal}/%{name}/java/
cp -rf ${BuildDir}/Python-3.6.15-tee-aarch64.tar.gz ${UsrLocal}/%{name}/python/

# install teecd tlogcat
cp ${BuildDir}/itrustee_client/dist/teecd ${UsrBin}/teecd
cp ${BuildDir}/itrustee_client/dist/tlogcat ${UsrBin}/tlogcat
cp ${BuildDir}/itrustee_client/dist/tee_teleport ${UsrBin}/tee_teleport

# install driver
cp -r ${BuildDir}/itrustee_tzdriver/ ${TzDriverDir}

# install tee_upgrade
cp -r ${BuildDir}/itrustee_sdk/test/CA/tee_upgrade ${UsrLocal}/%{name}/

%files
#%doc

%defattr(-,root,root,-)
/lib64/libboundscheck.so
/lib64/libteec.so
/lib64/libteec_adaptor.so
%{_bindir}/teecd
%{_bindir}/tlogcat
%{_bindir}/tee_teleport
/usr/local/%{name}

/lib64/libboundscheck.so.1
/lib64/libteec.so.1
/lib64/libteec_adaptor.so.1
/usr/local/%{name}/java/bisheng-jdk-8u372-tee-aarch64.tar.gz
/usr/local/%{name}/python/Python-3.6.15-tee-aarch64.tar.gz

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
rm -rf %{_bindir}/tee_teleport

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

# lanuch teecd
cnt4=$(ps -ef|grep teecd | grep -v "grep" | wc -l)
if [ ${cnt4} -eq 0 ]; then
    /usr/bin/teecd &
fi

cnt3=$(lsmod | grep tzdriver | wc -l)
if [ ${cnt3} -eq 0 ]; then
    echo "At present, you can only compile the code. If you want to run it, you need to ensure the tzdriver is loaded successfully. For details, please see build_driver.log in the kunpeng-sc/driver/itrustee_tzdriver directory."
else
    if [ -f /var/itrustee/image/trustedcore.img ]; then
        cd /usr/local/%{name}/tee_upgrade
        touch build_tee_upgrade.log
        make -C libboundscheck >> build_tee_upgrade.log  2>&1
        make >> build_tee_upgrade.log 2>&1
        
        # insmod should disable enforce
        cnt2=$(getenforce | grep -i "Enforcing" | wc -l)
        if [ ${cnt2} -ne 0 ]; then
            sudo setenforce 0
            sudo insmod tee_upgrade.ko
            sudo setenforce 1
        else
            sudo insmod tee_upgrade.ko
        fi
    fi
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
