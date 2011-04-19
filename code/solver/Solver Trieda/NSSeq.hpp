#ifndef NSSEQ_HPP_
#define NSSEQ_HPP_

#include "NS.hpp"
#include "NSIterator.hpp"

using namespace std;

class NSSeq: public NS
{
public:

	using NS::move; // prevents name hiding

   NSSeq()
   {
   }

	virtual ~NSSeq()
	{
	}

	virtual Move & move(const SolucaoOperacional& s) = 0;

   virtual NSIterator& getIterator(const SolucaoOperacional & s) = 0;

	virtual void print() = 0;
};

#endif /*NSSEQ_HPP_*/
