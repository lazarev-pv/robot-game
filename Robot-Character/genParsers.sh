#/bin/bash
rm  -rf src/Generated

mkdir -p src/Generated

xsd2cpp schemas/ched_settings.xsd --output_dir src/Generated
xsd2cpp schemas/save.xsd --output_dir src/Generated

sed -i 's/#include "CommonParser.h"/#include <ChibiEngine\/XMLParser\/CommonParser.h>/g' src/Generated/*.h

