#include "ProbDataAnalyzer.h"
#include "ProblemData.h"
#include "CentroDados.h"
#include "Indicadores.h"

#include <iostream>
#include <fstream>

const string PEACH			= "#ffdab9";
const string LIGHTSALMON	= "#ffa07a";
const string SALMON			= "#fa8072";
const string CRIMSON		= "#dc143c";
const string RED			= "#ff0000";
const string DARKRED		= "#8b0000";
const string BLACK			= "#000000";

const string GOLD1			= "gold1";
const string GOLD2			= "goldenrod2";
const string CORAL			= "coral";
const string ORANGED		= "orangered";
const string FIREBRICK		= "firebrick";

const int NrColors = 6;
//const string COLOR[] = {PEACH,		// 0
//						LIGHTSALMON,// 1
//						SALMON,		// 2
//						CRIMSON,	// 3
//						RED,		// 4
//						DARKRED,	// 5
//						BLACK };	// 6

const string COLOR[] = {GOLD1,		// 0
						GOLD2,		// 1
						CORAL,		// 2
						ORANGED,	// 3
						FIREBRICK,	// 4
						BLACK };	// 5

ProblemData* ProbDataAnalyzer::problemData_ = nullptr;

ProbDataAnalyzer::ProbDataAnalyzer(void)
{
}

ProbDataAnalyzer::~ProbDataAnalyzer(void)
{
}

// Set Problem data (evitar que pointer seja mudado por engano)
void ProbDataAnalyzer::setProblemData(ProblemData* const probData)
{
	ProbDataAnalyzer::problemData_ = probData;
}


// -----------------------------------------------------------------------------------------------

void ProbDataAnalyzer::printInputDataLog()
{
	printAlunosInputDataLog();
	printDemandasInputDataLog();
	printRelacaoDemProfInputDataLog();
	printDiscsInputDataLog();
	printProfsInputDataLog();
}

void ProbDataAnalyzer::printAlunosInputDataLog()
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

	ITERA_GGROUP_LESSPTR( itAluno, problemData_->alunos, Aluno )
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

}

void ProbDataAnalyzer::printDiscsInputDataLog()
{
	int nroDiscLab = 0;
	int nroDiscLabNaoAssoc = 0;
	double mediaOrigLabPorDisc = 0;
	double mediaFinalLabPorDisc = 0;
	int totalDisc = 0;
	int nroDiscSemProf = 0;

	ITERA_GGROUP_LESSPTR( itDisciplina, problemData_->disciplinas, Disciplina )
	{
		Disciplina *disciplina = *itDisciplina;
												
		if ( disciplina->getId() > 0 )
		{
			totalDisc++;
			if ( disciplina->getNroProfRealHabilit() == 0 )
				nroDiscSemProf++;
		}

		if ( disciplina->eLab() ) 
		{
			mediaFinalLabPorDisc += disciplina->cjtSalasAssociados.size();
			nroDiscLab++;

			int nroOrigLabAssoc=0;
			auto itDisc = problemData_->disc_Salas_Pref.find(disciplina);
			if ( itDisc != problemData_->disc_Salas_Pref.end() )
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

	ssDiscs << "\nMédia original de laboratórios associados por disciplina que exige laboratório: " << mediaOrigLabPorDisc;
	
	if ( nroDiscLabNaoAssoc > 0 )
		ssDiscs << "\nMédia de laboratórios associados por disciplina após possíveis associações automáticas (*): " << mediaFinalLabPorDisc;

	if ( nroDiscSemProf > 0 )
		ssDiscs << "\nDas " << totalDisc << " disciplinas cadastradas, " << nroDiscSemProf << " não possuem professor habilitado.";
	else
		ssDiscs << "\nDas " << totalDisc << " disciplinas cadastradas, todas possuem pelo menos 1 professor habilitado.";

	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ssDiscs.str() );
}

void ProbDataAnalyzer::printProfsInputDataLog()
{
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

	for( auto itProf = problemData_->refProfessores.begin(); itProf != problemData_->refProfessores.end(); itProf++ )
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
		else
		{
			ssProfs << "\nNão existe nenhuma associação de professor a disciplina.";
		}

		ssProfs << "\nDos " << totalProfs << " professores, " << nroProfAte2DiasDisp 
			<< " estão disponíveis apenas 1 ou 2 dias na semana.";
	}
	else
	{
		ssProfs << "\nNao ha professores cadastrados.";
	}

	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ssProfs.str() );
	Indicadores::printSeparator(4);
}

void ProbDataAnalyzer::printDemandasInputDataLog()
{
	std::unordered_map<Disciplina*, std::set<AlunoDemanda*>> mapDemandaDisc;
	std::map<int /*profPrior*/, int> mapDemandaProfPrior;
	std::unordered_map<int /*unid*/, std::unordered_set<int>> mapAlunosUnid;
	std::unordered_map<int /*unid*/, int> mapDemandaUnid;
	getDemandasMapeadas(mapDemandaDisc, mapDemandaProfPrior, mapAlunosUnid, mapDemandaUnid);

	std::map<int, int> mapNrCredDiscNrDemandas;
	int totalCredsDemanda=0;
	for (auto itDisc=mapDemandaDisc.cbegin(); itDisc!=mapDemandaDisc.cend(); itDisc++)
	{
		int nrCredDisc = itDisc->first->getTotalCreditos();
		totalCredsDemanda += nrCredDisc * itDisc->second.size();
		
		if (mapNrCredDiscNrDemandas.find(nrCredDisc) == mapNrCredDiscNrDemandas.end())
			mapNrCredDiscNrDemandas.insert(pair<int,int> (nrCredDisc, 0));
		mapNrCredDiscNrDemandas[nrCredDisc] += itDisc->second.size();
	}

	int totalDiscs=mapDemandaDisc.size();
	
	stringstream ss;
	ss << "\nNro disciplinas: " << totalDiscs;
	ss << "\nNro creditos demandados: " << totalCredsDemanda;
	ss << "\n----";
	for (auto itUnidDem=mapDemandaUnid.cbegin(); itUnidDem!=mapDemandaUnid.cend(); itUnidDem++)
	{
		int unid = itUnidDem->first;
		int nrAlunos = 0;
		auto finder = mapAlunosUnid.find(unid);
		if (finder != mapAlunosUnid.cend())
			nrAlunos = finder->second.size();

		ss << "\nUnidade " << unid << ": " << itUnidDem->second << " creditos demandados, " << nrAlunos << " alunos";
	}
	ss << "\n----";
	for (auto itNrCredDisc = mapNrCredDiscNrDemandas.cbegin(); itNrCredDisc != mapNrCredDiscNrDemandas.cend(); itNrCredDisc++)
	{
		ss << "\nDiscs com " << itNrCredDisc->first << " creditos: " << itNrCredDisc->second << " alunos demandando";
	}
	ss << "\n----";
	for (auto itPP=mapDemandaProfPrior.cbegin(); itPP!=mapDemandaProfPrior.cend(); itPP++)
	{
		ss << "\nNro creditos demandados que possuem habilitacao de professores de prioridade " << itPP->first << ": " << itPP->second;
	}

	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ss.str() );
}

void ProbDataAnalyzer::printRelacaoDemProfInputDataLog()
{
	// Agrupa aluno-demanda
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
			std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> mapDiscTurnoCalend;
	getDemandasMapeadas(mapDiscTurnoCalend);
		
	// Get demandas filtradas por intersecao de disponibilidade
	std::unordered_map<Disciplina*, std::map<Professor*,
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> mapDiscIntersec;
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> mapProfDiaDiscIntersec;	
	getDemandasIntersecDisponib(mapDiscTurnoCalend, mapDiscIntersec, mapProfDiaDiscIntersec);

	int totalCredsDisponib=0;
	map<int,int> totalCredsDisponibPorFase;
	for (auto itProf=mapProfDiaDiscIntersec.cbegin(); itProf!=mapProfDiaDiscIntersec.cend(); itProf++)
	{
		for (auto itDia=itProf->second.cbegin(); itDia!=itProf->second.cend(); itDia++)
		{
			totalCredsDisponib += itDia->second.size();
			for (auto itDt=itDia->second.cbegin(); itDt!=itDia->second.cend(); itDt++)
			{
				int fase = problemData_->getFaseDoDia(itDt->first);
				auto finder = totalCredsDisponibPorFase.find(fase);
				if (finder == totalCredsDisponibPorFase.end())
					finder = totalCredsDisponibPorFase.insert(pair<int,int> (fase,0)).first;
				finder->second = finder->second + 1;
			}
		}
	}
	stringstream ss;
	ss << "\nTotal de horarios disponiveis de profs: " << totalCredsDisponib;
	ss << "\n\t\t Por fase do dia: ";
	for (auto itCred=totalCredsDisponibPorFase.cbegin(); itCred!=totalCredsDisponibPorFase.cend(); itCred++)
	{
		ss << "\n\t\t\tFase " << itCred->first << ": " << itCred->second;
	}
	Indicadores::printSeparator(1);
	Indicadores::printIndicador( ss.str() );
}

void ProbDataAnalyzer::getDemandasMapeadas(
	std::unordered_map<Disciplina*, std::set<AlunoDemanda*>> &mapDemandaDisc,
	std::map<int /*profPrior*/, int /*nrCredsDemanda*/> &mapDemandaProfPrior,
	std::unordered_map<int /*unid*/, std::unordered_set<int>> &mapAlunosUnid,
	std::unordered_map<int /*unid*/, int> &mapDemandaUnid)
{
	// Agrupa aluno-demanda
	for (auto itAlDem = ProbDataAnalyzer::problemData_->alunosDemanda.begin(); 
		itAlDem != ProbDataAnalyzer::problemData_->alunosDemanda.end(); itAlDem++)
	{
		AlunoDemanda* const alDem = *itAlDem;
		Demanda* const demanda = alDem->demanda;
		Disciplina* const disciplina = demanda->disciplina;
		TurnoIES* const turno = demanda->getTurnoIES();
		Calendario* const calendario = demanda->getCalendario();

		mapDemandaDisc[disciplina].insert(alDem);

		int nrProfsP1 = disciplina->getNroProfRealImportHabilit(1);
		int nrProfsP2 = disciplina->getNroProfRealImportHabilit(2);
		if (nrProfsP1){
			if (mapDemandaProfPrior.find(1) == mapDemandaProfPrior.end())
				mapDemandaProfPrior.insert(pair<int,int> (1,0));
			mapDemandaProfPrior[1] += (disciplina->getTotalCreditos());
		}
		if (nrProfsP2){
			if (mapDemandaProfPrior.find(2) == mapDemandaProfPrior.end())
				mapDemandaProfPrior.insert(pair<int,int> (2,0));
			mapDemandaProfPrior[2] += (disciplina->getTotalCreditos());
		}
		
		std::unordered_set<int> unids;
		disciplina->getUnidsAssociadas(unids);
		for (auto it=unids.cbegin(); it!=unids.cend(); it++)
		{
			if (mapDemandaUnid.find(*it) == mapDemandaUnid.end())
				mapDemandaUnid.insert(pair<int,int> (*it,0));
			mapDemandaUnid[*it] += disciplina->getTotalCreditos();
						
			if (mapAlunosUnid.find(*it) == mapAlunosUnid.end())
			{
				std::unordered_set<int> empty;
				mapAlunosUnid.insert(pair<int,std::unordered_set<int>> (*it,empty));
			}
			mapAlunosUnid[*it].insert(alDem->getAlunoId());
		}
	}
}


// -----------------------------------------------------------------------------------------------

void ProbDataAnalyzer::getProfsIntersecMapeados(Disciplina* const disciplina, std::set<Calendario*> const &calends,
	std::set<TurnoIES*> const &turnos, std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, 
			std::unordered_map<Professor*, std::map<int, std::set<DateTime> >>>> &profsIntersec)
{
	std::map<int, std::map<DateTime, std::unordered_set<Professor*> >> diaDtsProfsIntersec;
	disciplina->getProfsIntersec(diaDtsProfsIntersec);

	for (auto itDiaDisc = diaDtsProfsIntersec.cbegin(); itDiaDisc != diaDtsProfsIntersec.cend(); itDiaDisc++)
	{
		int dia = itDiaDisc->first;

		for (auto itDtDisc = itDiaDisc->second.begin(); itDtDisc != itDiaDisc->second.end(); itDtDisc++)
		{
			DateTime dti = itDtDisc->first;
						
			for (auto itCalend = calends.begin(); itCalend != calends.end(); itCalend++)
			{
				Calendario* const calendario = *itCalend;
				
				if (!calendario->possuiHorarioDiaOuCorrespondente(disciplina->getTempoCredSemanaLetiva(), dti, dia))	continue;

				for (auto itTurno = turnos.begin(); itTurno != turnos.end(); itTurno++)
				{
					TurnoIES* const turno = *itTurno;

					if (!turno->possuiHorarioDiaOuCorrespondente(calendario, dti, dia)) continue;

					for (auto itProfDisc = itDtDisc->second.begin(); itProfDisc != itDtDisc->second.end(); itProfDisc++)
					{
						profsIntersec[turno][calendario][*itProfDisc][dia].insert(dti);
					}
				}
			}
		}
	}
}

void ProbDataAnalyzer::getProfsIntersecMapeados(Disciplina* const disciplina, std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, 
			std::unordered_map<Professor*, std::map<int, std::set<DateTime> >>>> &profsIntersec)
{
	std::map<int, std::map<DateTime, std::unordered_set<Professor*> >> diaDtsProfsIntersec;
	disciplina->getProfsIntersec(diaDtsProfsIntersec);

	for (auto itDiaDisc = diaDtsProfsIntersec.cbegin(); itDiaDisc != diaDtsProfsIntersec.cend(); itDiaDisc++)
	{
		int dia = itDiaDisc->first;

		for (auto itDtDisc = itDiaDisc->second.begin(); itDtDisc != itDiaDisc->second.end(); itDtDisc++)
		{
			DateTime dti = itDtDisc->first;
						
			for (auto itCalend = ProbDataAnalyzer::problemData_->calendarios.begin(); 
				itCalend != ProbDataAnalyzer::problemData_->calendarios.end(); itCalend++)
			{
				Calendario* const calendario = *itCalend;
				
				if (!calendario->possuiHorarioDiaOuCorrespondente(disciplina->getTempoCredSemanaLetiva(), dti, dia))	continue;

				for (auto itTurno = calendario->turnosIES.begin(); itTurno != calendario->turnosIES.end(); itTurno++)
				{
					TurnoIES* const turno = *itTurno;

					if (!turno->possuiHorarioDiaOuCorrespondente(calendario, dti, dia)) continue;

					for (auto itProfDisc = itDtDisc->second.begin(); itProfDisc != itDtDisc->second.end(); itProfDisc++)
					{
						profsIntersec[turno][calendario][*itProfDisc][dia].insert(dti);
					}
				}
			}
		}
	}
}

void ProbDataAnalyzer::getDemandasMapeadas(std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
			std::unordered_map<Calendario*, std::set<AlunoDemanda*>>>>> &mapDiscTurnoCalend)
{
	std::cout << "\ngetDemandasMapeadas";

	// Agrupa aluno-demanda
	for (auto itAlDem = ProbDataAnalyzer::problemData_->alunosDemanda.begin(); 
		itAlDem != ProbDataAnalyzer::problemData_->alunosDemanda.end(); itAlDem++)
	{
		AlunoDemanda* const alDem = *itAlDem;
		Demanda* const demanda = alDem->demanda;
		Disciplina* const disciplina = demanda->disciplina;
		TurnoIES* const turno = demanda->getTurnoIES();
		Calendario* const calendario = demanda->getCalendario();

		mapDiscTurnoCalend[disciplina->getCodigo()][disciplina][turno][calendario].insert(alDem);
	}
}

void ProbDataAnalyzer::getDemandasIntersecDisponib(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const & mapDiscDemandaTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> &mapDiscIntersec,
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> &mapProfDiaDiscIntersec)
{
	std::cout << "\ngetDemandasIntersecDisponib";

	// Get demandas filtradas por intersecao de disponibilidade

	for (auto itCodeDiscDemanda = mapDiscDemandaTurnoCalend.cbegin(); itCodeDiscDemanda != mapDiscDemandaTurnoCalend.cend(); itCodeDiscDemanda++)
	{
		for (auto itDiscDemanda = itCodeDiscDemanda->second.cbegin(); itDiscDemanda != itCodeDiscDemanda->second.cend(); itDiscDemanda++)
		{
			Disciplina* const disciplina = itDiscDemanda->first;
		
			// Acha a intersecao de disponibilidades da disc e dos profs habilitados
			std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, std::unordered_map<Professor*, 
				std::map<int, std::set<DateTime> >>>> profsIntersec;
			getProfsIntersecMapeados(disciplina, profsIntersec);
			if (profsIntersec.size()==0)
				continue;

			for (auto itTurnoDemanda = itDiscDemanda->second.cbegin(); itTurnoDemanda != itDiscDemanda->second.cend(); itTurnoDemanda++)
			{
				TurnoIES* const turno = itTurnoDemanda->first;

				auto finderTurnoProf = profsIntersec.find(turno);
				if (finderTurnoProf == profsIntersec.end())
					continue;

				for (auto itCalendDemanda = itTurnoDemanda->second.cbegin(); itCalendDemanda != itTurnoDemanda->second.cend(); itCalendDemanda++)
				{
					Calendario* const calendario = itCalendDemanda->first;

					auto finderCalendProf = finderTurnoProf->second.find(calendario);
					if (finderCalendProf == finderTurnoProf->second.end())
						continue;

					for (auto itProf = finderCalendProf->second.cbegin(); itProf != finderCalendProf->second.cend(); itProf++)
					{
						std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > par;
						par.first = itProf->second;
						par.second = itCalendDemanda->second;

						mapDiscIntersec[disciplina][itProf->first] = par;

						for (auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); itDia++)
						{
							for (auto itDti = itDia->second.cbegin(); itDti != itDia->second.cend(); itDti++)
							{
								mapProfDiaDiscIntersec[itProf->first][itDia->first][*itDti].insert(disciplina);
							}
						}
					}
				}
			}
		}
	}
}

void ProbDataAnalyzer::orderIntersecDisponibDemandas(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const &mapDiscDemandaTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> const &mapDiscIntersec,
		std::map<int, std::unordered_set<Disciplina*>> &orderDemandas)
{
	std::cout << "\norderIntersecDisponibDemandas";

	for (auto itCodeDiscDemanda = mapDiscDemandaTurnoCalend.cbegin(); 
		itCodeDiscDemanda != mapDiscDemandaTurnoCalend.cend(); itCodeDiscDemanda++)
	{
		for (auto itDiscDemanda = itCodeDiscDemanda->second.cbegin();
			itDiscDemanda != itCodeDiscDemanda->second.cend(); itDiscDemanda++)
		{
			Disciplina * const disciplina = itDiscDemanda->first;

			int nCredsDisc = disciplina->getTotalCreditos();
			double maxRazao=0;

			// Para cada disciplina com intersec de disponibilidade
			auto finderDiscDispon = mapDiscIntersec.find(disciplina);
			if (finderDiscDispon != mapDiscIntersec.cend())
			{			
				// Para cada prof com intersec de disponibilidade
				for (auto itProfDispon = finderDiscDispon->second.cbegin();
						itProfDispon != finderDiscDispon->second.cend(); itProfDispon++)
				{
					Professor * const professor = itProfDispon->first;

					// Para cada dia com intersec de disponibilidade				
					int nCredsProf=0;
					auto const * const mapDispon = & itProfDispon->second.first;
					for (auto itDiaDispon = mapDispon->cbegin(); itDiaDispon != mapDispon->cend(); itDiaDispon++)
					{
						nCredsProf += itDiaDispon->second.size();
					}
				
					int nDiasProf = mapDispon->size();
				
					double razao=0;
					if (nCredsDisc>0) razao = (double) nCredsProf / nCredsDisc;

					maxRazao = (razao > maxRazao ? razao : maxRazao);
				}			
			}

			orderDemandas[maxRazao].insert(disciplina);		
		}
	}
}

void ProbDataAnalyzer::getAlunosDisponMapeados(
	std::map<Aluno*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> & mapAlunoDispon,
	std::map<Aluno*, std::map<int, std::map<DateTime, std::set<Professor*>>>> &mapAlunoDiaDtiProfsDispon)
{
	for (auto itAluno = ProbDataAnalyzer::problemData_->alunos.begin(); 
		itAluno != ProbDataAnalyzer::problemData_->alunos.end(); itAluno++)
	{
		Aluno * const aluno = *itAluno;
		
		auto ptMapAluno = & mapAlunoDispon[aluno];
		auto ptMapAlunoProf = & mapAlunoDiaDtiProfsDispon[aluno];

		for (auto itAlDem = aluno->demandas.begin(); itAlDem != aluno->demandas.end(); itAlDem++)
		{
			AlunoDemanda* const alDem = *itAlDem;
			Disciplina* const disciplina = alDem->demanda->disciplina;
				
			// Acha a intersecao de disponibilidades da disc e dos profs habilitados
			std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, std::unordered_map<Professor*, 
				std::map<int, std::set<DateTime> >>>> profsIntersec;

			std::set<Calendario*> calends; calends.insert( alDem->demanda->getCalendario() );
			std::set<TurnoIES*> turnos; turnos.insert( alDem->demanda->getTurnoIES() );

			getProfsIntersecMapeados(disciplina, calends, turnos, profsIntersec);
			if (profsIntersec.size()==0)
				continue; // indica problema em dados de entrada!

			for (auto itTurnoDemanda = profsIntersec.cbegin(); 
				itTurnoDemanda != profsIntersec.cend(); itTurnoDemanda++)
			{
				TurnoIES* const turno = itTurnoDemanda->first;
				
				for (auto itCalendDemanda = itTurnoDemanda->second.cbegin(); 
					itCalendDemanda != itTurnoDemanda->second.cend(); itCalendDemanda++)
				{
					Calendario* const calendario = itCalendDemanda->first;
					
					for (auto itProf = itCalendDemanda->second.cbegin(); itProf != itCalendDemanda->second.cend(); itProf++)
					{						
						for (auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); itDia++)
						{
							auto ptMapAlunoDia = & (*ptMapAluno)[itDia->first];
							auto ptMapProfDia = & (*ptMapAlunoProf)[itDia->first];

							for (auto itDti = itDia->second.cbegin(); itDti != itDia->second.cend(); itDti++)
							{
								(*ptMapAlunoDia)[*itDti][disciplina->getCodigo()] = disciplina;
								(*ptMapProfDia)[*itDti].insert(itProf->first);
							}
						}
					}
				}
			}
		}
	}
}

void ProbDataAnalyzer::getUnidadesDisponMapeados(
	std::map<Unidade*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> & mapUnidadesDispon,
	std::map<Unidade*, std::map<string, Aluno*>> & mapUnidadesTurmasDispon,
	std::map<Unidade*, std::map<int, std::map<DateTime, std::set<Professor*>>>> & mapUnidadesDiaDtiProfsDispon)
{
	for (auto itAluno = ProbDataAnalyzer::problemData_->alunos.begin(); 
		itAluno != ProbDataAnalyzer::problemData_->alunos.end(); itAluno++)
	{
		Aluno * const aluno = *itAluno;
		
		for (auto itAlDem = aluno->demandas.begin(); itAlDem != aluno->demandas.end(); itAlDem++)
		{
			AlunoDemanda* const alDem = *itAlDem;
			Disciplina* const disciplina = alDem->demanda->disciplina;
				
			// Acha a intersecao de disponibilidades da disc e dos profs habilitados
			std::unordered_map<TurnoIES*, std::unordered_map<Calendario*, std::unordered_map<Professor*, 
				std::map<int, std::set<DateTime> >>>> profsIntersec;

			std::set<Calendario*> calends; calends.insert( alDem->demanda->getCalendario() );
			std::set<TurnoIES*> turnos; turnos.insert( alDem->demanda->getTurnoIES() );

			getProfsIntersecMapeados(disciplina, calends, turnos, profsIntersec);
			if (profsIntersec.size()==0)
				continue; // indica problema em dados de entrada!

			for (auto itCjtSala = disciplina->cjtSalasAssociados.begin();
				itCjtSala != disciplina->cjtSalasAssociados.end(); itCjtSala++)
			{
				Sala* const sala = itCjtSala->second->getSala();
				Unidade* const unidade = ProbDataAnalyzer::problemData_->refUnidade[ sala->getIdUnidade() ];
				
				mapUnidadesTurmasDispon[unidade][aluno->getNomeAluno()] = aluno;
				auto ptMapUnidProf = & mapUnidadesDiaDtiProfsDispon[unidade];
				auto ptMapUnidDisc = & mapUnidadesDispon[unidade];
				
				for (auto itTurnoDemanda = profsIntersec.cbegin(); 
					itTurnoDemanda != profsIntersec.cend(); itTurnoDemanda++)
				{
					TurnoIES* const turno = itTurnoDemanda->first;
				
					for (auto itCalendDemanda = itTurnoDemanda->second.cbegin(); 
						itCalendDemanda != itTurnoDemanda->second.cend(); itCalendDemanda++)
					{
						Calendario* const calendario = itCalendDemanda->first;
					
						for (auto itProf = itCalendDemanda->second.cbegin(); itProf != itCalendDemanda->second.cend(); itProf++)
						{						
							for (auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); itDia++)
							{
								auto ptMapDiscDia = & (*ptMapUnidDisc)[itDia->first];
								auto ptMapProfDia = & (*ptMapUnidProf)[itDia->first];

								for (auto itDti = itDia->second.cbegin(); itDti != itDia->second.cend(); itDti++)
								{
									(*ptMapDiscDia)[*itDti][disciplina->getCodigo()] = disciplina;
									(*ptMapProfDia)[*itDti].insert(itProf->first);
								}
							}
						}
					}
				}
			}
		}
	}
}

// ---------------------------

void ProbDataAnalyzer::getCompartilhProfUnid(
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec)
{
	std::unordered_map<Professor*, std::unordered_set<int>> mapProfUnidsId;
	getMapProfUnids(mapProfDiaDiscIntersec, mapProfUnidsId);
	
	std::unordered_map<int, std::unordered_set<Professor*>> mapUnidIdProfs;
	getMapUnidProfs(mapProfUnidsId, mapUnidIdProfs);
	
	std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> parUnidProfsComuns;
	calculaProfsComumParUnid(mapProfUnidsId, parUnidProfsComuns);
	
	std::unordered_map<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>> parProfUnidsComuns;
	calculaUnidsComumParProf(mapUnidIdProfs, parProfUnidsComuns);

	printGraphviz(mapUnidIdProfs,parUnidProfsComuns);
	//printGraphviz(parProfUnidsComuns);
}

void ProbDataAnalyzer::getMapProfUnids(
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec,
	std::unordered_map<Professor*, std::unordered_set<int>> &mapProfUnidsId)
{
	// Para cada prof com intersec de disponibilidade
	for (auto itProfDemanda = mapProfDiaDiscIntersec.cbegin();
		itProfDemanda != mapProfDiaDiscIntersec.cend(); itProfDemanda++)
	{
		Professor * const professor = itProfDemanda->first;
		if (professor->eVirtual()) continue;

		auto ptMapProfUnidsId = & mapProfUnidsId[professor];

		// Para cada dia com intersec de disponibilidade
		for (auto itDiaInters = itProfDemanda->second.cbegin();
				itDiaInters != itProfDemanda->second.cend(); itDiaInters++)
		{			
			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				for (auto itDisc = itDti->second.cbegin();
					itDisc != itDti->second.cend(); itDisc++)
				{
					std::unordered_set<int> unidIds;
					(*itDisc)->getUnidsAssociadas(unidIds);

					for (auto itUnidId = unidIds.cbegin(); itUnidId != unidIds.cend(); itUnidId++)
					{
						ptMapProfUnidsId->insert(*itUnidId);
					}
				}
			}
		}
	}
}

void ProbDataAnalyzer::getMapUnidProfs(
	std::unordered_map<Professor*, std::unordered_set<int>> const & mapProfUnidsId,
	std::unordered_map<int, std::unordered_set<Professor*>> & mapUnidIdProfs)
{
	for (auto itProfUnids = mapProfUnidsId.cbegin();
		itProfUnids != mapProfUnidsId.cend(); itProfUnids++)
	{
		Professor * const professor = itProfUnids->first;
		if (professor->eVirtual()) continue;

		for (auto itUnidId = itProfUnids->second.cbegin(); 
			itUnidId != itProfUnids->second.cend(); itUnidId++)
		{
			mapUnidIdProfs[*itUnidId].insert(professor);
		}
	}
}

void ProbDataAnalyzer::calculaProfsComumParUnid(
	std::unordered_map<Professor*, std::unordered_set<int>> const & mapProfUnidsId,
	std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> &parUnidProfsComuns)
{
	// -------------------------------------------
	// Calcula o cjt de professores em comum para cada par de unidades
	for ( auto pit = mapProfUnidsId.cbegin(); pit != mapProfUnidsId.cend(); pit++ )
    {
		if (pit->first->eVirtual()) continue;
		
		for ( auto uit1 = pit->second.cbegin(); uit1 != pit->second.cend(); uit1++ )
		{
			auto finder1 = parUnidProfsComuns.find(*uit1);
			if ( finder1 == parUnidProfsComuns.end())
			{
				std::unordered_map<int, std::unordered_set<Professor*>> empty;
				finder1 = parUnidProfsComuns.insert( 
					std::pair<int, std::unordered_map<int, std::unordered_set<Professor*>>>(*uit1, empty) ).first;
			}

			for ( auto uit2 = pit->second.cbegin(); uit2 != pit->second.cend(); uit2++ )
			{				
				if (uit1==uit2) continue;

				auto finder2 = finder1->second.find(*uit2);
				if ( finder2 == finder1->second.end())
				{
					std::unordered_set<Professor*> empty;
					finder2 = finder1->second.insert( std::pair<int, std::unordered_set<Professor*>>(*uit2, empty) ).first;
				}
				
				finder2->second.insert(pit->first);
			}
		}
	}
}

void ProbDataAnalyzer::calculaUnidsComumParProf(
	std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs,
	std::unordered_map<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>> &parProfUnidsComuns)
{
	// -------------------------------------------
	// Calcula o cjt de unidades em comum para cada par de professores
	for ( auto uit = mapUnidIdProfs.cbegin(); uit != mapUnidIdProfs.cend(); uit++ )
    {		
		for ( auto pit1 = uit->second.cbegin(); pit1 != uit->second.cend(); pit1++ )
		{
			auto finder1 = parProfUnidsComuns.find(*pit1);
			if ( finder1 == parProfUnidsComuns.end())
			{
				std::unordered_map<Professor*, std::unordered_set<int>> empty;
				finder1 = parProfUnidsComuns.insert( 
					std::pair<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>>(*pit1, empty) ).first;
			}

			for ( auto pit2 = uit->second.cbegin(); pit2 != uit->second.cend(); pit2++ )
			{				
				if (pit1==pit2) continue;

				auto finder2 = finder1->second.find(*pit2);
				if ( finder2 == finder1->second.end())
				{
					std::unordered_set<int> empty;
					finder2 = finder1->second.insert( 
						std::pair<Professor*, std::unordered_set<int>>(*pit2, empty) ).first;
				}
				
				finder2->second.insert(uit->first);
			}
		}
	}
}

int ProbDataAnalyzer::getMaxNrProfsComuns(
	std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> const &parUnidProfsComuns)
{
	int maximum = 0;
	for (auto itUnid1 = parUnidProfsComuns.cbegin();
		itUnid1 != parUnidProfsComuns.cend(); itUnid1++)
	{
		for (auto itUnid2 = itUnid1->second.cbegin();
			itUnid2 != itUnid1->second.cend(); itUnid2++)
		{		
			int nrProfs = itUnid2->second.size();
			if (nrProfs > maximum) maximum = nrProfs;
		}
	}
	return maximum;
}

int ProbDataAnalyzer::getNrProfsNaUnid(
	std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs, int unidId)
{
	auto finderU1 = mapUnidIdProfs.find(unidId);
	if (finderU1 == mapUnidIdProfs.end()) return 0;
	return finderU1->second.size();
}

void ProbDataAnalyzer::printGraphviz(
	std::unordered_map<int, std::unordered_set<Professor*>> const & mapUnidIdProfs,
	std::unordered_map<int, std::unordered_map<int, std::unordered_set<Professor*>>> const &parUnidProfsComuns)
{
	stringstream sName;
	sName << "parUnidProfsComuns-";
	sName << problemData_->getInputFileName();
	sName << ".gv";

	std::ofstream out(sName.str(),ios::out);
	if (!out) return;
	
	bool const digraph=false;

	int maxNrProfs = getMaxNrProfsComuns(parUnidProfsComuns);

	if (digraph) out << "digraph G {";
	else out << "graph G {";

	for (auto itUnid1 = parUnidProfsComuns.cbegin();
		itUnid1 != parUnidProfsComuns.cend(); itUnid1++)
	{
		int const unid1 = itUnid1->first;
		int const totalProfsU1 = getNrProfsNaUnid(mapUnidIdProfs,unid1);

		stringstream node1;
		node1 << "u" << unid1 << " [" << totalProfsU1 << "]";

		for (auto itUnid2 = itUnid1->second.cbegin();
			itUnid2 != itUnid1->second.cend(); itUnid2++)
		{
			int const unid2 = itUnid2->first;
			int const nrProfsComuns = itUnid2->second.size();
			int const totalProfsU2 = getNrProfsNaUnid(mapUnidIdProfs,unid2);
			int max = 0;

			stringstream node2;
			node2 << "u" << unid2 << " [" << totalProfsU2 << "]";

			if (!digraph)
			{
				if (unid1 >= unid2) continue; // relacao <--> nao repete
				out << std::endl << node1.str() << " -- " << node2.str();
				//max = maxNrProfs;
				max = min(totalProfsU1,totalProfsU2);
			}
			else
			{
				out << std::endl << node1.str() << " -> " << node2.str();
				max = totalProfsU1;
			}

			out << " [ label = \"" << nrProfsComuns << "\""
				<< " color=\"" << COLOR[getColorIdx(max,nrProfsComuns)] << "\" ];";
		}
	}
	out << std::endl << "}";
	out.flush();
	out.close();
}

void ProbDataAnalyzer::printGraphviz(
	std::unordered_map<Professor*, std::unordered_map<Professor*, std::unordered_set<int>>> const &parProfUnidsComuns)
{
	stringstream sName;
	sName << "parProfUnidsComuns-";
	sName << problemData_->getInputFileName();
	sName << ".gv";

	std::ofstream out(sName.str(),ios::out);
	if (!out) return;

	out << "graph G {";
	for (auto itProf1 = parProfUnidsComuns.cbegin();
		itProf1 != parProfUnidsComuns.cend(); itProf1++)
	{
		Professor* const prof1 = itProf1->first;

		for (auto itProf2 = itProf1->second.cbegin();
			itProf2 != itProf1->second.cend(); itProf2++)
		{
			Professor* const prof2 = itProf2->first;

			if (prof1->getId() >= prof2->getId()) continue; // relacao <--> nao repete

			out << std::endl << "p" << prof1->getId() << " -- " << "p" << prof2->getId();
			
			int nrUnids = itProf2->second.size();

			out << " [ label = \"" << nrUnids << "\" ];";
		}
	}
	out << std::endl << "}";
	out.flush();
	out.close();
}

int ProbDataAnalyzer::getColorIdx(int maximum, int value)
{
	double delta = (double) maximum / NrColors;
	
	double pos = (double) value / delta;
	int idx = (int) pos; // parte inteira
	
	if (idx == pos) idx--;
		
	return idx;
}

// ---------------------------

void ProbDataAnalyzer::printOrderedIntersecDisponibDemandas(
		std::map<int, std::unordered_set<Disciplina*>> const &orderDemandas)
{
	std::cout << "\nprintOrderedIntersecDisponibDemandas";

	// Print summary
	std::ofstream out("nrcreds-intersec-disponib-disc.txt",ios::out);
	if (!out) return;

	out << "\n\n\n-------------------------------------------------------------------------";
	out << "\nRazao ordenada entre nr creditos disponiveis do professor e nr de creditos da disciplina:\n";
	for (auto itOrderRazao = orderDemandas.cbegin(); itOrderRazao != orderDemandas.cend(); itOrderRazao++)
	{
		out << "\nCredsProf / CredsDisc = " << itOrderRazao->first;
		out << "\t\tQtd de discs = " << itOrderRazao->second.size();

		// Reordena por codigo, para que disciplinas semelhantes fiquem ordenadas
		std::map<string, Disciplina*> reorderedByCode;
		for (auto itDisc = itOrderRazao->second.cbegin(); itDisc != itOrderRazao->second.cend(); itDisc++)
		{
			reorderedByCode[(*itDisc)->getCodigo()] = (*itDisc);
		}

		for (auto itCodeDisc = reorderedByCode.cbegin(); itCodeDisc != reorderedByCode.cend(); itCodeDisc++)
		{
			out << "\n\tDisc " << itCodeDisc->second->getId() << " - " << itCodeDisc->second->getCodigo();
		}
	}
	out.flush();
	out.close();
}

void ProbDataAnalyzer::printFullIntersecDisponibDemandas(
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
		std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> const & mapDiscDemandaTurnoCalend,
	std::unordered_map<Disciplina*, std::map<Professor*, 
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> const &mapDiscIntersec)
{
	std::cout << "\nprintFullIntersecDisponibDemandas";

	std::ofstream out("disc-intersec-disponib.txt",ios::out);
	if (!out) return;

	for (auto itCodeDiscDemanda = mapDiscDemandaTurnoCalend.cbegin(); 
		itCodeDiscDemanda != mapDiscDemandaTurnoCalend.cend(); itCodeDiscDemanda++)
	{
		for (auto itDiscDemanda = itCodeDiscDemanda->second.cbegin();
			itDiscDemanda != itCodeDiscDemanda->second.cend(); itDiscDemanda++)
		{
			Disciplina * const disciplina = itDiscDemanda->first;

			int nCredsDisc = disciplina->getTotalCreditos();

			out << "\n------------------------";
			out << "\nDisciplina " << disciplina->getId() << " - " << disciplina->getCodigo();
			out << " --- " << nCredsDisc << " creditos";

			// Para cada disciplina com intersec de disponibilidade
			auto finderDiscDispon = mapDiscIntersec.find(disciplina);
			if (finderDiscDispon != mapDiscIntersec.cend())
			{			
				// Para cada prof com intersec de disponibilidade
				for (auto itProfDispon = finderDiscDispon->second.cbegin();
						itProfDispon != finderDiscDispon->second.cend(); itProfDispon++)
				{
					Professor * const professor = itProfDispon->first;

					// Para cada dia com intersec de disponibilidade
				
					int nCredsProf=0;
					auto const * const mapDispon = & itProfDispon->second.first;
					for (auto itDiaDispon = mapDispon->cbegin(); itDiaDispon != mapDispon->cend(); itDiaDispon++)
					{
						nCredsProf += itDiaDispon->second.size();
					}
				
					int nDiasProf = mapDispon->size();

					// Para cada aluno-demanda com intersec de disponibilidade
					auto const * const alunosDispon = & itProfDispon->second.second;
					
					double razao=0;
					if (nCredsDisc>0) razao = (double) nCredsProf / nCredsDisc;

					out << "\n\tProf " << professor->getId() << " - " << professor->getNome();
					out << "\n\t\t" << nCredsProf << " creditos do prof";
					out << "\n\t\t" << nDiasProf << " dias do prof";
					out << "\n\t\t" << alunosDispon->size() << " turmas";
					out << "\n\tCredsProf / CredsDisc = " << razao;
				}
			}
			out.flush();
		}
	}
	out.close();
}

void ProbDataAnalyzer::printProfDiaDisponibDiscs(
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> const &mapProfDiaDiscIntersec)
{
	std::cout << "\nprintFullIntersecDisponibDemandas";

	std::ofstream out("prof-dia-intersec-disponib.txt",ios::out);
	if (!out) return;

	// Para cada prof com intersec de disponibilidade
	for (auto itProfDemanda = mapProfDiaDiscIntersec.cbegin();
		itProfDemanda != mapProfDiaDiscIntersec.cend(); itProfDemanda++)
	{
		Professor * const professor = itProfDemanda->first;

		out << "\n------------------------";
		out << "\nProfessor " << professor->getId() << " - " << professor->getNome();
			
		// Para cada dia com intersec de disponibilidade
		for (auto itDiaInters = itProfDemanda->second.cbegin();
				itDiaInters != itProfDemanda->second.cend(); itDiaInters++)
		{
			int dia = itDiaInters->first;
			
			out << "\n\tDia " << dia;
			out << "\n\t\tMaximo de creds livres no dia: " << itDiaInters->second.size();
			out << "\n\t\tNr de discs associadas no dia: "  ;
						
			std::set<Disciplina*> discs;
			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				for (auto itDisc = itDti->second.cbegin();
					itDisc != itDti->second.cend(); itDisc++)
				{
					discs.insert(*itDisc);
				}
			}
			out << discs.size();

			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				out << "\n\t\t  " << itDti->first.hourMinToStr() 
					<< "  (" << itDti->second.size() << " disciplinas): ";
				
				// Reordena por codigo, para que disciplinas semelhantes fiquem ordenadas
				std::map<string, Disciplina*> reorderedByCode;
				for (auto itDisc = itDti->second.cbegin(); itDisc != itDti->second.cend(); itDisc++)
				{
					reorderedByCode[(*itDisc)->getCodigo()] = (*itDisc);
				}

				for (auto itCodeDisc = reorderedByCode.cbegin(); itCodeDisc != reorderedByCode.cend(); itCodeDisc++)
				{
					out << " " << itCodeDisc->second->getCodigo();
				}

			}
		}
		out.flush();
	}
	out.close();
}

void ProbDataAnalyzer::printAlunosDisponMapeados(
	std::map<Aluno*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> const & mapAlunoDispon,
	std::map<Aluno*, std::map<int, std::map<DateTime, std::set<Professor*>>>> const &mapAlunoDiaDtiProfsDispon)
{
	std::cout << "\nprintAlunosDisponMapeados";

	std::ofstream out("turma-dia-intersec-disponib.txt",ios::out);
	if (!out) return;

	// Para cada prof com intersec de disponibilidade
	for (auto itAluno = mapAlunoDispon.cbegin();
		itAluno != mapAlunoDispon.cend(); itAluno++)
	{
		Aluno * const aluno = itAluno->first;

		out << "\n------------------------";
		out << "\nTurma " << aluno->getNomeAluno() << " - " << aluno->getAlunoId();
		
		auto finderAlunoDiaDtiProf = mapAlunoDiaDtiProfsDispon.find(aluno);
		if (finderAlunoDiaDtiProf == mapAlunoDiaDtiProfsDispon.end()) continue;

		// Para cada dia com intersec de disponibilidade
		for (auto itDiaInters = itAluno->second.cbegin();
				itDiaInters != itAluno->second.cend(); itDiaInters++)
		{
			int dia = itDiaInters->first;
			
			out << "\n\tDia " << dia;
			out << "\n\t\tMaximo de creds livres no dia: " << itDiaInters->second.size();
			out << "\n\t\tNr de discs associadas no dia: "  ;
			
			std::set<Disciplina*> uniaoDiscs;
			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				for (auto itDisc = itDti->second.cbegin();
					itDisc != itDti->second.cend(); itDisc++)
				{
					uniaoDiscs.insert(itDisc->second);
				}
			}
			out << uniaoDiscs.size();
					
			auto finderDiaDtiProf = finderAlunoDiaDtiProf->second.find(dia);
			if (finderDiaDtiProf == finderAlunoDiaDtiProf->second.end()) continue;
		
			out << "\n\t\tNr de profs associados no dia: ";
			std::set<Professor*> uniaoProfs;
			for (auto itDti = finderDiaDtiProf->second.cbegin();
				itDti != finderDiaDtiProf->second.cend(); itDti++)
			{
				for (auto itProf = itDti->second.cbegin();
					itProf != itDti->second.cend(); itProf++)
				{
					uniaoProfs.insert(*itProf);
				}
			}
			out << uniaoProfs.size();

			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				out << "\n\t\t  " << itDti->first.hourMinToStr() 
					<< "  (" << itDti->second.size() << " disciplinas, ";
								
				auto finderDtiProf = finderDiaDtiProf->second.find(itDti->first);
				if (finderDtiProf == finderDiaDtiProf->second.end()) continue;

				out << finderDtiProf->second.size() << " profs): ";

				for (auto itCodeDisc = itDti->second.cbegin(); itCodeDisc != itDti->second.cend(); itCodeDisc++)
				{
					out << " " << itCodeDisc->first;
				}

			}
		}
		out.flush();
	}
	out.close();
}

void ProbDataAnalyzer::printUnidadesDisponMapeados(
	std::map<Unidade*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> const & mapUnidadesDispon,
	std::map<Unidade*, std::map<string, Aluno*>> const & mapUnidadesTurmasDispon,
	std::map<Unidade*, std::map<int, std::map<DateTime, std::set<Professor*>>>> const &mapUnidadesDiaDtiProfsDispon)
{
	std::cout << "\nprintUnidadesDisponMapeados";

	std::ofstream out("unidade-dia-intersec-disponib.txt",ios::out);
	if (!out) return;

	// Para cada prof com intersec de disponibilidade
	for (auto itUnid = mapUnidadesDispon.cbegin();
		itUnid != mapUnidadesDispon.cend(); itUnid++)
	{
		Unidade * const unidade = itUnid->first;

		out << "\n------------------------";
		out << "\nUnidade " << unidade->getNome() << " - " << unidade->getId();
		
		// Informa todos os alunos da unidade
		out << "\n\tTurmas:";
		auto finderUnidTurmas = mapUnidadesTurmasDispon.find(unidade);
		if (finderUnidTurmas == mapUnidadesTurmasDispon.end()) continue;

		for (auto itAluno = finderUnidTurmas->second.cbegin();
			itAluno != finderUnidTurmas->second.cend(); itAluno++)
		{
			out << " " << itAluno->first;
		}
		out << endl;

		auto finderUnidDiaDtiProf = mapUnidadesDiaDtiProfsDispon.find(unidade);
		if (finderUnidDiaDtiProf == mapUnidadesDiaDtiProfsDispon.end()) continue;

		// Para cada dia com intersec de disponibilidade
		for (auto itDiaInters = itUnid->second.cbegin();
				itDiaInters != itUnid->second.cend(); itDiaInters++)
		{
			int dia = itDiaInters->first;
			
			out << "\n\tDia " << dia;
			out << "\n\t\tMaximo de creds livres no dia: " << itDiaInters->second.size();

			out << "\n\t\tNr de discs associadas no dia: ";	
			std::set<Disciplina*> uniaoDiscs;
			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				for (auto itDisc = itDti->second.cbegin();
					itDisc != itDti->second.cend(); itDisc++)
				{
					uniaoDiscs.insert(itDisc->second);
				}
			}
			out << uniaoDiscs.size();
		
			auto finderDiaDtiProf = finderUnidDiaDtiProf->second.find(dia);
			if (finderDiaDtiProf == finderUnidDiaDtiProf->second.end()) continue;
		
			out << "\n\t\tNr de profs associados no dia: ";
			std::set<Professor*> uniaoProfs;
			for (auto itDti = finderDiaDtiProf->second.cbegin();
				itDti != finderDiaDtiProf->second.cend(); itDti++)
			{
				for (auto itProf = itDti->second.cbegin();
					itProf != itDti->second.cend(); itProf++)
				{
					uniaoProfs.insert(*itProf);
				}
			}
			out << uniaoProfs.size();


			for (auto itDti = itDiaInters->second.cbegin();
				itDti != itDiaInters->second.cend(); itDti++)
			{
				out << "\n\t\t  " << itDti->first.hourMinToStr() 
					<< "  (" << itDti->second.size() << " disciplinas, ";
								
				auto finderDtiProf = finderDiaDtiProf->second.find(itDti->first);
				if (finderDtiProf == finderDiaDtiProf->second.end()) continue;

				out << finderDtiProf->second.size() << " profs): ";

				for (auto itCodeDisc = itDti->second.cbegin(); itCodeDisc != itDti->second.cend(); itCodeDisc++)
				{
					out << " " << itCodeDisc->first;
				}

			}
		}
		out.flush();
	}
	out.close();
}

// ---------------------------

void ProbDataAnalyzer::estatisticasDemandasEscola()
{
	if (!CentroDados::getPrintLogs())
		return;

	// ---------------------
	// Agrupa aluno-demanda
	std::map<string, std::unordered_map<Disciplina*, std::unordered_map<TurnoIES*, 
			std::unordered_map<Calendario*, std::set<AlunoDemanda*>> >>> mapDiscTurnoCalend;
	getDemandasMapeadas(mapDiscTurnoCalend);
		
	// --------------------
	// Get demandas filtradas por intersecao de disponibilidade
	std::unordered_map<Disciplina*, std::map<Professor*,
		std::pair< std::map<int, std::set<DateTime>>, std::set<AlunoDemanda*> > >> mapDiscIntersec;
	std::unordered_map<Professor*, std::map<int, std::map<DateTime, std::unordered_set<Disciplina*>>>> mapProfDiaDiscIntersec;	
	getDemandasIntersecDisponib(mapDiscTurnoCalend, mapDiscIntersec, mapProfDiaDiscIntersec);
	
	// --------------------
	std::map<int, std::unordered_set<Disciplina*>> orderDemandas;
	orderIntersecDisponibDemandas(mapDiscTurnoCalend, mapDiscIntersec, orderDemandas);
	
	// --------------------
	std::map<Aluno*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> mapAlunoDispon;
	std::map<Aluno*, std::map<int, std::map<DateTime, std::set<Professor*>>>> mapAlunoDiaDtiProfsDispon;
	getAlunosDisponMapeados(mapAlunoDispon, mapAlunoDiaDtiProfsDispon);
	
	// --------------------
	std::map<Unidade*, std::map<int, std::map<DateTime, std::map<string,Disciplina*>>>> mapUnidadesDiscDispon;
	std::map<Unidade*, std::map<string, Aluno*>> mapUnidadesTurmasDispon;
	std::map<Unidade*, std::map<int, std::map<DateTime, std::set<Professor*>>>> mapUnidadesDiaDtiProfsDispon;
	getUnidadesDisponMapeados(mapUnidadesDiscDispon, mapUnidadesTurmasDispon, mapUnidadesDiaDtiProfsDispon);

	// --------------------
	getCompartilhProfUnid(mapProfDiaDiscIntersec);
	

	// --------------------
	printFullIntersecDisponibDemandas(mapDiscTurnoCalend, mapDiscIntersec);
	// --------------------
	printOrderedIntersecDisponibDemandas(orderDemandas);
	// --------------------
	printProfDiaDisponibDiscs(mapProfDiaDiscIntersec);
	// --------------------
	printAlunosDisponMapeados(mapAlunoDispon, mapAlunoDiaDtiProfsDispon);
	// --------------------
	printUnidadesDisponMapeados(mapUnidadesDiscDispon, mapUnidadesTurmasDispon, mapUnidadesDiaDtiProfsDispon);
}