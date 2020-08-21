%define pkgrelease  1
%if 0%{?openeuler}
%define specrelease %{pkgrelease}
%else
## allow specrelease to have configurable %%{?dist} tag in other distribution
%define specrelease %{pkgrelease}%{?dist}
%endif

Name:           deepin-reader
Version:        5.7.0.7
Release:        %{specrelease}
Summary:        Document Viewer is a simple PDF reader, supporting bookmarks, highlights and annotations
License:        GPLv3+
URL:            https://github.com/linuxdeepin/%{name}
Source0:        %{url}/archive/%{version}/%{name}-%{version}.tar.gz

BuildRequires: gcc-c++
BuildRequires: qt5-devel

BuildRequires: pkgconfig(dtkwidget)
BuildRequires: pkgconfig(dtkgui)
BuildRequires: pkgconfig(dtkcore)
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(ddjvuapi)

# BuildRequires: qt5-qtbase-devel
# BuildRequires: dtkwidget-devel
BuildRequires: kf5-karchive-devel
# BuildRequires: qt5-linguist
# BuildRequires: poppler-qt5
BuildRequires: poppler-qt5-devel
# BuildRequires: poppler
# BuildRequires: poppler-devel
# BuildRequires: djvulibre-devel
# BuildRequires: djvulibre-libs
BuildRequires: libspectre-devel
# BuildRequires: libspectre
# BuildRequires: qt5-qtsvg-devel
# BuildRequires: qt5-qtmultimedia-devel
# BuildRequires: qt5-qtx11extras-devel
# BuildRequires: libtiff
BuildRequires: libtiff-devel
# BuildRequires: libuuid-devel
# BuildRequires: libuuid

%description
%{Summary}.

%prep
%autosetup

%build
# help find (and prefer) qt5 utilities, e.g. qmake, lrelease
export PATH=%{_qt5_bindir}:$PATH
mkdir build && pushd build
%qmake_qt5 ../ 
%make_build
popd

%install
%make_install -C build INSTALL_ROOT="%buildroot"

%files
%doc README.md
%license LICENSE
%{_bindir}/%{name}
%{_libdir}/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}/translations/*.qm
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%changelog

