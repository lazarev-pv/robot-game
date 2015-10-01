#ifndef __PROTAGONIST_H_
#define __PROTAGONIST_H_

#include <Character.h>

namespace game{
    // todo Скорее всего нафиг ненужнен - выпилить в марте
    class CharacterSettings;
    class Protagonist : public Character{
    public:
        Protagonist(const CharacterSettings &settings, glm::vec2 startPosition, const Category& category, Layer* layer, int uid);

        virtual void hitted(float health);

    protected:
        virtual ~Protagonist();
    };


}


#endif