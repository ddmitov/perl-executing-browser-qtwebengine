# Perl Executing Browser QtWebEngine Project File

# This program is free software;
# you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License,
# as published by the Free Software Foundation;
# either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.
# Dimitar D. Mitov, 2018 - 2019, 2023
# https://github.com/ddmitov/perl-executing-browser-qtwebengine

lessThan (QT_MAJOR_VERSION, 5) {
    error ("Perl Executing Browser QtWebEngine requires minimal Qt version 5.6.")
}

lessThan (QT_MINOR_VERSION, 6) {
    error ("Perl Executing Browser QtWebEngine requires minimal Qt version 5.6.")
}

# Binary basics:
CONFIG += release
TEMPLATE = app
TARGET = peb

# HTML engine:
QT += widgets webenginewidgets

# Source files:
SOURCES += \
    main.cpp \
    file-reader.cpp \
    main-window.cpp \
    script-handler.cpp \
    webengine-page.cpp \
    webengine-view.cpp

# Header files:
HEADERS += \
    file-reader.h \
    script-handler.h \
    main-window.h \
    webengine-page.h \
    webengine-view.h

# Resources:
RESOURCES += resources/peb.qrc

# Destination directory for the compiled binary:
DESTDIR = $$PWD/../

# Temporary folder:
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp
