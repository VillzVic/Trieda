#include "NSShift.h"

NSShift::NSShift(ProblemData & _problemData) : problemData(_problemData)
{
	moveValidator = new MoveShiftValidator(&problemData);
}

NSShift::~NSShift()
{
}

//std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > NSShift::pickSomeClassAndNewSchedule( const SolucaoOperacional & s )
bool NSShift::pickSomeClassAndNewSchedule( const SolucaoOperacional & s ) // So para poder compilar !!!!
{
   // Critérios a serem respeitados para a escolha da aula.
   // 1 - Somente serão realocados os horários da aula se os novos horários assumidos por ela 
   // não conflitarem com algum horário de alguma outra aula do mesmo bloco. Tambem deve-se respeitar a
   // disponibilidade da sala.
   // 2 - A aula selecionada não pode ser virtual.
   // 3 - Os novos horarios escolhidos para a aula devem estar vagos e pertencer ao mesmo dia da aula.
   // 4 - As aulas a serem trocadas só podem ser do tipo teóricas (ou seja, a disciplina
   // da aula em questão deve ter o eLab == False ).

   std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > result;

   Aula * aula = NULL;

   GGroup<Aula*>::iterator itAula = problemData.aulas.begin();

   int maxIter = ( rand() % ( s.getMatrizAulas()->size() - 1 ) );
   for( int i = 0; i < maxIter; ++i, ++itAula );

   aula = ( *itAula );

   int attempt = -1;

   while(!moveValidator->isValid(*aula,aula->bloco_aula) && ++attempt < MAX_ATTEMPTS)
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();
      maxIter = ( rand() % ( s.getMatrizAulas()->size() - 1 ) );
      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

	  // Selecionando os novos horarios.
	  /*
	  Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
	  sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.
	  */
   }

   if(attempt >= MAX_ATTEMPTS)
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS (" << MAX_ATTEMPTS << ") iterations." << std::endl;
      
	  result.first = ( *itAula );
	  //result.second = ( * itAula );
   }

   //return result;
   return false;
}

Move & NSShift::move(const SolucaoOperacional & s)
{
	//std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > aux = pickSomeClassAndNewSchedule(s);

  /*   
   1 - IMPLEMENTAR O VALIDATOR ANTES.

   2 - Em seguida, implementar o metodo pickSomeClassAndSchedule para retornar uma aula a ser 
   realocada e o conjunto de horarios para o qual essa aula sera realocada.
   */


	Aula aula;
	Professor novoProfAula;
	vector<pair<Professor*,Horario*> > blocoHorariosVagos;

	std::cout << "Metodo Move & move(const SolucaoOperacional & s) de NSShift nao implementado ainda.\nSaindo.\n\n";
	exit(1);

   return *new MoveShift(aula,novoProfAula,blocoHorariosVagos);
}

void NSShift::print()
{
   std::cout << "NSShift" << std::endl;
}
