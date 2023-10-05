# Perl Executing Browser QtWebEngine - Requirements

## Compile-Time Requirements

* Qt development bundle versions 5.6 or higher

* QtWebEngine libraries and headers  

Compiled and tested successfully using [Qt 5.8.0](http://download.qt.io/archive/qt/5.8/5.8.0/), [Qt 5.9.1](http://download.qt.io/archive/qt/5.9/5.9.1/).

To compile PEB run the following commands in the root directory of the PEB project:

```bash
cd src
qmake -qt=qt5
make
```

## Runtime Requirements

* Qt 5 and QtWebEngine libraries

* Perl 5 distribution - any Linux or Windows Perl relocatable or standard distribution

  Tested with [Relocatable Perl](https://github.com/skaji/relocatable-perl) versions 5.24.1 and 5.26.1.

  PEB will use the first Perl on PATH if a relocatable Perl distribution is not available.
