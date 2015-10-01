
#ifndef EDITORLEVEL_H_
#define EDITORLEVEL_H_

#include <set>
#include <map>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include <ChibiEngine/GameState/Level.h>
#include <ChibiEngine/Log/Logger.h>

#include <EditorInputListener.h>
#include <ChibiEngine/Common/SortedList.h>
#include <Grid.h>
#include <ChibiEngine/Resource/LevelResources.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <EditorLevelImpl/EditorInputContainers.h>
#include <EditorLevelImpl/EditorGeometryObject.h>

class LinkType;
class RobotCameraSettings;
class SpriteLinkType;
namespace game{

    class Logger;
    class EditorLevel;
    class CommandsHistory;
    class EditorContextMenu;
    class PropertyBar;
    class OperableObject;
    class EditorCommand;
    class Camera;
    class PhysicsSystem;
    class MouseMoveStrategyReference;
    class TexturedObject;
    class UIElement;
    class Dialog;
    class ImagePicker;
    class Layer;
    class EditorPres3D;
    class Texture;
    class FXObject;
    class EventObject;
    class SpotLightObject;
    class EditorPres2D;
    class GeometryLineChain;
    class NamedPointObject;
    class LightObject;
    class DirLightObject;
    class EnemyObject;
    class EditorGeomJoint;

    class EditorLevel : public Level {
    public:
        EditorLevel(const std::string& worldFile);
        ~EditorLevel();

        static EditorLevel *instance();

        void draw(CameraCHandle camera) override;
        void save();
        bool selectObject(const EditorInputListener::MouseRay& ray, bool selectNext);
        void viewContextMenu(const glm::vec2& pnt);
        void endDraging();
        void moveCamera(const glm::vec2& delta);
        void zoomCamera(float delta);
        void hideMenus();
        inline const Logger& getLogger() const;

        // Select Object Group
        void selectPres2D( EditorPres2D* pres2D);
        void selectPres3D( EditorPres3D * pres3D);
        void selectGeomChain( GeometryLineChain* geomChain);
        void selectSpotLight( SpotLightObject* spotLight);
        void selectDirLight( DirLightObject* dirLight);
        void selectFxObject( FXObject * fxObject);
        void selectEvent( EventObject* event);
        void selectEnemy( EnemyObject* enemy);
        void selectPoint( NamedPointObject* enemy);
        void selectGeomObject( EditorGeometryObject* object );
        void selectFixtureObject( EditorFixtureObject* object );
        void selectGeomJoint( EditorGeomJoint* geomJoint);


        void add(EditorPres3D * box);
        void add(EventObject* box);
        void add(FXObject * fx);
        void add(EditorGeometryObject* obj);
        void add2Others(OperableObject* obj);

        /** if has selected is object - delete it,
        * if object's presentation - remove pres. from object,
        * if standalone pres. - jus remove it*/
        void deleteObject();
        void addChain(const glm::vec2& pos);
        void setZOrder(int zOrder);
        void moveObject(const glm::vec3& delta, bool silent=false);
        void moveObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);
        void scaleObject(const glm::vec2 newScale, bool silent=false);
        void scaleObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);
        void rotateObject(float newAngle, bool silent=false);
        void rotateObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);
        void verSpreadObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);
        void horSpreadObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);
        void setTexels(const glm::vec2& tex);

        void onMouseDrag(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin);

        void undo();
        void redo();
        void markForCopy();
        void paste();

        inline const glm::vec2 getMouseCoordsInZeroPlane() const;
        inline ButtonContextMenu* getObjectMenu();
        inline ButtonContextMenu* getPresMenu();
        inline ButtonContextMenu* getEditorMenu();
        inline const Color& getAmbientLight();
        void add2Pres(EditorPres2D* obj);

        void setSelection(const std::string& name, bool selectable);
        bool isSelectable(const std::string& name);
    private:
        //Callbacks
        void setCameraConstrained(const std::string& name);
        void onAddGeometry(const std::string& name);
        void onAddLight(const std::string& name);
        void onAddFX(const std::string& name);
        void onContextMenu(const std::string& name);
        void onImageSelect(const std::string& );
        void onMenuFile(const std::string &name);

        void onDayColor(const std::string& name);
        void onNightColor(const std::string& name);
        void onDaytime(const std::string& name);
        void onMenuWeather(const std::string& name);
        void onMenuLayers(const std::string& name);

        void onAddPres2GeomObject(const std::string& name);
        void onAddFixture2GeomObject(const std::string& name);

        //Utils
        void invalidateObject();
        OperableObject* findObject(const EditorInputListener::MouseRay& ray, bool selectNext);
        void grabObject(OperableObject* object, const glm::vec2& pos);
        void calcAmbLight();
        void initUIValues();
        void addLayerRecordInTable(Layer* newLayer);
        void removeLayerRecordInTable(Layer* l);

        void load();
        void loadSavedSettings();
        void saveSettings();

        //Imagepicker callbacks
        void addPresent();
        void addBox();
        void addPresent2GeomObj();

        struct ObjCompare {
            bool operator()(OperableObject* const lhs, OperableObject* const rhs) const;
        };

        std::map<std::string,Layer*> layersIndex;
        std::vector<Layer*> layers;
        Layer* activeLayer;

        Logger log;
        EditorInputListener inputListener;
        PrimitiveDrawer drawer;
        OperableObject* object4Copy;

        std::string worldFile;
        CommandsHistory* history;
        bool cameraConstrained;

        Pres3DInfo pres3DInfo;
        Pres2DInfo pres2DInfo;
        GeomChainInfo geomChainInfo;
        GeomObjectInfo geomObjectInfo;
        GeomFixtureInfo geomFixtureInfo;
        GeomJointInfo geomJointInfo;
        SpotLightInfo spotLightInfo;
        DirLightInfo dirLightInfo;
        FxObjectInfo fxObjectInfo;
        EventInfo eventInfo;
        EnemyInfo enemyInfo;
        PointInfo pointInfo;

        glm::vec2 imgDialogInvokeCoords;
        EditorFixtureObject* targetFixtureObject;
        Grid grid;

        // weather
        Color ambientLight, dayLight, nightLight;
        float daytime;

        OperableObject* selectedObject;

        //UI ELEMENTS
        ButtonContextMenu* contextMenu;
        ButtonContextMenu* objectMenu;
        ButtonContextMenu* presMenu;
        UIElement* dialog;
        ImagePicker* imagePicker;
        std::vector<OperableObject *> othersObjects;
        LevelResources levelResources;

        std::function<void()> onSpriteSelectCallback;

        struct CamSettings{
            CamSettings();
            CamSettings(RobotCameraSettings* iter);
            RobotCameraSettings* create();
            glm::vec3 pos;
            glm::vec4 rec;
        };

        CamSettings camSettings;
        std::unordered_map<std::string,bool> selectRef;
    };

    inline const Logger& EditorLevel::getLogger() const{
        return log;
    }

    inline const glm::vec2 EditorLevel::getMouseCoordsInZeroPlane() const{
        return inputListener.getWorldCoordinatesZeroPlane();
    }

    inline ButtonContextMenu* EditorLevel::getObjectMenu(){
        return objectMenu;
    }

    inline ButtonContextMenu* EditorLevel::getPresMenu(){
        return presMenu;
    }

    inline ButtonContextMenu* EditorLevel::getEditorMenu(){
        return contextMenu;
    }

    inline const Color& EditorLevel::getAmbientLight(){
        return ambientLight;
    }
}


#endif
