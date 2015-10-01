#ifndef __SOUNDSYSTEM_H_
#define __SOUNDSYSTEM_H_

#include <unordered_map>
#include <SFML/Audio.hpp>
#include <ChibiEngine/Game.h>

namespace game{

    class Logger;
    class SoundSystem : private UniqueClass {
    public:
        SoundSystem();

        int createMusic(const std::string& name, bool infinite);
        int createSound(const std::string& name);

        sf::Music& getMusic(int id);
        sf::Sound& getSound(int id);

        void stopAllSounds();
    private:
        int lastMusicId;
        int lastSoundId;
        Logger logger;
        std::unordered_map<int, sf::Music> musicReference;
        std::unordered_map<std::string, sf::SoundBuffer> soundBufferReference;
        std::unordered_map<int, sf::Sound> soundReference;

        sf::SoundBuffer& getSoundBuffer(const std::string& name);
    };

}

#endif
