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
BuildRequires	: qt4-devel python-sphinx
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
qmake-qt4 -recursive prefix=%{buildroot}/usr
make release

%install
make install

%clean
make distclean

%post
# empty

%preun
# empty

%postun
# empty

%files
%defattr(-,root,root)
#/usr/bin/*
%{_bindir}/*
%{_datadir}/raptor/doc/*
#%doc copying
#%doc ChangeLog.txt
#%doc README.txt

#%{instaldir_bin}/*

%changelog
* Fri Apr 22 2011 Christophe Duvernois <christophe.duvernois@gmail.com> 0.2.0
- Release 0.2.0
