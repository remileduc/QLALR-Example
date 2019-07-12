[![License GPL v2.1](https://img.shields.io/badge/license-GNU%20GPL%20v2.1-blue.svg)](./LICENSE)

QLALR-Example
=============

This is an (improved) example for using QLALR.

Back in the time, when Qt was owned by Nokia, they did a howto for QLALR. This can be found here:

- https://web.archive.org/web/20110813161301/qt.nokia.com/developer/qtquarterly/qlalr-adventures (EN)
- https://qt-quarterly.developpez.com/qq-33/qlalr/ (FR)

Unfortunately, the full code source of the example, originally called "qq33-qlalr.zip" is hard to find
(you can still get it from here: <https://doc.qt.io/archives/qq/>). This is why I created this repository.

What is it?
-----------

This code example is meant to understand how QLALR works, as there is *no* documentation for it...

The most important file is [plover.g](./plover.g). This file can be compiled with `qlalr` that will produce

- `CommandParser.hpp` et `CommandParser.cpp`
- `parser_p.h` et `parser.cpp`

These files are already given here so you can see what is produced, without having to run `qlalr` yourself.

The source code here is the same as the one provided in the original example, with the following changes:

- code updated to modern (C++17) style
- better and full error handling
	- improved output to see what happens
	- list of expected tokens id possible
- better separation of generic parser and grammar rules
	- add of a generic `QParser` class, coming from <https://code.woboq.org/qt5/qtbase/src/tools/qlalr/examples/qparser/qparser.h.html>

Compile
-------

To compile, you can run the steps described in the file [COMPILE](./COMPILE):

```bash
qlalr --no-debug --no-lines plover.g
qmake
make
```

Note that `qlalr` has already been run here, so you don't have to do it. The produced files are part of this repository for the
example sake.

`qlalr` is part of the default Qt installation since QtBase 5.3. You can find it in the Qt installation folder, in the subfolder
`bin/qlalr` or `bin/qlalr.exe` for Windows, next to `qmake`.
