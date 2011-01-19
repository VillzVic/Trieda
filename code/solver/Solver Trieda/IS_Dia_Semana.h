#pragma once

#include <iostream>

#include <vector>

using namespace std;

#include "CreditoDisponivel.h"

#include "IS_Atendimento_Tatico.h"

class IS_Dia_Semana
{
public:
   IS_Dia_Semana(CreditoDisponivel * cred_Disp);//, GGroup<IS_Atendimento_Tatico*> & is_At_Tat);

   IS_Dia_Semana(IS_Dia_Semana const & is_Dia_Sem);

   virtual ~IS_Dia_Semana(void);

   // Variáveis

   CreditoDisponivel * creditos_Disponiveis;

   // Informa quantos creditos ainda podem ser alocados para o dia em questão
   int creditos_Livres;

   GGroup<IS_Atendimento_Tatico*> is_Atendimentos_Taticos;

   // Métodos
   //void aloca(
   //   int turma,
   //   //pair<Disciplina*,int> & cjt_Dem,
   //   vector<pair<Demanda*,int/*Demanda atendida*/> > & demandas_A_Alocar,
   //   int demanda_Atendida,
   //   int num_Creds);

   void aloca(
      int turma,
      Demanda * ref_Demanda_Alocda, // Referência para a Demanda que está sendo atendida (parcialmente ou totalmente)
      int demanda_Atendida, // Indica a quantidade da demanda que está sendo atendida
      int num_Creds // total de créditos a serem alocados no dia em questão
      );

   // Operadores

   bool operator < (IS_Dia_Semana const & right);
   
   bool operator == (IS_Dia_Semana const & right);
};
