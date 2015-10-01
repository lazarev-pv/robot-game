#ifndef __CORE_UTILS_H
#define	 __CORE_UTILS_H
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <type_traits>
#include <utility>


namespace game{

    template<class T> void remove(std::vector<T>* container, const T& val){
        container->erase(std::remove(container->begin(), container->end(), val), container->end());
    }

    /* Получает из контейнера элемент, если ничего не нешел, возвращает 0 */
    template<class T, class K> T get(const K& key, const std::map<K, T> &container){
        typename std::map< K, T >::const_iterator it = container.find(key);
        return it!=container.end()?it->second:0;
    }

    template<class T, class K> T* get(const K& key, const std::map<K, T*> &container){
        typename std::map<K, T* >::const_iterator it = container.find(key);
        return it!=container.end()?it->second:0;
    }

    template<class T> inline void deleteAll(std::map<std::string, T*> &container){
        for(typename std::map<std::string, T*>::iterator it = container.begin();
                it!=container.end(); ++it){
            if(it->second != nullptr)
                delete it->second;
            it->second = nullptr;
        }
        container.clear();
    }

    template<class T> inline void deleteAll(std::multiset<T*> &container){
    	for(typename std::vector<T*>::iterator it = container.begin();
				it!=container.end(); ++it){
			if(*it != nullptr)
				delete *it;
			*it=nullptr;
		}
		container.clear();
	}


    template<class T> inline void deleteAll(std::vector<T*> &container){
        for(typename std::vector<T*>::iterator it = container.begin();
                it!=container.end(); ++it){
            if(*it != nullptr)
                delete *it;
            *it=nullptr;
        }
        container.clear();
    }

    template<class T> inline void deleteAll(std::list<T*> &container){
        for(auto it = container.begin();
                it!=container.end(); ++it){
            if(*it != nullptr)
                delete *it;
            *it=nullptr;
        }
        container.clear();
    }




}
#endif	

