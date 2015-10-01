#/bin/bash
rm -rf src/XMLParser
rm -rf include/ChibiEngine/XMLParser

mkdir -p tmp

xsd2cpp schemas/atlas.xsd --output_dir tmp
xsd2cpp schemas/character.xsd  --output_dir tmp
xsd2cpp schemas/common.xsd --output_dir tmp
xsd2cpp schemas/laf.xsd  --output_dir tmp
xsd2cpp schemas/resources_description.xsd --output_dir tmp
xsd2cpp schemas/settings.xsd  --output_dir tmp
xsd2cpp schemas/shaders.xsd --output_dir tmp
xsd2cpp schemas/user_interface.xsd --output_dir tmp
xsd2cpp schemas/world.xsd  --output_dir tmp

mkdir -p include/ChibiEngine/XMLParser
mkdir -p src/XMLParser

mv tmp/*.h include/ChibiEngine/XMLParser/
mv tmp/*.cpp src/XMLParser/
mv include/ChibiEngine/XMLParser/util.h src/XMLParser/
rm -rf tmp

for i in $(find src/XMLParser -name "*.cpp"); do
	if [[ "$i" != "src/XMLParser/util.cpp" ]]; then
		sed -e '2d' $i > tmp.file
		name=$(basename "$i")
		name="${name%.*}"
		echo "#include <ChibiEngine/XMLParser/$name.h>" > $i 
		cat tmp.file >> $i
		rm tmp.file
	fi
done

