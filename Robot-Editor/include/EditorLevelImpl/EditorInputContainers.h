#ifndef __EDITOR_INPUT_CONTAINERS_H_
#define __EDITOR_INPUT_CONTAINERS_H_

#include <ChibiEngine/UserInterface/FieldContainer.h>
#include "EditorGeometryObject.h"

namespace game{

    class EditorPres2D;
    class EditorPres3D;
    class GeometryLineChain;
    class SpotLightObject;
    class DirLightObject;
    class FXObject;
    class EventObject;
    class EnemyObject;
    class EditorLevel;
    class NamedPointObject;
    class EditorGeomJoint;
    class Pres2DInfo : public FieldsContainer<EditorPres2D, EditorLevel>{
    public:
        Pres2DInfo(EditorLevel* level);
        virtual ~Pres2DInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class Pres3DInfo : public FieldsContainer<EditorPres3D, EditorLevel>{
    public:
        Pres3DInfo(EditorLevel* level);
        virtual ~Pres3DInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class GeomChainInfo : public FieldsContainer<GeometryLineChain, EditorLevel>{
    public:
        GeomChainInfo(EditorLevel* level);
        virtual ~GeomChainInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class GeomObjectInfo : public FieldsContainer<EditorGeometryObject, EditorLevel>{
    public:
        GeomObjectInfo(EditorLevel* level);
        virtual ~GeomObjectInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class GeomFixtureInfo : public FieldsContainer<EditorFixtureObject, EditorLevel>{
    public:
        GeomFixtureInfo(EditorLevel* level);
        virtual ~GeomFixtureInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class GeomJointInfo : public FieldsContainer<EditorGeomJoint, EditorLevel>{
    public:
        GeomJointInfo(EditorLevel* level);
        virtual ~GeomJointInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class SpotLightInfo : public FieldsContainer<SpotLightObject, EditorLevel>{
    public:
        SpotLightInfo(EditorLevel* level);
        virtual ~SpotLightInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class DirLightInfo : public FieldsContainer<DirLightObject, EditorLevel>{
    public:
        DirLightInfo(EditorLevel* level);
        virtual ~DirLightInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class FxObjectInfo : public FieldsContainer<FXObject, EditorLevel>{
    public:
        FxObjectInfo(EditorLevel* level);
        virtual ~FxObjectInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class EventInfo : public FieldsContainer<EventObject, EditorLevel>{
    public:
        EventInfo(EditorLevel* level);
        virtual ~EventInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class EnemyInfo : public FieldsContainer<EnemyObject, EditorLevel>{
    public:
        EnemyInfo(EditorLevel* level);
        virtual ~EnemyInfo();
        virtual void updateObject();
        virtual void updateFields();
    };

    class PointInfo : public FieldsContainer<NamedPointObject, EditorLevel>{
    public:
        PointInfo(EditorLevel* level);
        virtual ~PointInfo();
        virtual void updateObject();
        virtual void updateFields();
    };
}


#endif