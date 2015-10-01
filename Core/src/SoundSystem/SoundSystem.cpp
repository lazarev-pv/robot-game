#include <ChibiEngine/SoundSystem/SoundSystem.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;
using namespace sf;

SoundSystem::SoundSystem():
        lastMusicId(-1),
        lastSoundId(-1),
        logger(Game::getLoggingSystem()->createLogger("SoundSystem")){
}

sf::SoundBuffer& SoundSystem::getSoundBuffer(const std::string& name){
    auto sbI = soundBufferReference.find(name);
    if(sbI==soundBufferReference.end()){

        soundBufferReference.emplace(std::piecewise_construct,
                std::forward_as_tuple(name),
                std::forward_as_tuple());
        sf::SoundBuffer& sb = soundBufferReference.find(name)->second;

        string fullName = Game::getFileSystem()->getFullName(name);
        if(!sb.loadFromFile(fullName)){
            logger.error("Failed loading sound " + fullName);
        }

        return sb;
    }
    return sbI->second;
}

int SoundSystem::createSound(const std::string& name){
    lastSoundId++;
    soundReference.emplace(std::piecewise_construct,
            std::forward_as_tuple(lastSoundId),
            std::forward_as_tuple(getSoundBuffer(name)));
    return lastSoundId;
}

int SoundSystem::createMusic(const std::string& name, bool infinite){
    lastMusicId++;
    musicReference.emplace(std::piecewise_construct,
            std::forward_as_tuple(lastMusicId),
            std::forward_as_tuple());
    Music& m = musicReference.find(lastMusicId)->second;

    m.setLoop(infinite);
    string fullName = Game::getFileSystem()->getFullName(name);
    if(!m.openFromFile(fullName)){
        logger.error("Failed loading music " + fullName);
        return -1;
    }

    return lastMusicId;
}

sf::Music& SoundSystem::getMusic(int id){
    auto i = musicReference.find(id);
    if(i==musicReference.end()){
        logger.error("Music with id %i not found", id);
        return musicReference.begin()->second;
    }

    return i->second;
}

void SoundSystem::stopAllSounds(){
    for(auto& i : soundReference){
        i.second.stop();
    }
}

sf::Sound& SoundSystem::getSound(int id){
    auto i = soundReference.find(id);
    if(i==soundReference.end()){
        logger.error("Sound with id %i not found", id);
        return soundReference.begin()->second;
    }

    return i->second;
}
