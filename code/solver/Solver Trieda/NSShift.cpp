#include "NSShift.h"

NSShift::NSShift(ProblemData & _problemData, bool _keepFeasibility)
: problemData(_problemData), keepFeasibility(_keepFeasibility)
{
   moveValidator = new MoveShiftValidator(&problemData);
}

NSShift::~NSShift()
{
   delete moveValidator;
}

std::pair<Aula*, std::pair<Professor*,std::vector<HorarioAula*> > > NSShift::pickSomeClassAndNewSchedule(SolucaoOperacional & solOp)
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

   GGroup<Aula*, LessPtr<Aula> >::iterator itAula = problemData.aulas.begin();

   int maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
   for( int i = 0; i < maxIter; ++i, ++itAula );

   aula = ( *itAula );

   std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> >, LessPtr<Aula> >::iterator
      itBlocoAulas = solOp.blocoAulas.find(aula);

   if(itBlocoAulas == solOp.blocoAulas.end())
   {
      std::cout << "NSShift : Aula nao encontrada no blocoAulas da solucao corrente." << std::endl;
      exit(1);
   }

   std::vector<HorarioAula*> novosHorariosAula (itBlocoAulas->second.second);
   Professor * professor = NULL;

   // Selecionando o novo professor.
   int idOpProf = rand() % solOp.getMatrizAulas()->size();
   Professor & novoProf = *solOp.getProfessorMatriz(idOpProf);

   int attempt = -1;

   // VAZIO POR EQTO.
   std::vector<std::string> checksToDo;

   while(!moveValidator->canShiftSchedule(*aula,novoProf,novosHorariosAula,solOp,checksToDo) && ++attempt < MAX_ATTEMPTS)
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();
      maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

      // Selecionando o novo professor.
      int idOpProf = rand() % solOp.getMatrizAulas()->size();
      professor = solOp.getProfessorMatriz(idOpProf);

      /*
      Selecionando os novos horarios (pertencentes ao novo professor selecionado anteriormente).

      Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
      sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.
      */

      int hrMaxIniPlus1 = solOp.getTotalHorarios() - aula->getTotalCreditos() + 1;

      int hrIni = (rand() % hrMaxIniPlus1);

      novosHorariosAula.clear();

      // Armazenando os possíveis novos horários.
      for(int i = 0; i < aula->getTotalCreditos(); ++i, ++hrIni)
      {
         HorarioAula * hrAula = problemData.horarios_aula_ordenados.at(hrIni);
         novosHorariosAula.push_back(hrAula);
      }
   }

   if(attempt >= MAX_ATTEMPTS)
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS (" << MAX_ATTEMPTS << ") iterations." << std::endl;
      return std::make_pair(aula,itBlocoAulas->second);
   }

   return std::make_pair(aula,std::make_pair(professor,novosHorariosAula));
}

std::pair<Aula*, std::pair<Professor*,std::vector<HorarioAula*> > > NSShift::pickSomeClassAndNewScheduleWithouChecks(SolucaoOperacional & solOp)
{
   // Critérios a serem respeitados para a escolha da aula.
   // 1 - Somente serão realocados os horários da aula se os novos horários a serem assumidos por ela 
   // estiverem disponíveis na sala em questão.
   // 2 - A aula selecionada não pode ser virtual.
   // 3 - Os novos horarios escolhidos para a aula devem estar vagos e pertencer ao mesmo dia da aula.
   // 4 - A aula a ser realocada só pode ser do tipo teórica (ou seja, a disciplina
   // da aula em questão deve ter o eLab == False ).

   Aula * aula = NULL;

   GGroup<Aula*, LessPtr<Aula> >::iterator itAula = problemData.aulas.begin();

   int maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
   for( int i = 0; i < maxIter; ++i, ++itAula );

   aula = ( *itAula );

   /* Obtendo referência para o(s) bloco(s) de aulas aos quais pertece a aula selecionada acima. */
   std::map<Aula*,std::pair<Professor*,std::vector<HorarioAula*> >, LessPtr<Aula> >::iterator
      itBlocoAulas = solOp.blocoAulas.find(aula);

   if(itBlocoAulas == solOp.blocoAulas.end())
   {
      std::cout << "NSShift : Aula nao encontrada na estrutura <blocoAulas> da solucao corrente." << std::endl;
      exit(1);
   }

   std::vector<HorarioAula*> novosHorariosAula (itBlocoAulas->second.second);
   Professor * professor = NULL;

   // Selecionando o novo professor.
   int idOpProf = rand() % solOp.getMatrizAulas()->size();
   Professor & novoProf = *solOp.getProfessorMatriz(idOpProf);

   int attempt = -1;

   // VAZIO POR EQTO.
   std::vector<std::string> checksToDo;

   while(!moveValidator->canShiftSchedule(*aula,novoProf,novosHorariosAula,solOp,checksToDo) && ++attempt < MAX_ATTEMPTS)
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();
      maxIter = ( rand() % ( solOp.getMatrizAulas()->size() - 1 ) );
      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

      // Selecionando o novo professor.
      int idOpProf = rand() % solOp.getMatrizAulas()->size();
      professor = solOp.getProfessorMatriz(idOpProf);

      /*
      Selecionando os novos horarios (pertencentes ao novo professor selecionado anteriormente).

      Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
      sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.
      */

      int hrMaxIniPlus1 = solOp.getTotalHorarios() - aula->getTotalCreditos() + 1;

      int hrIni = (rand() % hrMaxIniPlus1);

      novosHorariosAula.clear();

      // Armazenando os possíveis novos horários.
      for(int i = 0; i < aula->getTotalCreditos(); ++i, ++hrIni)
      {
         HorarioAula * hrAula = problemData.horarios_aula_ordenados.at(hrIni);
         novosHorariosAula.push_back(hrAula);
      }
   }

   if(attempt >= MAX_ATTEMPTS)
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS (" << MAX_ATTEMPTS << ") iterations." << std::endl;
      return std::make_pair(aula,itBlocoAulas->second);
   }

   return std::make_pair(aula,std::make_pair(professor,novosHorariosAula));
}

Move & NSShift::move(SolucaoOperacional & solOp)
{
   std::pair<Aula*, std::pair<Professor*,std::vector<HorarioAula*> > >
      classAndNewSchedule;

   if(keepFeasibility)
   { classAndNewSchedule = pickSomeClassAndNewSchedule(solOp); }
   else
   { classAndNewSchedule = pickSomeClassAndNewScheduleWithouChecks(solOp); }

   Aula & aula = *(classAndNewSchedule.first);
   Professor & novoProfAula = *(classAndNewSchedule.second.first);

   return *new MoveShift(aula,novoProfAula,classAndNewSchedule.second.second);
}

void NSShift::print()
{
   std::cout << "NSShift" << std::endl;
}
