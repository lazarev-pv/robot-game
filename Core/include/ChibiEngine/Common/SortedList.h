#ifndef __SORTEDLIST_H_
#define __SORTEDLIST_H_

#include <list>

namespace game{


template<class Type, class Compare>
class SortedList{
public:
	typedef typename std::list<Type>::iterator iterator;
	typedef typename std::list<Type>::reverse_iterator reverse_iterator;


	SortedList(){};

	void add(Type el){
		list.push_back(el);
		list.sort(Compare());
	}

	void remove(Type el){
		list.remove(el);
	}

	iterator begin(){
		return list.begin();
	}

	iterator end(){
		return list.end();
	}

	reverse_iterator rbegin(){
		return list.rbegin();
	}

	reverse_iterator rend(){
		return list.rend();
	}

	void clear(){
		list.clear();
	}
private:
	std::list<Type> list;
};

}



#endif
