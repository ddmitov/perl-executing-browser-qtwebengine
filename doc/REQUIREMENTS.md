# Perl Executing Browser QtWebEngine - Requirements

## Compile-Time Requirements

* Qt development bundle versions 5.6 or higher

* QtWebEngine libraries and headers  

To compile PEB run the following commands in the root directory of the PEB project:

```bash
cd src
qmake -qt=qt5
make
```

## Runtime Requirements

* Qt 5 and QtWebEngine libraries

* Perl 5 distribution - any Linux Perl relocatable or standard distribution

  PEB will use the first Perl on PATH if a relocatable Perl distribution is not available.
