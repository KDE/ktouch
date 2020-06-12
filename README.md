# KTouch

<a href='https://flathub.org/apps/details/org.kde.ktouch'><img height='56' alt='Get KTouch from Flathub' src='https://flathub.org/assets/badges/flathub-badge-en.png'></a>
<a href='https://snapcraft.io/ktouch'><img height='56' alt='Get KTouch from the Snap Store' src='https://snapcraft.io/static/images/badges/en/snap-store-black.svg'></a>

![Screenshot](https://cdn.kde.org/screenshots/ktouch/ktouch.png)

KTouch is a program to learn and practice touch typing. Every finger
has its place on the keyboard with associated keys to press. Starting
with only a few keys to remember you will advance through different
training levels where additional keys are introduced. Because you no
longer need to search for the keys on the keyboard you will be able
to type quickly and accurately.

# Build Status

| Jenkins CI Name | Master | Stable |
| --------------- | ------ | ------ |
| OpenSuse Qt 5.14 | [![Build Status](https://build.kde.org/job/Applications/job/ktouch/job/kf5-qt5%20SUSEQt5.14/badge/icon)](https://build.kde.org/job/Applications/job/ktouch/job/kf5-qt5%20SUSEQt5.14/) | [![Build Status](https://build.kde.org/job/Applications/job/ktouch/job/stable-kf5-qt5%20SUSEQt5.14/badge/icon)](https://build.kde.org/job/Applications/job/ktouch/job/stable-kf5-qt5%20SUSEQt5.14/) |
| FreeBSD Qt 5.14 | [![Build Status](https://build.kde.org/job/Applications/job/ktouch/job/kf5-qt5%20FreeBSDQt5.14/badge/icon)](https://build.kde.org/job/Applications/job/ktouch/job/kf5-qt5%20FreeBSDQt5.14/) | [![Build Status](https://build.kde.org/job/Applications/job/ktouch/job/stable-kf5-qt5%20FreeBSDQt5.14/badge/icon)](https://build.kde.org/job/Applications/job/ktouch/job/stable-kf5-qt5%20FreeBSDQt5.14/) |

# Development Setup

## Quick Start

In the top level directory there is a script called `build-install-run`
which can be used to configure, build, install and run KTouch in one
step. Execute:

    ./build-install-run

in the top-level directory of the KTouch repository to get started.
It is just simple wrapper arround CMake and set ups the necessary
environment variables to run KTouch. Build artifacts are stored in
the subdirectory `build/`, install files go to `install/`.

## Building

KTouch has the following build dependencies:

 * [CMake][cmake] ≥ 3.0.0
 * [Extra CMake Modules][ecm] ≥ 1.0.0
 * [Qt][qt] ≥ 5.9 with the following modules
   * Qt GUI
   * Qt QML
   * Qt Quick
   * Qt Quick Widgets
   * Qt Quick Controls 2
   * Qt SQL
   * Qt Test
   * Qt XML
   * Qt XML Patterns
 * [KDE Frameworks 5][kf5] with:
   * Completion
   * Config
   * ConfigWidgets
   * CoreAddons
   * Declarative
   * DocTools
   * I18n
   * ItemViews
   * KCMUtils
   * KIO
   * TextWidgets
   * WidgetsAddons
   * WindowSystem
   * XmlGui
   * IconThemes
 * optionally, for keyboard layout auto-detection:
   * Qt X11 Extras
   * [Xlib][xlib]
   * [XKB][xkb]
   * [libxkbfile][libxkbfile]
   * [libxcb][libxcb] with libxcb-xkb

[cmake]: https://cmake.org/
[ecm]: https://invent.kde.org/frameworks/extra-cmake-modules/
[qt]: https://www.qt.io/
[kf5]: https://api.kde.org/frameworks/
[xlib]: https://tronche.com/gui/x/xlib/
[xkb]: https://www.x.org/wiki/XKB/
[libxkbfile]: https://gitlab.freedesktop.org/xorg/lib/libxkbfile
[libxcb]: https://xcb.freedesktop.org/

Collecting and installing all dependencies by hand can be a challenge.
Far easier is to use the dependency resolution of your package
manager. Usually they can be instructed to install all dependencies
automatically by using the information of the packaged version of
KTouch:

    apt-get build-dep ktouch # Ubuntu, Debian, ...
    zypper source-install --build-deps-only ktouch # openSUSE

If the build requirements are met, execute the following commands in
the root directory of your working copy to built and install KTouch:

    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=../install
    make
    make install

These commands install KTouch to the directory `install` in the root of
your working copy. You can install KTouch to any directory of your
choice by passing a different path to `-DCMAKE_INSTALL_PREFIX`.

## Running

At runtime KTouch needs the following additional software packages to be
installed:

 * [Breeze icon set][breeze]
 * [kqtquickcharts][kqtquickcharts] ≥ 16.12
 * [plasma-desktop][plasma-desktop] (optional, for keyboard layout
   configuration inside the application)

[breeze]: https://phabricator.kde.org/source/breeze-icons/
[kqtquickcharts]: https://phabricator.kde.org/source/kqtquickcharts/
[plasma-desktop]: https://phabricator.kde.org/source/plasma-desktop/

KTouch needs the following environment variables to be set in order
to find all its installed resources:

    export KDEDIRS=<full-path-to-install-dir>:$KDEDIRS
    export XDG_DATA_DIRS=<full-path-to-install-dir>/share:$XDG_DATA_DIRS

After this, KTouch can finally be launched with:

    <path-to-install-dir>/bin/ktouch
