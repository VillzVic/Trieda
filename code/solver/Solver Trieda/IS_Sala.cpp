#include "IS_Sala.h"

IS_Sala::IS_Sala(Sala * _sala)
//IS_Sala::IS_Sala(vector<pair<int/*dias*/,int/*creditos livres*/> > cred_Livre_Dia_Let)
{
   sala = _sala;

   // A IS_Sala em questão recebe o id da sala que ela representa.
   this->setId(_sala->getId());
}

IS_Sala::IS_Sala(IS_Sala const & is_sala)
{
   sala = is_sala.sala;
}

IS_Sala::~IS_Sala(void)
{
}
