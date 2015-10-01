#ifndef __AI_CHARACTER_H_
#define __AI_CHARACTER_H_

#include <Character.h>

class b2Fixture;
namespace game{

    class CharacterSettings;
    class AICharacter : public Character {
    public:
        AICharacter(const CharacterSettings &settings, glm::vec2 startPosition, Layer* layer, int uid);

        virtual void BeginContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void EndContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void draw(CameraCHandle camera, const Color& ambientLight);
        virtual void hitted(float health);

        virtual void requestDelete(bool instant) override;

    protected:
        virtual ~AICharacter();
    private:

        struct Sensor{
            Sensor(AICharacter* parent,
                    const glm::vec2& area,
                    const Color& color,
                    const std::string& beginScript,
                    const std::string& endScript);

            void beginContact(b2Fixture *contactedFixture, b2Fixture *other);
            void endContact(b2Fixture *contactedFixture, b2Fixture *other);

            void draw(CameraCHandle camera);

        private:
            void callAiScript(const std::string& name, b2Fixture *contactedFixture, b2Fixture *other);

            glm::vec2 area;
            Color color;
            int contactedFixtures;
            std::string beginScript, endScript;
            AICharacter* parent;
            b2Fixture* fix;
        };

        bool markedDeleted = false;
        Sensor eye, contact;
    };
}


#endif
