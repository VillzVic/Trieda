#ifndef NSITERATOR_HPP_
#define NSITERATOR_HPP_

#include "Move.hpp"

using namespace std;

class IteratorOutOfBound
{
private:
	int id;
public:
	IteratorOutOfBound(int _id) :
		id(_id)
	{
	}

	int getId()
	{
		return id;
	}
};

class NSIterator
{
public:
	virtual ~NSIterator()
	{
	}

	virtual void first() = 0;
	virtual void next() = 0;
	virtual bool isDone() = 0;
	virtual Move & current() = 0;
};

#endif /*NSITERATOR_HPP_*/
