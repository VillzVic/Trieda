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
	//>>>
	divideDisciplinas();
	// <<<
	gera_refs();
	cria_blocos_curriculares();
	calcula_demandas();
	print_stats();
	estima_turmas();
	cache();
	print_csv();

}

template<class T> 
void ProblemDataLoader::find_and_set(int id, 
									 GGroup<T*>& haystack, 
									 T*& needle,bool print = false)
{
	T* finder = new T;
	finder->id = id;
	GGroup<T*>::iterator it_g = haystack.begin();
	//	haystack.find(finder);
	/* Vers�o lenta... Entender o porqu� depois */
	//it_g = haystack.begin();
	while(it_g != haystack.end() && it_g->getId() != finder->getId()){
		++it_g;
	}
	/* FIM */
	if (it_g != haystack.end()) {
		needle = *it_g;
		if(print) {
			std::cout << "Found " << id << std::endl;
		}
	}
	delete finder;
}

void ProblemDataLoader::divideDisciplinas() {
	GGroup<Disciplina*> disciplinas_novas;
	//GGroup<Disciplina> disciplinas_novas;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		if(it_disc->cred_teoricos > 0 && it_disc->cred_praticos > 0 && it_disc->e_lab){
			Disciplina *nova_disc = new Disciplina();

			// >>> Implementar o operador de atribui��o seria mais correto.
			//nova_disc = *it_disc;
			// <<<

			nova_disc->setId(-it_disc->getId()); // alterado

			nova_disc->codigo = it_disc->codigo + "-P";
			nova_disc->nome = it_disc->nome + "PRATICA";

			//nova_disc->cred_teoricos = it_disc->cred_teoricos;
			nova_disc->cred_teoricos = 0; // alterado

			nova_disc->cred_praticos = it_disc->cred_praticos; // alterado
			it_disc->cred_praticos = 0; // alterado

			nova_disc->max_creds = it_disc->max_creds;

			nova_disc->e_lab = it_disc->e_lab; // alterado
			it_disc->e_lab = false; // alterado

			nova_disc->max_alunos_t = it_disc->max_alunos_t; // alterado
			nova_disc->max_alunos_t = -1; // alterado

			nova_disc->max_alunos_p = it_disc->max_alunos_p; // alterado
			it_disc->max_alunos_p = -1; // alterado

			nova_disc->tipo_disciplina_id = it_disc->tipo_disciplina_id;
			nova_disc->nivel_dificuldade_id = it_disc->nivel_dificuldade_id;

			if(nova_disc->divisao_creditos != NULL){
				nova_disc->divisao_creditos = new DivisaoCreditos();
				nova_disc->divisao_creditos->setId(it_disc->divisao_creditos->getId());
				nova_disc->divisao_creditos->creditos = it_disc->divisao_creditos->creditos;

				for(int i=0;i<8;i++) {
					nova_disc->divisao_creditos->dia[i] = it_disc->divisao_creditos->dia[i];
				}
			}

			// NAO PRECISA DOS ITENS ABAIXO ?! CONFIRMAR COM O CHICO

			/*
			GGroup<int>::iterator it_eq = it_disc->equivalentes.begin();
			for(unsigned i=0;i<it_disc->equivalentes.size();i++) {
			nova_disc->equivalentes.add(*it_eq);
			it_eq++;
			}
			*/

			/*
			GGroup<int>::iterator it_inc = it_disc->incompativeis.begin();
			for(unsigned i=0;i<it_disc->incompativeis.size();i++) {
			nova_disc->incompativeis.add(*it_inc);
			it_inc++;
			}
			*/

			//>>> Copying HORARIO
			ITERA_GGROUP(it_hr,it_disc->horarios,Horario) {
				Horario *h =  new Horario;
				h->setId(it_hr->getId());

				//>>> >>> Copying DiaSemana
				GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
				for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
					h->dias_semana.add(*it_dia);
					it_dia++;
				}
				// <<< <<<

				h->horarioAulaId = it_hr->horarioAulaId;

				h->turnoId = it_hr->turnoId;

				// >>> >>> Copying TURNO
				Turno *tur;
				if(it_hr->turno != NULL) {
					tur = new Turno();
					tur->setId(it_hr->turno->getId());

					tur->nome = it_hr->turno->nome;

					tur->tempoAula = it_hr->turno->tempoAula;

					// >>> >>> >>> Copying HorariosAula
					HorarioAula *hr_aula;
					if(it_hr->turno->horarios_aula.size() > 0){
						ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
							//HorarioAula *hr_aula = new HorarioAula();
							hr_aula = new HorarioAula();
							hr_aula->setId(it_hr_aula->getId());
							hr_aula->inicio = it_hr_aula->inicio;

							GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
							for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
								hr_aula->diasSemana.add(*it_dia_sem);
								it_dia_sem++;
							}
							//tur->horarios_aula = hr_aula;
						}
						tur->horarios_aula.add(hr_aula);
					}
					// <<< <<< <<<
					h->turno = tur;
					// <<< <<<
				}

				HorarioAula *hr_aula;
				if(it_hr->horario_aula != NULL) {
					hr_aula = new HorarioAula();
					hr_aula->setId(it_hr->horario_aula->getId());
					hr_aula->inicio = it_hr->horario_aula->inicio;

					GGroup<int>::iterator it_dia_sem = it_hr->horario_aula->diasSemana.begin();
					for(unsigned dia =0;dia<it_hr->horario_aula->diasSemana.size();dia++) {
						hr_aula->diasSemana.add(*it_dia_sem);
						it_dia_sem++;
					}
				}

				nova_disc->horarios.add(h);
			}
			// <<<
			/*
			if(it_disc->tipo_disciplina != NULL){
			nova_disc->tipo_disciplina->setId(it_disc->tipo_disciplina->getId());
			nova_disc->tipo_disciplina->nome = it_disc->tipo_disciplina->nome;
			}

			if(it_disc->nivel_dificuldade != NULL) {
			nova_disc->nivel_dificuldade->setId(it_disc->nivel_dificuldade->getId());
			nova_disc->nivel_dificuldade->nome = it_disc->nivel_dificuldade->nome;
			}

			if( it_disc->demanda_total != -1){
			nova_disc->demanda_total = it_disc->demanda_total;
			}
			if( it_disc->max_demanda != -1){
			nova_disc->max_demanda = it_disc->max_demanda;
			}

			if( it_disc->num_turmas != -1){
			nova_disc->num_turmas = it_disc->num_turmas;
			}
			if( it_disc->min_creds != -1){
			nova_disc->min_creds = it_disc->min_creds;
			}
			*/

			ITERA_GGROUP(it_cp,problemData->campi,Campus) {
				// Adicionando os dados da nova disciplina em <Campi->Unidade->Sala->disciplinasAssociadas>:
				ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_und->salas,Sala) {
						/*
						if( it_sala->disciplinas_associadas.find(it_disc->getId()) != 
						it_sala->disciplinas_associadas.end() ){
						it_sala->disciplinas_associadas.add(nova_disc->getId());
						}
						*/

						if( (it_sala->disciplinas_associadas.find(it_disc->getId()) != 
							it_sala->disciplinas_associadas.end() ) &&
							(it_sala->tipo_sala_id != 1)){
								/*
								Removendo a associacao da disciplina teorica em questao com as salas 
								incompativeis, no caso qualquer uma que nao seja uma sala de aula (de acordo
								com inputTrivial)
								*/

								//it_sala->disciplinas_associadas.remove(it_disc->getId());

								/*
								Em relacao a nova disciplina (pratica), so adiciono uma associacao quando 
								for com uma sala compativel, no caso LABORATORIO
								*/

								it_sala->disciplinas_associadas.add(nova_disc->getId());
						}

					}
				}

				// Adicionando os dados da nova disciplina em <Campi->Professor->disciplinas>:
				Magisterio *novo_mag;
				ITERA_GGROUP(it_prof,it_cp->professores,Professor) {
					ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
						if( it_mag->disciplina_id == it_disc->getId()) {
							//Magisterio *novo_mag = new Magisterio();
							novo_mag = new Magisterio();

							novo_mag->setId(-1); // Nem precisava.
							novo_mag->nota = it_mag->nota;
							novo_mag->preferencia = it_mag->preferencia;
							novo_mag->disciplina_id = nova_disc->getId();
							it_prof->magisterio.add(novo_mag);

							break; // Garantido que um mesmo professor nao possui preferencias diferentes em relacao a uma mesma disciplina.
						}
					}
				}
			}


			/* ToDo : Fixacao (ToDo : futura issue : para criar uma nova fixacao, antes eh
			necessario saber se uma disciplina pode ser replicada. Pode acontecer  um caso 
			em que um determinada disciplina possua creditos teoricos e praticos e seja fixada
			em um determinado dia, numa sala para aulas teorica e em outro horario diferente seja
			fixada em um laboratorio. Nesse caso, nao seria necessario criar uma nova fixacao e sim,
			alterar o id da disciplina da fixacao da aula pratica para o id da nova disciplina que
			foi criada(se a nova discipliona for pratica).)

			ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao) {
			if(it_fix->disciplina_id == it_disc->getId() ) {
			}
			}
			*/

			// Adicionando os dados da nova disciplina em <GrupoCurso->curriculos>
			ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
				ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {
					/* 
					FIXME, isto est� errado, deveria-se, de algum jeito,
					saber o periodo da disciplina ou, iterar sobre todos os periodos 
					validos de um curso e nao sobre uma estimativa.
					*/
					for(unsigned num_periodos = 0; num_periodos < 20; num_periodos++) {
						DisciplinaPeriodo disc_periodo(num_periodos,it_disc->getId());
						//std::cout << "<periodo,disc_id> : <" << disc_periodo.first << "," << disc_periodo.second << ">\n";
						if(it_curriculo->disciplinas_periodo.find(disc_periodo) !=
							it_curriculo->disciplinas_periodo.end()) {
								//std::cout << "Found at <periodo,disc_id> : <" << disc_periodo.first << "," << disc_periodo.second << ">\n";
								//DisciplinaPeriodo nova_disc_periodo(disc_periodo.first, -disc_periodo.second);
								it_curriculo->disciplinas_periodo.add(DisciplinaPeriodo (disc_periodo.first, -disc_periodo.second));
								break; // Garantido que uma disciplina aparece apenas uma vez em um curriculo de um curso.
						}
					}
				}
			}

			// Adicionando os dados da nova disciplina em <Demanda>
			Demanda *nova_demanda = NULL;
			ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
				int num_vezes_ecncontrado = 0;
				if( it_dem->disciplina_id == it_disc->getId()) {
					nova_demanda = new Demanda();

					//nova_demanda->setId(-it_dem->getId());
					nova_demanda->oferta_id = it_dem->oferta_id;
					nova_demanda->disciplina_id = nova_disc->getId();
					nova_demanda->quantidade = it_dem->quantidade;

					if(num_vezes_ecncontrado > 0) {
						std::cout << "POSSIVEL ERRO EM <divideDisciplinas()> -> " << 
							"Encontrei mais de uma demanda para uma dada disciplina de um " <<
							"dado curso em um determinado campus." << std::endl;
						getchar();
					}
					num_vezes_ecncontrado++;
				}
			}

			if(nova_demanda) { // != NULL
				problemData->demandas.add(nova_demanda);
			}

			disciplinas_novas.add(nova_disc);
		}
	}

	//std::cout << "\nBEFORE: " << problemData->disciplinas.size();
	ITERA_GGROUP(it_disc,disciplinas_novas,Disciplina) {
		problemData->disciplinas.add(*it_disc);
	}
	//std::cout << "\nAFTER: " << problemData->disciplinas.size();

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
				TODO (ou n�o) */
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
				//std::cout << "it_mag->disciplina_id: " << it_mag->disciplina_id << std::endl;
				find_and_set(it_mag->disciplina_id,
					problemData->disciplinas,
					it_mag->disciplina);
				//getchar();
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
		/* � preciso procurar a unidade nos campi */
		ITERA_GGROUP(it_campi,problemData->campi,Campus) {
			/* posso fazer find_and_set em todos sem ifs, porque ele
			s� seta se encontrar. Posso continuar fazendo mesmo depois de 
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


	ITERA_GGROUP(it_campi,problemData->campi,Campus) {
		ITERA_GGROUP(it_unidades,it_campi->unidades,Unidade) {

			it_unidades->id_campus = it_campi->getId();

			ITERA_GGROUP(it_salas,it_unidades->salas,Sala) {
				it_salas->id_unidade = it_unidades->getId();
			}
		}
	}
	/*
	// >>> Debugging !!!
	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	std::cout << it_disc->getId() << std::endl;
	std::cout << it_disc->demanda_total<< std::endl;
	std::cout << it_disc->max_demanda<< std::endl;
	std::cout << it_disc->num_turmas<< std::endl;
	std::cout << it_disc->min_creds<< std::endl;
	std::cout << it_disc->max_creds<< std::endl;
	std::cout << std::endl;
	}
	// <<<
	*/
	//exit(1);


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

					/* Vers�o lenta, find n�o est� funcionando :( */
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

		// inicializa com zero caso ainda n�o exista;
		if(problemData->demandas_campus.find(dc) !=
			problemData->demandas_campus.end())
			problemData->demandas_campus[dc] = 0;

		problemData->demandas_campus[dc] += dem;
	}
}
void ProblemDataLoader::estima_turmas() {
	ITERA_GGROUP(it_disc,problemData->disciplinas, Disciplina) {
		/*
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
		*/

		int tam_turma = -1;

		if(it_disc->max_alunos_t > 0 && it_disc->max_alunos_p < 0){
			tam_turma = it_disc->max_alunos_t;
		}

		if(it_disc->max_alunos_p > 0 && it_disc->max_alunos_t < 0){
			tam_turma = it_disc->max_alunos_p;
		}

		// ToDo : Resolver melhor essa configura��o de turmas. Ver com o Chico
		/*
		O trecho de c�digo abaixo diz que se o tamanho de uma turma n�o for estabelecido
		nas condi��es acima, uma nova turma ser� criada com capacidade 10.
		Caso contr�rio, o tamanho de uma turma ser� calculado com base na metade do n�mero
		m�ximo de alunos que uma disciplina pode ter.
		*/
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
/* Salva algumas informa��es que s�o usadas frequentemente */
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

	// >>>
	/* Adicionando �s salas todas as disciplinas compativeis. 
	OBS: Se a sala n�o possui disciplina compat�vel informada na
	entrada, entao todas as disciplinas sao compativeis.*/
	GGroup<int> disc_proibidas;

	ITERA_GGROUP(it_cp,problemData->campi,Campus) {
		ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_und->salas,Sala) {
				GGroup<int>::iterator it_sala_disc_assoc = it_sala->disciplinas_associadas.begin();
				for(;it_sala_disc_assoc != it_sala->disciplinas_associadas.end();it_sala_disc_assoc++ ) {
					disc_proibidas.add(*it_sala_disc_assoc);
				}
			}
		}
	}

	ITERA_GGROUP(it_cp,problemData->campi,Campus) {
		ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_und->salas,Sala) {
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
					if( it_sala->disciplinas_associadas.find(it_disc->getId()) !=
						it_sala->disciplinas_associadas.end()) {
							it_sala->disc_assoc_PT.add(*it_disc);
					}
				}
			}
		}
	}

	ITERA_GGROUP(it_cp,problemData->campi,Campus) {
		ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_und->salas,Sala) {
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
					if( disc_proibidas.find(it_disc->getId()) == disc_proibidas.end() ) {
						if(it_disc->cred_praticos > 0) {
							if(it_sala->tipo_sala_id == 2 /*laboratorio, segundo instancia trivial*/) {
								it_sala->disc_assoc_PT.add(*it_disc);
							}
						}
						else {
							if(it_disc->cred_teoricos > 0) {
								it_sala->disc_assoc_PT.add(*it_disc);
							}
						}
					}
				}
			}
		}
	}
	// <<<
}

void ProblemDataLoader::print_csv(void)
{
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

	FILE *file = fopen("./CSV/PROBLEM_SETTINGS.csv","wt");
	fprintf(file,"Campi:\t%4d,\n",ncampi);
	fprintf(file,"Unidades:\t%4d,\n",nunidades);
	fprintf(file,"Salas:\t%4d,\n",problemData->campi.begin()->total_salas);
	fprintf(file,"Disciplinas:\t%4d,\n",ndiscs);
	fprintf(file,"Cursos:\t%4d,\n",ncursos);
	fprintf(file,"Professores:\t%4d,\n",nprofs);
	fprintf(file,"Ofertas:\t%4d,\n",nofertas);
	fprintf(file,"Demanda total:\t%4d,\n",tdemanda);

	if(file)
		fclose(file);
}
