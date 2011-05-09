#include "NSShift.h"

NSShift::NSShift(ProblemData & _problemData, SolucaoOperacional & _solOp) : problemData(_problemData), solOp(_solOp)
{
	moveValidator = new MoveShiftValidator(&problemData,solOp);
}

NSShift::~NSShift()
{
}

//std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > NSShift::pickSomeClassAndNewSchedule( const SolucaoOperacional & s )
//bool NSShift::pickSomeClassAndNewSchedule( const SolucaoOperacional & s ) // So para poder compilar !!!!
std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > NSShift::pickSomeClassAndNewSchedule()
{
   // Critérios a serem respeitados para a escolha da aula.
   // 1 - Somente serão realocados os horários da aula se os novos horários assumidos por ela 
   // não conflitarem com algum horário de alguma outra aula do mesmo bloco. Tambem deve-se respeitar a
   // disponibilidade da sala.
   // 2 - A aula selecionada não pode ser virtual.
   // 3 - Os novos horarios escolhidos para a aula devem estar vagos e pertencer ao mesmo dia da aula.
   // 4 - A aula a ser realocada só pode ser do tipo teórica (ou seja, a disciplina
   // da aula em questão deve ter o eLab == False ).

   Aula * aula = NULL;

   GGroup<Aula*>::iterator itAula = problemData.aulas.begin();

   int maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
   for( int i = 0; i < maxIter; ++i, ++itAula );

   aula = ( *itAula );

   std::vector< std::pair< Professor*, Horario* > > blocoAula (aula->bloco_aula);

   int attempt = -1;

   while(!moveValidator->isValid(*aula,blocoAula) && ++attempt < MAX_ATTEMPTS)
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();
      maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

      // Selecionando o novo professor.
      int idOpProf = rand() % solOp.getMatrizAulas()->size();

      // Selecionando os novos horarios pertencentes ao novo professor selecionado anteriormente.
      /*
      Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
      sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.
      */

      int hrMaxIniPlus1 = solOp.getTotalHorarios() - aula->getTotalCreditos() + 1;

      int hrIni = (rand() % hrMaxIniPlus1);

      blocoAula.clear();
     
      // Armazenando os possíveis novos horários.
      for(int i = 0; i < aula->getTotalCreditos(); ++i, ++hrIni)
      {
         Horario * hrAula = solOp.getHorario(idOpProf,aula->getDiaSemana(),hrIni);

         if(hrAula == NULL)
         { blocoAula.clear(); break; }

         blocoAula.push_back(
            std::make_pair(
            solOp.getProfessorMatriz(idOpProf),
            solOp.getHorario(idOpProf,aula->getDiaSemana(),hrIni)));
      }

      if(blocoAula.empty())
      {
         blocoAula = (aula->bloco_aula);
      }
   }

   if(attempt >= MAX_ATTEMPTS)
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS (" << MAX_ATTEMPTS << ") iterations." << std::endl;
      
      return std::make_pair(aula,aula->bloco_aula);
   }

   return std::make_pair(aula,blocoAula);
}

Move & NSShift::move(const SolucaoOperacional & s)
{
	std::pair<Aula*, std::vector< std::pair< Professor*, Horario* > > > aux = pickSomeClassAndNewSchedule();

	//std::cout << "Metodo Move & move(const SolucaoOperacional & s) de NSShift nao implementado ainda.\nSaindo.\n\n";
	//exit(1);

   Aula & aula = *aux.first;
   Professor & novoProfAula = *aux.second.begin()->first;
   
   return *new MoveShift(aula,novoProfAula,aux.second);
}

void NSShift::print()
{
   std::cout << "NSShift" << std::endl;
}
