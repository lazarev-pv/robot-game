#ifndef __SCHEMAS_USER_INTERFACE_XSD
#define __SCHEMAS_USER_INTERFACE_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include "CommonParser.h"

// ----- DECLARAIONS ------ //
struct AllFilterType;
struct ButtonImageType;
struct ButtonSlot;
struct CheckBoxSlot;
struct ContainerLineType;
struct ContainerSelectorSlot;
struct ContextMenuElemType;
struct ContextMenuSlot;
struct DDLBElemType;
struct DDLBSlot;
struct DialogSlot;
struct HealthbarSlot;
struct ImagePickerSlot;
struct ImageSlot;
struct InputSlot;
struct LabelsListSlot;
struct LineContainerSlot;
struct MenuType;
struct OnlyDigitsFilterType;
struct RGBSlot;
struct RadioSlot;
struct SliderSlot;
struct SlotType;
struct TableContainerSlot;
struct TextSlot;
struct UserInterfaceParserRoot;
struct UserInterfaceType;
// ----- DEFINITIONS ------ //

// ----- AllFilterType
struct AllFilterType{
	AllFilterType();
	AllFilterType(AllFilterType&& other);
	AllFilterType(const AllFilterType& other) = delete;
	AllFilterType& operator=(const AllFilterType& other) = delete;
	AllFilterType(rapidxml::xml_node<>* node);
	~AllFilterType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	int maxSize;

};

// ----- ButtonImageType
struct ButtonImageType{
	ButtonImageType();
	ButtonImageType(ButtonImageType&& other);
	ButtonImageType(const ButtonImageType& other) = delete;
	ButtonImageType& operator=(const ButtonImageType& other) = delete;
	ButtonImageType(rapidxml::xml_node<>* node);
	~ButtonImageType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string normal;
	std::string hovered;
	std::string pushed;

};

// ----- ButtonSlot
struct ButtonSlot{
	ButtonSlot();
	ButtonSlot(ButtonSlot&& other);
	ButtonSlot(const ButtonSlot& other) = delete;
	ButtonSlot& operator=(const ButtonSlot& other) = delete;
	ButtonSlot(rapidxml::xml_node<>* node);
	~ButtonSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ButtonImageType*  image;
	std::string text;
	std::string script;

};

// ----- CheckBoxSlot
struct CheckBoxSlot{
	CheckBoxSlot();
	CheckBoxSlot(CheckBoxSlot&& other);
	CheckBoxSlot(const CheckBoxSlot& other) = delete;
	CheckBoxSlot& operator=(const CheckBoxSlot& other) = delete;
	CheckBoxSlot(rapidxml::xml_node<>* node);
	~CheckBoxSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	bool value;
	std::string script;

};

// ----- ContainerLineType
struct ContainerLineType{
	ContainerLineType();
	ContainerLineType(ContainerLineType&& other);
	ContainerLineType(const ContainerLineType& other) = delete;
	ContainerLineType& operator=(const ContainerLineType& other) = delete;
	ContainerLineType(rapidxml::xml_node<>* node);
	~ContainerLineType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<SlotType* > slot;
	std::string weight;

};

// ----- ContainerSelectorSlot
struct ContainerSelectorSlot{
	ContainerSelectorSlot();
	ContainerSelectorSlot(ContainerSelectorSlot&& other);
	ContainerSelectorSlot(const ContainerSelectorSlot& other) = delete;
	ContainerSelectorSlot& operator=(const ContainerSelectorSlot& other) = delete;
	ContainerSelectorSlot(rapidxml::xml_node<>* node);
	~ContainerSelectorSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<SlotType* > slot;
	std::string initial;

};

// ----- ContextMenuElemType
struct ContextMenuElemType{
	ContextMenuElemType();
	ContextMenuElemType(ContextMenuElemType&& other);
	ContextMenuElemType(const ContextMenuElemType& other) = delete;
	ContextMenuElemType& operator=(const ContextMenuElemType& other) = delete;
	ContextMenuElemType(rapidxml::xml_node<>* node);
	~ContextMenuElemType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ContextMenuElemType* > elem;
	std::string name;

};

// ----- ContextMenuSlot
struct ContextMenuSlot{
	ContextMenuSlot();
	ContextMenuSlot(ContextMenuSlot&& other);
	ContextMenuSlot(const ContextMenuSlot& other) = delete;
	ContextMenuSlot& operator=(const ContextMenuSlot& other) = delete;
	ContextMenuSlot(rapidxml::xml_node<>* node);
	~ContextMenuSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ContextMenuElemType* > elem;

};

// ----- DDLBElemType
struct DDLBElemType{
	DDLBElemType();
	DDLBElemType(DDLBElemType&& other);
	DDLBElemType(const DDLBElemType& other) = delete;
	DDLBElemType& operator=(const DDLBElemType& other) = delete;
	DDLBElemType(rapidxml::xml_node<>* node);
	~DDLBElemType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;

};

// ----- DDLBSlot
struct DDLBSlot{
	DDLBSlot();
	DDLBSlot(DDLBSlot&& other);
	DDLBSlot(const DDLBSlot& other) = delete;
	DDLBSlot& operator=(const DDLBSlot& other) = delete;
	DDLBSlot(rapidxml::xml_node<>* node);
	~DDLBSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<DDLBElemType* > elem;
	std::string text;

};

// ----- DialogSlot
struct DialogSlot{
	DialogSlot();
	DialogSlot(DialogSlot&& other);
	DialogSlot(const DialogSlot& other) = delete;
	DialogSlot& operator=(const DialogSlot& other) = delete;
	DialogSlot(rapidxml::xml_node<>* node);
	~DialogSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<SlotType* > slot;
	std::string capture;

};

// ----- HealthbarSlot
struct HealthbarSlot{
	HealthbarSlot();
	HealthbarSlot(HealthbarSlot&& other);
	HealthbarSlot(const HealthbarSlot& other) = delete;
	HealthbarSlot& operator=(const HealthbarSlot& other) = delete;
	HealthbarSlot(rapidxml::xml_node<>* node);
	~HealthbarSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  color;
	float min;
	float max;
	float value;
	int percision;

};

// ----- ImagePickerSlot
struct ImagePickerSlot{
	ImagePickerSlot();
	ImagePickerSlot(ImagePickerSlot&& other);
	ImagePickerSlot(const ImagePickerSlot& other) = delete;
	ImagePickerSlot& operator=(const ImagePickerSlot& other) = delete;
	ImagePickerSlot(rapidxml::xml_node<>* node);
	~ImagePickerSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);


};

// ----- ImageSlot
struct ImageSlot{
	ImageSlot();
	ImageSlot(ImageSlot&& other);
	ImageSlot(const ImageSlot& other) = delete;
	ImageSlot& operator=(const ImageSlot& other) = delete;
	ImageSlot(rapidxml::xml_node<>* node);
	~ImageSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string presentation;

};

// ----- InputSlot
struct InputSlotUnion {
	InputSlotUnion();
	InputSlotUnion(InputSlotUnion&& other);
	struct OnlyDigitsFilterType* onlyDigits;
	struct AllFilterType* all;
};
struct InputSlot{
	InputSlot();
	InputSlot(InputSlot&& other);
	InputSlot(const InputSlot& other) = delete;
	InputSlot& operator=(const InputSlot& other) = delete;
	InputSlot(rapidxml::xml_node<>* node);
	~InputSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	InputSlotUnion choiceValue;

	std::string value;
};

// ----- LabelsListSlot
struct LabelsListSlot{
	LabelsListSlot();
	LabelsListSlot(LabelsListSlot&& other);
	LabelsListSlot(const LabelsListSlot& other) = delete;
	LabelsListSlot& operator=(const LabelsListSlot& other) = delete;
	LabelsListSlot(rapidxml::xml_node<>* node);
	~LabelsListSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<TextSlot* > elem;

};

// ----- LineContainerSlot
struct LineContainerSlot{
	LineContainerSlot();
	LineContainerSlot(LineContainerSlot&& other);
	LineContainerSlot(const LineContainerSlot& other) = delete;
	LineContainerSlot& operator=(const LineContainerSlot& other) = delete;
	LineContainerSlot(rapidxml::xml_node<>* node);
	~LineContainerSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  backColor;
	std::vector<ContainerLineType* > line;
	float yIntent;
	float xIntent;
	std::string caption;

};

// ----- MenuType
struct MenuType{
	MenuType();
	MenuType(MenuType&& other);
	MenuType(const MenuType& other) = delete;
	MenuType& operator=(const MenuType& other) = delete;
	MenuType(rapidxml::xml_node<>* node);
	~MenuType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ContextMenuElemType* > elem;
	std::string name;

};

// ----- OnlyDigitsFilterType
struct OnlyDigitsFilterType{
	OnlyDigitsFilterType();
	OnlyDigitsFilterType(OnlyDigitsFilterType&& other);
	OnlyDigitsFilterType(const OnlyDigitsFilterType& other) = delete;
	OnlyDigitsFilterType& operator=(const OnlyDigitsFilterType& other) = delete;
	OnlyDigitsFilterType(rapidxml::xml_node<>* node);
	~OnlyDigitsFilterType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	int maxSize;

};

// ----- RGBSlot
struct RGBSlot{
	RGBSlot();
	RGBSlot(RGBSlot&& other);
	RGBSlot(const RGBSlot& other) = delete;
	RGBSlot& operator=(const RGBSlot& other) = delete;
	RGBSlot(rapidxml::xml_node<>* node);
	~RGBSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  color;

};

// ----- RadioSlot
struct RadioSlot{
	RadioSlot();
	RadioSlot(RadioSlot&& other);
	RadioSlot(const RadioSlot& other) = delete;
	RadioSlot& operator=(const RadioSlot& other) = delete;
	RadioSlot(rapidxml::xml_node<>* node);
	~RadioSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	bool value;
	std::string group;

};

// ----- SliderSlot
struct SliderSlot{
	SliderSlot();
	SliderSlot(SliderSlot&& other);
	SliderSlot(const SliderSlot& other) = delete;
	SliderSlot& operator=(const SliderSlot& other) = delete;
	SliderSlot(rapidxml::xml_node<>* node);
	~SliderSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float min;
	float max;
	float value;
	int percision;

};

// ----- SlotType
struct SlotTypeUnion {
	SlotTypeUnion();
	SlotTypeUnion(SlotTypeUnion&& other);
	struct ButtonSlot* button;
	struct TextSlot* text;
	struct ImageSlot* image;
	struct DDLBSlot* ddlb;
	struct InputSlot* input;
	struct ContextMenuSlot* context;
	struct DialogSlot* dialog;
	struct ImagePickerSlot* imagePicker;
	struct CheckBoxSlot* checkbox;
	struct SliderSlot* slider;
	struct RGBSlot* rgb;
	struct RadioSlot* radio;
	struct LabelsListSlot* labelsList;
	struct TableContainerSlot* tableContainer;
	struct LineContainerSlot* lineContainer;
	struct ContainerSelectorSlot* containerSelector;
	struct HealthbarSlot* healthbar;
};
struct SlotType{
	SlotType();
	SlotType(SlotType&& other);
	SlotType(const SlotType& other) = delete;
	SlotType& operator=(const SlotType& other) = delete;
	SlotType(rapidxml::xml_node<>* node);
	~SlotType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	SlotTypeUnion choiceValue;

	float x;
	float y;
	float z;
	float hw;
	float hh;
	std::string name;
	bool visible;
	bool enabled;
};

// ----- TableContainerSlot
struct TableContainerSlot{
	TableContainerSlot();
	TableContainerSlot(TableContainerSlot&& other);
	TableContainerSlot(const TableContainerSlot& other) = delete;
	TableContainerSlot& operator=(const TableContainerSlot& other) = delete;
	TableContainerSlot(rapidxml::xml_node<>* node);
	~TableContainerSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  backColor;
	std::vector<float> weight;
	std::vector<SlotType* > slot;
	float intent;
	bool drawLines;
	std::string align;
	std::string caption;

};

// ----- TextSlot
struct TextSlot{
	TextSlot();
	TextSlot(TextSlot&& other);
	TextSlot(const TextSlot& other) = delete;
	TextSlot& operator=(const TextSlot& other) = delete;
	TextSlot(rapidxml::xml_node<>* node);
	~TextSlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string text;

};

// ----- UserInterfaceParserRoot
struct UserInterfaceParserRoot{
	UserInterfaceParserRoot();
	UserInterfaceParserRoot(UserInterfaceParserRoot&& other);
	UserInterfaceParserRoot(const UserInterfaceParserRoot& other) = delete;
	UserInterfaceParserRoot& operator=(const UserInterfaceParserRoot& other) = delete;
	UserInterfaceParserRoot(rapidxml::xml_node<>* node);
	~UserInterfaceParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	UserInterfaceType*  userInterface;

};

// ----- UserInterfaceType
struct UserInterfaceType{
	UserInterfaceType();
	UserInterfaceType(UserInterfaceType&& other);
	UserInterfaceType(const UserInterfaceType& other) = delete;
	UserInterfaceType& operator=(const UserInterfaceType& other) = delete;
	UserInterfaceType(rapidxml::xml_node<>* node);
	~UserInterfaceType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	MenuType*  menu;
	std::vector<SlotType* > slot;
	std::string scriptsFile;
	std::string lookAndFeel;

};
#endif
