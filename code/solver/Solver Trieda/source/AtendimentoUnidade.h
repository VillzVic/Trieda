#ifndef _ATENDIMENTO_UNIDADE_H_
#define _ATENDIMENTO_UNIDADE_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "Unidade.h"

class AtendimentoSala;

class AtendimentoUnidade :
   public OFBase
{
public:
   AtendimentoUnidade( int );
   AtendimentoUnidade( void );
   virtual ~AtendimentoUnidade( void );

   Unidade * unidade;
   GGroup< AtendimentoSala* , LessPtr<AtendimentoSala>> * atendimentos_salas;

   void setCodigoUnidade( std::string value ) { codigo_unidade = value; }
   std::string getCodigoUnidade() const { return codigo_unidade; }

   // procura o atendimento sala com um determinado id. se não encontra cria um novo e retorna-o
    AtendimentoSala* getAddAtendSala (int id);

private:
   std::string codigo_unidade;
};

std::ostream & operator << ( std::ostream &, AtendimentoUnidade & );

std::istream& operator >> ( std::istream &file, AtendimentoUnidade* const &ptrAtendUnidade);

#endif