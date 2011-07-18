Summary		: Raptor A Portable Text editOR
Name		: raptor-editor
Version		: 0.3.0
Release		: 1%{?dist}
License		: GPL3
Group		: Development/C++
URL		: http://code.google.com/p/raptor/
Source		: %{name}-%{version}.tar.gz
Packager	: Christophe Duvernois <christophe.duvernois@gmail.com>
BuildRoot	: %{_tmppath}/%{name}-%{version}-%{release}-builtroot
BuildRequires	: desktop-file-utils qt4-devel
Requires	: qt4

%description
raptor is a source code editor supporting several languages.
It is largely inspired by Notepad++ editor but has the advantage of being portable,
running on windows,linux and mac os.

The project is still in the development phase and has quite a few bugs,
however, it's usable and already has some nice features...

%prep
rm -rf $RPM_BUILD_ROOT

%setup

%build
%{_qt4_qmake} prefix=%{_prefix}
make release

%install
%{_qt4_qmake} prefix=%{_prefix}
make install INSTALL_ROOT=$RPM_BUILD_ROOT
install -d %{buildroot}%{_datadir}/pixmaps/
install -m 644 %{_builddir}/%{name}-%{version}/package/fedora/pixmaps/raptor.png %{buildroot}%{_datadir}/pixmaps/
desktop-file-install --dir=%{buildroot}%{_datadir}/applications/ %{_builddir}/%{name}-%{version}/package/fedora/applications/raptor.desktop

%clean
make distclean
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
%{_bindir}/*
%{_datadir}/raptor/doc/*
%{_datadir}/applications/raptor.desktop
%{_datadir}/pixmaps/raptor.png
#/usr/bin/*
#%doc copying
#%doc ChangeLog.txt
#%doc README.txt
#%{instaldir_bin}/*

%changelog
* Fri Apr 22 2011 Christophe Duvernois <christophe.duvernois@gmail.com> 0.2.0
- Release 0.2.0
