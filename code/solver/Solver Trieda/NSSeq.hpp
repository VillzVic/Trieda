#ifndef _NS_SEQ_HPP_
#define _NS_SEQ_HPP_

#include "NS.hpp"
#include "NSIterator.hpp"

class NSSeq: public NS
{
public:

   using NS::move; // prevents name hiding

   NSSeq() { }

   virtual ~NSSeq() { }

   virtual Move & move(SolucaoOperacional &) = 0;

   virtual NSIterator& getIterator(SolucaoOperacional &) = 0;

   virtual void print() = 0;

};

#endif //_NS_SEQ_HPP_
