## Generating Calypso module map files for Qt
```bash
  ./modularize.sh <Qt include folder>
```

This creates the .modulemap_d files straight into the Qt folders, so make sure they're writtable.

### Why is this needed?
Although Calypso should work without module map files (should, this hasn't been tested for a long time), module maps help split the global and the Qt:: namespaces' functions, function templates, and typedefs into multiple modules instead of one (otherwise they would all be aggregated with the rest of the global namespace in ``"_"```).
