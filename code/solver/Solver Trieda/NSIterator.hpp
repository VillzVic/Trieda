#ifndef _NS_ITERATOR_HPP_
#define _NS_ITERATOR_HPP_

#include "Move.hpp"

class IteratorOutOfBound
{
private:
	int id;
public:
	IteratorOutOfBound(int _id) :
		id(_id)
	{
	}

	int getId() { return id; }
};

class NSIterator
{
public:
	virtual ~NSIterator() { }

	virtual void first() = 0;
	virtual void next() = 0;
	virtual bool isDone() = 0;
	virtual Move & current() = 0;
};

#endif // _NS_ITERATOR_HPP_
