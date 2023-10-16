# Perl Executing Browser QtWebEngine - Requirements

## Compile-Time Requirements

* Qt development bundle versions 5.9 - 5.12

* QtWebEngine headers and libraries

Run the following commands in the root directory of the PEB project to compile PEB:

```bash
cd src
qmake -qt=qt5
make
```

## Runtime Requirements

* QtWebEngine 5.9 - 5.12 runtime libraries

* Perl 5 distribution - any Linux Perl relocatable or standard distribution

  PEB will use the first Perl on PATH if a relocatable Perl distribution is not available.
