%define prefix /usr

Summary: A text client for the WorldForge project
Name: clay
Version: 0.1.0
Release: 1
Copyright: GPL
Group: Amusements/Games
Source: ftp://ftp.worldforge.org/pub/WorldForge/clients/source/clay-0.1.0.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
Clay is a text client that will provide a MUD-like interface to playing
WorldForge, and may potentially act as a proxy for using regular MUD
clients, and as a gateway for character scripting.

%prep
%setup -q

%build

if [ -x ./configure ]; then
  CFLAGS=$RPM_OPT_FLAGS ./configure --prefix=%{prefix}
else 
  CFLAGS=$RPM_OPT_FLAGS ./autogen.sh --prefix=%{prefix}
fi
make

%install
rm -rf $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{prefix}/bin/clay
