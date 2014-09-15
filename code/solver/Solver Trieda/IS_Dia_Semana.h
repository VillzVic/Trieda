#ifndef _IS_DIA_SEMANA_H_
#define _IS_DIA_SEMANA_H_

#include <iostream>
#include <vector>

#include "CreditoDisponivel.h"
#include "IS_Atendimento_Tatico.h"

class IS_Dia_Semana
{
public:
   IS_Dia_Semana( CreditoDisponivel * );
   IS_Dia_Semana( IS_Dia_Semana const & );
   virtual ~IS_Dia_Semana( void );

   // Variáveis
   CreditoDisponivel * creditos_Disponiveis;

   // Informa quantos creditos ainda podem ser alocados para o dia em questão
   int creditos_Livres;

   GGroup< IS_Atendimento_Tatico * > is_Atendimentos_Taticos;

   void aloca( int,
      Demanda *, /* Referência para a Demanda que está sendo atendida (parcialmente ou totalmente)*/
      int, /* Indica a quantidade da demanda que está sendo atendida*/
      int /* total de créditos a serem alocados no dia em questão*/ );

   // Operadores
   bool operator < ( IS_Dia_Semana const & );
   bool operator == ( IS_Dia_Semana const & );
};

#endif