Name:           kunpeng-dc-devel
Version:        1.0.0
Release:        1
Summary:        kunpeng dc devel
License:        Commercial
Source:         kunpeng-dc-devel.tar.gz
Requires:       kunpeng-dc = 1.0.0

%description
kunpeng-dc-devel users the Kunpeng proprietary compression algorithm to enhance data compression performance. Support os: openEuler 22.03 LTS SP1.

%prep
%setup -q

%install
mkdir -p %{buildroot}/usr/include
cp -a %{_builddir}/%{name}-%{version}/glz.h %{buildroot}/usr/include/
mkdir -p %{buildroot}/usr/include/ksal
cp -a %{_builddir}/%{name}-%{version}/das.h %{buildroot}/usr/include/ksal/
cp -a %{_builddir}/%{name}-%{version}/ksal_crc.h %{buildroot}/usr/include/ksal/
cp -a %{_builddir}/%{name}-%{version}/ksal_erasure_code.h %{buildroot}/usr/include/ksal/

%files
%defattr(-,root,root,-)
/usr/include/glz.h
/usr/include/ksal/das.h
/usr/include/ksal/ksal_crc.h
/usr/include/ksal/ksal_erasure_code.h
/usr/include/ksal
