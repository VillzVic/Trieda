#include "NSShift.h"

NSShift::NSShift( ProblemData & _problemData, bool _keepFeasibility )
   : problemData( _problemData ), keepFeasibility( _keepFeasibility )
{
   moveValidator = new MoveShiftValidator( &problemData );
}

NSShift::~NSShift()
{
   delete moveValidator;
}

std::pair< Aula *, std::pair< Professor *, std::vector< HorarioAula * > > >
   NSShift::pickSomeClassAndNewSchedule( SolucaoOperacional & solOp )
{
   // Crit�rios a serem respeitados para a escolha da aula.
   // 1 - Somente ser�o realocados os hor�rios da aula se os novos hor�rios assumidos por ela 
   // n�o conflitarem com algum hor�rio de alguma outra aula do mesmo bloco. Tambem deve-se respeitar a
   // disponibilidade da sala.
   // 2 - A aula selecionada n�o pode ser virtual.
   // 3 - Os novos horarios escolhidos para a aula devem estar vagos e pertencer ao mesmo dia da aula.
   // 4 - A aula a ser realocada s� pode ser do tipo te�rica (ou seja, a disciplina
   // da aula em quest�o deve ter o eLab == False ).

   Aula * aula = NULL;
   GGroup< Aula *, LessPtr< Aula > >::iterator itAula = problemData.aulas.begin();

   int tam_matriz = solOp.getMatrizAulas()->size();
   int maxIter = ( rand() % ( tam_matriz > 1 ? tam_matriz - 1 : tam_matriz ) );

   for( int i = 0; i < maxIter; ++i, ++itAula );

   aula = ( *itAula );

   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr< Aula > >::iterator
      itBlocoAulas = solOp.blocoAulas.find( aula );

   if ( itBlocoAulas == solOp.blocoAulas.end() )
   {
      std::cout << "NSShift : Aula nao encontrada no "
                << "blocoAulas da solucao corrente." << std::endl;

      exit(1);
   }

   Professor * professor = NULL;
   std::vector< HorarioAula * > novosHorariosAula ( itBlocoAulas->second.second );

   // Selecionando o novo professor.
   int idOpProf = ( rand() % solOp.getMatrizAulas()->size() );
   Professor & novoProf = *( solOp.getProfessorMatriz( idOpProf ) );

   int attempt = -1;

   // VAZIO POR EQTO.
   std::vector< std::string > checksToDo;

   while ( !moveValidator->canShiftSchedule( *aula, novoProf, novosHorariosAula, solOp, checksToDo ) && ++attempt < MAX_ATTEMPTS )
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();

      tam_matriz = solOp.getMatrizAulas()->size();
      maxIter = ( rand() % ( tam_matriz > 1 ? tam_matriz - 1 : tam_matriz ) );

      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

      // Selecionando o novo professor.
      int idOpProf = ( rand() % solOp.getMatrizAulas()->size() );
      professor = solOp.getProfessorMatriz( idOpProf );

      // Selecionando os novos horarios (pertencentes ao novo professor selecionado anteriormente).
      // Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
      // sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.

      int hrMaxIniPlus1 = ( solOp.getTotalHorarios() - aula->getTotalCreditos() + 1 );
      int hrIni = ( rand() % hrMaxIniPlus1 );

      // Armazenando os poss�veis novos hor�rios.
      novosHorariosAula.clear();
      for ( int i = 0; i < aula->getTotalCreditos(); ++i, ++hrIni )
      {
         HorarioAula * hrAula = problemData.horarios_aula_ordenados.at( hrIni );
         novosHorariosAula.push_back( hrAula );
      }
   }

   if ( attempt >= MAX_ATTEMPTS )
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS ("
                << MAX_ATTEMPTS << ") iterations." << std::endl;

      return std::make_pair( aula, itBlocoAulas->second );
   }

   return std::make_pair( aula, std::make_pair( professor, novosHorariosAula ) );
}

std::pair< Aula *, std::pair< Professor *, std::vector< HorarioAula * > > >
   NSShift::pickSomeClassAndNewScheduleWithouChecks( SolucaoOperacional & solOp )
{
   // Crit�rios a serem respeitados para a escolha da aula.
   // 1 - Somente ser�o realocados os hor�rios da aula se os novos hor�rios a serem assumidos por ela 
   // estiverem dispon�veis na sala em quest�o.
   // 2 - A aula selecionada n�o pode ser virtual.
   // 3 - Os novos horarios escolhidos para a aula devem estar vagos e pertencer ao mesmo dia da aula.
   // 4 - A aula a ser realocada s� pode ser do tipo te�rica (ou seja, a disciplina
   // da aula em quest�o deve ter o eLab == False ).

   Aula * aula = NULL;
   GGroup< Aula *, LessPtr< Aula > >::iterator itAula = problemData.aulas.begin();

   int tam_matriz = solOp.getMatrizAulas()->size();
   int maxIter = ( rand() % ( tam_matriz > 1 ? tam_matriz - 1 : tam_matriz ) );

   for( int i = 0; i < maxIter && itAula != problemData.aulas.end(); ++i, ++itAula );
   if ( itAula == problemData.aulas.end() )
      itAula--;
   aula = ( *itAula );

   // Obtendo refer�ncia para o(s) bloco(s) de aulas aos quais pertece a aula selecionada acima.
   std::map< Aula *, std::pair< Professor *, std::vector< HorarioAula * > >, LessPtr< Aula > >::iterator
      itBlocoAulas = solOp.blocoAulas.find( aula );

   if ( itBlocoAulas == solOp.blocoAulas.end() )
   {
      std::cout << "NSShift : Aula nao encontrada na estrutura "
                << "<blocoAulas> da solucao corrente." << std::endl;

      exit(1);
   }

   std::vector< HorarioAula * > novosHorariosAula ( itBlocoAulas->second.second );
   Professor * professor = NULL;

   // Selecionando o novo professor.
   int idOpProf = ( rand() % solOp.getMatrizAulas()->size() );
   Professor & novoProf = *( solOp.getProfessorMatriz( idOpProf ) );

   int attempt = -1;
   professor = &( novoProf );

   // VAZIO POR EQTO.
   std::vector< std::string > checksToDo;
   checksToDo.push_back( "virtualClass" );

   while ( !moveValidator->canShiftSchedule( *aula, novoProf, novosHorariosAula, solOp, checksToDo ) && ++attempt < MAX_ATTEMPTS )
   {
      // Selecionando uma aula.
      itAula = problemData.aulas.begin();

      tam_matriz = solOp.getMatrizAulas()->size();
      maxIter = ( rand() % ( tam_matriz > 1 ? tam_matriz - 1 : tam_matriz ) );

      for ( int i = 0; i < maxIter; ++i, ++itAula );
      aula = ( *itAula );

      // Selecionando o novo professor.
      int idOpProf = rand() % solOp.getMatrizAulas()->size();
      professor = solOp.getProfessorMatriz( idOpProf );

      // Selecionando os novos horarios (pertencentes ao novo professor selecionado anteriormente).
      // Devo me basear no total de creditos da aula em questao para poder selecionar os horarios validos.
      // sortear o primeiro id e pegar em sequencia os demais horarios. TOMAR CUIDADO PARA NAO EXTRAPOLAR O DIA.
      int hrMaxIniPlus1 = ( solOp.getTotalHorarios() - aula->getTotalCreditos() + 1 );
      int hrIni = ( rand() % hrMaxIniPlus1 );

      // Armazenando os poss�veis novos hor�rios.
      novosHorariosAula.clear();
      for ( int i = 0; i < aula->getTotalCreditos(); ++i, ++hrIni )
      {
         HorarioAula * hrAula = problemData.horarios_aula_ordenados.at( hrIni );
         novosHorariosAula.push_back( hrAula );
      }
   }

   if ( attempt >= MAX_ATTEMPTS )
   {
      std::cout << "Warnning: No valid move generated after MAX_ATTEMPTS ("
                << MAX_ATTEMPTS << ") iterations." << std::endl;

      return std::make_pair( aula, itBlocoAulas->second );
   }

   return std::make_pair( aula, std::make_pair( professor, novosHorariosAula ) );
}

Move & NSShift::move( SolucaoOperacional & solOp )
{
   std::pair< Aula *, std::pair< Professor *,
              std::vector< HorarioAula * > > > classAndNewSchedule;

   if ( keepFeasibility )
   {
      classAndNewSchedule = pickSomeClassAndNewSchedule( solOp );
   }
   else
   {
      classAndNewSchedule = pickSomeClassAndNewScheduleWithouChecks( solOp );
   }

   Aula & aula = *( classAndNewSchedule.first );
   Professor & novoProfAula = *( classAndNewSchedule.second.first );

   return *( new MoveShift( aula, novoProfAula, classAndNewSchedule.second.second ) );
}

void NSShift::print()
{
   std::cout << "NSShift" << std::endl;
}
