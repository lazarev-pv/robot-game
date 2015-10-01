#ifndef __CHEDLEVEL_H_
#define __CHEDLEVEL_H_

#include <ChibiEngine/GameState/Level.h>
#include <map>
#include <string>
#include <Grid.h>
#include <ChibiEngine/Resource/LevelResources.h>
#include <ChibiEngine/Clock/ClockHandles.h>
#include <ChedConstants.h>
#include <memory>
#include <InstrumentHelper.h>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include "ChedBone.h"

namespace game{

    class BoneContainer;
    class PicContainer;
    class BoxContainer;

    class DropDownListBox;
    class Logger;
    class ChedSlot;
	class ChedBone;
	class AnimationControl;
	class ChedObject;
	class ChedInputListener;
	class ButtonContextMenu;
	class ImagePicker;
	class PatternSprite;
	class InputField;
	class Animation;
	class ChedPhysBox;
    class Button;
    class Checkbox;
	class ChedLevel : public Level {
	public:
		 ChedLevel(const std::string& worldFile);
		 ~ChedLevel();

        static ChedLevel *instance();

		 void draw(CameraCHandle camera) override;

		 void createBone(const glm::vec2& beg, const glm::vec2& end, const std::string& name, ChedBone* parent);
		 void selectBone(const glm::vec2& pnt);
		 void createPhysBox(const glm::vec2& pos, float hw, float hh);
		 void addPresentation();
		 void deleteBone();
		 void hover(const glm::vec2& pnt);
		 void viewContextMenu(const glm::vec2& pnt);
		 void hideMenus();
		 void mouseDrag(const glm::vec2& delta, const glm::vec2& grabPnt, const glm::vec2& currPnt);
        void mouseClickUp();
		 void save();
        void showInputNameDialog(const std::string title, const std::function<void(const std::string&)>& callback);

        void setCurrentTransformation(TransformId transform);

        void moveCamera(const glm::vec2& delta);
        void zoomCamera(float delta);
        void reSort();
        void reName(const std::string& newName);

        AnimationControl *getAnimationControl();

        inline ChedInputListener* getInputListener();
        inline InstrumentHelper* getInstrumentHelper();

        inline ChedBone* findBoneById(size_t id);
        inline Skeleton* getSkeleton();
        inline ChedBone* getCurrentBone() const;
        inline bool isDebugMode() const;
        inline void toggleDebugMode();
        std::vector<ChedSlot*> findSlotByBone(Animation::BoneId boneId);
        void refreshAllObjects();
	private:
        enum class Mode{CHAR, ANIM};

        friend class ChedInputListener;

        void onMenuFile(const std::string& name);
        void onMenuAnimation(const std::string& name);
        void onMenuSlot(const std::string& name);
        void onMenuView(const std::string& name);

        void onSelectPresent(const std::string& name);
        void onSelectSlotPresent(const std::string& name);
        void onSelectSlot(const std::string& name);

        void onSelectAnimation(const std::string& name);
        void onSelectAnimName(const std::string& name);
        void setFirstAnimation();
        void onSelectWorkTarget(const std::string& name);

        ChedObject* findObject(const glm::vec2& pnt);
        void reselect(ChedObject* bone);
        void load();
        void loadSavedSettings(ClockHandle clock);
        void saveSetting();
        void reSortBones();
        void reindexBones();

        // bones and indices
        std::vector<std::unique_ptr<ChedBone>> bones;
        std::map<std::string, ChedBone *> indexedBones;
        std::unordered_map<size_t, ChedBone *> indexedByIdBones;

        // slots
        std::vector<std::unique_ptr<ChedSlot>> slots;

        // boxes
        std::vector<std::unique_ptr<ChedPhysBox>> boxes;

        std::unique_ptr<ChedInputListener> inputListener;
        ChedObject *hovered, *selected;
        ChedBone* currentBone = nullptr;
        std::string action;
        Mode mode;
        std::string worldFile;
        bool drawBones, drawPics, drawBoxes;
        bool selectBones, selectPics, selectBoxes;

        PicContainer* picContainer;
        BoneContainer* boneContainer;
        BoxContainer* boxContainer;

        Grid grid;

        Logger log;
        ImagePicker *imagePicker;
        DropDownListBox *animMenu;

        AnimationControl *animationControl;
        std::string currentAnimationName = "";
        //Animation *animation;
        //std::map<std::string, Animation*> animationRef;

        LevelResources levelResources;
        InstrumentHelper instrumentHelper;

        std::function<void(const std::string&)> currentInputNameCallback;
        std::unique_ptr<Skeleton> skeleton = nullptr;
        size_t slotMaxId = -1;
        bool debugMode = false;
    };


    inline ChedInputListener* ChedLevel::getInputListener(){
        return inputListener.get();
    }

    inline ChedBone * ChedLevel::findBoneById(size_t id){
        auto i = indexedByIdBones.find(id);
        return i!=indexedByIdBones.end()?i->second:nullptr;
    }

    inline InstrumentHelper* ChedLevel::getInstrumentHelper(){
        return &instrumentHelper;
    }

    inline Skeleton* ChedLevel::getSkeleton(){
        return skeleton.get();
    }

    inline ChedBone* ChedLevel::getCurrentBone() const{
        return currentBone;
    }

    inline bool ChedLevel::isDebugMode() const{
        return debugMode;
    }

    inline void ChedLevel::toggleDebugMode(){
        debugMode = !debugMode;
    }
}




#endif
