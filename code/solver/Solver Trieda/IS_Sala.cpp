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
   this->setId(is_sala.getId());
   sala = is_sala.sala;
}

IS_Sala::~IS_Sala(void)
{
}

void IS_Sala::aloca(
                    int turma, 
                    pair<Disciplina*,int> & cjt_Dem,
                    vector<pair<Demanda*,int/*Demanda atendida*/> > & demandas_A_Alocar,
                    int demanda_Atendida,
                    vector<pair<int/*dia*/,int/*num Creds*/> > & regra_De_Credito)
                    //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator & _it_Regra_De_Credito)
{
   vector<pair<int/*dia*/,int/*num Creds*/> >::iterator 
      it_Regra_De_Credito = regra_De_Credito.begin();

   // Para cada par<dia,numCreds> da regra de credito
   for(; it_Regra_De_Credito != regra_De_Credito.end(); it_Regra_De_Credito++)
   {
      int num_Creds = it_Regra_De_Credito->second;

      // Se para o dia em questão, a regra de credito possui algum credito a ser alocado
      if(num_Creds > 0)
      {
         // Procurando o dia certo
         ITERA_GGROUP(it_Dia_Sem,is_Dia_Semana,IS_Dia_Semana)
         {
            if(it_Dia_Sem->creditos_Disponiveis->dia_semana == it_Regra_De_Credito->first)
            {
               it_Dia_Sem->aloca(turma,cjt_Dem,demandas_A_Alocar,demanda_Atendida,num_Creds);
               break;
            }
         }
      }
   }
}