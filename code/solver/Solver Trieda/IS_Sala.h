#pragma once

#include "Sala.h"

#include <vector>
#include <map>

#include "Demanda.h"

using namespace std;

class IS_Sala : public OFBase
{
public:
   IS_Sala(Sala * _sala);
   //IS_Sala(vector<pair<int/*dias*/,int/*creditos livres*/> > cred_Livre_Dia_Let);

   IS_Sala(IS_Sala const & is_sala);

   virtual ~IS_Sala(void);

   //Sala const * sala;
   Sala * sala;

   //map<int/*dia*/,pair<int/*credsLivres*/,
   //   vector<
   //   pair<Disciplina*,
   //   pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >

   map<int/*dia*/,pair<int/*credsLivres*/,
      vector<
      pair<Demanda*,
      pair<int/*Id Turma*/,int/*Demanda Atendida*/> > > > >

      atendimento_Tatico;

};
