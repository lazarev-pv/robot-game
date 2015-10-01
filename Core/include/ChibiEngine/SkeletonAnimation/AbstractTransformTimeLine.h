#ifndef __CHIBIENGINE_SKELETON_ANIMATION_ABSTRACT_TRANSFORM_TIMELINE_
#define __CHIBIENGINE_SKELETON_ANIMATION_ABSTRACT_TRANSFORM_TIMELINE_

#include <ChibiEngine/Game.h>

#include <vector>
#include <algorithm>

namespace game{

    template<typename TransType, typename ValueType, typename XMLType, ValueType const& defaultValue>
    class AbstractTransformTimeLine{
    public:
        using TransformIterator = typename std::vector<TransType>::iterator;

        AbstractTransformTimeLine() = default;
        AbstractTransformTimeLine(const std::vector<XMLType*>& xmlData){
            for(auto elem : xmlData){
                transforms.emplace_back(elem);
            }
            sortTransitions();
        }

        void save(std::vector<XMLType*>& dest) const{
            for(const auto& elem:transforms){
                dest.push_back(elem.save());
            }
        }

        TransformIterator find(float time){
            int compTime = static_cast<int>(time*100);
            auto it = transforms.begin();
            while(it!= transforms.end()){
                if(static_cast<int>(it->time*100)==compTime)
                    return it;
                ++it;
            }
            return it;
        }

        inline bool isEmpty() const{
            return transforms.size()==0;
        }

        void moveKey(float oldTime, float newTime){
            auto value = deleteKey(oldTime);
            auto tranIt = find(newTime);
            if(tranIt==transforms.end()) {
                transforms.emplace_back(value,newTime);
                sortTransitions();
            }else{
                tranIt->value = value;
            }
        }

        void copyKey(float oldTime, float newTime){
            auto tranIt = find(oldTime);
            if(tranIt==transforms.end()){
                Game::getLogger().warn("copyKey - Transform with time [%i] not found",time);
                return;
            }
            ValueType tmp = tranIt->value;

            auto newTranIt = find(newTime);
            if(newTranIt==transforms.end()) {
                transforms.emplace_back(tmp,newTime);
                sortTransitions();
            }else{
                newTranIt->value = tmp;
            }
        }

        ValueType deleteKey(float time){
            auto tranIt = find(time);
            if(tranIt==transforms.end()){
                Game::getLogger().warn("deleteKey with time [%f] not found",time);
                return defaultValue;
            }
            ValueType tmp = tranIt->value;
            transforms.erase(tranIt);
            return tmp;
        }

        void sortTransitions(){
            std::sort(transforms.begin(), transforms.end(),
                    [](const TransType& o1, const TransType& o2){
                        return o1.time<o2.time;
                    });
        }


        std::vector<TransType> transforms;
    protected:
        void setKey(float time, const ValueType& value, const std::function<void(TransformIterator, const ValueType&)>& merge){
            auto tranIt = find(time);
            if(tranIt==transforms.end()) {
                transforms.emplace_back(value,time);
                sortTransitions();
            }else{
                merge(tranIt, value);
            }
        }
    };


}

#endif