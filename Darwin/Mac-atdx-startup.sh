#!/bin/sh
atdx_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/.*//'`"
atdx_RESOURCES="$atdx_BUNDLE/Contents/Resources"
atdx_TEMP="/tmp/atdx/$UID"

echo "running $0"
echo "atdx_BUNDLE: $atdx_BUNDLE"

# Setup temporary runtime files
rm -rf "$atdx_TEMP"

export "DYLD_LIBRARY_PATH=$atdx_RESOURCES/lib"
export "PATH=$atdx_RESOURCES/bin:$PATH"

#export
exec "$atdx_RESOURCES/bin/atdx"
