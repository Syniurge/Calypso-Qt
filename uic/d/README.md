## uic D generator notes
Usage:
```bash
  uic -g d -o <output file> <.ui file>
```
The D generator was hacked from the C++ one, and there may still be oversights here and there that didn't lead to compilation errors in my use cases. If you encounter new errors, fixing them is usually easy and please consider sharing your fixes by submitting a PR.
 
One issue was that enum constants in .ui files are written without the parent enum name à la C++. Hence a map C++ enum constants -> D enum constant was needed, and such a map is generated from Qt headers by ``enumgen``, a small libclang tool.
 
The repository's ``qenum_constants.h`` was generated for Qt 5.5.1. You may need to regen it for newer versions if a new widget was introduced in the designer of Qt Creator.

### Why not simply load a C++ header from the uic C++ generator in Calypso?

Custom widgets are needed whenever you need to override a virtual method such as ``event()``, which is inescapable for many if not most applications. This means a D class (inheriting from a Qt C++ class), which the C++ generator cannot work with.

An added advantage is that a .ui file change doesn't result in a C++ AST change which would have triggered a complete recompilation of every C++ module by Calypso. Although the compilation speed could be much better if internal(static/anonymous namespace) C++ functions were emitted the D way (that's a TODO), it's probably at least 3-4 mins of compilation time regained in any case.