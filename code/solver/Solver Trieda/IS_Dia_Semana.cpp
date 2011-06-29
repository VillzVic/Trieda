#include "IS_Dia_Semana.h"

IS_Dia_Semana::IS_Dia_Semana( CreditoDisponivel * cred_Disp )
{
   creditos_Disponiveis = cred_Disp;
   creditos_Livres = creditos_Disponiveis->getMaxCreditos();
}

IS_Dia_Semana::IS_Dia_Semana( IS_Dia_Semana const & is_Dia_Sem )
{
   std::cerr << "COPY CONSTRUCTOR OF <IS_Dia_Semana> NOT IMPLEMENTED YET" << std::endl;
   exit(1);
}

IS_Dia_Semana::~IS_Dia_Semana( void )
{

}

void IS_Dia_Semana::aloca( int turma,
           Demanda * ref_Demanda_Alocda, /* Referência para a Demanda que está sendo atendida (parcialmente ou totalmente) */
           int demanda_Atendida, /* Indica a quantidade da demanda que está sendo atendida */
           int num_Creds )
{
   int total_Creds = is_Atendimentos_Taticos.size();

   // Calculando o índice do IS_Atendimento_Tatico a ser atendido
   int indice = total_Creds - creditos_Livres;

   // Alocando o total de créditos especificado pela regra de crédito para o dia em questão
   for ( int cred = 0; cred < num_Creds; cred++ )
   {
      GGroup< IS_Atendimento_Tatico * >::iterator it_IS_At_Tat =
         is_Atendimentos_Taticos.begin();

      for (; it_IS_At_Tat != is_Atendimentos_Taticos.end(); it_IS_At_Tat++ )
      {
         // Ou seja, se encontrei o credito certo
         if ( it_IS_At_Tat->credito == indice )
         {
            // Setando a turma
            it_IS_At_Tat->turma = turma;

            // Setando a demanda atendida
            it_IS_At_Tat->demanda_Atendida = demanda_Atendida;

            if ( it_IS_At_Tat->demandas.find( ref_Demanda_Alocda ) == it_IS_At_Tat->demandas.end() )
            {
               it_IS_At_Tat->demandas[ ref_Demanda_Alocda ] = demanda_Atendida;
            }
            else
            {
               std::cerr << "Essa demanda já foi alocada !!!" << std::endl;
               exit(1);
            }

            indice++;
            break;
         }
      }

      // Atualizando o total de créditos livres
      --creditos_Livres;
   }
}

bool IS_Dia_Semana::operator < ( IS_Dia_Semana const & right )
{
   return ( creditos_Disponiveis->getDiaSemana() < right.creditos_Disponiveis->getDiaSemana() );
}

bool IS_Dia_Semana::operator == ( IS_Dia_Semana const & right )
{
   return ( creditos_Disponiveis->getDiaSemana() == right.creditos_Disponiveis->getDiaSemana() );
}
