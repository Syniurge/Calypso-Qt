#!/bin/bash

# Create .modulemap_d files to split the global namespace functions/typedefs/global variables
# into one D module per Qt module (QtCore, QtGui, etc.)
# WARNING: the files are created in the passed Qt include folder

includeFolder="${BASH_ARGV[0]}"
pushd "$includeFolder"

for d in *; do
    pushd "$d"
    mmd="${d}.modulemap_d"

    echo "module ${d} {" >$mmd
    for f in *.h; do
        echo "    header \"./${f}\"" >>$mmd
    done
        echo "}" >>$mmd

    popd
done

popd
