#ifndef _IS_ATENDIMENTO_TATICO_H_
#define _IS_ATENDIMENTO_TATICO_H_

#include <iostream>
#include <map>

#include "Demanda.h"

class IS_Atendimento_Tatico
{
public:
   IS_Atendimento_Tatico( void );
   IS_Atendimento_Tatico( IS_Atendimento_Tatico const & );
   IS_Atendimento_Tatico( int );
   virtual ~IS_Atendimento_Tatico( void );

   // Vari�veis
   // Informa o credito ao qual corresponde o IS_Atendimento_Tatico em quest�o.
   int credito;

   // Conjunto de demandas de ofertas diferentes, mas que s�o compat�veis.
   std::map< Demanda *, int /*Fra��o da demanda que est� sendo considerada*/ > demandas;

   int turma;

   // Demanda total atendida
   int demanda_Atendida;

   // Indica se o cr�dito em quest�o est� alocado ou n�o
   // bool credito_Alocado;

   // Operadores
   bool operator < ( IS_Atendimento_Tatico const & );
   bool operator == ( IS_Atendimento_Tatico const & );
};

#endif