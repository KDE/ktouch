KTouch
======

[![Build Status](https://build.kde.org/view/Applications/job/Applications%20ktouch%20kf5-qt5%20SUSEQt5.9/badge/icon)](https://build.kde.org/view/Applications/job/Applications%20ktouch%20kf5-qt5%20SUSEQt5.9/)

KTouch is a program to learn and practice touch typing. Every finger
has its place on the keyboard with associated keys to press. Starting
with only a few keys to remember you will advance through different
training levels where additional keys are introduced. Because you no
longer need to search for the keys on the keyboard you will be able
to type quickly and accurately.

Building
========

KTouch has the following build dependencies:
 * CMake ≥ 3.0.0
 * CMake Extra Modules ≥ 1.0.0
 * Qt ≥ 5.9 with the following modules
   * Qt GUI
   * Qt QML
   * Qt Quick
   * Qt Quick Widgets
   * Qt Quick Controls 2
   * Qt SQL
   * Qt Test
   * Qt X11 Extras
   * Qt XML
   * Qt XML Patterns
 * KDE Frameworks 5:
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
 * libxkbfile (optional, for keyboard layout auto-detection)
 * libxcb (optional, for keyboard layout auto-detection)

If the build requirements are met, KTouch can be built and installed with:

    mkdir build
    cd build/
    cmake ..
    make
    sudo make install

Runtime dependencies
====================

At runtime KTouch needs the following software packages to be present:
 * [Breeze icon set](https://phabricator.kde.org/source/breeze-icons/)
 * [kqtquickcharts](https://phabricator.kde.org/source/kqtquickcharts/) >= 16.12
 * [plasma-desktop](https://phabricator.kde.org/source/plasma-desktop/)
   (optional, for keyboard layout configuration from within the application)

