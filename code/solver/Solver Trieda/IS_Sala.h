#pragma once

#include "Sala.h"

#include <vector>
#include <map>

using namespace std;

class IS_Sala
{
public:
   IS_Sala(void);
   //IS_Sala(vector<pair<int/*dias*/,int/*creditos livres*/> > cred_Livre_Dia_Let);

   IS_Sala(IS_Sala const & is_sala);

   virtual ~IS_Sala(void);

   //Sala const * sala;
   Sala * sala;

   map<int/*dia*/,pair<int/*credsLivres*/,vector<Disciplina*> > > atendimento_Tatico;
   //map<int/*dia*/,pair<int/*credsLivres*/,int> > atendimento_Tatico;
};
