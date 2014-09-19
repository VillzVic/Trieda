#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include "GGroup.h"

#include <iostream>
#include <iomanip>
#include <iterator>

#include "CentroDados.h"
#include "Indicadores.h"


ProblemDataLoader::ProblemDataLoader(
   char * inputFile, ProblemData * data )
{
   this->inputFile = inputFile;
   this->problemData = data;
}

ProblemDataLoader::~ProblemDataLoader()
{

}

void ProblemDataLoader::load()
{   	
   std::cout << "Carregando o arquivo..." << std::endl; fflush(NULL);
   
   int dif=0;
   CentroDados::startTimer();

   root = std::auto_ptr< TriedaInput >(
      TriedaInput_( inputFile, xml_schema::flags::dont_validate ) );
   
   dif = CentroDados::getLastRunTime();

   std::cout << left << std::setw(70) << "\nLendo arvore raiz..."; fflush(NULL);
   problemData->le_arvore( *root );   
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nGerando Referencias para TipoSalas..." ; fflush(NULL);
   geraRefsTipoSala();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nDefine fases do dia (Manha, tarde, noite)..."; fflush(NULL);
   problemData->defineFasesDosTurnos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nDefine fases do dia (Manha, tarde, noite)..."; fflush(NULL);
   problemData->calculaCalendarioSomaInterv();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nCalcula tempo de intervalo por fase do dia dos calendarios..."; fflush(NULL);
   problemData->copiaFasesDosTurnosParaSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nSet Fase Do Dia Dos HorariosAula..."; fflush(NULL);
   problemData->setFaseDoDiaDosHorariosAula();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nPreenche MapTurnoSemanasLetivas..."; fflush(NULL);
   problemData->preencheMapTurnoSemanasLetivas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nImprime arquivo com alunos formandos..."; fflush(NULL);
   problemData->imprimeFormandos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nPreencheTempoAulaHorarios..."; fflush(NULL);
   preencheTempoAulaHorarios();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nRelacionando demanda a alunos..."; fflush(NULL);
   relacionaDemandaAlunos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nRelacionando calendario horarios e aulas..."; fflush(NULL);
   relacionaCalendarioHorariosAula();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nPreenche Hash HorarioAula DateTime...";
   preencheHashHorarioAulaDateTime();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferenciando ofertas...";
   geraRefsOfertas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferenciando demandas...";
   geraRefsDemandas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferenciando disciplinas equivalentes e incompatibilidades...";
   referenciaDisciplinasEquivalentesIncompativeis();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferenciando disciplinas curriculos...";
   referenciaDisciplinasCurriculos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferencia os cursos e os calendarios dos curriculos..."; fflush(NULL);
   referenciaCursos_CurricCalendarios();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
       
   std::cout << left << std::setw(70) << "\nReferencia disciplinas equivalentes...";
   referenciaDisciplinasEquivalentes();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nRemovendo fixacoes com disciplinas substituidas...";
   removeFixacoesComDisciplinasSubstituidas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nReferencia calendarios dos curriculos...";
   referenciaCalendariosCurriculos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nRelacionando horarios aula dia semana...";
   relacionaHorariosAulaDiaSemana();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nRelacionando creds regras...";
   relacionaCredsRegras();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nCarregando dias letivos campus unidade sala..."; fflush(NULL);
   carregaDiasLetivosCampusUnidadeSala();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nCarregando dias letivos disciplinas...";
   carregaDiasLetivosDiscs();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nProcessando disciplinas de cursos compativeis...";
   disciplinasCursosCompativeis();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nPreenchendo disciplinas de cursos compativeis...";
   preencheDisciplinasDeCursosCompativeis();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nPreenchendo disciplinas de ofertas compativeis..."; fflush(NULL);
   preencheDisciplinasDeOfertasCompativeis();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nDividindo disciplinas..." ;
   divideDisciplinas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
#ifndef HEURISTICA
   std::cout << left << std::setw(70) << "\nValidando input tatico...";
   validaInputSolucaoTatico();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
#endif

   std::cout << left << std::setw(70) << "\nReferenciando campus unidades e salas...";
   referenciaCampusUnidadesSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nReferenciando disciplinas...";
   referenciaDisciplinas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nReferenciando ofertas..." ;
   referenciaOfertas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
        
   std::cout << left << std::setw(70) << "\nReferenciando profs...";
   referenciaProfessores();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nGerando referencias..." ; fflush(NULL);
   gera_refs();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nPreenche MapTurnoDisciplinas...";
   problemData->preencheMapTurnoDisciplinas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCriando alunos...";
   associaAlunoComAlunoDemanda();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nConfere Dados De Equivalencia Forcada..." ; fflush(NULL);
   problemData->confereDadosDeEquivalenciaForcada();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);

   std::cout << left << std::setw(70) << "\nCorrigindo fixacoes com disciplinas e salas..."; fflush(NULL);
   corrigeFixacoesComDisciplinasSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando demandas..." ;
   calculaDemandas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nRelacionando cursos a campus..." ;
   relacionaCursosCampus();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando tamanho das salas...";
   calculaTamanhoMedioSalasCampus();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nRelacionando campus a disciplinas...";
   relacionaCampusDiscs();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nRelacionado disciplinas e ofertas...";
   relacionaDiscOfertas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
        
   std::cout << left << std::setw(70) << "\nPreenche mapDisciplinaAlunosDemanda_atual...";
#ifdef EQUIVALENCIA_DESDE_O_INICIO
   if ( problemData->parametros->considerar_equivalencia_por_aluno )
	   problemData->preencheMapDisciplinaAlunosDemanda(true);
   else
	   problemData->preencheMapDisciplinaAlunosDemanda(false);
#else
   problemData->preencheMapDisciplinaAlunosDemanda(false);
#endif
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
    
   std::cout << left << std::setw(70) << "\nAssociando disciplinas a salas..."; fflush(NULL);
   associaDisciplinasSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
      
   std::cout << left << std::setw(70) << "\nCalculando media capacidade de sala por disc...";
   calculaCapacMediaSalaPorDisc();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
        
#ifndef HEURISTICA
   std::cout << left << std::setw(70) << "\nEstimando turmas...";
   estima_turmas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
#endif

   std::cout << left << std::setw(70) << "\nCriando blocos curriculares...";
   cria_blocos_curriculares();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCriando cjt salas..." ; fflush(NULL);
   criaConjuntoSalasUnidade();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nAssociando disciplinas a cjt de salas...";
   associaDisciplinasConjuntoSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCache...";
   cache();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
#ifndef HEURISTICA
   std::cout << left << std::setw(70) << "\nEstabelecendo Dias Letivos Bloco Campus..." ;
   estabeleceDiasLetivosBlocoCampus();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nEstabelecendo Dias Letivos Disciplinas Salas...";
   estabeleceDiasLetivosDisciplinasSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nEstabelecendo Dias Letivos Disciplinas Cjt Salas..."; fflush(NULL);
   estabeleceDiasLetivosDiscCjtSala();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando creditos livres de salas...";
   calculaCredsLivresSalas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
#endif

   std::cout << left << std::setw(70) << "\nCalculando divisão de creditos...";
   combinacaoDivCreditos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nEstabelecendo DiasLetivos Professor Disciplina..."; fflush(NULL);
   estabeleceDiasLetivosProfessorDisciplina();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
#ifndef HEURISTICA
   std::cout << left << std::setw(70) << "\nCriando aulas...";
   criaAulas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
#endif

   std::cout << left << std::setw(70) << "\nCalculando maximo de horarios professor...";
   calculaMaxHorariosProfessor();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCriando listas de horarios ordenados...";
   criaListaHorariosOrdenados();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
       
   std::cout << left << std::setw(70) << "\nVerificando fixacoes de dias letivos das disciplinas..."; fflush(NULL);
   verificaFixacoesDiasLetivosDisciplinas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nRelacionando fixacoes..." ;
   relacionaFixacoes();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nGerando horariosDia..." ;fflush(NULL);
   geraHorariosDia();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nReferenciando horarios aula...";fflush(NULL);
   referenciaHorariosAula();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nReferenciando turnos..." ;fflush(NULL);
   referenciaTurnos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalcula carga horaria original de P1 por aluno...";fflush(NULL);
   calculaCHOriginalPorAluno();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nRelacionando professor disciplinas associadas...";fflush(NULL);
   relacionaProfessorDisciplinasAssociadas();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando maximo de tempo disponivel por sala...";   fflush(NULL);
   calculaMaxTempoDisponivelPorSala();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando maximo de tempo disponivel por sala e semana letiva..."; fflush(NULL);
   calculaMaxTempoDisponivelPorSalaPorSL();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nCalculando compatibilidade de horarios..." ;fflush(NULL);
   calculaCompatibilidadeDeHorarios();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nImprimindo resumo de demandas de alunos...";fflush(NULL);
   problemData->imprimeResumoDemandasPorAluno();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nPreencheMapsTurmasTurnosIES...";fflush(NULL);
   problemData->preencheMapsTurmasTurnosIES();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nPreencheMapParCalendHorariosDiaComuns...";   fflush(NULL);
   problemData->preencheMapParCalendHorariosDiaComuns();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nImprimindo associacao entre disciplinas e salas...";  fflush(NULL);
   problemData->imprimeAssociacaoDisciplinaSala();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
        	
   std::cout << left << std::setw(70) << "\npreencheDiscSubstitutaPossivelPorDemanda...";  fflush(NULL);
   problemData->preencheDiscSubstitutaPossivelPorDemanda();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
     
   std::cout << left << std::setw(70) << "\nsetDateTimesSobrepostos...";  fflush(NULL);
   problemData->setDateTimesSobrepostos();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
          
   std::cout << left << std::setw(70) << "\nprintInputDataLog...";  fflush(NULL);
   this->printInputDataLog();
   dif = CentroDados::getLastRunTime();
   std::cout << left << std::setw(10) << " " << dif << "sec"; fflush(NULL);
       
   CentroDados::stopTimer();

    fflush(NULL);
}

void ProblemDataLoader::printInputDataLog()
{	
	Indicadores::printSeparator(3);
	Indicadores::printIndicador( "\n\t\t\tDados de entrada\n" );

	// ----------------------------------------------------------------------------------------------------------
	// ALUNOS
	
	int totalAlunos = 0;
	int totalAlunosVeter = 0;
	int nroFormandos = 0;
	int nroCalouros = 0;
	double nroMedioDiscPorAlunoP1 = 0;
	double nroMedioCredPorAlunoP1 = 0;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno* const aluno = *itAluno;

		totalAlunos++;
		if ( !aluno->ehCalouro() && aluno->ehFormando() ) nroFormandos++;
		if ( !aluno->ehCalouro() ) totalAlunosVeter++;
		if ( aluno->ehCalouro() ) nroCalouros++;
		
		nroMedioDiscPorAlunoP1 += aluno->getNroDiscsOrigRequeridosP1();
		nroMedioCredPorAlunoP1 += aluno->getNroCredsOrigRequeridosP1();
	}

	if ( totalAlunos!=0 )
	{
		nroMedioDiscPorAlunoP1 = nroMedioDiscPorAlunoP1 / totalAlunos;
		nroMedioCredPorAlunoP1 = nroMedioCredPorAlunoP1 / totalAlunos;
	}

	stringstream ssAlunos;
	
	ssAlunos << "\nDos " << totalAlunos << " alunos:"
		<< "\n\t" << totalAlunosVeter << " foram marcados como alunos veteranos;"
		<< "\n\t" << nroFormandos << " foram marcados como veteranos formandos;"
		<< "\n\t" << nroCalouros << " foram marcados como entrantes.";
	
	ssAlunos << "\nNúmero médio de disciplinas por aluno considerando demandas presenciais P1 e externas P1: " << nroMedioDiscPorAlunoP1;
	
	ssAlunos << "\nNúmero médio de créditos por aluno considerando demandas presenciais P1 e externas P1: " << nroMedioCredPorAlunoP1;
	
	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ssAlunos.str() );

	// ----------------------------------------------------------------------------------------------------------
	// DISCIPLINAS
	
	int nroDiscLab = 0;
	int nroDiscLabNaoAssoc = 0;
	double mediaOrigLabPorDisc = 0;
	double mediaFinalLabPorDisc = 0;
	int totalDisc = 0;
	int nroDiscSemProf = 0;

	ITERA_GGROUP_LESSPTR( itDisciplina, problemData->disciplinas, Disciplina )
	{
		Disciplina *disciplina = *itDisciplina;

		totalAlunos++;
										
		if ( disciplina->getId() > 0 )
		{
			totalDisc++;
			if ( problemData->getNroProfPorDisc(disciplina) == 0 )
				nroDiscSemProf++;
		}

		if ( disciplina->eLab() ) 
		{
			mediaFinalLabPorDisc += disciplina->cjtSalasAssociados.size();
			nroDiscLab++;

			int nroOrigLabAssoc=0;
			auto itDisc = problemData->disc_Salas_Pref.find(disciplina);
			if ( itDisc != problemData->disc_Salas_Pref.end() )
				nroOrigLabAssoc = itDisc->second.size();

			mediaOrigLabPorDisc += nroOrigLabAssoc;
			if ( nroOrigLabAssoc==0 )
				nroDiscLabNaoAssoc++;
		}
	}

	if ( nroDiscLab != 0 )
	{
		mediaFinalLabPorDisc /= nroDiscLab;
		mediaOrigLabPorDisc /= nroDiscLab;
	}
	
	stringstream ssDiscs;

	if ( nroDiscLabNaoAssoc > 0 )
	{
		ssDiscs << "\nDas " << nroDiscLab << " disciplinas que exigem laboratório, "
			<< nroDiscLabNaoAssoc << " não estão associadas a nenhum ambiente nos dados de entrada originais."
			<< " Nesses casos, as disciplinas são associadas automaticamente a todos os laboratórios (*).";
	}
	else if ( nroDiscLab > 0 )
	{
		ssDiscs << "\nDas " << nroDiscLab << " disciplinas que exigem laboratório, todas têm laboratórios associados.";
	}
	else
	{
		ssDiscs << "\nNenhuma disciplina exige laboratório.";
	}

	ssDiscs << "\nMédia original de laboratórios associados por disciplina que exige laboratório: "
		<< mediaOrigLabPorDisc;
	
	if ( nroDiscLabNaoAssoc > 0 )
		ssDiscs << "\nMédia de laboratórios associados por disciplina após possíveis associações automáticas (*): " << mediaFinalLabPorDisc;

	if ( nroDiscSemProf > 0 )
		ssDiscs << "\nDas " << totalDisc << " disciplinas cadastradas, " << nroDiscSemProf << " não possuem professor habilitado.";
	else
		ssDiscs << "\nDas " << totalDisc << " disciplinas cadastradas, todas possuem pelo menos 1 professor habilitado.";

	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ssDiscs.str() );

	// ----------------------------------------------------------------------------------------------------------
	
	int nroProfHor = 0;
	int nroProfParcial = 0;
	int nroProfIntegral = 0;
	double custoMinProf = 999999999;
	double custoMaxProf = 0;
	double mediaCustoProf = 0;
	int totalProfs = 0;
	int nroProfAte3Disc = 0;
	double nroMedioHabPorProf = 0;
	int nroMaxHab = 0;
	int nroProfAte2DiasDisp = 0;
	Professor *profComMaxHab=nullptr;

	for( auto itProf = problemData->refProfessores.begin(); itProf != problemData->refProfessores.end(); itProf++ )
	{
		Professor* const professor = itProf->second;

		if ( !professor->eVirtual() )
		{
			totalProfs++;
				
			// Número de habilitações do Professor
			int nroHabilit=0;
			ITERA_GGROUP_LESSPTR( itMagisterio, professor->magisterio, Magisterio )
			{
				if ( itMagisterio->getDisciplinaId() > 0 )
					nroHabilit++;		
			}			
			if ( nroHabilit > nroMaxHab )
			{
				nroMaxHab = nroHabilit;
				profComMaxHab = professor;
			}
			nroMedioHabPorProf += nroHabilit;
			if ( nroHabilit <= 3 ) nroProfAte3Disc++;
				
			// Tipo de Contrato do Professor
			if ( professor->tipo_contrato->getContrato() == TipoContrato::Integral )		// Integral (Mensalista)
				nroProfIntegral++;
			else if ( professor->tipo_contrato->getContrato() == TipoContrato::Parcial )	// Parcial  (Mensalista)
				nroProfParcial++;
			else																			// Horista
				nroProfHor++; 

			// Custo do Professor
			mediaCustoProf += professor->getValorCredito();
			if ( professor->getValorCredito() < custoMinProf )
				custoMinProf = professor->getValorCredito();
			if ( professor->getValorCredito() > custoMaxProf )
				custoMaxProf = professor->getValorCredito();

			// Dias de Disponibilidade do Professor
			if ( professor->getNroDiasDisponiv() <= 2 )
				nroProfAte2DiasDisp++;
		}
	}

	// Custo Médio dos Professores
	if ( totalProfs != 0 )
	{
		mediaCustoProf = mediaCustoProf/ totalProfs;
		nroMedioHabPorProf = nroMedioHabPorProf / totalProfs;
	}
	
	stringstream ssProfs;
	if ( totalProfs > 0 )
	{
		ssProfs << "\n" << nroProfHor << " professores foram cadastrados como horistas, " 
			<< nroProfIntegral << " como integrais e " << nroProfParcial << " como parciais, somando "
			<< nroProfParcial+nroProfIntegral << " mensalistas.";

		ssProfs << "\nO custo dos professores varia entre R$" << custoMinProf 
			<< " e R$" << custoMaxProf << ", sendo que a média é R$" << mediaCustoProf << ".";

		ssProfs << "\nDos " << totalProfs << " professores, " << nroProfAte3Disc 
			<< (nroProfAte3Disc==1? " está homologado " : " estão homologados " )
			<< "em apenas 1, 2 ou 3 disciplinas.";

		ssProfs << "\nNúmero médio de habilitações por docente: " << nroMedioHabPorProf;	
	
		if(profComMaxHab != nullptr)
		{
			ssProfs << "\nMáximo de habilitações: " << profComMaxHab->getNome()
				<< " (CPF: " << profComMaxHab->getCpf() << ") com "	<< nroMaxHab << " disciplinas habilitadas.";
		}
		ssProfs << "\nMáximo de habilitações: " << profComMaxHab->getNome()
			<< " (CPF: " << profComMaxHab->getCpf() << ") com "	<< nroMaxHab << " disciplinas habilitadas.";

		ssProfs << "\nDos " << totalProfs << " professores, " << nroProfAte2DiasDisp 
			<< " estão disponíveis apenas 1 ou 2 dias na semana.";
	}
	else
	{
		ssProfs << "\nNao ha professores cadastrados.";
	}

	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ssProfs.str() );

	// ----------------------------------------------------------------------------------------------------------

}

void ProblemDataLoader::preencheTempoAulaHorarios()
{
   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
         ITERA_GGROUP_LESSPTR( it_horario_aula, it_calendario->horarios_aula, HorarioAula )
         {
            HorarioAula * horario_aula = ( *it_horario_aula );
            horario_aula->setTempoAula( it_calendario->getTempoAula() );
         }    
   }
}

// Preenche o map problemData->mapDemandaAlunos
void ProblemDataLoader::relacionaDemandaAlunos()
{	
   ITERA_GGROUP_LESSPTR( it_demanda,
      this->problemData->demandas, Demanda )
   {
      Demanda * demanda = ( *it_demanda );

      ITERA_GGROUP_LESSPTR( it_aluno_demanda,
         this->problemData->alunosDemanda, AlunoDemanda )
      {
         AlunoDemanda * aluno_demanda = ( *it_aluno_demanda );

         if ( demanda->getId() == aluno_demanda->getDemandaId() )
         {
            this->problemData->mapDemandaAlunos[ demanda ].add( aluno_demanda );
         }
      }

      int quantidade = this->problemData->mapDemandaAlunos[ demanda ].size();
      if ( quantidade > 0 )
      {
         demanda->setQuantidade( quantidade );
      }
   }
}

// Método relacionado com a issue TRIEDA-1054
void ProblemDataLoader::relacionaCalendarioHorariosAula()
{
   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_horario_aula, calendario->horarios_aula, HorarioAula )
      {
         HorarioAula * horario_aula = ( *it_horario_aula );
		 horario_aula->setCalendario( calendario );      
      }
   }
}

//void ProblemDataLoader::relacionaDiscCalendariosOrig()
//{
//	ITERA_GGROUP_LESSPTR( it_disciplina, this->problemData->disciplinas, Disciplina )
//	{
//		ITERA_GGROUP_LESSPTR( it_hor, it_disciplina->horarios, Horario )
//	    {
//			int horAulaId = it_hor->getHorarioAulaId();
//			Calendario * calendario = ;
//
//			it_disciplina->addCalendariosOriginais(calendario);		  		  
//	   }
//	}
//}


void ProblemDataLoader::geraRefsTipoSala()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_salas, it_unidades->salas, Sala )
         {
            find_and_set_lessptr( it_salas->getTipoSalaId(),
               problemData->tipos_sala,
               it_salas->tipo_sala, false );
		 }
	  }
   }
}

void ProblemDataLoader::geraRefsOfertas()
{
   ITERA_GGROUP_LESSPTR( it_oferta, problemData->ofertas, Oferta )
   {
      find_and_set_lessptr( it_oferta->getCursoId(),
         problemData->cursos,
         it_oferta->curso, false );

      find_and_set_lessptr( it_oferta->getCurriculoId(),
         it_oferta->curso->curriculos,
         it_oferta->curriculo, false );

      //find_and_set_lessptr( it_oferta->getTurnoId(),
      //   problemData->todos_turnos,
      //   it_oferta->turno, false );

      find_and_set_lessptr( it_oferta->getCampusId(),
         problemData->campi,
         it_oferta->campus, false );
   }
}

void ProblemDataLoader::geraRefsDemandas()
{
   ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda ) 
   {
      find_and_set_lessptr( it_dem->getOfertaId(),
         problemData->ofertas,
         it_dem->oferta, false );

      find_and_set_lessptr( it_dem->getDisciplinaId(),
         problemData->disciplinas,
         it_dem->disciplina, false );
   }
}

void ProblemDataLoader::associaAlunoComAlunoDemanda()
{
   ITERA_GGROUP_LESSPTR( itAlunoDem, problemData->alunosDemandaTotal, AlunoDemanda )
   {
	   AlunoDemanda *alunoDemanda = *itAlunoDem;

	   int id = alunoDemanda->getAlunoId();
	   int demId = alunoDemanda->getDemandaId();
	   Oferta *oft = alunoDemanda->demanda->oferta;
	   int p = alunoDemanda->getPrioridade();

	   Aluno* aluno = problemData->retornaAluno( id );

	   if (aluno==NULL)
	   {
		   std::cout<<"\nERRO em ProblemDataLoader::criaAlunos: alunoDemanda "<<alunoDemanda->getId()
					<<" esta associada ao alunoId " << id <<" inexistente.\n";
		   continue;
	   }
	   
	   alunoDemanda->setAluno(aluno);

	   if ( p==1 )
	   {
		  aluno->demandas.add( alunoDemanda );
		  
		  if ( alunoDemanda->demanda->getDisciplinaId() > 0 )
			  aluno->addNroDiscsOrigRequeridosP1(1);
	   }

	   if ( aluno->getOferta( alunoDemanda->demanda ) == NULL )
	   {
		   aluno->setOferta( oft );					// ISSO NÃO TEM SENTIDO MAIS
		   aluno->setOfertaId( oft->getId() );		// ISSO NÃO TEM SENTIDO MAIS
	   }

	   // Usado em proibição de gap no horario do dia do aluno,
	   // para pegar calendario e calcular soma de intervalo maximo no dia
	   aluno->setOferta( oft );
	   aluno->setOfertaId( oft->getId() );
   }
      

	// Atribui true ao "possuiAlunoFormando" do campus, caso haja aluno formando
    // e define se o aluno é calouro
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		(*itAluno)->fillCampusIds();

		if ( itAluno->ehFormando() )
		{
			GGroup<int> cpIds = (*itAluno)->getCampusIds();
			ITERA_GGROUP_N_PT( itCpId, cpIds, int )
			{
				problemData->refCampus[ *itCpId ]->setPossuiAlunoFormando( true );			
			}
		}

		(*itAluno)->defineSeEhCalouro();		
	}

}

void ProblemDataLoader::criaFixacoesDisciplinasDivididas()
{
   Fixacao * fixacao = NULL;
   Disciplina * disciplina_pratica = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      // Verifica se a disciplina é prática
      if ( it_disciplina->getId() > 0 )
      {
         continue;
      }

      disciplina_pratica = ( *it_disciplina );

      // Procura se existe uma fixação
      // correspondente à disciplina teórica 
      ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
      {
         fixacao = ( *it_fixacao );

         if ( fixacao->disciplina != NULL
            && abs( fixacao->disciplina->getId() ) == abs( disciplina_pratica->getId() ) )
         {
            // Cria fixação com os mesmos dados, porém com 'id' negativo
            Fixacao * fixacao_disciplina_pratica = new Fixacao( *fixacao );

            // Atualiza as referência de disciplina para a disciplina prática
            fixacao_disciplina_pratica->setDisciplinaId( disciplina_pratica->getId() );
            fixacao_disciplina_pratica->disciplina = ( disciplina_pratica );

            // Assim como temos dois objetos 'disciplina', agora temos
            // dois objetos 'fixação', para os créditos teóricos e práticos
            problemData->fixacoes.add( fixacao_disciplina_pratica );
         }
      }
   }
}

bool ordena_horarios_aula( HorarioAula * h1, HorarioAula * h2 )
{
   if ( h1 == NULL && h2 == NULL )
   {
      return false;
   }
   else if ( h1 != NULL && h2 == NULL )
   {
      return false;
   }
   else if ( h1 == NULL && h2 != NULL )
   {
      return true;
   }

   return ( h1->getInicio() < h2->getInicio() );
}

void ProblemDataLoader::criaListaHorariosOrdenados()
{
   GGroup< HorarioAula * > horarios_aula;

   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator it_map_horarios
      = problemData->horarios_aula_dia_semana.begin();

   // Adiciona os horários de aula, sem repetição
   for (; it_map_horarios != problemData->horarios_aula_dia_semana.end();
          it_map_horarios++ )
   {
      GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios
         = it_map_horarios->second;

      ITERA_GGROUP_LESSPTR( it_horario, horarios, HorarioAula )
      {
         horarios_aula.add( *it_horario );
      }
   }

   // Insere os horarios de aula ( distintos ) no vector
   ITERA_GGROUP( it_h, horarios_aula, HorarioAula )
   {
      problemData->horarios_aula_ordenados.push_back( *it_h );

      problemData->mapCalendarioHorariosAulaOrdenados[ it_h->getCalendario() ].push_back( *it_h );
   }

   // Ordena os horarios de aula pelo inicio de cada um
   std::sort( problemData->horarios_aula_ordenados.begin(),
              problemData->horarios_aula_ordenados.end(), ordena_horarios_aula );

   std::map< Calendario *, std::vector< HorarioAula * >, LessPtr< Calendario > >::iterator
      it_map = problemData->mapCalendarioHorariosAulaOrdenados.begin();

   for (; it_map != problemData->mapCalendarioHorariosAulaOrdenados.end();
          it_map++ )
   {
      std::sort( ( *it_map ).second.begin(),
                 ( *it_map ).second.end(), ordena_horarios_aula );
   }
}

void ProblemDataLoader::calculaMaxHorariosProfessor()
{
   int temp = 0;
   int totalAtual = 0;

   ITERA_GGROUP_LESSPTR( it_campi,problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_professor, it_campi->professores, Professor )
      {
         temp = it_professor->horarios.size();

         if ( temp > totalAtual )
         {
            totalAtual = temp;
         }
      }
   }

   problemData->max_horarios_professor = totalAtual;
}

void ProblemDataLoader::relacionaCursosCampus()
{
   Campus * campus = NULL;
   Curso * curso = NULL;
   Oferta * oferta = NULL;

   ITERA_GGROUP_LESSPTR( it_oferta,
      problemData->ofertas, Oferta )
   {
      // Recupera o objeto 'oferta'
      oferta = ( *it_oferta );

      // Recupera o objeto 'campus' dessa 'oferta'
      campus = it_oferta->campus;

      // Recupera o objeto 'curso' dessa 'oferta'
      curso = it_oferta->curso;

      // Relaciona o 'curso' com o 'campus'
      if ( campus != NULL && curso != NULL )
      {
         campus->cursos.add( curso );
         campus->ofertas.add( oferta );
      }
      else
      {
         std::cout << "\nProblemDataLoader::relacionaCursosCampus()"
                   << "\nFoi encontrada uma oferta que não possui"
                   << "\ncampus e/ou curso valido( s )." << std::endl;

         exit( 1 );
      }
   }
}

void ProblemDataLoader::referenciaDisciplinasEquivalentesIncompativeis()
{
   Disciplina * disciplina = NULL;
   Disciplina * disc_equivalente = NULL;
   Disciplina * disc_incompativel = NULL;

   // Para cada disciplina, procuramos pelas referencias de suas
   // disciplinas equivalentes e incompatíveis
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disc );

      ITERA_GGROUP_LESSPTR( it_disc_aux, problemData->disciplinas, Disciplina )
      {
         if ( it_disc_aux->getId() == it_disc->getId() )
         {
            continue;
         }

         // Disciplinas equivalentes
         ITERA_GGROUP_N_PT( id_disc, disciplina->ids_disciplinas_equivalentes, int )
         {
            if ( it_disc_aux->getId() == ( *id_disc ) )
            {
               disc_equivalente = ( *it_disc_aux );
               disciplina->addDiscSubstituida( disc_equivalente );
			   disc_equivalente->addDiscSubstituta( disciplina );
               break;
            }
         }

         // Disciplinas incompatíveis
         ITERA_GGROUP_N_PT( id_disc, disciplina->ids_disciplinas_incompativeis, int )
         {
            if ( it_disc_aux->getId() == ( *id_disc ) )
            {
               disc_incompativel = ( *it_disc_aux );
               disciplina->discIncompativeis.add( disc_incompativel );
               break;
            }
         }
      }
   }
}

void ProblemDataLoader::referenciaDisciplinasCurriculos()
{
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
	   problemData->refCursos[(*it_curso)->getId()] = (*it_curso);
      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
         curriculo = ( *it_curriculo );
         curriculo->refDisciplinaPeriodo( problemData->disciplinas );
      }
   }
}

void ProblemDataLoader::relacionaHorariosAulaDiaSemana()
{
   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      ITERA_GGROUP_LESSPTR( it_horario_aula, it_calendario->horarios_aula, HorarioAula )
      {
         ITERA_GGROUP_N_PT( it_dia_semana, it_horario_aula->dias_semana, int )
         {
            // Recupera o conjunto de horários de aula do dia da semana atual
            GGroup< HorarioAula *, LessPtr< HorarioAula > > * horarios_dia
               = &( problemData->horarios_aula_dia_semana[ ( *it_dia_semana ) ] );

            // Adiciona o horário de aula atual no
            // conjunto de horários de aula do dia da semana
            horarios_dia->add( ( *it_horario_aula ) );
         }
      }
   }
}

void ProblemDataLoader::relacionaCredsRegras()
{
	ITERA_GGROUP_LESSPTR( it_Regra, problemData->regras_div, DivisaoCreditos )
   { 
      problemData->creds_Regras[ it_Regra->getCreditos() ].add( *it_Regra );
   }
}

void ProblemDataLoader::carregaDiasLetivosCampusUnidadeSala()
{
   // Para cada Campus
   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      // --------------------------------------------------------
      // Adicionando os dias letivos de cada Campus
      ITERA_GGROUP( it_Horario, it_Campus->horarios, Horario )
      {
         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
         {
            (*it_Campus)->diasLetivos.add( *it_Dias_Letivos );
         }
      }
      // --------------------------------------------------------

      // Para cada Unidade
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         // --------------------------------------------------------
         // Adicionando os dias letivos de cada Unidade
         ITERA_GGROUP( it_Horario, it_Unidade->horarios, Horario )
         {
            ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
            {
               (*it_Unidade)->dias_letivos.add( *it_Dias_Letivos );
            }
         }

         // --------------------------------------------------------
         // Para cada Sala
         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            // --------------------------------------------------------
            // Adicionando os dias letivos de cada Sala
            ITERA_GGROUP( it_Horario, it_Unidade->horarios, Horario )
            {
                ITERA_GGROUP ( itHorarioDisponivel, it_Sala->horarios_disponiveis, Horario )
                {
                    (*it_Sala)->diasLetivos.add( itHorarioDisponivel->dias_semana );
                }
            }
         }
      }
   }
}

void ProblemDataLoader::carregaDiasLetivosDiscs()
{
   ITERA_GGROUP_LESSPTR( it_Disc, problemData->disciplinas, Disciplina )
   {
      ITERA_GGROUP_LESSPTR( it_Horario, it_Disc->horarios, Horario )
      {       
		 ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Horario->dias_semana, int )
         {
            (*it_Disc)->diasLetivos.add( *it_Dias_Letivos );
         }
      }
   }
}

void ProblemDataLoader::criaConjuntoSalasUnidade()
{
   problemData->totalConjuntosSalas = 0;
   
   int idCjtSala = 1;

   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         // Conjunto de salas (LABORATORIOS) que tiveram de
         // ser criadas, dado que possuiam pelo menos 
         // uma disciplina com a FLAG <eLab> marcada (TRUE)
         GGroup< ConjuntoSala * > conjunto_Salas_Disc_eLab;

         // Conjunto de salas (SALAS OU LABORATORIOS) que
         // foram criadas sendo que não possuiam nenhuma 
         // disciplina com a FLAG <eLab> marcada (TRUE)
         GGroup< ConjuntoSala * > conjunto_Salas_Disc_GERAL;

         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            bool exige_Conjunto_Individual = false;

            // Checando se a sala em questão exige
            // a criação de um Conjunto de Salas só
            // pra ela. Ex.: Qdo uma sala, na verdade,
            // um laboratório possui pelo menos uma 
            // disciplina com a FLAG <eLab> marcada (TRUE).

            if ( it_Sala->ehLab() )
            {
               ITERA_GGROUP( it_Disc,
                  it_Sala->disciplinasAssociadas, Disciplina )
               {
                  // Procurando por, pelo menos, uma
                  // disciplina que possua a FLAG <eLab> marcada (TRUE)
                  if ( it_Disc->eLab() )
                  {
                     exige_Conjunto_Individual = true;
                     break;
                  }
               }
            }

            // Referência para algum dos GGroup de conjuntos de
            // salas (<conjunto_Salas_Disc_eLab> ou  <conjunto_Salas_Disc_GERAL>).
            GGroup< ConjuntoSala * > * gg_Cjt_Salas_Esc = &( conjunto_Salas_Disc_eLab );

            // Teste para escolher qual estrutura de dados
            // (<conjunto_Salas_Disc_eLab> ou  <conjunto_Salas_Disc_GERAL>) deve-se utilizar.
            if ( !exige_Conjunto_Individual )
            {
               gg_Cjt_Salas_Esc = &( conjunto_Salas_Disc_GERAL );
            }

            // Antes de criar um novo conjunto de salas (labs ou SA),
            // deve-se procurar por algum conjunto de salas existente
            // que represente a capacidade e o tipo da sala em questão.
            // Além disso, a diferença das disciplinas associadas de ambos 
            // tem que ser nula ( ou seja, todas as disciplinas que forem
            // associadas a sala em questão, tem de estar associadas ao
            // conjunto de salas encontrado, e vice versa ).
            bool encontrou_Conjunto_Compat = false;
			
			bool PERMITE_MAIS_DE_UMA_SALA_POR_CJT = false;	// TODO: tirar isso quando estiver estavel
			
			if ( PERMITE_MAIS_DE_UMA_SALA_POR_CJT ){

            ITERA_GGROUP( it_Cjt_Salas_Disc,
               ( *gg_Cjt_Salas_Esc ), ConjuntoSala )
            {
               // Se o conjunto de salas em questão representa a capacidade
               // da sala em questão. Estou testando tb se o conjunto de
               // salas representa o mesmo tipo da sala em questão. Acredito
               // que não seja necessário no caso em que estejamos lidando
               // APENAS com laboratórios. Como não sei qual GGroup está
               // sendo referenciado, testo os 2 casos pra lá.
               if (  it_Cjt_Salas_Disc->getCapacidadeRepr() == it_Sala->getCapacidade()
                     && it_Cjt_Salas_Disc->getTipoSalasRepr() == it_Sala->tipo_sala->getId() )
               {
                  // Checando se tem o mesmo tamanho.
                  bool mesmas_Disciplinas_Associadas = true;

                  if ( it_Cjt_Salas_Disc->disciplinas_associadas.size()
                           == it_Sala->disciplinasAssociadas.size() )
                  {
                     // Iterando sobre as disciplinas associadas da sala em questão.
                     ITERA_GGROUP( it_Disc_Assoc_Sala,
                        it_Sala->disciplinasAssociadas, Disciplina )
                     {
                        // Se encontrei alguma disciplina que não está
                        // associada ao conjunto de salas e  à sala em
                        // questão, paro o teste de compatibilidade
                        // entre a sala e o cjt em questão.
                        if ( it_Cjt_Salas_Disc->disciplinas_associadas.find( *it_Disc_Assoc_Sala )
                              == it_Cjt_Salas_Disc->disciplinas_associadas.end() )
                        {
                           mesmas_Disciplinas_Associadas = false;
                           break;
                        }
                     }
                  }
                  else
                  {
                     mesmas_Disciplinas_Associadas = false;
                  }

                  if ( mesmas_Disciplinas_Associadas )
                  {
                     (*it_Cjt_Salas_Disc)->addSala( **it_Sala );
					 			   
					 (*it_Sala)->setCjtSalaAssociado( *it_Cjt_Salas_Disc );
					 
                     // Adicionando os dias letivos ao conjunto de salas
                     ITERA_GGROUP( it_disc, it_Sala->disciplinasAssociadas, Disciplina )
                     {
						 Disciplina *disciplina = *it_disc;

                        GGroup< int > dias_fixados = retorna_foxacoes_dias_letivos( *it_disc );

                        if ( dias_fixados.size() == 0 )
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Sala->diasLetivos, int )
                           {
							   if ( disciplina->diasLetivos.find( *it_Dias_Letivos ) !=
								    disciplina->diasLetivos.end() )
							   {
									(*it_Cjt_Salas_Disc)->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
							   }
                           }
                        }
                        else
                        {
                           ITERA_GGROUP_N_PT( it_Dias_Letivos, dias_fixados, int )
                           {
							   if ( disciplina->diasLetivos.find( *it_Dias_Letivos ) !=
								    disciplina->diasLetivos.end() )
							   {
									(*it_Cjt_Salas_Disc)->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
							   }
                           }
                        }
                     }

                     // COMO AS DISCIPLINAS ASSOCIADAS SÃO AS MESMAS,
                     // NÃO HÁ NECESSIDADE DE ADICIONAR NENHUMA
                     // DISICIPLINA ASSOCIADA AO CONJUNTO DE SALAS EM QUESTÃO.
                     encontrou_Conjunto_Compat = true;

                     break;
                  }
               }
            }
			
			}
            
			if ( !encontrou_Conjunto_Compat )
            {
               ConjuntoSala * cjt_Sala = new ConjuntoSala();

               cjt_Sala->setId( idCjtSala );
               cjt_Sala->setCapacidadeRepr( it_Sala->getCapacidade() );
               cjt_Sala->setTipoSalasRepr( it_Sala->getTipoSalaId() );
			   cjt_Sala->setLab( it_Sala->ehLab() );

               cjt_Sala->addSala( **it_Sala );
			   (*it_Sala)->setCjtSalaAssociado( cjt_Sala );

               // Atualizando para o próximo id.
               ++idCjtSala;

               // Adicionando os dias letivos ao conjunto de salas
               ITERA_GGROUP( it_disc, it_Sala->disciplinasAssociadas, Disciplina )
               {
				   Disciplina *disciplina = *it_disc;

                  GGroup< int > dias_fixados = retorna_foxacoes_dias_letivos( *it_disc );

                  if ( dias_fixados.size() == 0 )
                  {
                     ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Sala->diasLetivos, int )
                     {
						if ( disciplina->diasLetivos.find( *it_Dias_Letivos ) !=
						     disciplina->diasLetivos.end() )
						{
							cjt_Sala->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
						}
                     }
                  }
                  else
                  {
                     ITERA_GGROUP_N_PT( it_Dias_Letivos, dias_fixados, int )
                     {
						if ( disciplina->diasLetivos.find( *it_Dias_Letivos ) !=
						     disciplina->diasLetivos.end() )
						{
							cjt_Sala->dias_letivos_disciplinas[ ( *it_disc ) ].add( *it_Dias_Letivos );
						}
                     }
                  }
               }

               // Associando as disciplinas ao conjunto.
               ITERA_GGROUP( it_Disc_Assoc_Sala,
                  it_Sala->disciplinasAssociadas, Disciplina )
               {
                  cjt_Sala->associaDisciplina( **it_Disc_Assoc_Sala );
               }

               // Adicionando ao respectivo conjunto.
               gg_Cjt_Salas_Esc->add( cjt_Sala );
            }
         }

         // AGORA QUE TENHO TODOS OS CONJUNTOS DE SALAS
         // CRIADOS, TENHO QUE ARMAZENA-LOS NA ESTRUTURA
         // <conjuntoSala> da Unidade em questão.

         ITERA_GGROUP( it_Cjt_Salas_Disc_Elab,
            conjunto_Salas_Disc_eLab, ConjuntoSala )
         {
            (*it_Unidade)->conjutoSalas.add( *it_Cjt_Salas_Disc_Elab );
         }

         ITERA_GGROUP( it_Cjt_Salas_Disc_GERAL,
            conjunto_Salas_Disc_GERAL, ConjuntoSala )
         {
            (*it_Unidade)->conjutoSalas.add( *it_Cjt_Salas_Disc_GERAL );
         }

         // std::cout << "Cod. Und.: " << it_Unidade->getCodigo() << std::endl;

         ITERA_GGROUP_LESSPTR( it_Cjt_Salas_Und,
            it_Unidade->conjutoSalas, ConjuntoSala )
         {
            // std::cout << "\tCod. Cjt. Sala: " << it_Cjt_Salas_Und->getId() << std::endl;

            std::map< int /*Id Sala*/, Sala * >::iterator 
               it_Salas_Cjt = it_Cjt_Salas_Und->salas.begin();

            for(; it_Salas_Cjt != it_Cjt_Salas_Und->salas.end();
               it_Salas_Cjt++ )
            {
               // std::cout << "\t\tCod. Sala: "
               //           << it_Salas_Cjt->second->getCodigo()
               //           << std::endl;
            }
         }
      }
   }
}

GGroup< int > ProblemDataLoader::retorna_foxacoes_dias_letivos( Disciplina * disciplina )
{
   GGroup< int > dias_letivos;
   dias_letivos.clear();

   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      if ( it_fixacao->disciplina != NULL
         && abs( it_fixacao->disciplina->getId() ) == abs( disciplina->getId() )
         && it_fixacao->getDiaSemana() >= 0 )
      {
         dias_letivos.add( it_fixacao->getDiaSemana() );
      }
   }

   return dias_letivos;
}

void ProblemDataLoader::estabeleceDiasLetivosBlocoCampus()
{
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		return;
	}

   // Analisar esse metodo e o de criacao de blocos curriculares.
   // Um bloco pode pertencer a mais de um campus !?
   ITERA_GGROUP_LESSPTR( it_Bloco_Curric, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_N_PT( it_Dia_Letivo, it_Bloco_Curric->diasLetivos, int )
      {
         if ( it_Bloco_Curric->campus->diasLetivos.find
            (*it_Dia_Letivo) != it_Bloco_Curric->campus->diasLetivos.end() )
         {
            problemData->bloco_Campus_Dias[ 
				std::make_pair( it_Bloco_Curric->getId(),
								it_Bloco_Curric->campus->getId() ) ].add( *it_Dia_Letivo );
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDisciplinasSalas()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itSala, itUnidade->salas, Sala )
         {
            ITERA_GGROUP( itDiscAssoc, itSala->disciplinasAssociadas, Disciplina )
            {
				std::pair< int, int > ids_Disc_Sala 
						( itDiscAssoc->getId(), itSala->getId() );

				// Adicionando informações referentes aos horários
				// comuns entre uma sala e uma disciplina para um dado dia.
				ITERA_GGROUP( itHorarioSala, itSala->horarios_disponiveis, Horario )
				{
					HorarioAula *h = itHorarioSala->horario_aula;
				
					ITERA_GGROUP_N_PT( itDiasSala, itHorarioSala->dias_semana, int )
					{
						if ( itDiscAssoc->possuiHorariosNoDia( h, h, *itDiasSala ) )
						{
							problemData->disc_Salas_Dias_HorariosAula
								[ ids_Disc_Sala ][ ( *itDiasSala ) ].add( h );

							problemData->disc_Salas_Dias[ ids_Disc_Sala ].add( *itDiasSala );
						}
					}
				}

				/*
               ITERA_GGROUP_N_PT( itDiasDisc, itDiscAssoc->diasLetivos, int )
               {
                  // Se o dia letivo da disciplina é também um
                  // dia letivo da sala em questão, adiciona-se
                  // ao map <disc_Salas_Dias> o dia em comum.
                  if ( itSala->diasLetivos.find( *itDiasDisc )
                     != itSala->diasLetivos.end() )
                  {
						std::pair< int, int > ids_Disc_Sala 
						( itDiscAssoc->getId(), itSala->getId() );

						problemData->disc_Salas_Dias[ ids_Disc_Sala ].add( *itDiasDisc );

						// Adicionando informações referentes aos horários
						// comuns entre uma sala e uma disciplina para um dado dia.
						ITERA_GGROUP( itHorarioSala, itSala->horarios_disponiveis, Horario )
						{ 
							// Checando o dia em questão para a sala
							if ( itHorarioSala->dias_semana.find( *itDiasDisc )
								!= itHorarioSala->dias_semana.end() )
							{
								ITERA_GGROUP_LESSPTR( itHorarioDisc, itDiscAssoc->horarios, Horario )
								{
									// Checando o dia em questão para a disciplina
									if ( itHorarioDisc->dias_semana.find( *itDiasDisc )
										!= itHorarioDisc->dias_semana.end() )
									{
										// Checando se é um horário comum entre a disc e a sala.
										if ( itHorarioSala->horario_aula == itHorarioDisc->horario_aula )
										{
											problemData->disc_Salas_Dias_HorariosAula
												[ ids_Disc_Sala ][ ( *itDiasDisc ) ].add(
												itHorarioSala->horario_aula );

											break;
										}
									}
								}
							}
						}
                  }
               }
			   */
            }
         }
      }
   }
}

void ProblemDataLoader::estabeleceDiasLetivosDiscCjtSala()
{
   // Os dias letivos das disciplinas em relação aos
   // conjuntos de salas são obtidos via união dos
   // dias letivos das disciplinas em relação às salas
   // pertencentes ao conjunto de salas em questão.

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         // p tds conjuntos de salas de um campus
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            // p tds as discAssoc de um conjunto
            ITERA_GGROUP_LESSPTR( itDiscAssoc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               std::map<int/*Id Sala*/,Sala*>::iterator itSala =
                  itCjtSala->salas.begin();

               for(; itSala != itCjtSala->salas.end();
                  itSala++ )
               {
                  std::pair< int /*idDisc*/, int /*idSala*/> ids_Disc_Sala 
                     ( itDiscAssoc->getId(), itSala->second->getId() );

                  // Se a disciplina se relaciona com a sala em questao.
                  // Como estamos  lidando com um conjunto de salas,
                  // podemos ter o caso em que uma disciplina
                  // é associada a uma sala do conjunto e a outra não.
                  if ( problemData->disc_Salas_Dias.find(ids_Disc_Sala) !=
                     problemData->disc_Salas_Dias.end() )
                  {
                     ITERA_GGROUP_N_PT( itDiasLetDisc,
                        problemData->disc_Salas_Dias[ ids_Disc_Sala ], int )
                     {
                        problemData->disc_Conjutno_Salas__Dias[ std::make_pair< int, int >
                           ( itDiscAssoc->getId(), itCjtSala->getId() ) ].add( *itDiasLetDisc );
                     }
                  }
               }
            }
         }
      }
   }
}

void ProblemDataLoader::calculaCredsLivresSalas()
{
   //ITERA_GGROUP( itCampus, problemData->campi, Campus )
   //{
   //   ITERA_GGROUP( itUnidade, itCampus->unidades, Unidade )
   //   {
   //      ITERA_GGROUP( itSala, itUnidade->salas, Sala )
   //      {
   //         for ( int dia = 0; dia < 8; dia++ )
   //         {
   //	(*itSala)->credsLivres.push_back(0);
   //}

   //         ITERA_GGROUP( itCredsDisp, itSala->creditos_disponiveis, CreditoDisponivel )
   //         {
   //	(*itSala)->credsLivres.at( itCredsDisp->dia_semana ) = itCredsDisp->max_creditos;
   //}
   //      }
   //   }
   //}
}

void ProblemDataLoader::estabeleceDiasLetivosProfessorDisciplina()
{
   problemData->disc_Dias_Prof_Tatico.clear();
   problemData->usarProfDispDiscTatico = false;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
		ITERA_GGROUP_LESSPTR( it_prof, itCampus->professores, Professor )
		{							  
			ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
			{
				int discId = it_mag->getDisciplinaId();
				Disciplina *disc = problemData->refDisciplinas[ discId ];

				problemData->mapDiscProfsHabilit[disc].add( *it_prof );

				ITERA_GGROUP( it_hor, it_prof->horarios, Horario )
				{
					GGroup< int >::iterator itDiasLetDisc =
					disc->diasLetivos.begin();

					for(; itDiasLetDisc != disc->diasLetivos.end(); itDiasLetDisc++ )
					{
						if ( it_hor->dias_semana.find( *itDiasLetDisc )
							!= it_hor->dias_semana.end() )
						{
							std::pair< int, int > ids_Prof_Disc 
								( it_prof->getId(), discId );

							problemData->prof_Disc_Dias[ ids_Prof_Disc ].add( *itDiasLetDisc );
							problemData->disc_Dias_Prof_Tatico[ discId ].add( *itDiasLetDisc );
							problemData->usarProfDispDiscTatico = true;
						}					
					}
				}
			}
		}
   }
}

void ProblemDataLoader::combinacaoDivCreditos()
{
	bool espelhar = true;

   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
	   Disciplina *disciplina = *itDisc;
	   
	   // --------------------------------------------------------------------------------
	   // Elimina regra específica de divisão de créditos, caso o parâmetro esteja desabilitado.
	   if ( ! problemData->parametros->regrasEspecificasDivisaoCredito )
	   {
		   if ( disciplina->divisao_creditos.size() != 0 )
		   {
			   disciplina->divisao_creditos.deleteElements();
			   disciplina->divisao_creditos.clear();
		   }
	   }
	   
	   // --------------------------------------------------------------------------------
	   // Se a disciplina não tem regra de divisão de créditos especificada, procura regras
	   // gerais com o mesmo número total de créditos
	   if ( disciplina->divisao_creditos.size() == 0 )
	   {
		   if ( problemData->parametros->regrasGenericasDivisaoCredito )
		   {
			    auto it_Creds_Regras = problemData->creds_Regras.find( disciplina->getTotalCreditos() );
				if ( it_Creds_Regras != problemData->creds_Regras.end() )
				{
					ITERA_GGROUP_LESSPTR( itDiv, it_Creds_Regras->second, DivisaoCreditos )
						disciplina->divisao_creditos.add( *itDiv );
				}
		   }
	   }
	   
	   // --------------------------------------------------------------------------------
	   if ( disciplina->divisao_creditos.size() == 0 )
	   {
		   if ( disciplina->getTotalCreditos() == 1 )
		   {
				DivisaoCreditos * divisao = new DivisaoCreditos(1,0,0,0,0,0,0);
				disciplina->divisao_creditos.add( divisao );
				problemData->regras_div.add( divisao );
		   }
		   else
		   {
			   stringstream msg;
			   msg << "Sem regra de divisao de " << disciplina->getTotalCreditos()
				   << " creditos para a disciplina " << disciplina->getId();
			   CentroDados::printWarning( "void ProblemDataLoader::combinacaoDivCreditos()", msg.str() );
			   continue;
		   }
	   }
	   
	   // --------------------------------------------------------------------------------
	   // Calcula as combinações

	   std::set< DivCredType > combinacoes;

	   ITERA_GGROUP_LESSPTR( itDiv, disciplina->divisao_creditos, DivisaoCreditos )
	   {
		    DivisaoCreditos *divisao = *itDiv;
						
			std::map< int/*dia*/, int/*ncred*/ > mapDiaNCred;

			for ( int i = 0; i < 7; i++ )
			{
				mapDiaNCred[i+2] = divisao->dia[i];
			}
			
			// Para cada regra de divisão de creditos podem existir a original e mais 6
			for ( int k = 0; k <= 6; k++ )
			{
				std::map< int/*dia*/, int/*ncred*/ > mapDiaNCredShift;

				for ( auto it = mapDiaNCred.begin(); it != mapDiaNCred.end(); it++ )
				{
					std::map< int/*dia*/, int/*ncred*/ >::iterator itMinus1;
					if ( it == mapDiaNCred.begin() )
						itMinus1 = std::prev(mapDiaNCred.end());
					else
						itMinus1 = std::prev(it);

					mapDiaNCredShift[it->first] = itMinus1->second;
				}

				mapDiaNCred.clear();
				mapDiaNCred = mapDiaNCredShift;

				bool adicionar = true;

				// Verifica se as regras de divisão de créditos criadas possui dias válidos
				for ( auto itDia = mapDiaNCred.begin(); itDia != mapDiaNCred.end(); itDia++ )
				{
					if ( itDia->second > 0 )
					if ( disciplina->diasLetivos.find( itDia->first ) ==
						 disciplina->diasLetivos.end() )
					{
						adicionar = false;
						break;
					}
				}

				if ( adicionar )
				{
					DivCredType div( mapDiaNCred );
					combinacoes.insert(div);
				}
			}
	   }
	   
	   // --------------------------------------------------------------------------------
	   // Espelha as combinações calculadas anteriormente

	   if ( espelhar )
	   {
		   std::set< DivCredType > espelhados;

		   // Constroi as combinações espelhadas
		   auto itK = combinacoes.begin();
		   for ( ; itK != combinacoes.end(); itK++ )
		   {
			   DivCredType original = (*itK);
			   		   
			   DivCredType espelho; 
			   
			   bool adicionar=true;

			   bool impar = ((int)original.diaNCred.size()) % 2;

			   // Espelha
			   auto itEsq = original.diaNCred.begin();
			   auto itDir = original.diaNCred.end();
			   bool cruzou = (itEsq == itDir);
			   itDir--;

			   while ( !cruzou && adicionar )			   
			   {
				   int diaEsq = itEsq->first;
				   int ncredEsq = itEsq->second;				   
				   int diaDir = itDir->first;
				   int ncredDir = itDir->second;
				   
				   if ( ncredDir > 0 )	// Se ncred > 0
				   {
					   if ( disciplina->diasLetivos.find( diaEsq ) !=
							disciplina->diasLetivos.end() )	// Se a disciplina possui o dia
					   {
						   espelho.diaNCred[diaEsq] = ncredDir;
					   }
					   else
					   {
						   adicionar=false;
					   }
				   }
				   else espelho.diaNCred[diaEsq] = ncredDir;

				   if ( ncredEsq > 0 )	// Se ncred > 0
				   {
					   if ( disciplina->diasLetivos.find( diaDir ) !=
							disciplina->diasLetivos.end() )	// Se a disciplina possui o dia
					   {
						   espelho.diaNCred[diaDir] = ncredEsq;
					   }
					   else
					   {
						   adicionar=false;
					   }
				   }
				   else espelho.diaNCred[diaDir] = ncredEsq;
				   
				   itEsq++;
				   itDir--;
				   
				   if (impar) cruzou = (std::prev(itEsq) == std::next(itDir));
				   else cruzou = (std::prev(itEsq) == itDir);
			   }
			   
			   // Salva o espelho
			   if ( adicionar )
			   {
				   if ( espelhados.find( espelho) == espelhados.end() )
					   espelhados.insert( espelho );
			   }
		   }

		   // Adiciona os espelhos
		   for ( auto itEspelho = espelhados.begin(); itEspelho != espelhados.end(); itEspelho++ )
		   {
			   if ( combinacoes.find( *itEspelho ) == combinacoes.end() )
				   combinacoes.insert(*itEspelho);
		   }
	   }

	   // --------------------------------------------------------------------------------
	   // Adiciona as combinações na disciplina
	   for ( auto itK = combinacoes.begin(); itK != combinacoes.end(); itK++ )
	   {
		   DivCredType div = (*itK);

		   std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > combK;

		   for ( auto itMapDiaCred = div.diaNCred.begin(); itMapDiaCred != div.diaNCred.end(); itMapDiaCred++ )
		   {			   
			   combK.push_back( std::make_pair(itMapDiaCred->first, itMapDiaCred->second) );
		   }

		   disciplina->combinacao_divisao_creditos.push_back( combK );
	   }
   }

   problemData->imprimeCombinacaoCredsDisciplinas();
}

/*
	Define um map de compatibilidade e incompatibilidade entre 2 cursos.
	É setado mesmo que não seja permitido compartilhamento.
*/
void ProblemDataLoader::disciplinasCursosCompativeis()
{
      ITERA_GGROUP_LESSPTR( it_fix_curso, problemData->cursos, Curso )
      {
         ITERA_GGROUP_LESSPTR( it_alt_curso, problemData->cursos, Curso )
         {
            std::pair< Curso *, Curso * > idCursos = 
               std::make_pair( ( *it_fix_curso ), ( *it_alt_curso ) );

            problemData->compat_cursos[ idCursos ] = true;
         }
      }

	   ITERA_GGROUP_LESSPTR( it_fix_curso, problemData->cursos, Curso )
	   {
		  int curso1Id = it_fix_curso->getId();

		  GGroup< GGroup< int > * >::iterator it_list_compat =
			 problemData->parametros->nao_permite_compart_turma.begin();
		  
		  for (; it_list_compat != problemData->parametros->nao_permite_compart_turma.end(); it_list_compat++ )
		  {
			 if ( it_list_compat->find( curso1Id ) != it_list_compat->end() )
			 {
				ITERA_GGROUP_INIC_LESSPTR( it_alt_curso, it_fix_curso, problemData->cursos, Curso )
				{
				   int curso2Id = it_alt_curso->getId();
				   if ( curso1Id == curso2Id ) continue;

				   if ( it_list_compat->find( curso2Id ) != it_list_compat->end() )
				   {
					  std::pair<Curso*, Curso*> parCursos = std::make_pair( *it_fix_curso, *it_alt_curso );
					  
					  problemData->compat_cursos.erase( parCursos );
					  std::swap(parCursos.first, parCursos.second);
					  problemData->compat_cursos.erase( parCursos );
				   }
				}
			 }
		  }
	   }

}


void ProblemDataLoader::preencheDisciplinasDeCursosCompativeis()
{
	problemData->preencheCursosCompDisc();
}

void ProblemDataLoader::preencheDisciplinasDeOfertasCompativeis()
{
	problemData->preencheOftsCompDisc();
}

bool ProblemDataLoader::contemFixacao(
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes,
   Professor * professor, Disciplina * disciplina,
   Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = NULL;

   ITERA_GGROUP_LESSPTR( it_fixacao, fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );

      if ( professor != NULL )
      {
         if ( fixacao->professor == NULL
            || professor->getId() != fixacao->professor->getId() )
         {
            continue;
         }
      }

      if ( disciplina != NULL )
      {
         if ( fixacao->disciplina == NULL
            || disciplina->getId() != fixacao->disciplina->getId() )
         {
            continue;
         }
      }

      if ( sala != NULL )
      {
         if ( fixacao->sala == NULL
            || sala->getId() != fixacao->sala->getId() )
         {
            continue;
         }
      }

      if ( dia_semana >= 0 )
      {
         if ( fixacao->getDiaSemana() < 0
            || dia_semana != fixacao->getDiaSemana() )
         {
            fixacao->setDiaSemana( dia_semana );
         }
      }

      if ( horario_aula != NULL )
      {
         if ( fixacao->horario_aula == NULL
            || horario_aula->getId() != fixacao->horario_aula->getId() )
         {
            continue;
         }
      }

      // Chegando aqui, verificamos que a fixação foi encontrada
      return true;
   }

   return false;
}

bool ProblemDataLoader::contemFixacaoExato(
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes,
   Professor * professor, Disciplina * disciplina,
   Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = NULL;

   ITERA_GGROUP_LESSPTR( it_fixacao, fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );

      if ( professor != NULL || fixacao->professor != NULL )
      {
         if ( ( professor != NULL && fixacao->professor == NULL )
            || ( professor == NULL && fixacao->professor != NULL )
            || ( professor->getId() != fixacao->professor->getId() ) )
         {
            continue;
         }
      }

      if ( disciplina != NULL || fixacao->disciplina != NULL )
      {
         if ( ( disciplina != NULL && fixacao->disciplina == NULL )
            || ( disciplina == NULL && fixacao->disciplina != NULL )
            || ( disciplina->getId() != fixacao->disciplina->getId() ) )
         {
            continue;
         }
      }

      if ( sala != NULL || fixacao->sala != NULL )
      {
         if ( ( sala != NULL && fixacao->sala == NULL )
            || ( sala == NULL && fixacao->sala != NULL )
            || ( sala->getId() != fixacao->sala->getId() ) )
         {
            continue;
         }
      }

      if ( dia_semana >= 0 || fixacao->getDiaSemana() >= 0 )
      {
         if ( ( dia_semana >= 0 && fixacao->getDiaSemana() < 0 )
            || ( dia_semana < 0 && fixacao->getDiaSemana() >= 0 )
            || ( dia_semana != fixacao->getDiaSemana() ) )
         {
            continue;
         }
      }

      if ( horario_aula != NULL || fixacao->horario_aula != NULL )
      {
         if ( ( horario_aula != NULL && fixacao->horario_aula == NULL )
            || ( horario_aula == NULL && fixacao->horario_aula != NULL )
            || ( horario_aula->getId() != fixacao->horario_aula->getId() ) )
         {
            continue;
         }
      }

      // Chegando aqui, verificamos que a fixação foi encontrada
      return true;
   }

   return false;
}

Fixacao * ProblemDataLoader::criaFixacao(
   int id_fixacao, Professor * professor, Disciplina * disciplina,
   Sala * sala, int dia_semana, HorarioAula * horario_aula )
{
   Fixacao * fixacao = new Fixacao();
   fixacao->setId( id_fixacao );

   if ( professor != NULL )
   {
      fixacao->setProfessorId( professor->getId() );
      fixacao->professor = professor;
   }

   if ( disciplina != NULL )
   {
      fixacao->setDisciplinaId( disciplina->getId() );
      fixacao->disciplina = disciplina;
   }

   if ( sala != NULL )
   {
      fixacao->setSalaId( sala->getId() );
      fixacao->sala = sala;
   }

   if ( dia_semana >= 0 )
   {
      fixacao->setDiaSemana( dia_semana );
   }

   if ( horario_aula != NULL )
   {
      fixacao->setHorarioAulaId( horario_aula->getId() );
      fixacao->horario_aula = horario_aula;
   }

   return fixacao;
}

// Realiza a separação da fixações por tipo de fixação
void ProblemDataLoader::relacionaFixacoes()
{
   GGroup< Fixacao *, LessPtr< Fixacao > > novas_fixacoes;

   int id_fixacao = -1;
   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      if ( it_fixacao->getId() > id_fixacao )
      {
         id_fixacao = it_fixacao->getId();
      }
   }
   id_fixacao++;

   ITERA_GGROUP_LESSPTR( it_fixacao, this->problemData->fixacoes, Fixacao )
   {
      Fixacao * fixacao = ( *it_fixacao );

      //--------------------------------------------------------------------------
      // TÁTICO
      // Apenas disciplina/sala
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Sala,
            NULL, fixacao->disciplina, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Sala.add( nova_fixacao );
         }
      }
      // Apenas disciplina/sala/dia/horario
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL
         && fixacao->getDiaSemana() > 0 && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Sala_Dia_Horario,
            NULL, fixacao->disciplina, fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina,
               fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Sala_Dia_Horario.add( nova_fixacao );
         }
      }
      // Apenas disciplina/dia/horario
      if ( fixacao->disciplina != NULL && fixacao->getDiaSemana() > 0
         && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Disc_Dia_Horario,
            NULL, fixacao->disciplina, NULL, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, NULL, fixacao->disciplina,
               NULL, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Disc_Dia_Horario.add( nova_fixacao );
         }
      }
      //--------------------------------------------------------------------------
      // OPERACIONAL
      // professor/disciplina/sala/dia/horário
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->sala != NULL && fixacao->getDiaSemana() > 0
         && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Sala_Dia_Horario,
            fixacao->professor, fixacao->disciplina, fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina,
               fixacao->sala, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Sala_Dia_Horario.add( nova_fixacao );
         }
      }
      // professor/disciplina/dia/horário
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->getDiaSemana() > 0 && fixacao->horario_aula != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Dia_Horario,
            fixacao->professor, fixacao->disciplina, NULL, fixacao->getDiaSemana(), fixacao->horario_aula ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina,
               NULL, fixacao->getDiaSemana(), fixacao->horario_aula );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Dia_Horario.add( nova_fixacao );
         }
      }
      // professor/disciplina
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc,
            fixacao->professor, fixacao->disciplina, NULL, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina, NULL, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc.add( nova_fixacao );
         }
      }
      // professor/disciplina/sala
      if ( fixacao->professor != NULL && fixacao->disciplina != NULL
         && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Disc_Sala, fixacao->professor,
            fixacao->disciplina, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, fixacao->disciplina, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Disc_Sala.add( nova_fixacao );
         }
      }
      // professor/sala
      if ( fixacao->professor != NULL && fixacao->sala != NULL )
      {
         if ( !contemFixacao( this->problemData->fixacoes_Prof_Sala,
            fixacao->professor, NULL, fixacao->sala, -1, NULL ) )
         {
            Fixacao * nova_fixacao
               = criaFixacao( id_fixacao, fixacao->professor, NULL, fixacao->sala, -1, NULL );

            id_fixacao++;
            novas_fixacoes.add( nova_fixacao );
            this->problemData->fixacoes_Prof_Sala.add( nova_fixacao );
         }
      }
      //--------------------------------------------------------------------------

      //--------------------------------------------------------------------------
      // Adiciona mais um crédito fixado da
      // disciplina atual no dia da semana correspondente
      if ( fixacao->disciplina != NULL && fixacao->getDiaSemana() >= 0 )
      {
         std::pair< Disciplina *, int > disciplina_dia
            = std::make_pair( fixacao->disciplina, fixacao->getDiaSemana() );

         this->problemData->map_Discicplina_DiaSemana_CreditosFixados[ disciplina_dia ]++;
      }

      // Informa a fixação entre o par disciplina/sala
      if ( fixacao->disciplina != NULL && fixacao->sala != NULL )
      {
         this->problemData->map_Discicplina_Sala_Fixados[ fixacao->disciplina ] = fixacao->sala;
      }

      // Preenche a estrutura <fixacoesProfDisc>.
      if ( fixacao->professor && fixacao->disciplina )
      {
         std::pair< Professor *, Disciplina * > chave = std::make_pair(
            fixacao->professor, fixacao->disciplina );

         problemData->fixacoesProfDisc[ chave ].add( fixacao );
      }
      //--------------------------------------------------------------------------
   }

   // Adiciona todas as restrições criadas acima no conjunto de fixações
   ITERA_GGROUP_LESSPTR( it_fixacao, novas_fixacoes, Fixacao )
   {
      if ( !contemFixacaoExato( problemData->fixacoes, it_fixacao->professor, it_fixacao->disciplina,
         it_fixacao->sala, it_fixacao->getDiaSemana(), it_fixacao->horario_aula ) )
      {
         problemData->fixacoes.add( ( *it_fixacao ) );
      }
   }
}

void ProblemDataLoader::removeFixacoesComDisciplinasSubstituidas()
{
	bool recomecar = true;

	// Remove todas as fixações referentes a disciplinas que foram substituídas
	while ( recomecar )
	{
	   recomecar = false;

	   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
	   {
		   int discId = it_fixacao->getDisciplinaId();

		   // Para fixações com disciplinas
		   if ( discId != -1 )
		   {
			   Disciplina *d = problemData->refDisciplinas[ discId ];

			   // Se a disciplina d tiver sido substituída
			   if ( problemData->mapDiscSubstituidaPor.find( d ) !=
					problemData->mapDiscSubstituidaPor.end() &&
					!problemData->ehSubstituta( d ) )
			   {
				   // Remove a fixação referente a d
				   GGroup< Fixacao *, LessPtr< Fixacao > >::iterator it = problemData->fixacoes.remove( *it_fixacao );
				   recomecar = true;
				   break;
			   }
		   }
	   }
	}
}

void ProblemDataLoader::corrigeFixacoesComDisciplinasSalas()
{
	// Relaciona corretamente as fixações de salas para disciplinas práticas
	ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
	{
		int discId = it_fixacao->getDisciplinaId();
		int salaId = it_fixacao->getSalaId();
		
		Sala * sala = it_fixacao->sala;

		// Para fixações com disciplinas e salas
		if ( discId != -1 && salaId != -1)
		{
			int credsP = it_fixacao->disciplina->getCredPraticos();

			if ( sala->tipo_sala->getNome() == "Laboratório" &&
				 it_fixacao->disciplina->getCredPraticos() == 0 )
			{
				Disciplina *discPratica = problemData->refDisciplinas[ -discId ];

				if ( discPratica != NULL )
				{
					it_fixacao->disciplina = discPratica;
					it_fixacao->setDisciplinaId( -discId );				
				}
				else
				{
					std::cout << "\nAtencao em ProblemDataLoader::corrigeFixacoesComDisciplinasSalas()" << endl;
					std::cout << "Fixacao da disciplina teorica " << discId << "no laboratorio " << salaId << endl;
				}
			}
		}
	}
}

// Quando uma disciplina tiver um dia da semana fixado, devo
// remover os demais dias da semana do seu conjunto de dias letivos
void ProblemDataLoader::verificaFixacoesDiasLetivosDisciplinas()
{
   Fixacao * fixacao = NULL;
   Disciplina * disciplina = NULL;
   ConjuntoSala * conjunto_sala = NULL;

   int dia_semana = 0;
   bool encontrou_disciplina = false;

   ITERA_GGROUP_LESSPTR( it_fixacao,
      this->problemData->fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );
      disciplina = ( fixacao->disciplina );
      dia_semana = ( fixacao->getDiaSemana() );

      // Verifica se o dia da semana e a disciplina foram fixados
      if ( disciplina != NULL && dia_semana >= 0 )
      {
         // Como a disciplina atual possui um dia da semana fixado,
         // procuro co conjunto de sala ao qual ela está relacionada
         encontrou_disciplina = false;
         GGroup< Campus *, LessPtr< Campus > >::iterator itCampus
            = problemData->campi.begin();
         for (; itCampus != problemData->campi.end() && !encontrou_disciplina;
            itCampus++ )
         {
            GGroup< Unidade *, LessPtr< Unidade > >::iterator itUnidade
               = itCampus->unidades.begin();
            for (; itUnidade != itCampus->unidades.end() && !encontrou_disciplina;
               itUnidade++ )
            {
               GGroup< ConjuntoSala *, LessPtr< ConjuntoSala > >::iterator itCjtSala
                  = itUnidade->conjutoSalas.begin();
               for (; itCjtSala != itUnidade->conjutoSalas.end() && !encontrou_disciplina;
                  itCjtSala++ )
               {
                  conjunto_sala = ( *itCjtSala );

                  std::map< Disciplina *, GGroup< int > >::iterator
                     it_disc_dias = conjunto_sala->dias_letivos_disciplinas.begin();
                  for (; it_disc_dias != conjunto_sala->dias_letivos_disciplinas.end();
                     it_disc_dias++ )
                  {
                     // Procura pela disciplina da fixação no conjunto de salas
                     if ( it_disc_dias->first->getId() == disciplina->getId() )
                     {
                        // Deixa apenas o dia letivo fixado no
                        // conjunto de dias letivos da disciplina
                        GGroup< int > * dias_letivos = &( it_disc_dias->second );
                        dias_letivos->clear();
                        dias_letivos->add( dia_semana );
                        encontrou_disciplina = true;
                        break;
                     }
                  }
               }
            }
         }
      }
   }
}

//template< class T > 
//void ProblemDataLoader::find_and_set(
//   int id, GGroup< T *, LessPtr > & haystack,
//   T * & needle, bool print = false )
//{
//   T * finder = new T;
//   finder->setId( id );
//
//   // Versão lenta... Entender o porquê depois
//
//#ifndef WIN32
//   typename GGroup< T * >::iterator it_g = haystack.begin();
//#else
//   GGroup< T * >::iterator it_g = haystack.begin();
//#endif
//
//   while ( it_g != haystack.end()
//      && it_g->getId() != finder->getId() )
//   {
//      ++it_g;
//   }
//   // FIM
//
//   if ( it_g != haystack.end() )
//   {
//      needle = ( *it_g );
//
//      if ( print )
//      {
//         std::cout << "Found " << id << std::endl;
//      }
//   }
//   else
//   {
//      std::cout << "Warnning: Problema na funcao"
//                << "FindAndSet do ProblemDataLoader." << std::endl;
//
//      exit( 1 );
//   }
//
//   delete finder;
//}

template< class T > 
void ProblemDataLoader::find_and_set_lessptr(
   int id, GGroup< T *, LessPtr< T > > & haystack,
   T * & needle, bool print = false )
{
   //T * finder = new T;
   //finder->setId( id );

   // Versão lenta... Entender o porquê depois
#ifndef WIN32
   typename GGroup< T *, LessPtr< T > >::iterator it_g = haystack.begin();
#else
   GGroup< T *, LessPtr< T > >::iterator it_g = haystack.begin();
#endif
   //while ( it_g != haystack.end()
   //   && it_g->getId() != finder->getId() )
   while ( it_g != haystack.end()
      && it_g->getId() != id )
   {
      ++it_g;
   }
   // FIM

   if ( it_g != haystack.end() )
   {
      needle = ( *it_g );

      if ( print )
      {
         std::cout << "Found " << id << std::endl;
      }
   }
   else
   {
      std::cout << "Warning: Problema na funcao"
                << "FindAndSet do ProblemDataLoader." << std::endl;
	  std::cout << " Id " << id << " not found." << std::endl;
      exit( 1 );
   }

   //delete finder;
}

void ProblemDataLoader::substituiDisciplinasEquivalentes()
{
    bool atualizar_demandas = false;

    ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
    {
		Curso *curso = *it_curso;

        ITERA_GGROUP_LESSPTR( it_curriculo, curso->curriculos, Curriculo )
        {
			Curriculo *curriculo = *it_curriculo;

			map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_periodo_disc
				= curriculo->disciplinas_periodo.begin();
			
			bool reiniciar = false;
			
			// Para cada disciplina do 'curriculo', procuro se devo
			// substituir por alguma disciplina equivalente
			while ( it_periodo_disc != curriculo->disciplinas_periodo.end() )
			{
				Disciplina *discAntiga = it_periodo_disc->first;
				int periodo = it_periodo_disc->second;

				std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > >::iterator
					itMapEquiv = problemData->mapDiscSubstituidaPor.find( discAntiga );

				if ( itMapEquiv != problemData->mapDiscSubstituidaPor.end() ) // Se a disciplina tiver uma equivalente
				if ( !problemData->ehSubstitutaDe( discAntiga, std::make_pair(curso, curriculo) ) ) // E se não já for uma substituta
				{

				//-----------------------------------------------------------------------------
				// Substituir disciplinas equivalentes
					Disciplina * discNova = problemData->mapDiscSubstituidaPor[ discAntiga ];

					atualizar_demandas = true;

					// Troca a disciplina e mantém o período
					curriculo->disciplinas_periodo.erase( discAntiga );
					curriculo->disciplinas_periodo[discNova] = periodo;

					reiniciar = true;
					
                    // Adiciona a 'discAntiga' no conjunto de disciplinas que foram substituídas por 'discNova'
					std::pair< Curso*, Curriculo* > parCursoCurr = std::make_pair( curso, curriculo );
                    problemData->mapGroupDisciplinasSubstituidas[ parCursoCurr ][ discNova ].add( discAntiga );

					if ( problemData->mapGroupDisciplinasSubstituidas[ parCursoCurr ][ discNova ].size() > 1 )
					{
						std::cout << "\nATENCAO em void ProblemDataLoader::substituiDisciplinasEquivalentes():"
								  << " a disciplina " << discNova->getId() << " esta substituindo "
								  << "mais de uma disciplina no curriculo " << curriculo->getId() << "\n";
					}

					it_periodo_disc = curriculo->disciplinas_periodo.begin();
				 //-----------------------------------------------------------------------------
				 }

				 if ( reiniciar )
				 {
					 it_periodo_disc = curriculo->disciplinas_periodo.begin();
					 reiniciar = false;
				 }
				 else
				 {
					 it_periodo_disc++;
				 }

			}
        }
    }

	// Caso alguma disciplina tenha sido
	// substituída, devo substituir também as demandas
	if ( atualizar_demandas )
	{
		atualizaOfertasDemandas();
	}
}

void ProblemDataLoader::atualizaOfertasDemandas()
{
    int id_demanda = problemData->retornaMaiorIdDemandas();
    id_demanda++;

    // A partir de cada demanda existente de disciplinas que
    // forem substituídas, troca-se a disciplina pela substituta
    ITERA_GGROUP_LESSPTR( itDemanda, problemData->demandasTotal, Demanda )
    {
		if ( problemData->mapDiscSubstituidaPor.find( itDemanda->disciplina ) ==
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue; // Disciplina que não foi substituída
		}
			
		Disciplina *discAntiga = (*itDemanda)->disciplina;
		Disciplina *discNova = problemData->mapDiscSubstituidaPor[ discAntiga ];
			  
		(*itDemanda)->setDisciplinaId( discNova->getId() );
		(*itDemanda)->disciplina = discNova;
	}
}



void ProblemDataLoader::referenciaCursos_CurricCalendarios()
{ 
   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      find_and_set_lessptr( it_curso->getTipoId(),
         problemData->tipos_curso,
         it_curso->tipo_curso, false );

      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
         find_and_set_lessptr( it_curriculo->getSemanaLetivaId(),
            problemData->calendarios,
            it_curriculo->calendario, false );
      }
   }
}

void ProblemDataLoader::referenciaCalendariosCurriculos()
{    
   // Referencia as semanas letivas (calendarios) de cada curriculo
   // Tem que ser chamada depois das substituições de equivalência!

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
      {
		  map < Disciplina*, int, LessPtr< Disciplina > >::iterator
			  itPeriodoDisc = it_curriculo->disciplinas_periodo.begin();
			  
		  Calendario *calendarioCurr = it_curriculo->calendario;		  
		  
		  for ( ; itPeriodoDisc != it_curriculo->disciplinas_periodo.end(); itPeriodoDisc++ )
		  {
			  int periodo = itPeriodoDisc->second;
			  Disciplina *disciplina = itPeriodoDisc->first;
			  
			  if ( disciplina->substituidaPor() != NULL )
			  {
				  // Caso a disciplina tenha sido substituída, verifica se o calendario do curriculo corrente
				  // está presente na disciplina substituta. Se não estiver, acrescenta na lista de semana letivas
				  // do curriculo/periodo todos os calendarios associados à substituta.
				  GGroup< Calendario*, LessPtr<Calendario> > calendarios = disciplina->getCalendarios();
				  if ( calendarios.find( calendarioCurr ) == calendarios.end() )
				  {
					  ITERA_GGROUP_LESSPTR( it_calend, calendarios, Calendario )
						 it_curriculo->semanasLetivas[ periodo ].add( *it_calend );
				  }
			  }
			  it_curriculo->semanasLetivas[ periodo ].add( calendarioCurr );
		  }
      }
   }

}

void ProblemDataLoader::referenciaDisciplinasEquivalentes()
{
	if ( ! problemData->parametros->considerar_equivalencia )
	{
		return;
	}

	// Não considera transitividade entre equivalências
	if ( ! problemData->EQUIV_TRANSITIVIDADE )
	{
		// Preenche mapDiscSubstituidaPor
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			Disciplina *discNova = *itDisc;

			ITERA_GGROUP_LESSPTR( itDiscAntiga, discNova->discEquivalentes, Disciplina )
			{
				Disciplina *discAntiga = *itDiscAntiga;
					
				// Se a disciplina antiga não possuir uma substituição no map (não for uma chave do map)
				if ( problemData->mapDiscSubstituidaPor.find( discAntiga ) ==
					 problemData->mapDiscSubstituidaPor.end() )
				{
					problemData->mapDiscSubstituidaPor[ discAntiga ] = discNova;
				}
				else
				{
					std::cout << "\nAtencao em ProblemDataLoader::referenciaDisciplinasEquivalentes():";
					std::cout << "\nJa existe uma substituicao da disciplina " << discAntiga->getId()
							  << " pela disciplina " <<  problemData->mapDiscSubstituidaPor[ discAntiga ]->getId()
							  << ". Nao eh possivel substitui-la pela disciplina " << discNova->getId();
				}
			}
		}
	}

	// Considera transitividade entre equivalências
	else
	{
		// Preenche mapDiscSubstituidaPor
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			Disciplina *discNova = *itDisc;

			ITERA_GGROUP_LESSPTR( itDiscAntiga, discNova->discEquivalentes, Disciplina )
			{
				Disciplina *discAntiga = *itDiscAntiga;
					
				// Se a disciplina antiga já possuir uma substituição no map (for uma chave do map)
				if ( problemData->mapDiscSubstituidaPor.find( discAntiga ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					// Não faz nada. A disciplina que foi inserida primeiro no map prevalece.
				}
				else // discAntiga não é chave
				{
					bool jaSubstituiu = false;

					std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > >::iterator itMap;
					for ( itMap = problemData->mapDiscSubstituidaPor.begin();
						  itMap != problemData->mapDiscSubstituidaPor.end();
						  itMap++ )
					{
						if ( itMap->second == discAntiga )
						{
							jaSubstituiu = true; // Se a disciplina antiga já tiver substituído alguma disciplina no map (for substituta)
						}
					}

					if ( jaSubstituiu ) // discAntiga já é substituta
					{
						if ( problemData->mapDiscSubstituidaPor.find( discNova ) != 
							 problemData->mapDiscSubstituidaPor.end() ) // discNova já é chave do map
						{
							Disciplina* equiv = ( problemData->mapDiscSubstituidaPor.find( discNova ) )->second;

							for ( itMap = problemData->mapDiscSubstituidaPor.begin();
								  itMap != problemData->mapDiscSubstituidaPor.end();
								  itMap++ )
							{
								if ( itMap->second == discAntiga )
								{
									problemData->mapDiscSubstituidaPor[ itMap->first ] = equiv;
								}
							}

							problemData->mapDiscSubstituidaPor[discAntiga] = equiv;
						}
						else  // discNova NÃO é chave do map
						{
							for ( itMap = problemData->mapDiscSubstituidaPor.begin();
								  itMap != problemData->mapDiscSubstituidaPor.end();
								  itMap++ )
							{
								if ( itMap->second == discAntiga )
								{
									problemData->mapDiscSubstituidaPor[ itMap->first ] = discNova;
								}
							}

							problemData->mapDiscSubstituidaPor[discAntiga] = discNova;
						}

					}
					else // Se a disciplina antiga não estiver no map
					{
						itMap = problemData->mapDiscSubstituidaPor.find( discNova );
			
						// Se a disciplina nova já possuir uma substituição no map (for uma chave do map)
						if ( itMap != problemData->mapDiscSubstituidaPor.end() )
						{
							problemData->mapDiscSubstituidaPor[discAntiga] = itMap->second;
						
							if ( problemData->mapDiscSubstituidaPor.find( itMap->second ) != 
								 problemData->mapDiscSubstituidaPor.end() )
							{
								std::cout<<"\nNão deveria entrar aqui\n";
							}

						}
						else
						{
							// Se a disciplina nova já tiver substituído alguma disciplina no map
							// ou se também não estiver no map
							problemData->mapDiscSubstituidaPor[discAntiga] = discNova;

							if ( problemData->mapDiscSubstituidaPor.find( discNova ) != 
								 problemData->mapDiscSubstituidaPor.end() )
							{
								std::cout<<"\nNão deveria entrar aqui\n";
							}

						}
					}			
				}
			}
		}
	}

	substituiDisciplinasEquivalentes();

}


void ProblemDataLoader::divideDisciplinas()
{
   int idNovaMag = -1;

   ITERA_GGROUP_LESSPTR( it_disc,
      problemData->disciplinas, Disciplina )
   {
      if ( it_disc->getCredTeoricos() > 0
         && it_disc->getCredPraticos() > 0
         && it_disc->eLab() == true )
      {
         Disciplina * nova_disc = new Disciplina();

         nova_disc->setId( -(*it_disc)->getId() );
         nova_disc->setCodigo( (*it_disc)->getCodigo() + "-P" );
         nova_disc->setNome( (*it_disc)->getNome() + "PRATICA" );
         nova_disc->setCredTeoricos( 0 );
         nova_disc->setCredPraticos( (*it_disc)->getCredPraticos() );
         it_disc->setCredPraticos( 0 );
		 
		 nova_disc->setTemParPratTeor(true);
		 it_disc->setTemParPratTeor(true);

         nova_disc->setMaxCreds( nova_disc->getCredPraticos() );
         it_disc->setMaxCreds( (*it_disc)->getCredTeoricos() );

         nova_disc->setELab( (*it_disc)->eLab() );
         it_disc->setELab( false );

         nova_disc->setMaxAlunosT( 0 );
         nova_disc->setMaxAlunosP( (*it_disc)->getMaxAlunosP() );
         it_disc->setMaxAlunosP( 0 );

         nova_disc->setTipoDisciplinaId( (*it_disc)->getTipoDisciplinaId() );
         nova_disc->setNivelDificuldadeId( (*it_disc)->getNivelDificuldadeId() );
		 
		 nova_disc->setProfUnicoDiscPT( (*it_disc)->getProfUnicoDiscPT() );
		 nova_disc->setAulasContinuas( (*it_disc)->aulasContinuas() );
		 
		 // Enquanto não há divisão da regra no input para práticas e teóricas, o solver
		 // vai separar sempre essas regras de acordo com as genéricas existentes. Portanto,
		 // isso deve ser feito sempre, mesmo quando it_disc->divisao_creditos == NULL
         if(1)
         {			
			std::map< int /*Num. Creds*/ ,
               GGroup< DivisaoCreditos *, LessPtr< DivisaoCreditos > > >::iterator it_Creds_Regras;

            // Limpa as divisões de créditos relacionadas à disciplina antiga, que envolviam creds prat E teor
			(*it_disc)->divisao_creditos.deleteElements();
			(*it_disc)->divisao_creditos.clear();

			// Se for para usar somente regras já estipuladas pelo usuário, adiciona as novas regras das disciplinas
			//if ( problemData->parametros->regrasGenericasDivisaoCredito )
			//{
			//	// Alterações relacionadas à disciplina antiga
			//	it_Creds_Regras = problemData->creds_Regras.find( it_disc->getCredTeoricos() );

			//	// Checando se existem regras de crédito
			//	// cadastrada para o total de créditos da disciplina teórica.
			//	if ( it_Creds_Regras != problemData->creds_Regras.end() )
			//	{
			//		ITERA_GGROUP_LESSPTR( itDiv, it_Creds_Regras->second, DivisaoCreditos )
			//			(*it_disc)->divisao_creditos.add( *itDiv );
			//	}
			//	
			//	// ---------------------------------------------------

			//	// Alterações relacionadas à nova disciplina
			//	it_Creds_Regras = problemData->creds_Regras.find(
			//	   nova_disc->getCredPraticos() );

			//	// Checando se existem regras de crédito
			//	// cadastrada para o total de créditos da nova disciplina prática.
			//	if ( it_Creds_Regras != problemData->creds_Regras.end() )
			//	{
			//		ITERA_GGROUP_LESSPTR( itDiv, it_Creds_Regras->second, DivisaoCreditos )
			//			nova_disc->divisao_creditos.add( *itDiv );
			//	}
			//}
         }

         //>>> Copiando HORARIO
         ITERA_GGROUP_LESSPTR( it_hr, (*it_disc)->horarios, Horario )
         {	   
            Horario * h =  new Horario();
            h->setId( (*it_hr)->getId() );

            //>>> >>> Copiando DiaSemana
            GGroup< int >::iterator it_dia
               = it_hr->dias_semana.begin();

            for ( unsigned dia = 0; dia < it_hr->dias_semana.size(); dia++ )
            {
               h->dias_semana.add( *it_dia );
               it_dia++;
            }
            // <<< <<<

            h->setHorarioAulaId( it_hr->getHorarioAulaId() );
            h->setTurnoIESId( it_hr->getTurnoIESId() );
			//h->horario_aula->setFaseDoDia( problemData->getTurno( h->horario_aula->getInicio() ) );

            if ( it_hr->horario_aula != NULL )
            {
			   h->horario_aula = it_hr->horario_aula;
            }

            nova_disc->horarios.add( h );
			nova_disc->addTurnoIES( h->getTurnoIESId(), h );
         }
		 
         //>>> Copiando calendarios originais
		 GGroup<Calendario*, LessPtr<Calendario>> calendsOrig = (*it_disc)->getCalendariosOriginais();
		 ITERA_GGROUP_LESSPTR( it_calend, calendsOrig, Calendario )
         {
			 nova_disc->addCalendariosOriginais( *it_calend );
		 }


         int idDisc = nova_disc->getId();

         ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
         {	   
            // Adicionando os dados da nova disciplina
            // em <Campi->Unidade->Sala->disciplinasAssociadas>:
            ITERA_GGROUP_LESSPTR( it_und, it_cp->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
               {
                  if( ( it_sala->disciplinas_associadas.find( it_disc->getId() )
                     != it_sala->disciplinas_associadas.end() )
                     && it_sala->ehLab() )
                  {
                     // Removendo a associacao da disciplina teorica em
                     // questao com as salas incompativeis, no caso qualquer
                     // uma que nao seja uma sala de aula (de acordo com inputTrivial)
                     it_sala->disciplinas_associadas.remove( it_disc->getId() );

                     // Em relacao a nova disciplina (pratica), so adiciono uma
                     // associacao quando  for com uma sala compativel, no caso LABORATORIO
                     it_sala->disciplinas_associadas.add( nova_disc->getId() );
                  }
               }
            }

            // Adicionando os dados da nova disciplina
            // em < Campi->Professor->disciplinas >:
            Magisterio * novo_mag;

            ITERA_GGROUP_LESSPTR( it_prof, it_cp->professores, Professor )
            {
               ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
               {
                  if ( it_mag->getDisciplinaId() == it_disc->getId() )
                  {
                     novo_mag = new Magisterio();

                     idNovaMag--;
                     novo_mag->setId( idNovaMag );
                     novo_mag->setNota( it_mag->getNota() );
                     novo_mag->setPreferencia( it_mag->getPreferencia() );
                     novo_mag->setDisciplinaId( nova_disc->getId() );
                     it_prof->magisterio.add( novo_mag );

                     // Garantindo que um mesmo professor nao possui
                     // preferencias diferentes em relacao a uma mesma disciplina.
                     break;
                  }
               }
            }
         }

		 // Adicionando os dados da nova disciplina em < GrupoCurso->curriculos >
         ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
         {		
            ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
            {
				map < Disciplina*, int, LessPtr< Disciplina > >::iterator 
					itDiscPer = it_curriculo->disciplinas_periodo.find( *it_disc );
				if ( itDiscPer != it_curriculo->disciplinas_periodo.end())
				{
					int periodo = itDiscPer->second;
					it_curriculo->disciplinas_periodo[nova_disc] = periodo;
				}
            }
         }


		 // -----------------------------------------------------------------
		 //							DEMANDAS

		 // Só cria nova demanda para disciplinas que não foram substituidas ou
		 // para aquelas que foram substituidas E substitutas (caso em que a 
		 // equivalencia é não-transitiva).
		 // Disciplina substituidas tiveram suas demandas tambem substituidas,
		 // e não criadas.
		 if ( problemData->mapDiscSubstituidaPor.find( *it_disc ) ==
			  problemData->mapDiscSubstituidaPor.end() ||
			  problemData->ehSubstituta( *it_disc ) )
		 {
			 // Procura pelo maior id de demanda já cadastrado
			 int id = problemData->retornaMaiorIdDemandas();

			 // Procura pelo maior id de alunoDemanda já cadastrado
			 int idAlDemanda = problemData->retornaMaiorIdAlunoDemandas();

			 // Adicionando os dados da nova disciplina em <Demanda>			  
			 ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda )
			 {
				if( (*it_dem)->getDisciplinaId() == (*it_disc)->getId())
				{
					bool inseriuNovaDemanda=false;

					// Copia demanda da disciplina teorica correspondente
					Demanda *nova_demanda = new Demanda();
					id++;
					nova_demanda->setId( id );
					nova_demanda->setOfertaId( (*it_dem)->getOfertaId() );
					nova_demanda->setDisciplinaId( nova_disc->getId() );
					nova_demanda->setQuantidade( (*it_dem)->getQuantidade() );
					nova_demanda->oferta = (*it_dem)->oferta;
					nova_demanda->disciplina = nova_disc;

					problemData->demandasTotal.add( nova_demanda );
					 
					if ( problemData->demandas.find( *it_dem ) !=
						 problemData->demandas.end() )
					{
						problemData->demandas.add( nova_demanda );
						inseriuNovaDemanda = true;
					}

					// Adicionando novo AlunoDemanda com a nova_demanda (disciplina pratica)
					// para cada aluno que já possuia demanda para a disciplina teorica correspondente
					ITERA_GGROUP_LESSPTR( it_aluno_dem, problemData->alunosDemandaTotal, AlunoDemanda )
					{
						if ( it_aluno_dem->getDemandaId() == it_dem->getId() )
						{
							int alunoId = it_aluno_dem->getAlunoId();
							int prior = it_aluno_dem->getPrioridade();
							idAlDemanda++;

							AlunoDemanda *aluno_demanda = new AlunoDemanda( idAlDemanda, alunoId, prior, nova_demanda, it_aluno_dem->getExigeEquivalenciaForcada() );
							
							aluno_demanda->setADemParTeorPrat( *it_aluno_dem );		// par de AlunoDemanda teórico-prático
							(*it_aluno_dem)->setADemParTeorPrat( aluno_demanda);	// par de AlunoDemanda teórico-prático

							problemData->alunosDemandaTotal.add( aluno_demanda );

							if ( problemData->alunosDemanda.find( *it_aluno_dem ) !=
								 problemData->alunosDemanda.end() )
							{
								if (!inseriuNovaDemanda)
									std::cout<<"\nERRO1! Incluindo AlunoDemanda sem ter incluido a NovaDemanda"; 
								fflush(NULL);								
								if ( problemData->demandas.find( *it_dem ) == problemData->demandas.end() ) 
									std::cout<<"\nERRO2! Incluindo AlunoDemanda sem ter incluido a Demanda"; 
								fflush(NULL);

								problemData->alunosDemanda.add( aluno_demanda );
								this->problemData->mapDemandaAlunos[ nova_demanda ].add( aluno_demanda );
							}
						}
					}
				}
			 }
		 }
		
		 // -----------------------------------------------------------------

		 GGroup< int >::iterator itDiasLetivosDiscs = it_disc->diasLetivos.begin();

         for (; itDiasLetivosDiscs != it_disc->diasLetivos.end();
                itDiasLetivosDiscs++ )
         {
            nova_disc->diasLetivos.add( *itDiasLetivosDiscs );
         }

         problemData->novasDisciplinas.add( nova_disc );
      }
   }

   // Testa se existe referencia para todas as demandas corretamente
	ITERA_GGROUP_LESSPTR( it_aluno_dem, problemData->alunosDemanda, AlunoDemanda )
	{
		bool achou = false;
		int id = (*it_aluno_dem)->getDemandaId();
		ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda )
		{
			if ( ( *it_dem )->getId() == id )
			{
				achou = true;
			}
		}
		if (!achou){
			std::cout<<"\nERRO FINAL! Inclui AlunoDemanda sem ter incluido a Demanda\n"
			<<"DemandaId = "<<id<<" AlDemId="<<(*it_aluno_dem)->getId() << " P"
			<<(*it_aluno_dem)->getPrioridade()<<" aluno"<<(*it_aluno_dem)->getAlunoId();
		}
	}

   // Equivalências (conceito novo) envolvendo disciplinas praticas
   if ( problemData->parametros->considerar_equivalencia_por_aluno )
	  addEquivDisciplinasPraticasEmTeoricas();

   ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
   {
      problemData->disciplinas.add( *itDisciplina );
   }

   // Equivalências (conceito antigo) para disciplinas praticas
   if ( problemData->parametros->considerar_equivalencia )
	  relacionaEquivalenciasDisciplinasPraticas();

   // ---------
   criaFixacoesDisciplinasDivididas();
}

void ProblemDataLoader::addEquivDisciplinasPraticasEmTeoricas()
{
	std::map< int, Disciplina* >::iterator itMapP;
	std::map< int, Disciplina* > mapPraticas;
    ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
    {
		mapPraticas[ (*itDisciplina)->getId() ] = *itDisciplina;
    }

	ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
	{
		Disciplina *disciplina = *it_disc;
		GGroup<Disciplina*, LessPtr<Disciplina>> praticasEquiv;
		ITERA_GGROUP_LESSPTR ( itEquiv, disciplina->discEquivSubstitutas, Disciplina )
		{
			int idP = - itEquiv->getId();
			itMapP = mapPraticas.find( idP );
			if ( itMapP != mapPraticas.end() )
			{
				praticasEquiv.add( itMapP->second );
			}
		}
		
		disciplina->addDiscSubstituta( praticasEquiv );
	}
}

void ProblemDataLoader::relacionaEquivalenciasDisciplinasPraticas()
{
	// Para cada nova disciplina Pratica criada, verifica se a sua Teorica
	// correspondente substituiu alguma equivalência. Se sim, acrescenta essa
	// Pratica nos maps necessários, substituindo a original prática
	// correspondente ou, se essa não existir, a teórica corresponte.
   ITERA_GGROUP_LESSPTR( itDisciplina, problemData->novasDisciplinas, Disciplina )
   {
	    Disciplina *dp = *itDisciplina;
		Disciplina *dt = NULL;

		int idT = - dp->getId(); // Id da disciplina teorica correspondente
   
		ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
		{
			if ( it_disc->getId() == idT )
			{	dt = *it_disc; break;}
		}

		if ( dt != NULL )
		{
			GGroup< std::pair< Curso *, Curriculo * > > cursoCurriculos =
				problemData->retornaCursosCurriculosDisciplina( dt );

			GGroup< std::pair< Curso *, Curriculo * > >::iterator it_curso_curr = 
				cursoCurriculos.begin();

			for ( ; it_curso_curr != cursoCurriculos.end(); it_curso_curr++ )
			{
				Disciplina *discOriginalT = problemData->ehSubstitutaDe( dt, *it_curso_curr );

				if ( discOriginalT != NULL )
				{
					int idOrigP = - discOriginalT->getId();

					Disciplina * discOriginalP = NULL;
					ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
					{
						if ( it_disc->getId() == idOrigP )
							discOriginalP = *it_disc;
					}

					if ( discOriginalP != NULL )
					{
						problemData->mapDiscSubstituidaPor[ discOriginalP ] = dp;
						problemData->mapGroupDisciplinasSubstituidas[*it_curso_curr][dp].add( discOriginalP );

						if ( problemData->mapGroupDisciplinasSubstituidas[ *it_curso_curr ][ dp ].size() > 1 )
						{
							std::cout<<"ATENCAO em void ProblemDataLoader::():relacionaEquivalenciasDisciplinasPraticas"
								<<" a disciplina "<<dp->getId()<<" esta substituindo "
								<<"mais de uma disciplina no curriculo "<< (*it_curso_curr).second->getId();
						}
					}
					else
					{
						problemData->mapDiscSubstituidaPor[ discOriginalT ] = dp;
						problemData->mapGroupDisciplinasSubstituidas[*it_curso_curr][dp].add( discOriginalT );

						if ( problemData->mapGroupDisciplinasSubstituidas[ *it_curso_curr ][ dp ].size() > 1 )
						{
							std::cout<<"ATENCAO em void ProblemDataLoader::():relacionaEquivalenciasDisciplinasPraticas"
								<<" a disciplina "<<dp->getId()<<" esta substituindo "
								<<"mais de uma disciplina no curriculo "<< (*it_curso_curr).second->getId();
						}
					}
				}
			}
		}
    }

   	#ifndef PRINT_LOGS
		return;
	#endif
		
   // ------------------------------------
	// Imprime o mapDiscSubstituidaPor

	ofstream outTestFile;
	char equivFilename[] = "mapDiscSubstituidaPor.txt";
	outTestFile.open(equivFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << equivFilename << endl;
		exit(1);
	}

   for ( std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > >::iterator it = problemData->mapDiscSubstituidaPor.begin();
	     it != problemData->mapDiscSubstituidaPor.end(); it++ )
   {
	   outTestFile <<"\nAntiga "<< it->first->getId() <<" Substituta "<< it->second->getId();
   }
   outTestFile.close();
   
   // ------------------------------------
   // Imprime o mapGroupDisciplinasSubstituidas

	ofstream outTestFile2;
	char equivFilename2[] = "mapGroupDisciplinasSubstituidasProblematicas.txt";
	outTestFile2.open(equivFilename2, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << equivFilename2 << endl;
		exit(1);
	}

   for ( std::map< std::pair< Curso *, Curriculo * >, 
				   std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > >::iterator it
				   = problemData->mapGroupDisciplinasSubstituidas.begin();
	     it != problemData->mapGroupDisciplinasSubstituidas.end(); it++ )
   {	   
	   for ( std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > >::iterator it2 = it->second.begin();
			 it2 != it->second.end(); it2++ )
	   {
		   if ( it2->second.size() > 1 )
		   {
				outTestFile2 <<"\nCurso "<< it->first.first->getId() <<" Curric "<< it->first.second->getId();
				outTestFile2 <<"  Substituta "<< it2->first->getId();
			    ITERA_GGROUP_LESSPTR( itDisciplina, it2->second, Disciplina )
				{
					outTestFile2 <<" Antiga "<< itDisciplina->getId();
				}
		   }
	   }
   }
   outTestFile2.close();

}

void ProblemDataLoader::referenciaCampusUnidadesSalas()
{
   ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
   {
      problemData->refCampus[ it_cp->getId() ] = ( *it_cp );

      ITERA_GGROUP_LESSPTR( it_Unidade, it_cp->unidades, Unidade )
      {
         problemData->refUnidade[ it_Unidade->getId() ] = ( *it_Unidade );

         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            problemData->refSala[ it_Sala->getId() ] = ( *it_Sala );
         }
      }
   }
}

void ProblemDataLoader::referenciaDisciplinas()
{
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      problemData->refDisciplinas[ it_disc->getId() ] = ( *it_disc );
   }
}

void ProblemDataLoader::referenciaHorariosAula()
{
   ITERA_GGROUP_LESSPTR( it_horario_dia, problemData->horariosDia, HorarioDia )
   {
      problemData->refHorarioAula[ it_horario_dia->getHorarioAula()->getId() ]
         = ( it_horario_dia->getHorarioAula() );
   }
}

void ProblemDataLoader::referenciaTurnos()
{
   ITERA_GGROUP_LESSPTR( it_turno, problemData->turnosIES, TurnoIES )
   {
      problemData->refTurnos[ it_turno->getId() ] = ( *it_turno );
   }
}

void ProblemDataLoader::referenciaOfertas()
{
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      problemData->refOfertas[ itOferta->getId() ] = ( *itOferta );
   }
}


void ProblemDataLoader::referenciaProfessores()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   ITERA_GGROUP_LESSPTR( itProfessor, itCampus->professores, Professor )
	   {
	      problemData->refProfessores[ itProfessor->getId() ] = ( *itProfessor );
	   }
   }
}


void ProblemDataLoader::gera_refs()
{
   GGroup< HorarioAula *, LessPtr< HorarioAula > > todos_horarios_aula;
   
   std::cout << "\nGerando referencias para calendarios...";
   ITERA_GGROUP_LESSPTR( it_calendario,
      this->problemData->calendarios, Calendario )
   {
         ITERA_GGROUP_LESSPTR( it_horario_aula,
            it_calendario->horarios_aula, HorarioAula )
         {
            todos_horarios_aula.add( *it_horario_aula );
         }      
   }
   
   std::cout << "\nGerando referencias para turnos de ofertas...";
   ITERA_GGROUP_LESSPTR( it_oft, problemData->ofertas, Oferta )
   {
	   find_and_set_lessptr( it_oft->getTurnoId(),
		   problemData->turnosIES,
           it_oft->turno, false );

   } // ofertas
   
   std::cout << "\nGerando referencias para horariosAula, tipos_sala, discs, tipos de contrato...";
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP( it_horario, it_unidades->horarios, Horario )
         {
            //find_and_set_lessptr( it_horario->getTurnoId(),
            //   problemData->todos_turnos,
            //   it_horario->turno, false );

            find_and_set_lessptr( it_horario->getHorarioAulaId(),
               todos_horarios_aula,
               it_horario->horario_aula, false );
         }

         ITERA_GGROUP_LESSPTR( it_salas, it_unidades->salas, Sala )
         {
            find_and_set_lessptr( it_salas->getTipoSalaId(),
               problemData->tipos_sala,
               it_salas->tipo_sala, false );

            ITERA_GGROUP( it_horario, it_salas->horarios_disponiveis,
               Horario )
            {
               //find_and_set_lessptr( it_horario->getTurnoId(),
               //   problemData->todos_turnos,
               //   it_horario->turno, false );

               find_and_set_lessptr( it_horario->getHorarioAulaId(),
                  todos_horarios_aula,
                  it_horario->horario_aula, false );
            }

            // Disciplinas associadas
            ITERA_GGROUP_N_PT( it_id_Disc, it_salas->disciplinas_associadas, int )
            {
               ITERA_GGROUP_LESSPTR( it_Disc, problemData->disciplinas, Disciplina )
               {
                  if ( ( *it_id_Disc ) == it_Disc->getId() )
                  {
                     it_salas->disciplinas_Associadas_Usuario.add( *it_Disc );
                  }
               }
            }
         } // end salas
      }

      ITERA_GGROUP_LESSPTR( it_prof, it_campi->professores, Professor )
      {
         find_and_set_lessptr( it_prof->getTipoContratoId(),
            problemData->tipos_contrato, 
            it_prof->tipo_contrato, false );

         ITERA_GGROUP( it_horario, it_prof->horarios, Horario )
         {
            //find_and_set_lessptr( it_horario->getTurnoId(),
            //   problemData->todos_turnos,
            //   it_horario->turno, false );

            find_and_set_lessptr( it_horario->getHorarioAulaId(),
               todos_horarios_aula,
               it_horario->horario_aula, false );
         }

         ITERA_GGROUP_LESSPTR( it_mag, it_prof->magisterio, Magisterio )
         {
            find_and_set_lessptr( it_mag->getDisciplinaId(),
               problemData->disciplinas,
               it_mag->disciplina, false );
         }
      } // end professores

      ITERA_GGROUP( it_horario, it_campi->horarios, Horario )
      {
         //find_and_set_lessptr( it_horario->getTurnoId(),
         //   problemData->todos_turnos,
         //   it_horario->turno, false );

         find_and_set_lessptr( it_horario->getHorarioAulaId(),
            todos_horarios_aula,
            it_horario->horario_aula, false );
      } 
   } // campus
   
   std::cout << "\nGerando referencias para tempo_campi...";
   ITERA_GGROUP_LESSPTR( it_desl, problemData->tempo_campi, Deslocamento )
   {
	   if ( problemData->refCampus.find( it_desl->getOrigemId() ) != problemData->refCampus.end() )
       {
         find_and_set_lessptr( it_desl->getOrigemId(),
            problemData->campi, ( Campus * & ) it_desl->origem, false );
       }

       if ( problemData->refCampus.find( it_desl->getDestinoId() ) != problemData->refCampus.end() )
       {
         find_and_set_lessptr( it_desl->getDestinoId(),
            problemData->campi, ( Campus * & ) it_desl->destino, false );
       }
   } // deslocamento campi
   
   std::cout << "\nGerando referencias para disciplinas...";
   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
      find_and_set_lessptr( it_disc->getTipoDisciplinaId(),
         problemData->tipos_disciplina,
         it_disc->tipo_disciplina, false );

      find_and_set_lessptr( it_disc->getNivelDificuldadeId(),
         problemData->niveis_dificuldade,
         it_disc->nivel_dificuldade, false );
	  
      ITERA_GGROUP_LESSPTR( it_horario, it_disc->horarios, Horario )
      {
		  find_and_set_lessptr( it_horario->getTurnoIESId(),
			  problemData->turnosIES,
			  it_horario->turnoIES, false );

         find_and_set_lessptr( it_horario->getHorarioAulaId(),
            todos_horarios_aula,
            it_horario->horario_aula, false );
      }

   } // disciplinas
   
   std::cout << "\nGerando referencias para niveis_dificuldade_horario...";
   ITERA_GGROUP( it_ndh, problemData->parametros->niveis_dificuldade_horario,
      NivelDificuldadeHorario )
   {
      find_and_set_lessptr( it_ndh->nivel_dificuldade_id,
         problemData->niveis_dificuldade,
         it_ndh->nivel_dificuldade, false );
   }
   
   std::cout << "\nGerando referencias para fixacoes...";
   ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
   {
      // Seta a referência à disciplina da fixação
      if ( it_fix->getDisciplinaId() >= 0 )
      {
         find_and_set_lessptr( it_fix->getDisciplinaId(), problemData->disciplinas,
            it_fix->disciplina, false );
      }

      // Seta a referência ao turno da fixação
      if ( it_fix->getTurnoId() >= 0 )
      {
         //find_and_set_lessptr( it_fix->getTurnoId(),
         //   problemData->todos_turnos,
         //   it_fix->turno, false );
      }

      // Seta a referência ao horário de aula da fixação
      if ( it_fix->getHorarioAulaId() >= 0 )
      {
         if ( it_fix->turno != NULL )
         {
            find_and_set_lessptr( it_fix->getHorarioAulaId(),
               todos_horarios_aula,
               it_fix->horario_aula, false );
         }
         else
         {
            // Como o turno não foi fixado, mas o horário de
            // aula foi, então procuramos o horário aula fixado
            // dentre todos os horários aula (entre todos os turnos)
			 //GGroup< HorarioAula *, LessPtr< HorarioAula > > todos_horarios_aula;

    //        ITERA_GGROUP_LESSPTR( it_turno, problemData->todos_turnos, TurnoIES )
    //        {
    //           ITERA_GGROUP_LESSPTR( it_horario_aula, it_turno->horarios_aula, HorarioAula )
    //           {
    //              todos_horarios_aula.add( ( *it_horario_aula ) );
    //           }
    //        }

    //        find_and_set_lessptr( it_fix->getHorarioAulaId(),
    //           todos_horarios_aula, it_fix->horario_aula, false );
         }
      }

      ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
      {
         // Seta a referência ao professor da fixação
         if ( it_fix->getProfessorId() >= 0 )
         {
            find_and_set_lessptr( it_fix->getProfessorId(),
               it_campi->professores, it_fix->professor, false );
         }

         // Seta a referência à sala da fixação
         if ( it_fix->getSalaId() >= 0 )
         {
            ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade )
            {
				if ( it_unidades->possuiSala( it_fix->getSalaId() ) )
				{
					find_and_set_lessptr( it_fix->getSalaId(),
					it_unidades->salas, it_fix->sala, false );

					break;
				}
            }
         }
      }
   } // fixações
   
   std::cout << "\nGerando referencias para unidades e campi...";
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidades, it_campi->unidades, Unidade ) 
      {
         it_unidades->setIdCampus( it_campi->getId() );

         ITERA_GGROUP_LESSPTR( it_salas, it_unidades->salas, Sala ) 
         {
            it_salas->setIdUnidade( it_unidades->getId() );
            it_salas->setIdCampus( it_campi->getId() );
         }
      }
   }
   
   std::cout << "\nGerando referencias para tempo_unidades...";
   ITERA_GGROUP_LESSPTR( it_desl, problemData->tempo_unidades, Deslocamento )
   {
	   int uOrigemId = it_desl->getOrigemId();
	   int uDestinoId = it_desl->getDestinoId();
	  
	   // É preciso procurar a unidade nos campi
      
	   Unidade *uOrigem = problemData->refUnidade[ uOrigemId ];	   
	   Unidade *uDestino = problemData->refUnidade[ uDestinoId ];
	   
	  if ( uOrigem == NULL ){
		  std::cout << "\nErro: unidade origem " << uOrigemId << " nao encontrada.";
		  continue;
	  }  
	  if ( uDestino == NULL ){
		  std::cout << "\nErro: unidade destino " << uDestinoId << " nao encontrada.";
		  continue;
	  }
	  
	  Campus *cpo = problemData->refCampus[ uOrigem->getIdCampus() ];
	  Campus *cpd = problemData->refCampus[ uDestino->getIdCampus() ];
	   
	  if ( cpo == NULL ){
		  std::cout << "\nErro: campus origem " << uOrigem->getIdCampus() << " nao encontrado.";
		  continue;
	  }  
	  if ( cpd == NULL ){
		  std::cout << "\nErro: campus destino " << uDestino->getIdCampus() << " nao encontrado.";
		  continue;
	  }
	  	
	  find_and_set_lessptr( uOrigemId, cpo->unidades, ( Unidade * & ) it_desl->origem, false );	           
	  find_and_set_lessptr( uDestinoId, cpd->unidades, ( Unidade * & ) it_desl->destino, false );

   } // deslocamento unidades 

   std::cout << "\nGerando referencias para demandas...";
   ITERA_GGROUP_LESSPTR( it_aluno_demanda,
      problemData->alunosDemanda, AlunoDemanda )
   {
      bool encontrouDemanda = false;

      ITERA_GGROUP_LESSPTR( it_dem, problemData->demandas, Demanda )
      {
         if ( it_dem->getId() == it_aluno_demanda->getDemandaId() )
         {
            encontrouDemanda = true;
            break;
         }
      }

      if ( !encontrouDemanda )
      {
         std::cout << "Demanda nao encontrada em gera_refs(): "
				   << "AlunoId: " << (*it_aluno_demanda)->getAlunoId()
				   << "  DemandaId: " << (*it_aluno_demanda)->getDemandaId() << std::endl;
		 int v; cin>>v;
         continue;
      }

      find_and_set_lessptr( it_aluno_demanda->getDemandaId(),
         problemData->demandas, it_aluno_demanda->demanda, false );
   }
  
   std::cout << "\nGerando referencias para demandasTotal...";
   ITERA_GGROUP_LESSPTR( it_aluno_demanda,
      problemData->alunosDemandaTotal, AlunoDemanda )
   {
	   if ( it_aluno_demanda->demanda == NULL )
	   {
			bool encontrouDemanda = false;

			ITERA_GGROUP_LESSPTR( it_dem, problemData->demandasTotal, Demanda )
			{
				if ( it_dem->getId() == it_aluno_demanda->getDemandaId() )
				{
					encontrouDemanda = true;
					break;
				}
			}

			if ( !encontrouDemanda )
			{
				std::cout << "Demanda nao encontrada em gera_refs(): "
						<< it_aluno_demanda->getDemandaId() << std::endl;       
				continue;
			}

			find_and_set_lessptr( it_aluno_demanda->getDemandaId(),
				problemData->demandasTotal, it_aluno_demanda->demanda, false );
	   }
   }
}

void ProblemDataLoader::cria_blocos_curriculares()
{
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		return;
	}

   // Contador de blocos
   int id_Bloco = 1;

   Campus * campus = NULL;
   Curso * curso = NULL;
   Oferta * oferta = NULL;
   Demanda * demanda = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      campus = ( *it_campus );

      ITERA_GGROUP_LESSPTR( it_curso, it_campus->cursos, Curso )
      {
         curso = ( *it_curso );

         ITERA_GGROUP_LESSPTR( it_curriculo, it_curso->curriculos, Curriculo )
         {
            curriculo = ( *it_curriculo );

            // Descobrindo oferta em questão
            oferta = NULL;

            ITERA_GGROUP_LESSPTR( it_oferta, campus->ofertas, Oferta )
            {
               if ( it_oferta->campus->getId() == campus->getId()
                  && it_oferta->curriculo->getId() == curriculo->getId()
                  && it_oferta->curso->getId() == curso->getId() )
               {
                  oferta = ( *it_oferta );
                  break;
               }
            }

            // Checando se foi encontrada alguma oferta válida.
            if ( oferta == NULL )
            {
               continue;
            }

            map < Disciplina*, int, LessPtr< Disciplina > >::iterator
               it_disc_periodo = curriculo->disciplinas_periodo.begin();

            // Percorrendo todas as disciplinas de
            // um curso cadastradas para um currículo

            for (; it_disc_periodo != curriculo->disciplinas_periodo.end();
                   it_disc_periodo++ )
            {
               int periodo = it_disc_periodo->second;
               disciplina = it_disc_periodo->first;
               
			   int id_disciplina = abs( disciplina->getId() );
               int id_oferta = oferta->getId();
			   
			   Disciplina *d = problemData->ehSubstitutaDe( disciplina, std::make_pair(curso, curriculo) );

               std::pair< Campus *, Curso * > campus_curso
                  = std::make_pair( campus, curso );

               // Encontrando e armazenando a demanda específica da disciplina em questão
               demanda = NULL;

               ITERA_GGROUP_LESSPTR( it_demanda, problemData->demandas, Demanda )
               {
                  if ( it_demanda->disciplina->getId() == id_disciplina
                     && it_demanda->oferta->getId() == id_oferta )
                  {
                     demanda = ( *it_demanda );
                     break;
                  }
               }

               if ( demanda == NULL )
               {
                  std::cout << "\nERRO: DEMANDA NAO ENCONTRADA EM:"
                            << "\nProblemDataLoadaer::cria_blocos_curriculares()"
                            << "\nDisciplina : " << id_disciplina
                            << "\nOferta : " << id_oferta
                            << std::endl << std::endl;

                  exit( 0 );

                  demanda = new Demanda();
                  demanda->setDisciplinaId( id_disciplina );
                  demanda->disciplina = disciplina;
                  demanda->setOfertaId( id_oferta );
                  demanda->oferta = oferta;
                  demanda->setQuantidade( 0 );

                  problemData->map_campus_curso_demanda[ campus_curso ].add( demanda );
                  problemData->demandas.add( demanda );
               }

               bool found = false;

               // Verificando a existência do bloco
               // curricular para a disciplina em questão.

               ITERA_GGROUP_LESSPTR( it_bloco_curricular,
                  problemData->blocos, BlocoCurricular )
               {
                  if ( it_bloco_curricular->campus->getId() == campus->getId()
                     && it_bloco_curricular->curso->getId() == curso->getId()
                     && it_bloco_curricular->curriculo->getId() == curriculo->getId()
                     && it_bloco_curricular->getPeriodo() == periodo
					 && it_bloco_curricular->oferta->getId() == oferta->getId() )
                  {
                     (*it_bloco_curricular)->disciplinas.add( disciplina );
                     (*it_bloco_curricular)->disciplina_Demanda[ disciplina ] = demanda;

                     found = true;
                     break;
                  }
               }

               if ( !found )
               {
                  BlocoCurricular * bloco_curricular = new BlocoCurricular();

                  bloco_curricular->setId( id_Bloco );
                  bloco_curricular->setPeriodo( periodo );
                  bloco_curricular->campus = campus;
                  bloco_curricular->curso = curso;
                  bloco_curricular->curriculo = curriculo;
				  bloco_curricular->oferta = oferta;
                  bloco_curricular->disciplinas.add( disciplina );
                  bloco_curricular->disciplina_Demanda[ disciplina ] = demanda;
				  
                  problemData->blocos.add( bloco_curricular );
                  id_Bloco++;
               }
            }
         }
      }
   }

   // Setando os dias letivos de cada bloco.
   BlocoCurricular * bloco = NULL;

   ITERA_GGROUP_LESSPTR( it_bc,
      problemData->blocos, BlocoCurricular )
   {
      bloco = ( *it_bc );
      curso = it_bc->curso;
      Curriculo* curriculo = it_bc->curriculo;

      int totalTurmas = 0;

      ITERA_GGROUP_LESSPTR( it_Disc,
         it_bc->disciplinas, Disciplina )
      {
         disciplina = ( *it_Disc );

         // Associa o curso correspondente ao bloco atual
         // e a disciplina 'it_disc' ao bloco curricular corrente
         Trio<Curso*,Curriculo*,Disciplina*> auxTrio;
         auxTrio.first = curso;
         auxTrio.second = curriculo;
         auxTrio.third = disciplina;

         problemData->mapCursoDisciplina_BlocoCurricular
            [ auxTrio ] = bloco;

         ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Disc->diasLetivos, int )
         { 
            (*it_bc)->diasLetivos.add( *it_Dias_Letivos );
         }

         totalTurmas += it_Disc->getNumTurmas();
      }

      (*it_bc)->setTotalTurmas( totalTurmas );
   }
   
   /* Preenche, para cada bloco curricular, o numero maximo de creditos possiveis
	  de serem alocados para cada dia letivo */
   ITERA_GGROUP_LESSPTR( it_bc, problemData->blocos, BlocoCurricular )
   {
		(*it_bc)->preencheMaxCredsPorDia();
   }

}

void ProblemDataLoader::relacionaCampusDiscs()
{
   ITERA_GGROUP_LESSPTR( it_oferta,
      problemData->ofertas, Oferta )
   {
      Curso * curso = it_oferta->curso;

      ITERA_GGROUP_LESSPTR( it_curric,
         curso->curriculos, Curriculo )
      {
         map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc_prd =
            it_curric->disciplinas_periodo.begin();

         for (; it_disc_prd != it_curric->disciplinas_periodo.end(); it_disc_prd++ )
         {
			 problemData->cp_discs[ it_oferta->getCampusId() ].add( it_disc_prd->first->getId() );
			 
			 ITERA_GGROUP_LESSPTR( itDisc, it_disc_prd->first->discEquivSubstitutas, Disciplina )
			 {
				 if ( problemData->ehSubstituivel( (*it_disc_prd).first->getId(), itDisc->getId(), curso ) )
					problemData->cp_discs[ it_oferta->getCampusId() ].add( itDisc->getId() );			 
			 }
         }
      }
   }
}

void ProblemDataLoader::calculaDemandas()
{
	problemData->calculaDemandas();
}

void ProblemDataLoader::calculaTamanhoMedioSalasCampus()
{
   ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
   {
      unsigned somaCapSalas = 0;
      unsigned total_Salas = 0;

      ITERA_GGROUP_LESSPTR( it_und, it_cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
         {
            somaCapSalas += ( it_sala->getCapacidade() );
			int maiorAtual = (int)it_und->getMaiorSala();
			int cap = (int)it_sala->getCapacidade();
            it_und->setMaiorSala( (maiorAtual > cap) ? maiorAtual : cap );
		 }

         total_Salas += (*it_und)->getNumSalas();

		 int maiorAtualCp = (int)it_cp->getMaiorSala();
		 int maiorUnid = (int)it_und->getMaiorSala();
         (*it_cp)->setMaiorSala( (maiorAtualCp > maiorUnid) ? maiorAtualCp : maiorUnid );
      }

      problemData->cp_medSalas[ it_cp->getId() ] =
         ( ( total_Salas > 0 ) ? ( somaCapSalas / total_Salas ) : 0 );
   }
}

void ProblemDataLoader::calculaMenorCapacSalaPorDisc()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   int campusId = itCampus->getId();
	   GGroup< int > disciplinas = problemData->cp_discs[campusId];

	   ITERA_GGROUP_N_PT( it_disc, disciplinas, int )
	   {	   
		    Disciplina* disciplina = problemData->refDisciplinas[ *it_disc ];

			#pragma region Equivalência de disciplinas
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() &&
				!problemData->ehSubstituta( disciplina ) )
			{
				continue;
			}
			#pragma endregion

			int menorCapacSala = 10000;

			GGroup< Sala *, LessPtr< Sala > >::iterator it_sala = problemData->discSalas[ disciplina ].begin();
			for ( ; it_sala != problemData->discSalas[disciplina].end(); it_sala++ )
			{
				if ( problemData->retornaCampus( ( *it_sala )->getIdUnidade() )->getId() != campusId )
			   {
				   continue;
			   }

		 	   menorCapacSala = ( menorCapacSala < ( *it_sala )->getCapacidade() ) ? menorCapacSala : ( *it_sala )->getCapacidade();
			}

			disciplina->setMenorCapacSala(menorCapacSala, campusId);
	   }
   }

}

void ProblemDataLoader::calculaCapacMediaSalaPorDisc()
{
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   int campusId = itCampus->getId();
	   GGroup< int > disciplinas = problemData->cp_discs[campusId];

	   ITERA_GGROUP_N_PT( it_disc, disciplinas, int )
	   {	   
		   Disciplina* disciplina = problemData->refDisciplinas[ *it_disc ];

			#pragma region Equivalência de disciplinas
			if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
				 problemData->mapDiscSubstituidaPor.end() &&
				!problemData->ehSubstituta( disciplina ) )
			{
				continue;
			}
			#pragma endregion

		   int soma = 0;
		   int size = 0;

		   GGroup< Sala *, LessPtr< Sala > >::iterator it_sala = problemData->discSalas[ disciplina ].begin();
		   for ( ; it_sala != problemData->discSalas[ disciplina ].end(); it_sala++ )
		   {
			   if ( problemData->retornaCampus( ( *it_sala )->getIdUnidade() )->getId() != campusId )
			   {
				   continue;
			   }

			   soma += ( *it_sala )->getCapacidade();
			   size++;
		   }

		   if ( soma == 0 )
			   std::cout<<"ATENCAO: disciplina " << disciplina->getId() << ", lab " << disciplina->eLab()
			   << " nao possui sala associada no campus " << campusId << std::endl;
	   
		   int capacMediaSala = ( ( size > 0 ) ? ( soma / size ) : 0 );
	   
		   disciplina->setCapacMediaSala( capacMediaSala, campusId );
		   
	   }
   }

}

void ProblemDataLoader::estima_turmas()
{
	if ( problemData->parametros->permite_compartilhamento_turma_sel )
	{
#ifdef UNIT
		estima_turmas_com_compart_ordena_cap_salas();
#else // Estacio
		estima_turmas_sem_compart();
#endif
	}
	else
		estima_turmas_sem_compart();
}

/*
 TRIEDA-416
    Estimando o número máximo de turmas de cada
    disciplina, quando não é permitido o compartilhamento
    de turmas de cursos diferentes, de acordo com o seguinte cálculo:
	
	numTurmasDisc = sum{ demDiscCurso_{i} / tamMedioSalasCP }

    Onde:
    demDiscOferta -> representa a demanda total do curso i de uma dada disciplina.
    tamMedioSalasCP -> representa o tamanho medio das salas de um campus que têm a disciplina associada.
*/
void ProblemDataLoader::estima_turmas_sem_compart()
{
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int campusId = itCpDisc->first;
	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		   int discId = *itDisc;
		   Disciplina *d = problemData->refDisciplinas[discId];

		   int capacMediaSala = d->getCapacMediaSala( cp->getId() );

		   int maxP = d->getMaxAlunosP();
		   int maxT = d->getMaxAlunosT();
		   int maximoAlunosTurma = ( (maxP > maxT) ? maxP : maxT );

		   if ( maximoAlunosTurma < capacMediaSala )
			   capacMediaSala = maximoAlunosTurma;

		   if ( d->getNumTurmas() < 0 ) d->setNumTurmas( 0 );
		   
		   int numTurmas = d->getNumTurmas();
		   
		   // Para cada curso que contem a disciplina
		   ITERA_GGROUP_LESSPTR( itCurso, cp->cursos, Curso )
		   {
			   Curso *c = *itCurso;

			   if ( !c->possuiDisciplina( d ) )
			   {
					continue;
			   }
			   
			   int quantidade = 0;

			   GGroup< Demanda*, LessPtr< Demanda > > demandas = problemData->buscaTodasDemandas( c , d, cp );

			   ITERA_GGROUP_LESSPTR( itDemanda, demandas, Demanda )
			   {
				   quantidade += itDemanda->getQuantidade();  
			   }

			   // Numero de turmas necessarias para atender a demanda da disciplina d do curso c
			   if ( capacMediaSala > 0 )
					numTurmas += int( std::floor( double(quantidade+capacMediaSala-1)/capacMediaSala ) );
		   }

		   d->setNumTurmas( numTurmas + 1 );
	   }
   }

}

void ProblemDataLoader::estima_turmas_com_compart()
{
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int campusId = itCpDisc->first;
	   Campus * cp = problemData->refCampus[campusId];

	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		   int discId = *itDisc;
		   Disciplina *d = problemData->refDisciplinas[discId];

		   int capacMediaSala = d->getCapacMediaSala( cp->getId() );
		   
		   int maxP = d->getMaxAlunosP();
		   int maxT = d->getMaxAlunosT();
		   int maximoAlunosTurma = ( (maxP > maxT) ? maxP : maxT );

		   if ( maximoAlunosTurma < capacMediaSala )
			   capacMediaSala = maximoAlunosTurma;

		   if ( d->getNumTurmas() < 0 ) d->setNumTurmas( 0 );
		   
		   int numTurmas = d->getNumTurmas();
		   int quantidade = 0;

		   // Para cada curso que contem a disciplina
		   ITERA_GGROUP_LESSPTR( itCurso, cp->cursos, Curso )
		   {
			   Curso *c = *itCurso;
			   if ( !c->possuiDisciplina( d ) )
			   {
					continue;
			   }
			   
			   GGroup< Demanda*, LessPtr< Demanda > > demandas = problemData->buscaTodasDemandas( c , d, cp );
			   ITERA_GGROUP_LESSPTR( itDemanda, demandas, Demanda )
			   {
				   quantidade += itDemanda->getQuantidade();  
			   }
		   }
		   
		   // Numero de turmas necessarias para atender a demanda da disciplina d do curso c
		   if ( capacMediaSala > 0 )
				numTurmas += int( std::floor( double(quantidade+capacMediaSala-1)/capacMediaSala ) );

		   d->setNumTurmas( numTurmas + 1 );
	   }
   }

}

void ProblemDataLoader::estima_turmas_com_compart_ordena_cap_salas()
{
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCpDisc = problemData->cp_discs.begin();
   for ( ; itCpDisc != problemData->cp_discs.end(); itCpDisc++ )
   {
	   int campusId = itCpDisc->first;
	   
	   // Para cada disciplina do campus
	   GGroup< int >::iterator itDisc = itCpDisc->second.begin();
	   for ( ; itDisc != itCpDisc->second.end(); itDisc++ )
	   {
		    int discId = *itDisc;
		    Disciplina *disciplina = problemData->refDisciplinas[discId];
		   
			std::multiset<int> cap_salas;
			GGroup< Sala *, LessPtr< Sala > > salas = problemData->discSalas[disciplina];
			ITERA_GGROUP_LESSPTR( itSala, salas, Sala )
			{
				cap_salas.insert( (*itSala)->getCapacidade() );
			}

		    int maxP = disciplina->getMaxAlunosP();
		    int maxT = disciplina->getMaxAlunosT();
		    int maximoAlunosTurma = ( (maxP > maxT) ? maxP : maxT );
			
			int quantidade = problemData->retornaDemandasDiscNoCampus( disciplina->getId(), campusId, 1 ).size();	
			int quantidadeAcumulada = 0;
			int numTurmas = 0;

			typedef std::multiset<int>::iterator multiset_it;
			std::reverse_iterator<multiset_it> rev_end ( cap_salas.begin() );
			std::reverse_iterator<multiset_it> it_rev ( cap_salas.end() );

			for ( ; it_rev != rev_end; ++it_rev )
			{
				int cap = *it_rev;

				if ( cap > maximoAlunosTurma )
					quantidadeAcumulada += maximoAlunosTurma;
				else
					quantidadeAcumulada += cap;

				numTurmas++;
				if ( quantidadeAcumulada >= quantidade )
					break;
			}
			if ( quantidade > quantidadeAcumulada )
				numTurmas++;

			if ( quantidade < 30 )
				disciplina->setNumTurmas( numTurmas + 1 );
			else if ( quantidade >= 30 && quantidade < 100 )
				disciplina->setNumTurmas( numTurmas + 2 );
			else if ( quantidade >= 100 )
				disciplina->setNumTurmas( numTurmas + 3 );
	   }
	}
}


void ProblemDataLoader::print_stats()
{
   int ncampi( 0 ), nunidades( 0 ), nsalas( 0 ), nconjuntoSalas( 0 ),
       ndiscs( 0 ), ndiscsDiv( 0 ), nturmas( 0 ), nturmasDiscDiv( 0 ),
       nprofs( 0 ), ncursos( 0 ),   nofertas( 0 ), tdemanda( 0 ), tdemandaDiv( 0 );

   ncampi = problemData->campi.size();

   ITERA_GGROUP_LESSPTR( it_campi,
      problemData->campi, Campus )
   {
      nunidades += ( it_campi->unidades.size() );
      ITERA_GGROUP_LESSPTR( it_und, it_campi->unidades, Unidade )
      {
         nsalas += it_und->salas.size();
         nconjuntoSalas += it_und->conjutoSalas.size();
      }

      nprofs += it_campi->professores.size();
      ncursos += problemData->cursos.size();
   }

   nofertas = problemData->ofertas.size();

   ITERA_GGROUP_LESSPTR( itOferta,
      problemData->ofertas, Oferta )
   {
      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc =
         itOferta->curriculo->disciplinas_periodo.begin();

      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end();
            itPrdDisc++ )
      {
		  Disciplina * disc = itPrdDisc->first;

         if ( disc->getId() > 0 )
         {
            nturmas += disc->getNumTurmas();
         }
         else
         {
            nturmasDiscDiv += ( disc->getNumTurmas() );
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_disc,
      problemData->disciplinas, Disciplina )
   {
      if ( it_disc->getId() > 0 )
      {
         ndiscs++;
         tdemanda += ( it_disc->getDemandaTotal() );
      }
      else
      {
         ndiscsDiv++;
         tdemandaDiv += ( it_disc->getDemandaTotal() );
      }
   }

   std::cout << "\nEstatisticas de dados de entrada:\n\n";

   printf( "<*> Campi:                 \t%4d\n", ncampi );
   printf( "<*> Unidades:              \t%4d\n", nunidades );
   printf( "<*> Salas:                 \t%4d\n", nsalas );

   printf( "<*> Conjuntos de Salas:    \t%4d\n", nconjuntoSalas );

   printf( "<*> Disciplinas:\n");
   printf( "\t - Entrada:    \t%4d\n", ndiscs );
   printf( "\t - Divididas:  \t%4d\n", ndiscsDiv );   
   printf( "\t - Total:  \t%4d\n", ndiscs + ndiscsDiv );

   printf( "<*> Blocos Curriculares:  \t%4d\n", problemData->blocos.size() );

   printf( "<*> Turmas:\n");
   printf( "\t - Entrada:    \t%4d\n", nturmas );
   printf( "\t - Divididas:  \t%4d\n", nturmasDiscDiv );
   printf( "\t - Total:  \t%4d\n", nturmas + nturmasDiscDiv );

   printf( "<*> Professores:  \t%4d\n", nprofs );
   printf( "<*> Cursos:       \t%4d\n", ncursos );

   printf( "<*> Ofertas:      \t%4d\n", nofertas );

   printf( "<*> Demanda total\n");
   printf( "\t - Entrada:    \t%4d\n", tdemanda );
   printf( "\t - Divididas:  \t%4d\n", tdemandaDiv );   
   printf( "\t - Total:  \t%4d\n", tdemanda + tdemandaDiv );

   std::cout << "================================"
             << std::endl << std::endl;
}

// Salva algumas informações que são usadas frequentemente
void ProblemDataLoader::cache()
{
   problemData->totalSalas = 0;
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      int total_salas = 0;
      ITERA_GGROUP_LESSPTR( it_u, it_campus->unidades, Unidade )
      {
         total_salas += it_u->salas.size();
      }

      it_campus->setTotalSalas( total_salas );
      problemData->totalSalas += total_salas;
   }

   problemData->totalTurmas = 0;
   ITERA_GGROUP_LESSPTR( it_disciplinas, problemData->disciplinas, Disciplina )
   {
      problemData->totalTurmas += it_disciplinas->getNumTurmas();
   }

   problemData->totalTurmas_AlDem = 0;
   ITERA_GGROUP_LESSPTR( it_aldem, problemData->alunosDemanda, AlunoDemanda )
   {
	   problemData->totalTurmas_AlDem += it_aldem->demanda->disciplina->getNumTurmas();
   }

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      int totalTurmas = 0;
      ITERA_GGROUP_LESSPTR( it_disciplinas, it_bloco->disciplinas, Disciplina )
      {
         totalTurmas += it_disciplinas->getNumTurmas();
      }
   }

   ITERA_GGROUP_LESSPTR( it_disc, problemData->disciplinas, Disciplina )
   {
	   int nCredsP = it_disc->getCredPraticos();
	   int nCredsT = it_disc->getCredTeoricos();

	   // Se o total de creditos é par
	   if ( (nCredsP + nCredsT)%2 == 0 )
		   it_disc->setMinCreds( 2 );

	   // Se o total de creditos é ímpar
	   else
			it_disc->setMinCreds( 1 );

	   // acho que esse trecho comentado abaixo é inutil
	  // if ( it_disc->divisao_creditos.size() != 0 )
   //    {
   //      it_disc->setMaxCreds(0);
		 //ITERA_GGROUPS_LESSPTR( itDiv, it_disc->divisao_creditos, DivisaoCreditos )
		 //{
			// for ( int t = 0; t < 8; t++ )
			// {
			//	if ( itDiv->dia[t] > 0 )
			//	{
			//	   it_disc->setMaxCreds( 
			//		  std::max(it_disc->getMaxCreds(),
			//		  it_disc->divisao_creditos->dia[t]) );
			//	}
			// }
		 //}
   //    }
   //    else
   //    {
         it_disc->setMaxCreds( nCredsP + nCredsT );
      // }

   }
}

void ProblemDataLoader::associaDisciplinasSalas()
{
   // Dado o id de um campus, retorna o conjunto
   // dos ids das disciplinas relacionadas a esse campus
   std::map< int, GGroup< int > >::iterator it_Cp_Discs
      = problemData->cp_discs.begin();

   // Para cada Campus
   for (; it_Cp_Discs != problemData->cp_discs.end(); it_Cp_Discs++ )
   {	   
      Campus * campus = problemData->refCampus[ it_Cp_Discs->first ];

	  GGroup<Sala*, LessPtr<Sala>> labsAssociadosFake;

	  //------------------------------------------------------------------------
	  // PASSO 1
	  // Armazenando as disciplinas que foram associadas pelo usuário e não 
	  // deverão ser consideradas para a associação automática.
      ITERA_GGROUP_LESSPTR( it_und, campus->unidades, Unidade )
      {		  
         ITERA_GGROUP_LESSPTR( it_sala, it_und->salas, Sala )
         {
			 if ( it_sala->ehLab() )
				 labsAssociadosFake.add( *it_sala );

            ITERA_GGROUP( it_Disc,
               it_sala->disciplinas_Associadas_Usuario, Disciplina )
            {
               // Adicionando uma preferência de sala para uma dada disciplina.
               problemData->disc_Salas_Pref[ *it_Disc ].add( *it_sala );
			   
               problemData->discSalas[ *it_Disc ].add( *it_sala );
			   it_sala->disciplinasAssociadas.add( *it_Disc );
			}
         }
	  }
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // PASSO 2
      // Associando as demais disciplinas às salas

      // Para cada disciplina associada ao campus em questao
      ITERA_GGROUP_N_PT( it_disciplina, it_Cp_Discs->second, int )
      {
			Disciplina * disciplina
				= ( problemData->refDisciplinas.find( *it_disciplina )->second );

			// Demanda para a disciplina
			//bool haDemanda = problemData->haDemandaDiscNoCampus( disciplina->getId(), campus->getId() );
			//if ( !haDemanda ) continue;

			// Se a disciplina foi associada pelo usuário para o campus corrente.
			std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > >::iterator
				it_salas_associadas = problemData->disc_Salas_Pref.find( disciplina );
			
			bool salas_associadas = false;		

			if ( it_salas_associadas != problemData->disc_Salas_Pref.end() )
			{
				GGroup< Sala *, LessPtr< Sala > > *salas = & it_salas_associadas->second;
				ITERA_GGROUP_LESSPTR( itSala, (*salas), Sala )
				{
					if ( itSala->getIdCampus() == campus->getId() )
					{
						salas_associadas = true; break;
					}
				}
			}
						
			if ( disciplina->eLab() && !salas_associadas )
			{
				std::cout << "\nATENCAO!!! ProblemDataLoader::associaDisciplinasSalas()"
							<< "\nUma disciplina pratica nao foi"
							<< "\nassociada a nenhuma sala pelo usuario."
							<< "\nId da disciplina   : " << disciplina->getId()
							<< "\nNome da disciplina : " << disciplina->getNome()
							<< "\nEla sera associada a todos os laboratorios do campus. "
							<< std::endl << std::endl;
								
				ITERA_GGROUP_LESSPTR( itLab, labsAssociadosFake, Sala )
				{
					// Estabelecendo o critério de intereseção de dias letivos.
					// I.E. Só associo uma sala de aula a uma disciplina se a sala tem, 
					// pelo menos, um dia letivo comum com a disciplina.
					ITERA_GGROUP_N_PT( it_Dias_Let_Disc, disciplina->diasLetivos, int )
					{
						// Só continuo quando a sala possuir o dia
						// letivo (pertencente à disciplina) em questão.
						if ( itLab->diasLetivos.find( *it_Dias_Let_Disc )
							!= itLab->diasLetivos.end() )
						{
							itLab->disciplinas_Associadas_AUTOMATICA.add( disciplina );
							problemData->discSalas[ disciplina ].add( *itLab );
							itLab->disciplinasAssociadas.add( disciplina );
							break;
						}
					}
				}

			}
			else if ( !salas_associadas )
			{
				// CRIA ASSOCIACOES ENTRE A DISCIPLINA E TODAS AS SALAS DE AULA.

				ITERA_GGROUP_LESSPTR( it_unidade, campus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
					{					
						// Somente se for uma sala de aula.
						if ( ! it_sala->ehLab() )
						{							
							// Estabelecendo o critério de intereseção de dias letivos.
							// I.E. Só associo uma sala de aula a uma disciplina se a sala tem, 
							// pelo menos, um dia letivo comum com a disciplina.
							ITERA_GGROUP_N_PT( it_Dias_Let_Disc, disciplina->diasLetivos, int )
							{
								// Só continuo quando a sala possuir o dia
								// letivo (pertencente à disciplina) em questão.
								if ( it_sala->diasLetivos.find( *it_Dias_Let_Disc )
									!= it_sala->diasLetivos.end() )
								{
									it_sala->disciplinas_Associadas_AUTOMATICA.add( disciplina );
									problemData->discSalas[ disciplina ].add( *it_sala );
									it_sala->disciplinasAssociadas.add( disciplina );
									break;
								}
							}
						}
					}
				}
			}
      }
      //------------------------------------------------------------------------
   }

}

void ProblemDataLoader::associaDisciplinasConjuntoSalas()
{
   // Guarda uma referência para os 'ids' dos
   // novos conjuntos salas que forem criados aqui
   int maior_id_conjunto_salas = 0;

   //-------------------------------------------------------------------------------
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            // Guarda sempre o maior 'id' dos conjuntos de sala
            if ( itCjtSala->getId() > maior_id_conjunto_salas )
            {
               maior_id_conjunto_salas = itCjtSala->getId();
            }

            // Estrutura do map: Dado o 'id' de uma
            // sala, retorna-se o ponteiro da sala
            std::map< int, Sala * >::iterator
               itSala = itCjtSala->salas.begin();

            for (; itSala != itCjtSala->salas.end(); itSala++ )
            {
               GGroup< Disciplina * >::iterator itDiscs
                  = itSala->second->disciplinasAssociadas.begin();

               for (; itDiscs != itSala->second->disciplinasAssociadas.end();
                      itDiscs++ )
               {
                  itCjtSala->disciplinas_associadas.add( *itDiscs );
				  itDiscs->cjtSalasAssociados[ itCjtSala->getId() ] = *itCjtSala;
               }
            }
         }
      }
   }
   //-------------------------------------------------------------------------------

   maior_id_conjunto_salas++;

   Sala * sala = NULL;
   Fixacao * fixacao = NULL;
   Disciplina * disciplina = NULL;

   //-------------------------------------------------------------------------------
   // Dividindo os conjuntos de sala, em caso de fixação 'disciplina - sala'
   ITERA_GGROUP_LESSPTR( it_fixacao, problemData->fixacoes, Fixacao )
   {
      fixacao = ( *it_fixacao );
      sala = fixacao->sala;
      disciplina = fixacao->disciplina;

      // Verifica se é uma fixação de disciplina e sala
      if ( sala == NULL || disciplina == NULL )
      {
         continue;
      }

      bool encontrou = false;
      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         if ( encontrou )
         {
            break;
         }

         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
         {
            if ( encontrou )
            {
               break;
            }

            ITERA_GGROUP_LESSPTR( it_conjunto_sala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               // Verifica se esse conjunto sala possui a sala da fixação
               std::map< int, Sala * >::iterator
                  find_sala = it_conjunto_sala->salas.find( sala->getId() );

               if ( find_sala == it_conjunto_sala->salas.end() )
               {
                  continue;
               }

               // Caso o conjunto de salas possua apenas a sala fixada,
               // não precisamos então criar um novo conjunto de salas
               if ( it_conjunto_sala->salas.size() == 1 )
               {
                  continue;
               }

               // Verifica se já existe nessa unidade um
               // conjunto de sala que corresponda a essa fixação
               if ( existe_conjunto_sala__fixacao( *itUnidade, disciplina, sala ) )
               {
                  continue;
               }

               // Criamos um novo conjunto de sala, que conterá todas
               // as disciplinas do conjunto de sala atual, exceto a disciplina fixada
               ConjuntoSala * novo_conjunto_sala = new ConjuntoSala( **it_conjunto_sala );
               novo_conjunto_sala->setId( maior_id_conjunto_salas );
               maior_id_conjunto_salas++;

               // Chegando aqui, sabemos que o
               // conjunto sala possui a sala que está fixada
               novo_conjunto_sala->salas.clear();

               std::map< int, Sala * >::iterator it_sala
                  = it_conjunto_sala->salas.begin();

               for (; it_sala != it_conjunto_sala->salas.end(); it_sala++ )
               {
                  // Devo incluir no novo conjunto de
                  // salas apenas as salas que NÃO estão fixadas
                  if ( it_sala->second->getId() != sala->getId() )
                  {
                     novo_conjunto_sala->salas[ it_sala->second->getId() ] = ( it_sala->second );
                  }
               }

               // O novo conjunto de salas não possui a disciplina fixada,
               // pois ela está apenas no outro conjunto de salas, já existente
               novo_conjunto_sala->disciplinas_associadas.remove( disciplina );
               novo_conjunto_sala->dias_letivos_disciplinas.erase( disciplina );

               // No conjunto de salas da sala fixada, apenas
               // existirá uma sala, que é exatamente a sala fixada
               it_conjunto_sala->salas.clear();
               it_conjunto_sala->salas[ sala->getId() ] = sala;

               // No novo conjunto de salas, devo remover a sala fixada
               novo_conjunto_sala->salas.erase( sala->getId() );

               // Adiciona o novo conjunto de salas criado na unidade atual
               itUnidade->conjutoSalas.add( novo_conjunto_sala );

               encontrou = true;
               break;
            }
         }
      }
   }
   //-------------------------------------------------------------------------------

   
   // --------------------------------------------------------------------------
   // Conta para cada disciplina a quantas salas ela está associada.

   ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
   {
		ITERA_GGROUP_LESSPTR( itUnidade, itCp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{							
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					itDisc->setNSalasAptas( itDisc->getNSalasAptas() + 1 );
				}
			}
		}

    }

}

bool ProblemDataLoader::existe_conjunto_sala__fixacao(
   Unidade * unidade, Disciplina * disciplina, Sala * sala )
{
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         // Analiso apenas a unidade em questão
         if ( it_unidade->getId() != unidade->getId() )
         {
            continue;
         }

         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            if ( it_conjunto_sala->salas.size() == 1 // O conjunto possui apenas a sala fixada
               && it_conjunto_sala->salas.begin()->second->getId() == sala->getId()
               && it_conjunto_sala->disciplinas_associadas.size() == 1 // O conjunto possui apenas a disciplina fixada
               && it_conjunto_sala->disciplinas_associadas.begin()->getId() == disciplina->getId() )
            {
               // Já existe o conjunto de salas correspondente à fixação.
               // Portanto, não precisamos criar outro conjunto de salas
               return true;
            }
         }
      }
   }

   return false;
}

void ProblemDataLoader::relacionaDiscOfertas()
{
   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
      map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();

      for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
             it_Prd_Disc++ )
      { 
		  disciplina = it_Prd_Disc->first;
         int disc = disciplina->getId();

         problemData->ofertasDisc[ disc ].add( *it_Oferta );

         // Utilizado em equivalências de disciplinas
         std::pair< Curso *, Curriculo * > curso_curriculo
            = std::make_pair( it_Oferta->curso, it_Oferta->curriculo );

         problemData->map_Disc_CursoCurriculo[ disciplina ] = curso_curriculo;

		 problemData->discOfertasEquiv[ disciplina->getId() ].add( *it_Oferta );
		 ITERA_GGROUP_LESSPTR( itDiscEquiv, disciplina->discEquivSubstitutas, Disciplina )
		 {
			 if ( problemData->ehSubstituivel( disciplina->getId(), itDiscEquiv->getId(), it_Oferta->curso ) )
				 problemData->discOfertasEquiv[ itDiscEquiv->getId() ].add( *it_Oferta );
		 }
      }
   }
}


// Método de pré-processamento
// relacionado com a issue TRIEDA-700
#ifndef HEURISTICA

void ProblemDataLoader::criaAulas()
{
	std::cout<<"\nCriando aulas...";
	
   // Checando se o XML de entrada possui a saída do TÁTICO,
   if ( problemData->atendimentosTatico )
   {	   
	   problemData->mapAluno_CampusTurmaDisc.clear();				
	   problemData->mapCampusTurmaDisc_AlunosDemanda.clear();

      AtendimentoOfertaSolucao * atendOferta = NULL;

	  ITERA_GGROUP_LESSPTR( it_atend_campus,
         ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
      {
		  int campusId = it_atend_campus->getCampusId();

		  Campus *campus = problemData->refCampus[campusId];

         ITERA_GGROUP( it_atend_unidade,
            it_atend_campus->atendimentosUnidades, AtendimentoUnidadeSolucao )
         {					 
			 Unidade *unidade = problemData->refUnidade.find(
				it_atend_unidade->getId() )->second;

            ITERA_GGROUP( it_atend_sala,
               it_atend_unidade->atendimentosSalas, AtendimentoSalaSolucao )
            {                     
				Sala * sala = problemData->refSala.find(
                    it_atend_sala->getSalaId() )->second;

               ITERA_GGROUP( it_atend_dia_semana,
                  it_atend_sala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
               {
                  // Informa o dia da semana da aula
                  int diaSemana = it_atend_dia_semana->getDiaSemana();

                  ITERA_GGROUP( it_atend_tatico,
                     it_atend_dia_semana->atendimentosTatico, AtendimentoTaticoSolucao )
                  {
                     atendOferta = ( it_atend_tatico->atendimento_oferta );

					 Oferta* oferta = problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ];

                     // Informa a 'turma' da aula
                     int turma = atoi( atendOferta->getTurma().c_str() );
					 
					 // Informa a disciplina atendida (caso tenha havido substituição, é a substituta)
                     Disciplina * disciplina = problemData->refDisciplinas.find( atendOferta->getDisciplinaId() )->second;
					 
                     int idDisc = disciplina->getId();
					 
					 // Informa a disciplina substituida (caso não tenha havido substituição, é NULL)
					 Disciplina * disciplinaSubstituida = NULL;
					 if ( atendOferta->getDisciplinaSubstituidaId() != NULL )
					 {
						 disciplinaSubstituida = problemData->refDisciplinas.find( atendOferta->getDisciplinaSubstituidaId() )->second;
					 }
					 else // se disciplinaSubstituida == disciplina, significa que NÃO houve substituição por equivalencia
						 disciplinaSubstituida = disciplina;

                     // Informa os créditos teóricos da aula
                     int creditos_teoricos = it_atend_tatico->getQtdeCreditosTeoricos();

                     // Informa os créditos práticos da aula
                     int creditos_praticos = it_atend_tatico->getQtdeCreditosPraticos();

                     // Informa a demanda atendida
                     int demandaAtendida = it_atend_tatico->atendimento_oferta->getQuantidade();

                     if ( ( creditos_praticos > 0 )
                        && ( creditos_teoricos > 0 ) )
                     {
                        std::cout << "Aula com ambos os creditos teoricos e praticos maiores que 0.\n";
                        exit( 1 );
                     }
					 					 
					 // Preenche os maps de atendimentos de alunos
					 Trio< int, int, Disciplina* > trio;
					 trio.set( campusId, turma, disciplina );
					  
					 ITERA_GGROUP_N_PT( itAlunoDem, atendOferta->alunosDemandasAtendidas, int )
					 {						 
						 int alunoDemId = *itAlunoDem;
						
						 AlunoDemanda *alunoDemanda = problemData->retornaAlunoDemanda( alunoDemId );
						 Aluno *aluno = problemData->retornaAluno( alunoDemanda->getAlunoId() );
						 
						 if ( alunoDemanda == NULL )
						 {
							 std::cout<<"\nErro em criaAulas(). AlunoDemanda original "<<alunoDemId<<" nao encontrado.\n";
							 continue;
						 }
						 
						 if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
						 {
							problemData->mapAluno_CampusTurmaDisc[aluno].add( trio );					 						
							problemData->mapCampusTurmaDisc_AlunosDemanda[trio].add( alunoDemanda );
						 }
						 else if ( problemData->parametros->modo_otimizacao == "TATICO" )
						 {
							 // TODO
							 bool fixar = it_atend_tatico->getFixaAlunos(); 

							 if ( alunoDemanda->demanda->getDisciplinaId() == - idDisc )
							 {
								 // prática
								 alunoDemanda = problemData->procuraAlunoDemanda( idDisc, aluno->getAlunoId(), alunoDemanda->getPrioridade() );
							 }
							 else if ( abs(alunoDemanda->demanda->getDisciplinaId()) != abs(idDisc) )
							 {
								 // equivalencia => procura a substituída se for caso de fixação
								 if ( fixar )
								 {
									alunoDemanda = problemData->atualizaAlunoDemandaEquiv( turma, disciplina, campusId, aluno, 1 );
								 }
							 }
							 
							 if ( alunoDemanda == NULL )
							 {
								 std::cout<<"\nErro em criaAulas(). AlunoDemanda "<<alunoDemId<<" nao encontrado.\n";
								 continue;
							 }						 						 
							 if ( alunoDemanda->demanda->getDisciplinaId() != idDisc )
							 {
								std::cout<<"\nAtencao: erro em criaAulas()!\n";
							 }
#ifdef BUILD_COM_SOL_INICIAL							 							 
							 problemData->getSolTaticoInicial()->addAlunoDem(campus, disciplina, turma, alunoDemanda, fixar );
							 problemData->getSolTaticoInicial()->addAlunoTurma(campusId, disciplina, turma, aluno, fixar );
#endif
						 }
					 }

                     // Procurando nas aulas cadastradas, se existe
                     // alguma aula que possui os mesmos índices de 
                     // dia da semana, sala e turma. Caso encontre,
                     // devo apenas add a oferta à aula existente.
                     bool novaAula = true;
                     Aula * aulaAntiga = NULL;
					 						 
					 if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
				     {
						 ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
						 {
							if( ( itAula->getTurma() == turma )
							   && ( *(itAula->getDisciplina()) == *disciplina )
							   && ( itAula->getDiaSemana() == diaSemana )
							   && ( *(itAula->getSala()) == *sala ) 
							   && ( itAula->getCreditosPraticos() == creditos_praticos )
							   && ( itAula->getCreditosTeoricos() == creditos_teoricos ) )
							{
								aulaAntiga = *itAula;
								novaAula = false;
								problemData->aulas.remove(itAula);														
								break;
							}
						 }
					 }
					 else if ( problemData->parametros->modo_otimizacao == "TATICO" )
					 {
#ifdef BUILD_COM_SOL_INICIAL	
					 	 Aula * a = problemData->getSolTaticoInicial()->getAula( campusId, disciplina, turma, diaSemana, sala );
						 if ( a!=NULL )
						 {
							 aulaAntiga = a;
							 novaAula = false;
							 problemData->getSolTaticoInicial()->removeAula( campusId, disciplina, turma, aulaAntiga );							 
						 }
#endif
					 }


					 if ( atendOferta->alunosDemandasAtendidas.size() == 0 )
						 std::cout<<"\nErro, turma vazia! Turma " << turma << " disc " << disciplina->getId();
						

                     if( novaAula )
                     {
                        // Monta o objeto 'aula'
                        Aula * aula = new Aula();
                        aula->ofertas.add( problemData->refOfertas[
                           atendOferta->getOfertaCursoCampiId() ] );

                        aula->setTurma( turma );
                        aula->setDisciplina( disciplina );
                        aula->setCampus( campus );
						aula->setSala( sala );
						aula->setUnidade( unidade );
                        aula->setDiaSemana( diaSemana );
                        aula->setCreditosTeoricos( creditos_teoricos );
                        aula->setCreditosPraticos( creditos_praticos );
						aula->setQuantPorOferta( demandaAtendida, oferta );
						aula->addCursoAtendido( oferta->curso, demandaAtendida );
                        aula->setDisciplinaSubstituida( disciplinaSubstituida, oferta );						
						aula->addAlunoDemanda( oferta, disciplinaSubstituida, atendOferta->alunosDemandasAtendidas );


						// Informa os horariosAula
						GGroup<int> horarios = it_atend_tatico->getHorariosAula();
						
						HorarioAula *hi=NULL;
						HorarioAula *hf=NULL;

						if ( horarios.size() > 0 )
						{
							int totalCreds = creditos_teoricos + creditos_praticos;

							GGroup<HorarioAula*, LessPtr<HorarioAula>> 
								horsAula = problemData->horarios_aula_dia_semana[diaSemana];

							ITERA_GGROUP_N_PT( itHor, horarios, int )
							{
								int id = *itHor;
								aula->addHorariosAulaId(id);

								// Acha o objeto HorarioAula correspondente ao id
								HorarioAula *ha=NULL;							
								ITERA_GGROUP_LESSPTR( itHorDia, horsAula, HorarioAula )
								{
									if ( itHorDia->getId() == id )
									{
										ha = *itHorDia;
										break;
									}
								}
								if ( ha == NULL ) { std::cout<<"\nErro grave, horarioAula nao encontrado na sala!!!\n"; continue;}
								if ( hi == NULL ) hi = ha;
								if ( hf == NULL ) hf = ha;
								if ( *hi > *ha ) hi = ha;
								if ( *hf < *ha ) hf = ha;
							}

							// Somente horario inicial vindo do input 
							if ( hi->getInicio() == hf->getInicio() && totalCreds > 1 )
							{
								Calendario * c = hi->getCalendario();
								HorarioAula *h = hi;
								int n = 1;
								while ( n <= totalCreds && h!=NULL )
								{
									hf = h;
									aula->addHorariosAulaId( h->getId() );

									h = c->getProximoHorario(h);
									n++;
								}

								if ( n-1 != totalCreds )
								{
									std::cout << "\nErro nos horarios da aula vindo do tatico."
										<< "\nTotal de horarios da aula nao bate! Aula: " << aula->toString();
								}
							}
						}
						else
						{
							if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
								std::cout << "\nATENCAO: horarios da aula " << aula->toString() 
									<< "da solucao tatica nao encontrados. Serao redefinidos.";
						}

						aula->setHorarioAulaInicial( hi );
						aula->setHorarioAulaFinal( hf );

						DateTime *dti = NULL;
						DateTime *dtf = NULL;
						if ( hi != NULL && hf != NULL )
						{
							dti = problemData->horarioAulaDateTime[hi->getId()].first;
							dtf = problemData->horarioAulaDateTime[hf->getId()].first;							
						}
						aula->setDateTimeInicial( dti );
						aula->setDateTimeFinal( dtf );

						// Fixações para solução inicial						
						aula->fixaSala( it_atend_tatico->getFixaSala() );
						aula->fixaDia( it_atend_tatico->getFixaDia() );
					    aula->fixaCredsTeor( it_atend_tatico->getFixaHorarios() );
					    aula->fixaCredsPrat( it_atend_tatico->getFixaHorarios() );
						aula->fixaHi( it_atend_tatico->getFixaHorarios() );
						aula->fixaHf( it_atend_tatico->getFixaHorarios() );
						aula->fixaAbertura( it_atend_tatico->getFixaAbertura() );
												
						if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
						{
							problemData->aulas.add( aula );
							problemData->addMapAula( campusId, turma, disciplina, aula );
						}

						if ( turma > disciplina->getNumTurmas() )
							disciplina->setNumTurmas( turma );
												
						if ( problemData->parametros->modo_otimizacao == "TATICO" )
						{
							#ifdef BUILD_COM_SOL_INICIAL
							problemData->getSolTaticoInicial()->addAula(campusId, disciplina, turma, aula );
							#endif
						}
                     }
                     else
                     {
                        aulaAntiga->ofertas.add( problemData->refOfertas[ atendOferta->getOfertaCursoCampiId() ] );
                        aulaAntiga->setQuantPorOferta( demandaAtendida, oferta );
						aulaAntiga->addCursoAtendido( oferta->curso, demandaAtendida );
                        aulaAntiga->setDisciplinaSubstituida( disciplinaSubstituida, oferta );
						aulaAntiga->addAlunoDemanda( oferta, disciplinaSubstituida, atendOferta->alunosDemandasAtendidas );

						if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
						{
							problemData->aulas.add( aulaAntiga );
						}
						else if ( problemData->parametros->modo_otimizacao == "TATICO" )
						{
							#ifdef BUILD_COM_SOL_INICIAL
							problemData->getSolTaticoInicial()->addAula(campusId, disciplina, turma, aulaAntiga );
							#endif
						}
					 }
                  }
               }
            }
         }
      }
   }

   #ifdef BUILD_COM_SOL_INICIAL
   // Confere solução inicial tática
   if ( problemData->parametros->modo_otimizacao == "TATICO" )
   {
	   problemData->getSolTaticoInicial()->confereSolucao();
	
	    string fileName("aulasSolInicial_");
		fileName += problemData->inputIdToString();
		fileName += "_";
		fileName += problemData->getInputFileName();
		fileName += ".txt";

	   problemData->getSolTaticoInicial()->imprimeAulas( fileName );
   }
	#endif

   std::cout<<" done!\n";
}

void ProblemDataLoader::validaInputSolucaoTatico()
{
   if ( problemData->atendimentosTatico != NULL )
   {
      GGroup< int /*id Sala*/> labs;

      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnd, itCampus->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itSala, itUnd->salas, Sala )
            {
               if ( itSala->ehLab() )
               {
                  labs.add( itSala->getId() );
               }
            }
         }
      }

	  ITERA_GGROUP_LESSPTR( itAtCampus, ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
      {
         ITERA_GGROUP( itAtUnd, itAtCampus->atendimentosUnidades, AtendimentoUnidadeSolucao )
         {
            ITERA_GGROUP( itAtSala, itAtUnd->atendimentosSalas, AtendimentoSalaSolucao )
            {
               if ( labs.find( itAtSala->getSalaId() ) != labs.end() )
               {
                  ITERA_GGROUP( itAtDiaSemana, itAtSala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
                  {
                     ITERA_GGROUP( itAtTatico, itAtDiaSemana->atendimentosTatico, AtendimentoTaticoSolucao )
                     {
                        int idDisc = itAtTatico->atendimento_oferta->getDisciplinaId();

                        ITERA_GGROUP_LESSPTR( itDisciplinaDiv, problemData->novasDisciplinas, Disciplina )
                        {
                           if ( -( itDisciplinaDiv->getId() ) == idDisc )
                           {
                              itAtTatico->atendimento_oferta->setDisciplinaId( ( -idDisc ) );
                              break;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}

#endif

void ProblemDataLoader::geraHorariosDia()
{
   int contador = 0;

   problemData->maxHorariosDif = 0;

   ITERA_GGROUP_LESSPTR( it_calendario,
      problemData->calendarios, Calendario )
   {      
         ITERA_GGROUP_LESSPTR( itHA,
            it_calendario->horarios_aula, HorarioAula )
         {
            HorarioAula * horarioAula = ( *itHA );

            if ( horarioAula->getId() > problemData->maxHorariosDif )
            {
               problemData->maxHorariosDif = horarioAula->getId();
            }

            ITERA_GGROUP_N_PT( itDia,
               horarioAula->dias_semana, int )
            {
               HorarioDia * horarioDia = new HorarioDia();

               horarioDia->setId( contador );
               horarioDia->setHorarioAulaId( horarioAula->getId() );
               horarioDia->setHorarioAula( horarioAula );
               horarioDia->setDia( *itDia );
               contador++;

               problemData->horariosDia.add( horarioDia );
            }
         }      
   }

   problemData->horariosDiaIdx.resize( ( problemData->maxHorariosDif + 1 ) * 8, NULL );

   ITERA_GGROUP_LESSPTR( itHorario, problemData->horariosDia, HorarioDia )
   {
      HorarioDia * horarioDia = ( *itHorario );

      problemData->horariosDiaIdx[ problemData->getHorarioDiaIdx( horarioDia ) ] = horarioDia;
   }

   // Professores
   ITERA_GGROUP_LESSPTR( itCamp, problemData->campi, Campus )
   {
      Campus * campus = ( *itCamp );

      ITERA_GGROUP_LESSPTR( itProf, campus->professores, Professor )
      {
         Professor * professor = ( *itProf );

         ITERA_GGROUP( itHor, professor->horarios, Horario )
         {
            Horario * horario = ( *itHor );
 		    HorarioAula *ha = horario->horario_aula;

            ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
            {
               HorarioDia * auxHD = new HorarioDia();

               auxHD->setDia( *itD );
               auxHD->setHorarioAula( ha );
               auxHD->setHorarioAulaId( ha->getId() );

               GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
                  itHorarioDia = problemData->horariosDia.find( auxHD );
               if ( itHorarioDia == problemData->horariosDia.end() )
               {
                  exit( 1 );
               }

               delete auxHD;

               professor->horariosDia.add( *itHorarioDia );
			   professor->addDisponibilidade( *itD, ha->getInicio(), ha->getFinal() );
            }
         }
      }
   }

   // Salas
   ITERA_GGROUP_LESSPTR( itCamp, problemData->campi, Campus )
   {
      Campus * campus = ( *itCamp );

      ITERA_GGROUP_LESSPTR( itUnidade, campus->unidades, Unidade )
      {
         Unidade * unidade = ( *itUnidade );

         ITERA_GGROUP_LESSPTR( itSala, unidade->salas, Sala )
         {
            Sala * sala = ( *itSala );

            ITERA_GGROUP( itHor, sala->horarios_disponiveis, Horario )
            {
               Horario * horario = ( *itHor );

               ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
               {
                  HorarioDia * auxHD = new HorarioDia();

                  auxHD->setDia( *itD );
                  auxHD->setHorarioAula( horario->horario_aula );
                  auxHD->setHorarioAulaId( horario->getHorarioAulaId() );

                  GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
                     itHorarioAula = problemData->horariosDia.find( auxHD );

                  if ( itHorarioAula == problemData->horariosDia.end() )
                  {
                     exit( 1 );
                  }

                  delete auxHD;

                  sala->horariosDia.add( *itHorarioAula );
                  sala->horariosDiaMap[*itD].add(*itHorarioAula);
				  sala->mapDiaDtiDtf[*itD][horario->horario_aula->getInicio()].add( horario->horario_aula->getFinal() );
               }      
            }
         }
      }
   }

   std::map< Disciplina*, GGroup< Horario *, LessPtr<Horario> > > mapDiscHorariosNovos;

   // Disciplinas
   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
      Disciplina * disciplina = ( *itDisc );

      ITERA_GGROUP_LESSPTR( itHor, disciplina->horarios, Horario )
      {
         Horario * horario = ( *itHor );
		
         ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
         {
			HorarioDia *hd = problemData->getHorarioDiaCorrespondente( horario->horario_aula, *itD );
            if ( hd==NULL )
			{
				std::cout << "\nERRO 1 em ProblemDataLoader::geraHorariosDia(): horarioDia nao encontrado.\n"
					<< "\nDisc " << disciplina->getId() << " ha " << horario->horario_aula->getId()
					<< "  dia " << *itD;

				exit(1);
			}

			disciplina->addHorarioDia( horario->horario_aula->getCalendario(), hd );
         }      
      }
      // Absorve os horarios das discs que ela pode substituir e que são comuns com os dela
      ITERA_GGROUP_LESSPTR( itDiscEquiv, disciplina->discEquivalentes, Disciplina )
      {
		  // disc equiv
		  Disciplina *discEq = *itDiscEquiv;

		  ITERA_GGROUP_LESSPTR( itHorEquiv, discEq->horarios, Horario )
		  {
			 // horario equiv
			 Horario * horarioEquiv = ( *itHorEquiv );
			 HorarioAula * horarioAulaEquiv = horarioEquiv->horario_aula;
			 
			 ITERA_GGROUP_N_PT( itDiaEquiv, horarioEquiv->dias_semana, int )
			 {
				 // dia equiv
				 int diaEquiv = *itDiaEquiv;

				 if ( disciplina->possuiHorarioDia( diaEquiv, horarioAulaEquiv ) )
					continue;
				 	
				 // Se a disciplina possui horarioDia correspondente
				 if ( disciplina->possuiHorarioDiaOuAnalogo( diaEquiv, horarioAulaEquiv ) )
				 {
					HorarioDia *hd = problemData->getHorarioDiaCorrespondente( horarioAulaEquiv, diaEquiv );
					if ( hd==NULL )
					{
						std::cout<<"\nERRO 2 em ProblemDataLoader::geraHorariosDia(): horarioDia nao encontrado.\n";
						exit(1);
					}

					disciplina->addHorarioDia( horarioAulaEquiv->getCalendario(), hd );
					 
					GGroup< Horario *, LessPtr<Horario> > *ggroupHorariosNovos = &mapDiscHorariosNovos[disciplina];
					GGroup< Horario *, LessPtr<Horario> >::iterator itHorario = ggroupHorariosNovos->find( horarioEquiv );
					if ( itHorario == ggroupHorariosNovos->end() )
					{
						Horario *h = new Horario( horarioEquiv );
						h->setHorarioAulaId( horarioAulaEquiv->getId() );
						h->horario_aula = horarioAulaEquiv;
						h->setTurnoIESId( 0 );
						h->dias_semana.add( diaEquiv );

						mapDiscHorariosNovos[disciplina].add( h );
					}
					else
					{
						itHorario->dias_semana.add( diaEquiv );
					}
				 }
			 }

		  }
		  
	  }
   }

   // adicionando horarios novos equivalentes para as Disciplinas
   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
      Disciplina * disciplina = ( *itDisc );
	  std::map< Disciplina*, GGroup< Horario *, LessPtr<Horario> > >::iterator
		  itMap = mapDiscHorariosNovos.find( disciplina );
	  if ( itMap != mapDiscHorariosNovos.end() )
	  {
		  disciplina->horarios.add( itMap->second ); // Adiciona os novos horarios obtidos por equivalencia à teórica

		  std::map< int, Disciplina * >::iterator
			  itDisc = problemData->refDisciplinas.find( -disciplina->getId() );
		  if ( itDisc != problemData->refDisciplinas.end() )
		  {
			  // Adiciona os novos horarios obtidos por equivalencia à prática
			  itDisc->second->horarios.add( itMap->second );

			  ITERA_GGROUP_LESSPTR( itHor, itMap->second, Horario )
			  {
				  ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
				  {
					  HorarioDia *hd = problemData->getHorarioDiaCorrespondente( itHor->horario_aula, *itDia );
					  itDisc->second->addHorarioDia( itHor->horario_aula->getCalendario(), hd );
				  }
			  }
		  }
	  }
   }

	#ifdef PRINT_LOGS
   ofstream outFileDiscHor;
   outFileDiscHor.open( "DiscHor.txt", ios::out );
   // Imprime horarios das discs para conferencia
   ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
   {
	    Disciplina * disciplina = ( *itDisc );
		outFileDiscHor << "\nDisciplina: " << disciplina->getId();
		
		outFileDiscHor << "\n\tCalends originais associados: ";
		GGroup< Calendario*, LessPtr<Calendario> > calendsOrig = disciplina->getCalendariosOriginais();
		ITERA_GGROUP_LESSPTR( itCalend, calendsOrig, Calendario )
		{
			outFileDiscHor << "  " << itCalend->getId();
		}
		ITERA_GGROUP_LESSPTR( itHor, disciplina->horarios, Horario )
		{
			 Horario * horario = ( *itHor );
			
			 outFileDiscHor << "\n\tHorarioAula: " << horario->getHorarioAulaId()
				 << ",   " << horario->horario_aula->getInicio()
				 << " - " << horario->horario_aula->getFinal()
				 << ",   " << horario->horario_aula->getCalendario()->getId() << "  - Dias ";
			 ITERA_GGROUP_N_PT( itD, horario->dias_semana, int )
			 {
				 outFileDiscHor << " " << *itD;
			 }
		}
   }
   outFileDiscHor.close();
	#endif
}

void ProblemDataLoader::relacionaProfessorDisciplinasAssociadas( void )
{
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      Campus * campus = ( *it_campus );

      ITERA_GGROUP_LESSPTR( it_professor, campus->professores, Professor )
      {
         Professor * professor = ( *it_professor );

         ITERA_GGROUP_LESSPTR( it_mag, professor->magisterio, Magisterio )
         {
            Magisterio * magisterio = ( *it_mag );
            problemData->mapProfessorDisciplinasAssociadas[ professor ].add( magisterio->disciplina );
         }
      }
   }
}


void ProblemDataLoader::calculaMaxTempoDisponivelPorSala()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
			 it_sala->calculaTempoDispPorDia();
         }
      }
   }
}


void ProblemDataLoader::calculaMaxTempoDisponivelPorSalaPorSL()
{
   ITERA_GGROUP_LESSPTR( it_campi, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campi->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_sala, it_unidade->salas, Sala )
         {
			 it_sala->calculaTempoDispPorDiaSL();

			 it_sala->calculaTempoDispPorTurno();
         }
      }
   }
}

/*
	Preenche o map compatibilidadesDeHorarios, que relaciona para
	cada horarioAula H existente, os horarioAula h tal que a semana letiva
	de h é diferente da semana letiva de H, e h e H não possuem interseção
	(não se sobrepõem).
*/
void ProblemDataLoader::calculaCompatibilidadeDeHorarios()
{
	// calendario 1
	ITERA_GGROUP_LESSPTR( it_calendarios1, problemData->calendarios, Calendario )
    {		
		ITERA_GGROUP_LESSPTR( it_hor1, it_calendarios1->horarios_aula, HorarioAula )
		{
			HorarioAula *h1 = *it_hor1;

			// calendario 2
			ITERA_GGROUP_LESSPTR( it_calendarios2, problemData->calendarios, Calendario )
			{
				if ( (*it_calendarios2)->getId() == (*it_calendarios1)->getId() )
					continue;

				ITERA_GGROUP_LESSPTR( it_hor2, it_calendarios2->horarios_aula, HorarioAula )
				{
					HorarioAula *h2 = *it_hor2;
							
					if ( h1->getCalendario()->getId() == h2->getCalendario()->getId() )
					{
						std::cout<<"\nErro! acho que nao deveria entrar aqui, ja que os horarios sao criados por calendario.";
						continue;
					}
					if ( !h1->sobrepoe( *h2 ) )
						problemData->compatibilidadesDeHorarios[ h1 ].insert( h2 );
				}
			}
		}		
	}
}

void ProblemDataLoader::preencheHashHorarioAulaDateTime()
{
   problemData->horarioAulaDateTime.clear();
   problemData->horariosInicioValidos.clear();

   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_horario_aula, calendario->horarios_aula, HorarioAula )
      {
         HorarioAula* hAula = *it_horario_aula;
         DateTime *newDt = new DateTime(hAula->getInicio());

		 // inicio
         GGroup<DateTime*,LessPtr<DateTime> >::iterator itDti = problemData->horariosInicioValidos.find(newDt);
         if ( itDti != problemData->horariosInicioValidos.end() )
         {
            delete newDt;
         }
         else
         {
            problemData->horariosInicioValidos.add(newDt);
         }
		 
		 // fim
		 DateTime *newFinalDt = new DateTime(hAula->getFinal());
         GGroup<DateTime*,LessPtr<DateTime> >::iterator itDtf = problemData->horariosTerminoValidos.find(newFinalDt);
         if ( itDtf != problemData->horariosTerminoValidos.end() )
         {
            delete newFinalDt;
         }
         else
         {
            problemData->horariosTerminoValidos.add(newFinalDt);
         }	  
	  
	  }
   }

   ITERA_GGROUP_LESSPTR( it_calendario, this->problemData->calendarios, Calendario )
   {
      Calendario * calendario = ( *it_calendario );

      ITERA_GGROUP_LESSPTR( it_horario_aula, calendario->horarios_aula, HorarioAula )
      {
         HorarioAula * horario_aula = ( *it_horario_aula );
         DateTime *busca = new DateTime(horario_aula->getInicio());

         GGroup<DateTime*,LessPtr<DateTime> >::iterator itDt = this->problemData->horariosInicioValidos.find(busca);

         delete busca;

         if ( itDt != this->problemData->horariosInicioValidos.end() )
         {
            DateTime *dateTime = *itDt;
            std::pair<DateTime*,int> auxP;
            auxP.first = dateTime;
            auxP.second = (int)(horario_aula->getTempoAula()+0.5);
            problemData->horarioAulaDateTime[horario_aula->getId()] = auxP; 
         }
      }
   }
}

void ProblemDataLoader::calculaCHOriginalPorAluno()
{
   ITERA_GGROUP_LESSPTR( it_aluno_demanda,
      problemData->alunosDemanda, AlunoDemanda )
   {            
	  if ( it_aluno_demanda->getPrioridade()==1 )
	  {
		  Aluno * aluno = problemData->retornaAluno( it_aluno_demanda->getAlunoId() );
		  aluno->addCargaHorariaOrigRequeridaP1( 
			  it_aluno_demanda->demanda->disciplina->getTempoCredSemanaLetiva() *
			  it_aluno_demanda->demanda->disciplina->getTotalCreditos() );
		  aluno->addNroCredsOrigRequeridosP1(
			  it_aluno_demanda->demanda->disciplina->getTotalCreditos() );
	  }
   }
}