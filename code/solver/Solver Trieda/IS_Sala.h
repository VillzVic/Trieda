#pragma once

#include "Sala.h"

#include <vector>
#include <map>

#include "Demanda.h"

#include "Disciplina.h"

using namespace std;

//#include "IS_Atendimento_Tatico.h"

#include "IS_Dia_Semana.h"

class IS_Sala : public OFBase
{
public:
   IS_Sala(Sala * _sala);
   //IS_Sala(vector<pair<int/*dias*/,int/*creditos livres*/> > cred_Livre_Dia_Let);

   IS_Sala(IS_Sala const & is_sala);

   virtual ~IS_Sala(void);

   Sala * sala;

   GGroup<IS_Dia_Semana*> is_Dia_Semana;

   void aloca(
      int turma,
      //GGroup<Demanda*> & cjt_Dem,
      pair<Disciplina*,int> & cjt_Dem,
      vector<pair<Demanda*,int/*Demanda atendida*/> > & demandas_A_Alocar,
      int demanda_Atendida,
      vector<pair<int,int> > & regra_De_Credito);
      //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator & _it_Regra_De_Credito);
};
