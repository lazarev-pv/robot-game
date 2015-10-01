#ifndef __SCHEMAS_LAF_XSD
#define __SCHEMAS_LAF_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include "CommonParser.h"
#include "ResourcesDescriptionParser.h"

// ----- DECLARAIONS ------ //
struct ButtonLAFType;
struct ButtonStateLAFType;
struct CaptionLAFType;
struct CheckboxLAFType;
struct DialogBackgroundLAFType;
struct DialogCaptureLAFType;
struct DialogsLAFType;
struct FontLAFType;
struct HealthbarLAFType;
struct ImagePickerLAFType;
struct InputFieldLAFType;
struct LabelsListLAFType;
struct LafParserRoot;
struct LookAndFeelType;
struct MenuLAFType;
struct PaddingType;
struct RGBSliderLAFType;
struct ScrollerLAFType;
struct SliderLAFType;
// ----- DEFINITIONS ------ //

// ----- ButtonLAFType
struct ButtonLAFType{
	ButtonLAFType();
	ButtonLAFType(ButtonLAFType&& other);
	ButtonLAFType(const ButtonLAFType& other) = delete;
	ButtonLAFType& operator=(const ButtonLAFType& other) = delete;
	ButtonLAFType(rapidxml::xml_node<>* node);
	~ButtonLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ButtonStateLAFType*  normal;
	ButtonStateLAFType*  hovered;
	ButtonStateLAFType*  pushed;
	PaddingType*  padding;
	float borderThickness;
	std::string animatedImage;

};

// ----- ButtonStateLAFType
struct ButtonStateLAFType{
	ButtonStateLAFType();
	ButtonStateLAFType(ButtonStateLAFType&& other);
	ButtonStateLAFType(const ButtonStateLAFType& other) = delete;
	ButtonStateLAFType& operator=(const ButtonStateLAFType& other) = delete;
	ButtonStateLAFType(rapidxml::xml_node<>* node);
	~ButtonStateLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FontLAFType*  font;
	ColorType*  backColor;
	ColorType*  borderColor;
	std::string image;

};

// ----- CaptionLAFType
struct CaptionLAFType{
	CaptionLAFType();
	CaptionLAFType(CaptionLAFType&& other);
	CaptionLAFType(const CaptionLAFType& other) = delete;
	CaptionLAFType& operator=(const CaptionLAFType& other) = delete;
	CaptionLAFType(rapidxml::xml_node<>* node);
	~CaptionLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  color;
	ColorType*  textColor;
	float hh;

};

// ----- CheckboxLAFType
struct CheckboxLAFType{
	CheckboxLAFType();
	CheckboxLAFType(CheckboxLAFType&& other);
	CheckboxLAFType(const CheckboxLAFType& other) = delete;
	CheckboxLAFType& operator=(const CheckboxLAFType& other) = delete;
	CheckboxLAFType(rapidxml::xml_node<>* node);
	~CheckboxLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string normal_image;
	std::string checked_image;
	float hw;
	float hh;

};

// ----- DialogBackgroundLAFType
struct DialogBackgroundLAFType{
	DialogBackgroundLAFType();
	DialogBackgroundLAFType(DialogBackgroundLAFType&& other);
	DialogBackgroundLAFType(const DialogBackgroundLAFType& other) = delete;
	DialogBackgroundLAFType& operator=(const DialogBackgroundLAFType& other) = delete;
	DialogBackgroundLAFType(rapidxml::xml_node<>* node);
	~DialogBackgroundLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  backColor;
	ColorType*  borderColor;
	float borderThickness;

};

// ----- DialogCaptureLAFType
struct DialogCaptureLAFType{
	DialogCaptureLAFType();
	DialogCaptureLAFType(DialogCaptureLAFType&& other);
	DialogCaptureLAFType(const DialogCaptureLAFType& other) = delete;
	DialogCaptureLAFType& operator=(const DialogCaptureLAFType& other) = delete;
	DialogCaptureLAFType(rapidxml::xml_node<>* node);
	~DialogCaptureLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FontLAFType*  font;
	ColorType*  color;
	ButtonLAFType*  controlButtons;
	float height;
	float controlsWidth;

};

// ----- DialogsLAFType
struct DialogsLAFType{
	DialogsLAFType();
	DialogsLAFType(DialogsLAFType&& other);
	DialogsLAFType(const DialogsLAFType& other) = delete;
	DialogsLAFType& operator=(const DialogsLAFType& other) = delete;
	DialogsLAFType(rapidxml::xml_node<>* node);
	~DialogsLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	DialogCaptureLAFType*  capture;
	DialogBackgroundLAFType*  background;
	std::string image;

};

// ----- FontLAFType
struct FontLAFType{
	FontLAFType();
	FontLAFType(FontLAFType&& other);
	FontLAFType(const FontLAFType& other) = delete;
	FontLAFType& operator=(const FontLAFType& other) = delete;
	FontLAFType(rapidxml::xml_node<>* node);
	~FontLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  textColor;
	int size;
	std::string face;

};

// ----- HealthbarLAFType
struct HealthbarLAFType{
	HealthbarLAFType();
	HealthbarLAFType(HealthbarLAFType&& other);
	HealthbarLAFType(const HealthbarLAFType& other) = delete;
	HealthbarLAFType& operator=(const HealthbarLAFType& other) = delete;
	HealthbarLAFType(rapidxml::xml_node<>* node);
	~HealthbarLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float hh;
	int defaultPercision;
	std::string image;
	std::string backImage;
	float borderThickness;

};

// ----- ImagePickerLAFType
struct ImagePickerLAFType{
	ImagePickerLAFType();
	ImagePickerLAFType(ImagePickerLAFType&& other);
	ImagePickerLAFType(const ImagePickerLAFType& other) = delete;
	ImagePickerLAFType& operator=(const ImagePickerLAFType& other) = delete;
	ImagePickerLAFType(rapidxml::xml_node<>* node);
	~ImagePickerLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  labelsOffset;
	Point2D*  labelsSize;
	ColorType*  imageBorderColor;
	Point3D*  imageOffset;
	Point2D*  imageSize;
	Point2D*  baseSize;

};

// ----- InputFieldLAFType
struct InputFieldLAFType{
	InputFieldLAFType();
	InputFieldLAFType(InputFieldLAFType&& other);
	InputFieldLAFType(const InputFieldLAFType& other) = delete;
	InputFieldLAFType& operator=(const InputFieldLAFType& other) = delete;
	InputFieldLAFType(rapidxml::xml_node<>* node);
	~InputFieldLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FontLAFType*  font;
	ColorType*  backColor;
	ColorType*  disBackColor;
	ColorType*  selectColor;
	ColorType*  borderColor;
	ColorType*  selectedBorderColor;
	ColorType*  cursorColor;
	PaddingType*  padding;
	float borderThickness;
	float cursorHW;
	std::string image;
	std::string normal;
	std::string focused;

};

// ----- LabelsListLAFType
struct LabelsListLAFType{
	LabelsListLAFType();
	LabelsListLAFType(LabelsListLAFType&& other);
	LabelsListLAFType(const LabelsListLAFType& other) = delete;
	LabelsListLAFType& operator=(const LabelsListLAFType& other) = delete;
	LabelsListLAFType(rapidxml::xml_node<>* node);
	~LabelsListLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	PaddingType*  padding;
	ColorType*  normalTextColor;
	ColorType*  selectedTextColor;
	ColorType*  selectionColor;
	ColorType*  backgroundColor;
	ColorType*  borderColor;
	ScrollerLAFType*  scroller;
	int borderWidth;

};

// ----- LafParserRoot
struct LafParserRoot{
	LafParserRoot();
	LafParserRoot(LafParserRoot&& other);
	LafParserRoot(const LafParserRoot& other) = delete;
	LafParserRoot& operator=(const LafParserRoot& other) = delete;
	LafParserRoot(rapidxml::xml_node<>* node);
	~LafParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	LookAndFeelType*  lookAndFeel;

};

// ----- LookAndFeelType
struct LookAndFeelType{
	LookAndFeelType();
	LookAndFeelType(LookAndFeelType&& other);
	LookAndFeelType(const LookAndFeelType& other) = delete;
	LookAndFeelType& operator=(const LookAndFeelType& other) = delete;
	LookAndFeelType(rapidxml::xml_node<>* node);
	~LookAndFeelType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ResourceDescriptionList*  resources;
	ColorType*  defaultColor;
	FontLAFType*  font;
	ButtonLAFType*  buttons;
	DialogsLAFType*  dialogs;
	MenuLAFType*  menu;
	InputFieldLAFType*  inputField;
	CheckboxLAFType*  checkbox;
	SliderLAFType*  slider;
	RGBSliderLAFType*  rgbSlider;
	LabelsListLAFType*  labelsList;
	ImagePickerLAFType*  imagePicker;
	HealthbarLAFType*  healthbar;
	CaptionLAFType*  caption;

};

// ----- MenuLAFType
struct MenuLAFType{
	MenuLAFType();
	MenuLAFType(MenuLAFType&& other);
	MenuLAFType(const MenuLAFType& other) = delete;
	MenuLAFType& operator=(const MenuLAFType& other) = delete;
	MenuLAFType(rapidxml::xml_node<>* node);
	~MenuLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FontLAFType*  font;
	ColorType*  normalColor;
	ColorType*  hoveredColor;
	ColorType*  borderColor;
	PaddingType*  padding;
	float borderThickness;
	std::string image;
	std::string hoverImage;

};

// ----- PaddingType
struct PaddingType{
	PaddingType();
	PaddingType(PaddingType&& other);
	PaddingType(const PaddingType& other) = delete;
	PaddingType& operator=(const PaddingType& other) = delete;
	PaddingType(rapidxml::xml_node<>* node);
	~PaddingType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float left;
	float top;
	float right;
	float down;

};

// ----- RGBSliderLAFType
struct RGBSliderLAFType{
	RGBSliderLAFType();
	RGBSliderLAFType(RGBSliderLAFType&& other);
	RGBSliderLAFType(const RGBSliderLAFType& other) = delete;
	RGBSliderLAFType& operator=(const RGBSliderLAFType& other) = delete;
	RGBSliderLAFType(rapidxml::xml_node<>* node);
	~RGBSliderLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float hh;
	float colorHW;

};

// ----- ScrollerLAFType
struct ScrollerLAFType{
	ScrollerLAFType();
	ScrollerLAFType(ScrollerLAFType&& other);
	ScrollerLAFType(const ScrollerLAFType& other) = delete;
	ScrollerLAFType& operator=(const ScrollerLAFType& other) = delete;
	ScrollerLAFType(rapidxml::xml_node<>* node);
	~ScrollerLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  backColor;
	ColorType*  borderColor;
	int width;
	int height;
	int borderWidth;
	std::string hoveredImage;
	std::string image;

};

// ----- SliderLAFType
struct SliderLAFType{
	SliderLAFType();
	SliderLAFType(SliderLAFType&& other);
	SliderLAFType(const SliderLAFType& other) = delete;
	SliderLAFType& operator=(const SliderLAFType& other) = delete;
	SliderLAFType(rapidxml::xml_node<>* node);
	~SliderLAFType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ColorType*  recColor;
	ColorType*  markColor;
	ColorType*  texColor;
	ColorType*  hoveredColor;
	float baseHH;
	float hh;
	float markLen;
	int defaultPercision;

};
#endif
