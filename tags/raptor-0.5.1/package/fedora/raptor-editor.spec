Summary		: Raptor A Portable Text editOR
Name		: raptor-editor
Version		: 0.5.0
Release		: 1%{?dist}
License		: GPLv3+
Group		: Applications/Editors
URL		: http://code.google.com/p/raptor/
Source		: %{name}-%{version}.tar.gz
BuildRoot	: %{_tmppath}/%{name}-%{version}-%{release}-builtroot
BuildRequires	: desktop-file-utils qt4-devel python
Requires	: qt4

%description
raptor is a source code editor supporting several languages.
It is largely inspired by Notepad++ editor but has the advantage 
of being portable, running on Windows, Linux and MacOs.
More information on Raptor's Homepage: http://code.google.com/p/raptor

%prep
%setup -q

%build
#%{_qt4_qmake} prefix=%{_prefix}
#make release
./waf configure --prefix=%{_prefix}
./waf release

%install
#rm -rf $RPM_BUILD_ROOT
#%{_qt4_qmake} prefix=%{_prefix}
#make install INSTALL_ROOT=$RPM_BUILD_ROOT
DESTDIR=$RPM_BUILD_ROOT ./waf install
install -d %{buildroot}%{_datadir}/pixmaps/
install -m 644 %{_builddir}/%{name}-%{version}/package/fedora/pixmaps/raptor.png %{buildroot}%{_datadir}/pixmaps/
desktop-file-install --dir=%{buildroot}%{_datadir}/applications/ %{_builddir}/%{name}-%{version}/package/fedora/applications/raptor.desktop

%clean
#make distclean
./waf distclean
rm -rf $RPM_BUILD_ROOT

%post
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache -q %{_datadir}/icons/hicolor;
fi
update-mime-database %{_datadir}/mime &> /dev/null || :
update-desktop-database &> /dev/null || :


%preun
# empty

%postun
touch --no-create %{_datadir}/icons/hicolor
if [ -x %{_bindir}/gtk-update-icon-cache ]; then
  %{_bindir}/gtk-update-icon-cache -q %{_datadir}/icons/hicolor;
fi
update-mime-database %{_datadir}/mime &> /dev/null || :
update-desktop-database &> /dev/null || :


%files
%defattr(-,root,root)
%doc LICENSE.GPL3
%{_bindir}/*
%{_datadir}/applications/raptor.desktop
%{_datadir}/pixmaps/raptor.png
%{_datadir}/raptor/translations/*

%changelog
* Mon Oct 17 2011 Christophe Duvernois <christophe.duvernois@gmail.com> 0.4.0-1%{?dist}
- Release 0.4.0
* Mon Jul 18 2011 Christophe Duvernois <christophe.duvernois@gmail.com> 0.3.0-1%{?dist}
- Release 0.3.0
