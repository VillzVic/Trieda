#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include <iostream>

using namespace std;

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
	this->inputFile = inputFile;
	this->problemData  = data;
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
	std::cout << "Loading file..." << std::endl;
	root = std::auto_ptr<TriedaInput>(TriedaInput_(inputFile, xml_schema::flags::dont_validate));
	std::cout << "Extracting data..." << std::endl;
	problemData->le_arvore(*root);

	std::cout << "Some preprocessing..." << std::endl;
	/* processamento */
	gera_refs();
	cria_blocos_curriculares();
	calcula_demandas();
	print_stats();
	estima_turmas();
	cache();
}
template<class T> 
void ProblemDataLoader::find_and_set(int id, 
									 GGroup<T*>& haystack, 
									 T*& needle)
{
	T* finder = new T;
	finder->id = id;
	GGroup<T*>::iterator it_g = 
		haystack.find(finder);
	/* Versão lenta... Entender o porquê depois */
	it_g = haystack.begin();
	while(it_g != haystack.end() && it_g->getId() != finder->getId())
		++it_g;
	/* FIM */
	if (it_g != haystack.end()) 
		needle = *it_g;
	delete finder;
}

void ProblemDataLoader::gera_refs() {
	ITERA_GGROUP(it_campi,problemData->campi,Campus) {
		ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade) {
			ITERA_GGROUP(it_horario,it_unidades->horarios,Horario) {
				find_and_set(it_horario->turnoId,
					problemData->calendario->turnos,
					it_horario->turno);
				find_and_set(it_horario->horarioAulaId,
					it_horario->turno->horarios_aula,
					it_horario->horario_aula);
			}
			ITERA_GGROUP(it_salas,it_unidades->salas,Sala) {
				find_and_set(it_salas->tipo_sala_id,
					problemData->tipos_sala,
					it_salas->tipo_sala);
				ITERA_GGROUP(it_horario,it_salas->horarios_disponiveis,Horario)
				{
					find_and_set(it_horario->turnoId,
						problemData->calendario->turnos,
						it_horario->turno);
					find_and_set(it_horario->horarioAulaId,
						it_horario->turno->horarios_aula,
						it_horario->horario_aula);
				}
				ITERA_GGROUP(it_credito,it_salas->creditos_disponiveis,
					CreditoDisponivel) 
				{
					find_and_set(it_credito->turno_id,
						problemData->calendario->turnos,
						it_credito->turno);
				}
				/* Disciplinas associadas ? 
				TODO (ou não) */
			} // end salas
		}
		ITERA_GGROUP(it_prof,it_campi->professores,Professor) {
			find_and_set(it_prof->tipo_contrato_id, 
				problemData->tipos_contrato, 
				it_prof->tipo_contrato);
			ITERA_GGROUP(it_horario,it_prof->horarios,Horario)
			{
				find_and_set(it_horario->turnoId,
					problemData->calendario->turnos,
					it_horario->turno);
				find_and_set(it_horario->horarioAulaId,
					it_horario->turno->horarios_aula,
					it_horario->horario_aula);
			}
			ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
				find_and_set(it_mag->disciplina_id,
					problemData->disciplinas,
					it_mag->disciplina);
			}
		} // end professores
		ITERA_GGROUP(it_horario,it_campi->horarios,Horario)
		{
			find_and_set(it_horario->turnoId,
				problemData->calendario->turnos,
				it_horario->turno);
			find_and_set(it_horario->horarioAulaId,
				it_horario->turno->horarios_aula,
				it_horario->horario_aula);
		} 
	} // campus

	ITERA_GGROUP(it_desl,problemData->tempo_campi,Deslocamento) {
		find_and_set(it_desl->origem_id,
			problemData->campi, (Campus*&) it_desl->origem);
		find_and_set(it_desl->destino_id,
			problemData->campi, (Campus*&) it_desl->destino);
	} // deslocamento campi

	ITERA_GGROUP(it_desl,problemData->tempo_unidades,Deslocamento) {
		/* É preciso procurar a unidade nos campi */
		ITERA_GGROUP(it_campi,problemData->campi,Campus) {
			/* posso fazer find_and_set em todos sem ifs, porque ele
			só seta se encontrar. Posso continuar fazendo mesmo depois de 
			encontrar pelo mesmo motivo */
			find_and_set(it_desl->origem_id,
				it_campi->unidades, (Unidade*&) it_desl->origem);
			find_and_set(it_desl->destino_id,
				it_campi->unidades, (Unidade*&) it_desl->destino);
		}
	} // deslocamento unidades 

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		find_and_set(it_disc->tipo_disciplina_id,
			problemData->tipos_disciplina, it_disc->tipo_disciplina);
		find_and_set(it_disc->nivel_dificuldade_id,
			problemData->niveis_dificuldade, it_disc->nivel_dificuldade);

		ITERA_GGROUP(it_horario,it_disc->horarios,Horario)
		{
			find_and_set(it_horario->turnoId,
				problemData->calendario->turnos,
				it_horario->turno);
			find_and_set(it_horario->horarioAulaId,
				it_horario->turno->horarios_aula,
				it_horario->horario_aula);
		} 
	} // disciplinas
	ITERA_GGROUP(it_curso, problemData->cursos, Curso) {
		find_and_set(it_curso->tipo_id,
			problemData->tipos_curso, it_curso->tipo_curso);
	}

	ITERA_GGROUP(it_oferta,problemData->ofertas,Oferta) {
		find_and_set(it_oferta->curso_id,
			problemData->cursos,
			it_oferta->curso);
		find_and_set(it_oferta->curriculo_id,
			it_oferta->curso->curriculos,
			it_oferta->curriculo);
		find_and_set(it_oferta->turno_id,
			problemData->calendario->turnos,
			it_oferta->turno);
		find_and_set(it_oferta->campus_id,
			problemData->campi,it_oferta->campus);
	}

	ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
		find_and_set(it_dem->oferta_id,
			problemData->ofertas, it_dem->oferta);
		find_and_set(it_dem->disciplina_id,
			problemData->disciplinas, it_dem->disciplina);
	}
	/* Falta: parametros (?) e fixacoes */
	ITERA_GGROUP(it_ndh,
		problemData->parametros->niveis_dificuldade_horario,
		NivelDificuldadeHorario) {
			find_and_set(it_ndh->nivel_dificuldade_id,
				problemData->niveis_dificuldade,
				it_ndh->nivel_dificuldade);
	}
	ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao) {
		find_and_set(it_fix->disciplina_id, 
			problemData->disciplinas,it_fix->disciplina);
		find_and_set(it_fix->turno_id, 
			problemData->calendario->turnos,
			it_fix->turno);
		find_and_set(it_fix->horario_id,
			it_fix->turno->horarios_aula,
			it_fix->horario);

		ITERA_GGROUP(it_campi,problemData->campi,Campus) {
			find_and_set(it_fix->professor_id, 
				it_campi->professores, it_fix->professor);
			ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade) {
				find_and_set(it_fix->sala_id,
					it_unidades->salas, it_fix->sala);
			}
		}
	}
}

void ProblemDataLoader::cria_blocos_curriculares() {
	/* cria blocos curriculares */
	ITERA_GGROUP(it_campi,problemData->campi,Campus) {

		ITERA_GGROUP(it_curso,problemData->cursos,Curso)
		{
			ITERA_GGROUP(it_curr,it_curso->curriculos,Curriculo)
			{
				GGroup<DisciplinaPeriodo>::iterator it_dp = 
					it_curr->disciplinas_periodo.begin();
				for(;it_dp != it_curr->disciplinas_periodo.end(); ++it_dp)
				{
					DisciplinaPeriodo dp = *it_dp;
					int p = dp.first;
					int disc_id = dp.second;
					Disciplina* d = new Disciplina;
					d->id = disc_id;

					if (problemData->disciplinas.find(d) != 
						problemData->disciplinas.end())
						d = *problemData->disciplinas.find(d);

					/* Versão lenta, find não está funcionando :( */
					for(GGroup<Disciplina*>::iterator it_d = 
						problemData->disciplinas.begin();
						it_d != problemData->disciplinas.end(); ++it_d) 
					{
						if (it_d->getId() == d->getId())
							d = *it_d;
					}
					/* FIM */            

					BlocoCurricular* b = new BlocoCurricular;
					b->curso = *it_curso;
					b->periodo = p;
					b->campus = *it_campi;
					GGroup<BlocoCurricular*>::iterator it_bc = 
						problemData->blocos.find(b);
					if (it_bc != problemData->blocos.end())
					{
						delete b;
						b = *it_bc;
					}

					else {
						problemData->blocos.add(b);
					}
					b->disciplinas.add(d);
				}
			}
		}
	}
}

void ProblemDataLoader::calcula_demandas() {
	ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
		int dem = it_dem->quantidade;
		it_dem->disciplina->max_demanda = 
			std::max(it_dem->disciplina->max_demanda,dem);
		it_dem->disciplina->demanda_total += dem;

		std::pair<int,int> dc = std::make_pair(
			it_dem->disciplina->getId(),
			it_dem->oferta->campus->getId());

		// inicializa com zero caso ainda não exista;
		if(problemData->demandas_campus.find(dc) !=
			problemData->demandas_campus.end())
			problemData->demandas_campus[dc] = 0;

		problemData->demandas_campus[dc] += dem;
	}
}
void ProblemDataLoader::estima_turmas() {
	ITERA_GGROUP(it_disc,problemData->disciplinas, Disciplina) {
		int tam_turma; // 10 alunos por turma
		if(it_disc->max_alunos_t > 0 && 
			it_disc->max_alunos_p > 0) 
			tam_turma = std::min(it_disc->max_alunos_p,
			it_disc->max_alunos_t);
		else 
			tam_turma = std::max(it_disc->max_alunos_p,
			it_disc->max_alunos_t);
		if (tam_turma < 0)
			tam_turma = 10; 
		else 
			tam_turma /= 2; 

		if(it_disc->demanda_total == 0)
			it_disc->num_turmas = 0;
		else
			it_disc->num_turmas = it_disc->demanda_total/tam_turma + 1;

		std::cout << "Decidi abrir " << it_disc->num_turmas << 
			" turmas da disciplina " << it_disc->codigo << std::endl;
	}
}
void ProblemDataLoader::print_stats() {
	int ncampi,nunidades,ndiscs,nprofs,ncursos,nofertas,tdemanda;

	ncampi = problemData->campi.size();
	nunidades = 0, nprofs = 0, ncursos = 0;
	ITERA_GGROUP(it_campi,problemData->campi,Campus) {
		nunidades += it_campi->unidades.size();
		nprofs += it_campi->professores.size();
		ncursos += problemData->cursos.size();
	}
	nofertas = problemData->ofertas.size();
	ndiscs = problemData->disciplinas.size();
	tdemanda = 0;
	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		tdemanda += it_disc->demanda_total;
	}


	std::cout << std::endl;
	std::cout << "Estatisticas de dados de entrada" << std::endl;
	std::cout << "================================" << std::endl;
	printf("Campi:        \t%4d\n",ncampi);
	printf("Unidades:     \t%4d\n",nunidades);
	printf("Disciplinas:  \t%4d\n",ndiscs);
	printf("Professores:  \t%4d\n",nprofs);
	printf("Cursos:       \t%4d\n",ncursos);
	printf("Ofertas:      \t%4d\n",nofertas);
	printf("Demanda total:\t%4d vagas\n",tdemanda);
	std::cout << "================================" << std::endl
		<< std::endl;

}
/* Salva algumas informações que são usadas frequentemente */
void ProblemDataLoader::cache() {
	problemData->total_salas = 0;
	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		it_campus->total_salas = 0;
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			it_campus->total_salas += it_u->salas.size();
		}
		problemData->total_salas += it_campus->total_salas;
	}

	problemData->total_turmas = 0;
	ITERA_GGROUP(it_disciplinas,problemData->disciplinas,Disciplina) {
		problemData->total_turmas += it_disciplinas->num_turmas;
	}

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
		it_bloco->total_turmas = 0;
		ITERA_GGROUP(it_disciplinas,it_bloco->disciplinas,Disciplina) {
			it_bloco->total_turmas += it_disciplinas->num_turmas;
		}
	}

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		if (it_disc->divisao_creditos != NULL) {
			it_disc->min_creds = 24;
			it_disc->max_creds = 0;
			for(int t=0;t<8;t++) {
				it_disc->min_creds = 
					std::min(it_disc->min_creds,
					it_disc->divisao_creditos->dia[t]);
				it_disc->max_creds = 
					std::max(it_disc->max_creds,
					it_disc->divisao_creditos->dia[t]);

			}
		}
	}
	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		it_campus->maior_sala = 0;
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			it_u->maior_sala = 0;
			ITERA_GGROUP(it_sala,it_u->salas,Sala)
				it_u->maior_sala = std::max(it_u->maior_sala,
				it_sala->capacidade);

			it_campus->maior_sala = std::max(it_campus->maior_sala,
				it_u->maior_sala);
		}
	}

	// >>>
	
	//Definindo um map de compatibilidade e incompatibilidade entre 2 turmas.

	ITERA_GGROUP(it_fix_curso,problemData->cursos,Curso) {
		ITERA_GGROUP(it_alt_curso,problemData->cursos,Curso) {
			//std::pair<int,int> idCursos = std::make_pair(it_fix_curso->getId(),it_alt_curso->getId());

			std::pair<Curso*,Curso*> idCursos = 
				std::make_pair(*it_fix_curso,*it_alt_curso);

			problemData->compat_cursos[idCursos] = false;
		}
	}

	ITERA_GGROUP(it_fix_curso,problemData->cursos,Curso) {

		GGroup<GGroup<int>*>::iterator it_list_compat =
			problemData->parametros->permite_compart_turma.begin();

		for(;it_list_compat!=problemData->parametros->permite_compart_turma.end();++it_list_compat) {
			if( it_list_compat->find(it_fix_curso->getId()) != it_list_compat->end() ) {
				ITERA_GGROUP(it_alt_curso,problemData->cursos,Curso) {
					if(it_list_compat->find(it_alt_curso->getId()) != it_list_compat->end()) {
						//std::pair<int,int> idCursos = std::make_pair(it_fix_curso->getId(),it_alt_curso->getId());
						std::pair<Curso*,Curso*> idCursos =
							std::make_pair(*it_fix_curso,*it_alt_curso);

						problemData->compat_cursos[idCursos] = true;
					}
				}
			}
		}
	}
	

/*
	Impressao para validar.

	ITERA_GGROUP(it_fix_curso,problemData->cursos,Curso) {
		ITERA_GGROUP(it_alt_curso,problemData->cursos,Curso) {
			std::pair<int,int> idCursos = 
				std::make_pair(it_fix_curso->getId(),it_alt_curso->getId());

			if(problemData->compat_cursos.find(idCursos) != problemData->compat_cursos.end()) {
				std::cout << "it_fix_curso <(" << it_fix_curso->getId() 
					<< ") - it_alt_curso (" << it_alt_curso->getId() << ")> , " 
					<< problemData->compat_cursos[idCursos] << std::endl;
			}
		}
	}
	exit(1);
*/

	// <<<
	

}