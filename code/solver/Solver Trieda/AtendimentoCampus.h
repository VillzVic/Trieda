#ifndef _ATENDIMENTO_CAMPUS_H_
#define _ATENDIMENTO_CAMPUS_H_

#include "ofbase.h"
#include "Campus.h"

class AtendimentoUnidade;

class AtendimentoCampus:
   public OFBase
{
public:
   AtendimentoCampus( int id );
   AtendimentoCampus( void );
   virtual ~AtendimentoCampus( void );

   GGroup< AtendimentoUnidade*, LessPtr<AtendimentoUnidade> > * atendimentos_unidades;
   Campus * campus;

   void setCampusId( std::string s ) { this->campus_id = s; }
   std::string getCampusId() const { if(campus != NULL) return campus->getCodigo(); else return this->campus_id; }

	// procura o atendimento unidade com um determinado id. se não encontra cria um novo e retorna-o
    AtendimentoUnidade* getAddAtendUnidade (int id);


private:
   std::string campus_id;
};

std::ostream& operator << ( std::ostream &, AtendimentoCampus & );

std::istream& operator >> ( std::istream &file, AtendimentoCampus* const &ptrAtendCampus);

#endif
