#include "IS_Sala.h"

IS_Sala::IS_Sala( Sala * _sala )
{
   sala = _sala;

   // A IS_Sala em questão recebe o id da sala que ela representa.
   this->setId( _sala->getId() );
}

IS_Sala::IS_Sala( IS_Sala const & is_sala )
{
   this->setId( is_sala.getId() );
   sala = is_sala.sala;
}

IS_Sala::~IS_Sala(void)
{

}

void IS_Sala::aloca( int turma,
                    Demanda * ref_Demanda_Alocada, /* Referência para a Demanda que está sendo atendida (parcialmente ou totalmente) */
                    int demanda_Atendida, /* Indica a quantidade da demanda que está sendo atendida */
                    std::vector< std::pair< int /*dia*/, int /*num creds*/ > > & regra_De_Credito )
{
   std::vector< std::pair< int /*dia*/, int /*num Creds*/ > >::iterator 
      it_Regra_De_Credito = regra_De_Credito.begin();

   // Para cada par<dia,numCreds> da regra de credito
   for (; it_Regra_De_Credito != regra_De_Credito.end(); it_Regra_De_Credito++ )
   {
      int num_Creds = it_Regra_De_Credito->second;

      // Se para o dia em questão, a regra de credito possui algum credito a ser alocado
      if ( num_Creds > 0 )
      {
         // Procurando o dia certo
         ITERA_GGROUP( it_Dia_Sem, is_Dia_Semana, IS_Dia_Semana )
         {
            if ( it_Dia_Sem->creditos_Disponiveis->getDiaSemana() == it_Regra_De_Credito->first )
            {
               it_Dia_Sem->aloca( turma, ref_Demanda_Alocada, demanda_Atendida, num_Creds );
               break;
            }
         }
      }
   }
}

bool IS_Sala::regraValida( std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > & regra )
{
   std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > >::iterator 
      it_Dia_Regra = regra.begin();

   // Para cada dia da regra de crédito em questão
   for (; it_Dia_Regra != regra.end(); ++it_Dia_Regra )
   {
      int dia = it_Dia_Regra->first;

      // Procurando o dia correto
      ITERA_GGROUP( it_Is_Dia_Semana, is_Dia_Semana, IS_Dia_Semana )
      {
         // Verificando se o dia em questão possui o mínimo de créditos livres necessários.
         // Caso não possua, retorno false indicando que a regra de crédito não é aplicável à
         // sala em questão.
         if ( it_Is_Dia_Semana->creditos_Disponiveis->getDiaSemana() == dia )
         {
            if ( it_Is_Dia_Semana->creditos_Livres < it_Dia_Regra->second )
            {
               return false;
            }

            break;
         }
      }
   }

   return true;
}
