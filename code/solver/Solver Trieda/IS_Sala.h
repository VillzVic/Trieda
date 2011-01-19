#pragma once

#include "Sala.h"

#include <vector>
#include <map>

#include "Demanda.h"

#include "Disciplina.h"

#include "Oferta.h"

using namespace std;

#include "IS_Dia_Semana.h"

class IS_Sala : public OFBase
{
public:
   IS_Sala(Sala * _sala);

   IS_Sala(IS_Sala const & is_sala);

   virtual ~IS_Sala(void);

   Sala * sala;

   GGroup<IS_Dia_Semana*> is_Dia_Semana;

   //void aloca(
   //   int turma,
   //   //pair<Disciplina*,int> & cjt_Dem,
   //   vector<pair<Demanda*,int/*Demanda atendida*/> > & demandas_A_Alocar,
   //   int demanda_Atendida,
   //   vector<pair<int,int> > & regra_De_Credito);


   void aloca(
      int turma,
      Demanda * ref_Demanda_Alocda, // Referência para a Demanda que está sendo atendida (parcialmente ou totalmente)
      int demanda_Atendida, // Indica a quantidade da demanda que está sendo atendida
      vector<pair<int/*dia*/,int/*num creds*/> > & regra_De_Credito
      );



   bool regraValida(vector<pair<int/*dia*/, int/*numCreditos*/> > & regra);
};
