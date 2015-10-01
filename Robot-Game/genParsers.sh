#/bin/bash
rm  -rf src/Generated

mkdir -p src/Generated

xsd2cpp schemas/robot_config.xsd --output_dir src/Generated

sed -i 's/#include "CommonParser.h"/#include <ChibiEngine\/XMLParser\/CommonParser.h>/g' src/Generated/*.h

