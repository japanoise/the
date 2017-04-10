Summary: The Hessling Editor Documentation
Name: THE-doc
Version: %{myversion}
Release: 1
License: GPL
Group: Applications/Editors
Source: THE-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Vendor: Mark Hessling
Packager: Mark Hessling
URL: http://hessling-editor.sourceforge.net
Prefix: /usr
Icon: the64.xpm

%description
This package contains the documentation for THE; The Hessling Editor

THE is a full-screen character mode text editor based on the VM/CMS editor
XEDIT and many features of KEDIT written by Mansfield Software.

THE uses Rexx as its macro language which provides a rich and powerful
capability to extend THE.

For more information on THE, visit http://hessling-editor.sourceforge.net/

For more information on Rexx, visit http://www.rexxla.org
%prep

%setup -n THE-%{version}

%build
./configure --prefix=%{prefix} --with-rexx=rexxtrans --with-ncurses
make THE_Help.txt helpviewer

%install
make DESTDIR=%{buildroot} installdoc

%files
%doc COPYING
%doc README
%doc TODO
%doc index.html
%doc misc
%doc images
%doc commset
%doc commsos
%doc comm
