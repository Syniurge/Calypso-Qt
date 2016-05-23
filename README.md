# Enabling Qt5 development in D with LDC and Calypso

## moc package
MOC (the Meta-Object Compiler) is a tool that generates additional code for Qt classes, code essential for the signal/slot system, properties and other features of the Qt metaobject system.

This package replicates MOC's functionality entirely with D's metaprogramming capabilities: compile-time reflection, CTFE, mixins, templates. Currently only signals and slots have been tested.

```d
  class MyDClass : QObject
  {
    mixin signal!("testSignal", void function(int)); // signals need to be declared before Q_OBJECT
    
    mixin Q_OBJECT;
    (...)
    
    public extern(C++) @slots {
        void signalRecv(int n) { ... }
    }
  }
  
  connect2!(MyDClass.testSignal, MyDClass.signalRecv) (firstObj, secondObj);
```

See the [Calypso Qt5 widgets demo](examples/qt5demo.d) for a more detailed example.

## uic D generator
A D generator was added to uic, which makes working with Qt Designer's .ui files seamless.

```bash
  uic -g d -o <output file> <.ui file>
```

Why a new generator instead of simply loading generated C++ files with Calypso? Custom widgets are needed whenever overriding a virtual method such as ``event()`` is needed, which is inescapable for many if not most applications. This means a D class (inheriting from a Qt C++ class), which means the generated file has to be in D.

## Clang module map files
Module map files need to be copied in the Qt include folder for moc and the example to work properly.

Although Calypso should work without Clang module map files (*should*, this hasn't been tested for a long time), module maps help split the global and the ``Qt::`` namespaces' functions, function templates, and typedefs into smaller modules instead of a huge one (otherwise they would all be aggregated with the rest of the global namespace in ``"_"```).
