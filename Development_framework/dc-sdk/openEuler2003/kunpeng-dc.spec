Name:		kunpeng-dc
Version:	1.0.0
Release:	1
Summary:	kunpeng dc
License:	Commercial
Source: 	kunpeng-dc.tar.gz

%description
kunpeng-dc users the Kunpeng proprietary compression algorithm to enhance data compression performance. Support os: openEuler 20.03 LTS SP1.

%prep
%setup -q

%install
mkdir -p %{buildroot}/usr/lib64
cp -a %{_builddir}/%{name}-%{version}/*.so %{buildroot}/usr/lib64/

%files
%defattr(-,root,root,-)
/usr/lib64/libdas.so
/usr/lib64/libglz.so
/usr/lib64/libksal.so


