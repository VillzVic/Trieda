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

   // Variáveis
   // Informa o credito ao qual corresponde o IS_Atendimento_Tatico em questão.
   int credito;

   // Conjunto de demandas de ofertas diferentes, mas que são compatíveis.
   std::map< Demanda *, int /*Fração da demanda que está sendo considerada*/ > demandas;

   int turma;

   // Demanda total atendida
   int demanda_Atendida;

   // Indica se o crédito em questão está alocado ou não
   // bool credito_Alocado;

   // Operadores
   bool operator < ( IS_Atendimento_Tatico const & );
   bool operator == ( IS_Atendimento_Tatico const & );
};

#endif