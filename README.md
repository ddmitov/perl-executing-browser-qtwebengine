# Perl Executing Browser QtWebEngine

Perl Executing Browser (PEB) is an HTML5 user interface for [Perl 5](https://www.perl.org/) desktop applications. It is a C++ [Qt 5](https://www.qt.io/) application running local Perl 5 scripts as child processes without server. Inspired by [Electron](http://electron.atom.io/) and [NW.js](http://nwjs.io/), PEB is another reuse of web technologies in desktop applications with Perl doing the heavy lifting instead of [Node.js](https://nodejs.org/en/).

PEB QtWebEngine is an experimental version of [Perl Executing Browser](https://github.com/ddmitov/perl-executing-browser) using [QtWebEngine](https://wiki.qt.io/QtWebEngine) instead of [QtWebkit](https://doc.qt.io/archives/qt-5.5/qtwebkit-index.html).  
It is not recommended for production use due to the following additional issues and limitations:

* High memory usage
* Larger number of dependencies
* Can not be packed as a 64-bit Linux AppImage  
* Can not be compiled on any Windows OS

## Compile-Time Requirement

The only compile-time requirement of PEB QtWebEngine is a Linux Qt development bundle version 5.6 or higher.  
Compiled and tested successfully using:

* [Qt 5.8.0](http://download.qt.io/archive/qt/5.8/5.8.0/) on 64-bit Lubuntu 16.04
* [Qt 5.9.1](http://download.qt.io/archive/qt/5.9/5.9.1/) on 64-bit Lubuntu 16.04

## Runtime Requirements

* Qt 5 libraries.  
* Perl 5 distribution - any Linux or Windows Perl relocatable or standard distribution.  
  Tested with Linux 64-bit [Relocatable Perl](https://github.com/skaji/relocatable-perl) versions 5.24.1, 5.26.1.  
  PEB will use the first Perl on PATH if a relocatable Perl distribution is not available.

## Logging

PEB has unified logging of Perl and JavaScript errors in the JavaScript console.  
All you need to read Perl errors is to open the JavaScript console by  
starting PEB with the following command-line argument supplying an arbitrary private port:

```bash
./peb --remote-debugging-port=8080
```

and open the local URL with the supplied port using another browser  

``http://localhost:8080``

to access the ``QtWebEngine Developer Tools``, then go to the ``Console`` tab.

## Credits

http://www.iconarchive.com/show/animal-icons-by-jen/Camel-icon.html  

https://github.com/OtterBrowser/otter-browser/blob/master/src/modules/backends/web/qtwebengine/QtWebEnginePage.cpp  
https://github.com/OtterBrowser/otter-browser/blob/master/src/modules/backends/web/qtwebengine/QtWebEnginePage.h  

https://doc.qt.io/qt-5/qtwebenginewidgets-qtwebkitportingguide.html  
https://doc.qt.io/qt-5/qwebenginepage.html#runJavaScript  
https://doc.qt.io/qt-5/qwebenginepage.html#fullScreenRequested  
https://doc.qt.io/qt-5/qwebenginepage.html#windowCloseRequested  
https://doc.qt.io/qt-5/qtwebengine-webenginewidgets-simplebrowser-example.html#adding-context-menu-items  
https://doc.qt.io/qt-5/qwebenginefullscreenrequest.html  
https://doc.qt.io/qt-5.11/qtwebengine-debugging.html  

## [License](./LICENSE.md)

This program is free software;  
you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License,  
as published by the Free Software Foundation;  
either version 3 of the License, or (at your option) any later version.  
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;  
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## Author

Dimitar D. Mitov, 2018 - 2019  
