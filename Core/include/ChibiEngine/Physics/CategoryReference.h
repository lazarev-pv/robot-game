#ifndef __CATEGORY_REFERENCE_H_
#define __CATEGORY_REFERENCE_H_

#include <unordered_map>
#include <string>
#include <ChibiEngine/Common/ClassUtils.h>
#include <Box2D/Box2D.h>

namespace game{

    struct Category{
        Category(uint16 category, uint16 mask, const std::string& name);
        Category();
        uint16 category,mask;
        std::string name;
    };

    class CategoryReference : private UniqueClass {
    public:
        static const std::string PROTAGONIST;
        static const std::string ENEMY;
        static const std::string DECORATION;
        static const std::string PHYS;
        static const std::string INTERACTIVE;
        static const std::string PROTAGONIST_WEAPON_SENSOR;
        static const std::string ENEMY_WEAPON_SENSOR;

        static const Category& get(const std::string &name);
    private:
        CategoryReference();
        std::unordered_map<std::string, Category> reference;

        static const Category nullCategory;
    };

}


#endif