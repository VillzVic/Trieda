#ifndef OFBASE_H
#define OFBASE_H

#include "TRIEDA-InputXSD.h"
#include "GGroup.h"
#include <iostream>

/* O XSD Code Synthesis gera uma estrutura de dados para listas de
   objetos que � um pouco estranha e contra-intuitiva. A macro abaixo
   gera uma itera��o por ela que pode ser representada por um c�digo
   mais leg�vel e intuitivo, baseada nos padr�es de projeto escolhidos */
#ifndef ITERA_SEQ
#define ITERA_SEQ(it,addr,type) for (Grupo##type##::##type##_iterator it = \
   (addr).##type##().begin(); it != (addr).##type##().end(); ++it) 
#endif

class OFBase
{
public:
   OFBase(void);
   ~OFBase(void);
   virtual int getId() const { return id; }
   virtual void setId(int id) { this->id = id; }
   virtual void le_arvore(::xml_schema::type& raiz) { };// = 0;
   virtual bool operator < (const OFBase& right) 
   { 
      return (getId() < right.getId()); 
   }
   virtual bool operator == (const OFBase& right) { 
      return (getId() == right.getId()); 
   }

//protected:
   int id;

   }; 

#endif