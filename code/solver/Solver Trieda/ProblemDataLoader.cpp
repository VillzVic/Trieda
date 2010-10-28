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
	// >>> 14/10/2010
	//armz_disc_curriculo();
	// <<< 14/10/2010

   referenciaCampus();
   referenciaDisciplinas();

	gera_refs();
	cria_blocos_curriculares();
	
   calculaTamanhoMedioSalasCampus();
   relacionaCampusDiscs();
   calculaDemandas();

   carregaDisciplinasAssociadasSalas();
	
   estima_turmas();
	cache();

   print_stats();
	//print_csv();

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
	/* Versão lenta... Entender o porquê depois */
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

			// >>> Implementar o operador de atribuição seria mais correto.
			//nova_disc = *it_disc;
			// <<<

			nova_disc->setId(-it_disc->getId()); // alterado
         //nova_disc->setId(it_disc->getId()*1000); // alterado

			nova_disc->codigo = it_disc->codigo + "-P";
         //nova_disc->codigo = it_disc->codigo;
			nova_disc->nome = it_disc->nome + "PRATICA";

			//nova_disc->cred_teoricos = it_disc->cred_teoricos;
			nova_disc->cred_teoricos = 0; // alterado

			nova_disc->cred_praticos = it_disc->cred_praticos; // alterado
			//it_disc->cred_praticos = 0; // alterado
         it_disc->setCredsPraticos(0);

			//nova_disc->max_creds = it_disc->max_creds;
         nova_disc->max_creds = nova_disc->cred_praticos;
         //it_disc->max_creds = it_disc->cred_teoricos;
         it_disc->setMaxCreds(it_disc->cred_teoricos);

			nova_disc->e_lab = it_disc->e_lab; // alterado
			it_disc->e_lab = false; // alterado

			//nova_disc->max_alunos_t = it_disc->max_alunos_t; // alterado
			nova_disc->max_alunos_t = -1; // alterado

			nova_disc->max_alunos_p = it_disc->max_alunos_p; // alterado
			it_disc->max_alunos_p = -1; // alterado

			nova_disc->tipo_disciplina_id = it_disc->tipo_disciplina_id;
			nova_disc->nivel_dificuldade_id = it_disc->nivel_dificuldade_id;

			//if(nova_disc->divisao_creditos != NULL){
			if(it_disc->divisao_creditos){
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

								it_sala->disciplinas_associadas.remove(it_disc->getId());

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
					FIXME, isto está errado, deveria-se, de algum jeito,
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

			// >>> 14/10/2010
			//it_disc->foi_dividida = true;
			//nova_disc->foi_dividida = true;
			// <<< 14/10/2010

			disciplinas_novas.add(nova_disc);
		}
	}

	//std::cout << "\nBEFORE: " << problemData->disciplinas.size();
	ITERA_GGROUP(it_disc,disciplinas_novas,Disciplina) {
		problemData->disciplinas.add(*it_disc);
	}
	//std::cout << "\nAFTER: " << problemData->disciplinas.size();

}

void ProblemDataLoader::referenciaCampus()
{
   ITERA_GGROUP(it_cp,problemData->campi,Campus) {
      problemData->refCampus[it_cp->getId()] = *it_cp;
   }
}

void ProblemDataLoader::referenciaDisciplinas()
{
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      problemData->refDisciplinas[it_disc->getId()] = *it_disc;
   }
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
/**/
   ITERA_GGROUP(it_campi,problemData->campi,Campus) {
      ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
         ITERA_GGROUP(it_curr,it_curso->curriculos,Curriculo) {

            GGroup<DisciplinaPeriodo>::iterator it_dp = 
               it_curr->disciplinas_periodo.begin();

            // Percorrendo todas as disciplinas de um curso cadastradas para um currículo.
            for(;it_dp != it_curr->disciplinas_periodo.end(); ++it_dp) {

               DisciplinaPeriodo dp = *it_dp;
               int periodo = dp.first;
               int disc_id = dp.second;

               Disciplina * d = new Disciplina();

               // Encontrando a disciplina em questão.
               for(GGroup<Disciplina*>::iterator it_d = 
                  problemData->disciplinas.begin();
                  it_d != problemData->disciplinas.end(); ++it_d) {

                     if (it_d->getId() == disc_id) {
                        //d = new Disciplina();
                        d = *it_d;
                        break;
                     }
               }

               GGroup<BlocoCurricular*>::iterator it_bc = 
                  problemData->blocos.begin();

               int id_blc = it_curso->getId() * 100 + periodo;

               bool found = false;

               // Verificando a existência do bloco curricular para a disciplina em questão.
               for(;it_bc != problemData->blocos.end(); ++it_bc) {
                  if(it_bc->getId() == id_blc) {
                     it_bc->disciplinas.add(d);
                     found = true;
                     break;
                  }
               }

               if(!found) {
                  BlocoCurricular * b = new BlocoCurricular();

                  b->setId(id_blc);
                  b->periodo = periodo;
                  b->campus = *it_campi;
                  b->curso = *it_curso;

                  b->disciplinas.add(d);

                  problemData->blocos.add(b);
               }

            }

         }
      }
   }

/**/

   /*
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

   // Versão lenta, find não está funcionando :(
   for(GGroup<Disciplina*>::iterator it_d = 
   problemData->disciplinas.begin();
   it_d != problemData->disciplinas.end(); ++it_d) 
   {
   if (it_d->getId() == d->getId())
   d = *it_d;
   }
   // FIM

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
   */
}

void ProblemDataLoader::relacionaCampusDiscs()
{
   ITERA_GGROUP(it_oferta,problemData->ofertas,Oferta) {

      GGroup<int> ids_discs;

      ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

         if(it_curso->getId() == it_oferta->curso_id) {

            ITERA_GGROUP(it_curric,it_curso->curriculos,Curriculo) {

               if(it_curric->getId() == it_oferta->curriculo_id) {

                  GGroup<DisciplinaPeriodo>::iterator it_disc_prd =
                     it_curric->disciplinas_periodo.begin();

                  for(; it_disc_prd != it_curric->disciplinas_periodo.end(); it_disc_prd++) {
                     problemData->cp_discs[it_oferta->campus_id].add((*it_disc_prd).second);
                  }

                  break;
               }
            }
            break;
         }
      }
   }

}

/**/
void ProblemDataLoader::calculaTamanhoMedioSalasCampus()
{
   unsigned somaCapSalas = 0;
   unsigned totalSalas = 0;

   ITERA_GGROUP(it_cp,problemData->campi,Campus) {
      ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
         ITERA_GGROUP(it_sala,it_und->salas,Sala) {

            somaCapSalas += it_sala->capacidade;

            it_und->maiorSala = std::max(((int)it_und->maiorSala),((int)it_sala->capacidade));
         }

         totalSalas += it_und->getNumSalas();

         it_cp->maiorSala = std::max(((int)it_cp->maiorSala),((int)it_und->maiorSala));
      }

      problemData->cp_medSalas[it_cp->getId()] = somaCapSalas / totalSalas;
   }
}
/**/

void ProblemDataLoader::calculaDemandas() {
	ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
		int dem = it_dem->quantidade;

      it_dem->disciplina->setMaxDemanda(
         std::max(it_dem->disciplina->getMaxDemanda(),dem));

      it_dem->disciplina->adicionaDemandaTotal(dem);

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

   unsigned id_turma = 0;

   std::map<int/*Id Campus*/,GGroup<int>/*Id Discs*/>::iterator it_cp_discs =
      problemData->cp_discs.begin();

   // Para todos os campus
   for(; it_cp_discs != problemData->cp_discs.end(); it_cp_discs++) {

      GGroup<int>::iterator it_discs = it_cp_discs->second.begin();

      // Para todas as disciplinas do campus em questão
      for(; it_discs != it_cp_discs->second.end(); it_discs++) {

         // >>>
         // Configurando o tamanho da turma.
         int tamTurma = -1;

         // Número de turmas considerado para a disc em questão.
         unsigned numTurmas = 0;

         if (problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo() > 0 ||
            problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat() > 0 ) {

               tamTurma = std::min(problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo(),
                  problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat());

               /*numTurmas = problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() /
                  tamTurma + 1;*/
         }
         else {

            tamTurma = std::max(problemData->refDisciplinas[(*it_discs)]->getMaxAlunosTeo(),
               problemData->refDisciplinas[(*it_discs)]->getMaxAlunosPrat());

            if(tamTurma <= 0) {

               // >>>
               /* Armazenando as capacidades das salas compatíveis com a disciplina em questão. */
               std::vector<int/*Capacidade Sala*/> salasComaptiveis;

               ITERA_GGROUP(it_und,problemData->refCampus[it_cp_discs->first]->unidades,Unidade) {            
                  ITERA_GGROUP(it_sala,it_und->salas,Sala) {
                     ITERA_GGROUP(it_d,it_sala->disciplinasAssociadas,Disciplina) {
                        if(it_d->getId() == *it_discs) {
                           salasComaptiveis.push_back(it_sala->capacidade);
                           break;
                        }
                     }
                  }
               }

               // Ordem crescente de capacidade.
               std::sort(salasComaptiveis.begin(),salasComaptiveis.end());

               // <<<

               // Número máximo de turmas para uma disciplina. /* FIX ME */
               unsigned maxTurmasDisc = 10;

               std::vector<int/*Capacidade Sala*/>::iterator
                  it_salasCompativeis = salasComaptiveis.begin();

               for(;it_salasCompativeis != salasComaptiveis.end(); it_salasCompativeis++) {
                  numTurmas = problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() / 
                     *it_salasCompativeis + 1;

                  if( numTurmas <= maxTurmasDisc ) {
                     tamTurma = *it_salasCompativeis;
                     break;
                  }
               }

               /* Pode ser que o número de turmas tenha sido sempre 
               calculado acima de <maxTurmasDisc>. Daí, calcula-se o
               número de turmas de acordo com o tamanho médio das
               salas de um campus. */
               if(it_salasCompativeis == salasComaptiveis.end() && 
                  numTurmas > maxTurmasDisc ) {
                     numTurmas = problemData->cp_medSalas[it_cp_discs->first];
               }
            }
         }
         // <<<

         for(unsigned i = 0; i < numTurmas; i++) {
            problemData->disc_turmas[(*it_discs)].add(std::make_pair<int,int>
               (id_turma++,tamTurma));
         }

         problemData->refDisciplinas[(*it_discs)]->num_turmas = numTurmas;

         /*GAMB*/
         /*
         tamTurma = 10;

         if(problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() == 0) {
         problemData->refDisciplinas[(*it_discs)]->num_turmas = 0;
         }
         else {
         problemData->refDisciplinas[(*it_discs)]->num_turmas = 
         problemData->refDisciplinas[(*it_discs)]->getDemandaTotal() 
         / tamTurma + 1;
         }
         */

         /*
         std::cout << "Decidi abrir " << problemData->refDisciplinas[(*it_discs)]->num_turmas << 
            " turmas da disciplina " << problemData->refDisciplinas[(*it_discs)]->codigo << std::endl;
         */
      }
   }

}

void ProblemDataLoader::print_stats() {
	int ncampi(0),nunidades(0),nsalas(0),ndiscs(0),
      nturmas(0),nprofs(0),ncursos(0),nofertas(0),tdemanda(0);

	ncampi = problemData->campi.size();

	ITERA_GGROUP(it_campi,problemData->campi,Campus) {
		nunidades += it_campi->unidades.size();

      ITERA_GGROUP(it_und,it_campi->unidades,Unidade) {
         nsalas += it_und->salas.size();
      }

		nprofs += it_campi->professores.size();
		ncursos += problemData->cursos.size();
	}

	nofertas = problemData->ofertas.size();
	
   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      if(it_disc->getId() > 0) {
         ndiscs++;
         nturmas += it_disc->num_turmas;
      }
   }

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
      tdemanda += it_disc->getDemandaTotal();
	}

   std::cout << std::endl;
   std::cout << "Estatisticas de dados de entrada" << std::endl;
   std::cout << "================================" << std::endl;
   printf("Campi:        \t%4d\n",ncampi);
   printf("Unidades:     \t%4d\n",nunidades);
   printf("Salas:        \t%4d\n",nsalas);
   printf("Disciplinas:  \t%4d\n",ndiscs);
   printf("Turmas:       \t%4d\n",nturmas);
   printf("Professores:  \t%4d\n",nprofs);
   printf("Cursos:       \t%4d\n",ncursos);
   printf("Ofertas:      \t%4d\n",nofertas);
   printf("Demanda total:\t%4d vagas\n",tdemanda);
   std::cout << "================================" << std::endl
      << std::endl;
}
/* Salva algumas informações que são usadas frequentemente */
void ProblemDataLoader::cache() {
   problemData->totalSalas = 0;
	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		it_campus->totalSalas = 0;
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			it_campus->totalSalas += it_u->salas.size();
		}
		problemData->totalSalas += it_campus->totalSalas;
	}

	problemData->totalTurmas = 0;
	ITERA_GGROUP(it_disciplinas,problemData->disciplinas,Disciplina) {
		problemData->totalTurmas += it_disciplinas->num_turmas;
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
   /*
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
   */

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
	///* Adicionando às salas todas as disciplinas compativeis. 
	//OBS: Se a sala não possui disciplina compatível informada na
	//entrada, entao todas as disciplinas são compativeis.*/
	//GGroup<int> disc_proibidas;

	//ITERA_GGROUP(it_cp,problemData->campi,Campus) {
	//	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
	//		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
	//			GGroup<int>::iterator it_sala_disc_assoc = it_sala->disciplinas_associadas.begin();
	//			for(;it_sala_disc_assoc != it_sala->disciplinas_associadas.end();it_sala_disc_assoc++ ) {
	//				disc_proibidas.add(*it_sala_disc_assoc);
	//			}
	//		}
	//	}
	//}

	//ITERA_GGROUP(it_cp,problemData->campi,Campus) {
	//	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
	//		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
	//			ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//				if( it_sala->disciplinas_associadas.find(it_disc->getId()) !=
	//					it_sala->disciplinas_associadas.end()) {
	//						//it_sala->disc_assoc_PT.add(*it_disc);
 //                    it_sala->disciplinasAssociadas.add(*it_disc);
	//				}
	//			}
	//		}
	//	}
	//}

	//ITERA_GGROUP(it_cp,problemData->campi,Campus) {
	//	ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
	//		ITERA_GGROUP(it_sala,it_und->salas,Sala) {
	//			ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//				if( disc_proibidas.find(it_disc->getId()) == disc_proibidas.end() ) {
	//					if(it_disc->cred_praticos > 0) {
	//						if(it_sala->tipo_sala_id == 2 /*laboratorio, segundo instancia trivial*/) {
	//							//it_sala->disc_assoc_PT.add(*it_disc);
 //                       it_sala->disciplinasAssociadas.add(*it_disc);
	//						}
	//					}
	//					else {
	//						if(it_disc->cred_teoricos > 0) {
	//							//it_sala->disc_assoc_PT.add(*it_disc);
 //                       it_sala->disciplinasAssociadas.add(*it_disc);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	// <<<

}

void ProblemDataLoader::carregaDisciplinasAssociadasSalas() {

   /* Adicionando às salas todas as disciplinas compativeis. 
   OBS: Se a sala não possui disciplina compatível informada na
   entrada, entao todas as disciplinas são compativeis.*/
   //GGroup<int> disc_proibidas;

   /* Armazenando as disciplinas que são específicas de alguma sala e, portanto, não
   deverão ser adicionadas às listas de disciplinas associadas de outras salas.*/
   ITERA_GGROUP(it_cp,problemData->campi,Campus) {

      GGroup<int> disc_proibidas;

      ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
         ITERA_GGROUP(it_sala,it_und->salas,Sala) {

            GGroup<int>::iterator it_sala_disc_assoc = it_sala->disciplinas_associadas.begin();

            for(; it_sala_disc_assoc != it_sala->disciplinas_associadas.end(); it_sala_disc_assoc++) {
               disc_proibidas.add(*it_sala_disc_assoc);

               it_sala->disciplinasAssociadas.add(problemData->refDisciplinas[*it_sala_disc_assoc]);

            }
         }
      }
      //}

      // Adicionando as disciplinas, que não possuem restrição quanto à sala, as salas do campus em questão.
      //ITERA_GGROUP(it_cp,problemData->campi,Campus) {
      ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
         ITERA_GGROUP(it_sala,it_und->salas,Sala) {
            //ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
            GGroup<int>::iterator it_disc = problemData->cp_discs[it_cp->getId()].begin();
            for(; it_disc != problemData->cp_discs[it_cp->getId()].end(); it_disc++ ) {

               if(disc_proibidas.find(*it_disc) == disc_proibidas.end()) {

                  //if(it_disc->eLab()) {
                  if(problemData->refDisciplinas[*it_disc]->eLab()) {
                     if(it_sala->getTipoSalaId() == 2) /*laboratório, segundo instancia trivial*/ {
                        //it_sala->disciplinasAssociadas.add(*it_disc);
                        it_sala->disciplinasAssociadas.add(problemData->refDisciplinas[*it_disc]);
                     }
                  }
                  else {
                     /* Se o if abaixo estiver comentado disciplinas teóricas que não possuem restrição
                     quanto a salas poderão ser associadas à laboratórios. Isso aumenta o número de 
                     variáveis e restrições criadas. O ideal é o usuário cadastrar corretamente as 
                     associações de disciplinas às salas. */
                     //if(it_sala->getTipoSalaId() != 2) {
                     //it_sala->disciplinasAssociadas.add(*it_disc);
                     it_sala->disciplinasAssociadas.add(problemData->refDisciplinas[*it_disc]);
                     //}
                  }
               }
            }
         }
      }
   }

}

// >>> 15/10/2010
void ProblemDataLoader::armz_disc_curriculo()
{
	unsigned novo_id = 1;

	// >>>

	/*
	Armazenando um map de pares onde cada posição possui um par de inteiros,
	para associar o id de uma dada disciplina a um determinado curriculo.
	A chave de cada posição do map será utilizada para setar um novo id para cada
	disciplina.
	Um outro map aramzena os maps acima, dividindo-os por curso. Logo, a chave para
	cada posição do map externo é o id de cada curso.
	*/
	ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
		int id_curso = it_curso->getId();

		ITERA_GGROUP(it_curric,it_curso->curriculos,Curriculo) {
			
			std::pair<int,int> disc_curric = std::make_pair(
				0,
				it_curric->getId());

			GGroup<DisciplinaPeriodo>::iterator it_prd_disc =
				it_curric->disciplinas_periodo.begin();

			for(;it_prd_disc != it_curric->disciplinas_periodo.end();it_prd_disc++) {
				int id_disc = (*it_prd_disc).second;
				if( id_disc > 0) {
					disc_curric.first = id_disc;
					problemData->relacao_curso_discs_curric[id_curso][novo_id] =
						disc_curric;
					novo_id++;
				}
			}

			/*
			Tratando as disciplinas que foram divididas e agora possuem id negativo.
			O novo id delas deve ser o msm id que foi criado para a disciplina correspondente (com id positivo)
			porém, negado.
			*/

			it_prd_disc = it_curric->disciplinas_periodo.begin();

			for(;it_prd_disc != it_curric->disciplinas_periodo.end();it_prd_disc++) {

				int id_disc = (*it_prd_disc).second;

				if( id_disc < 0) {

						disc_curric.first = -id_disc;

						if( problemData->relacao_curso_discs_curric.find(id_curso) !=
							problemData->relacao_curso_discs_curric.end() ){

								std::map<int/*novo_id_Disc*/,
									std::pair<int/*id_Disc*/,int/*id_Curriculo*/> >::iterator
									
									it_relacao_disc_curriculo =

									problemData->relacao_curso_discs_curric[id_curso].begin();

								for(; it_relacao_disc_curriculo !=
									problemData->relacao_curso_discs_curric[id_curso].end();
									it_relacao_disc_curriculo++){

										if(it_relacao_disc_curriculo->second == disc_curric ) {
											
											disc_curric.first = id_disc;

											problemData->relacao_curso_discs_curric[id_curso]
											[-it_relacao_disc_curriculo->first] =
												disc_curric;

											break;
										}
								}
						}
				}
			}
		}
	}

	// <<<

	// >>>

	/*
	Armazenando informações para saber qdo será necessário criar uma, ou mais, réplicas de
	uma dada disciplina.

	Acho que não será necessário separar por curso.
	*/

	// Qdo a disc pertence a mais de um curric, o set vai possuir mais de um elemento
	std::map<int/*id_original_disc*/,
		std::set<int/*cjto com os novos ids da disc em questao*/> > replicas_to_do;

	// Contabilizando o numero de replicas de cada disciplina a serem realizadas
	std::map<int/*id do curso*/,
		std::map<int/*novo_id_Disc*/,
		std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > >::iterator

		it_relacao_curso_discs_curric = problemData->relacao_curso_discs_curric.begin();
	
	for(; it_relacao_curso_discs_curric != 
		problemData->relacao_curso_discs_curric.end();
		it_relacao_curso_discs_curric++) {

			std::map<int/*novo_id_Disc*/,
				std::pair<int/*id_Disc*/,int/*id_Curriculo*/> >::iterator

				it_relacao_disc_curric = it_relacao_curso_discs_curric->second.begin();

			for(; it_relacao_disc_curric !=
				it_relacao_curso_discs_curric->second.end();
				it_relacao_disc_curric++) {

					replicas_to_do[it_relacao_disc_curric->second.first].insert(
						it_relacao_disc_curric->first);
			}

	}

	// <<<

	// >>>

	/*
	Configurando as disciplinas com os novos ids e, quando necessário,
	criando as réplicas.
	*/

	std::map<int/*id_original_disc*/,
		std::set<int/*cjto com os novos ids da disc em questao*/> >::iterator
		it_replicas_to_do = replicas_to_do.begin();

	// Usado somente para quando uma disciplina constar em mais de um curric.
	GGroup<Disciplina*> disciplinas_novas;

	for(;it_replicas_to_do != replicas_to_do.end();it_replicas_to_do++) {
		if(it_replicas_to_do->second.size() == 1) { // ou seja, não é necessário replicar.

			// >>> >>>

			ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {

				if(it_disc->getId() == it_replicas_to_do->first) {

					// >>> >>> >>>

					ITERA_GGROUP(it_cp,problemData->campi,Campus) {

						// >>> >>> >>> >>>

						// Alterando os dados da disciplina em <Campi->Professor->disciplinas>:
						ITERA_GGROUP(it_prof,it_cp->professores,Professor) {
							ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
								if( it_mag->disciplina_id == it_disc->getId()) {

									/*
									Ja posso alterar aqui.
									Fiz alguns testes e, aparentemente deu tudo certo.
									*/

									it_mag->disciplina_id = (*it_replicas_to_do->second.begin());
									std::cout << std::endl;

									break; // Garantido que um mesmo professor nao possui preferencias diferentes em relacao a uma mesma disciplina.
								}
							}
						}

						// <<< <<< <<< <<<

					}

					// <<< <<< <<<

					// >>> >>> >>>

					// Alterando os dados da disciplina em <Demanda>
					ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {

						if( it_dem->disciplina_id == it_disc->getId()) {

							/*
							Ja posso alterar aqui.
							Fiz alguns testes e, aparentemente deu tudo certo.
							*/

							it_dem->disciplina_id = (*it_replicas_to_do->second.begin());
							std::cout << std::endl;
							break;

						}
					}

					// <<< <<< <<<

					it_disc->setId(*it_replicas_to_do->second.begin());

					std::cout << std::endl;

					break;
				}
			}

			// <<< <<<

		}
		else if(it_replicas_to_do->second.size() > 1) {

			//  >>> >>>

			// Procurando pela disciplina desejada
			GGroup<Disciplina*>::iterator it_disc = problemData->disciplinas.begin();

			ITERA_GGROUP(it_d,problemData->disciplinas,Disciplina) {
				if(it_d->getId() == it_replicas_to_do->first) {
					it_disc = it_d;
					break;
				}
			}

			// <<< <<<

			// >>> >>>

			// Criando réplicas

			std::set<int/*cjto com os novos ids da disc em questao*/>::iterator
				it_novos_ids = it_replicas_to_do->second.begin();

			++it_novos_ids; /* A primeira disciplina já consta no cjto de 
							disciplinas originais. Não precisa replicar, 
							basta alterar os dados referentes à ela.
							
							Como as réplicas são baseadas na disc. original,
							deve-se alterar a disc. original depois que todas
							as réplicas forem criadas.
							*/

			for(;it_novos_ids != it_replicas_to_do->second.end();it_novos_ids++) {
				Disciplina *nova_disc = new Disciplina();

					nova_disc->setId(*it_novos_ids);

					nova_disc->codigo = it_disc->codigo;
					nova_disc->nome = it_disc->nome;

					nova_disc->cred_teoricos = it_disc->cred_teoricos;
					nova_disc->cred_praticos = it_disc->cred_praticos;

					nova_disc->max_creds = it_disc->max_creds;

					nova_disc->e_lab = it_disc->e_lab;

					nova_disc->max_alunos_t = it_disc->max_alunos_t;

					nova_disc->max_alunos_p = it_disc->max_alunos_p;

					nova_disc->tipo_disciplina_id = it_disc->tipo_disciplina_id;
					nova_disc->nivel_dificuldade_id = it_disc->nivel_dificuldade_id;

					if(it_disc->divisao_creditos) {
						nova_disc->divisao_creditos = new DivisaoCreditos();
						nova_disc->divisao_creditos->setId(it_disc->divisao_creditos->getId());
						nova_disc->divisao_creditos->creditos = it_disc->divisao_creditos->creditos;

						for(int i=0;i<8;i++) {
							nova_disc->divisao_creditos->dia[i] = it_disc->divisao_creditos->dia[i];
						}
					}

					// >>> >>> >>>

					// Copiando HORARIO
					ITERA_GGROUP(it_hr,it_disc->horarios,Horario) {
						Horario *h =  new Horario;
						h->setId(it_hr->getId());

						// >>> >>> >>> >>>

						// Copiando DiaSemana				
						GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
						for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
							h->dias_semana.add(*it_dia);
							it_dia++;
						}

						h->horarioAulaId = it_hr->horarioAulaId;

						h->turnoId = it_hr->turnoId;
						
						// <<< <<< <<< <<<

						// >>> >>> >>> >>>

						// Copiando TURNO
						Turno *tur;
						if(it_hr->turno != NULL) {
							tur = new Turno();

							tur->setId(it_hr->turno->getId());
							tur->nome = it_hr->turno->nome;
							tur->tempoAula = it_hr->turno->tempoAula;

							// >>> >>> >>> >>> >>>

							// Copiando HORARIOS_AULA
							HorarioAula *hr_aula;
							if(it_hr->turno->horarios_aula.size() > 0){
								ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
									hr_aula = new HorarioAula();

									hr_aula->setId(it_hr_aula->getId());
									hr_aula->inicio = it_hr_aula->inicio;

									GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
									for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
										hr_aula->diasSemana.add(*it_dia_sem);
										it_dia_sem++;
									}
								}

								tur->horarios_aula.add(hr_aula);
							}

							// <<< <<< <<< <<< <<<
							
							h->turno = tur;
						}

						// <<< <<< <<< <<<

						// >>> >>> >>> >>>

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

						// <<< <<< <<< <<<

						nova_disc->horarios.add(h);
					}

					// <<< <<< <<<

					// >>> >>> >>>

					ITERA_GGROUP(it_cp,problemData->campi,Campus) {

						//// Adicionando os dados da nova disciplina em <Campi->Unidade->Sala->disciplinasAssociadas>:
						//ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
						//	ITERA_GGROUP(it_sala,it_und->salas,Sala) {

						//		/*
						//		Em relacao a nova disciplina (pratica), so adiciono uma associacao quando 
						//		for com uma sala compativel, no caso LABORATORIO
						//		*/
						//		if( (it_sala->disciplinas_associadas.find(it_disc->getId()) != 
						//			it_sala->disciplinas_associadas.end() ) &&
						//			(it_sala->tipo_sala_id != 1)){

						//				//it_sala->disciplinas_associadas.add(nova_disc->getId());

						//				sala_disc_assoc[it_sala->getId()].add(
						//					*it_replicas_to_do->second.begin());

						//		}

						//	}
						//}

						// Adicionando os dados da nova disciplina em <Campi->Professor->disciplinas>:
						Magisterio *novo_mag;
						ITERA_GGROUP(it_prof,it_cp->professores,Professor) {
							ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
								if( it_mag->disciplina_id == it_disc->getId()) {
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

					// <<< <<< <<<

					// >>> >>> >>>

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

					// <<< <<< <<<

					// >>> >>> >>>

					//// Adicionando os dados da nova disciplina em <GrupoCurso->curriculos>
					//ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
					//	ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {
					//		/* 
					//		FIXME, isto está errado, deveria-se, de algum jeito,
					//		saber o periodo da disciplina ou, iterar sobre todos os periodos 
					//		validos de um curso e nao sobre uma estimativa.
					//		*/
					//		for(unsigned num_periodos = 0; num_periodos < 20; num_periodos++) {
					//			DisciplinaPeriodo disc_periodo(num_periodos,it_disc->getId());

					//			if(it_curriculo->disciplinas_periodo.find(disc_periodo) !=
					//				it_curriculo->disciplinas_periodo.end()) {
					//					
					//					it_curriculo->disciplinas_periodo.add(
					//						DisciplinaPeriodo (disc_periodo.first, -disc_periodo.second));

					//					//curso_prd_disc[it_curso->getId()].add(
					//					curso_prd_disc[
					//						std::make_pair<int,int>(it_curso->getId(),it_curriculo->getId())].add(
					//						DisciplinaPeriodo (disc_periodo.first, -disc_periodo.second));

					//					break; // Garantido que uma disciplina aparece apenas uma vez em um curriculo de um curso.
					//			}
					//		}
					//	}
					//}

					// <<< <<< <<<

					// >>> >>> >>>

					// Adicionando os dados da nova disciplina em <Demanda>
					Demanda *nova_demanda = NULL;
					ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
						if( it_dem->disciplina_id == it_disc->getId()) {
							nova_demanda = new Demanda();

							nova_demanda->oferta_id = it_dem->oferta_id;
							nova_demanda->disciplina_id = nova_disc->getId();
							nova_demanda->quantidade = it_dem->quantidade;

							break;
						}
					}

					if(nova_demanda) {
						problemData->demandas.add(nova_demanda);
					}

					// <<< <<< <<<

					disciplinas_novas.add(nova_disc);
			}

			// <<< <<<

			// >>> >>> >>>

			ITERA_GGROUP(it_cp,problemData->campi,Campus) {

				// >>> >>> >>> >>>

				// Alterando os dados da disciplina em <Campi->Unidade->Sala->disciplinasAssociadas>
				ITERA_GGROUP(it_und,it_cp->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_und->salas,Sala) {

						GGroup<int> disc_assoc;
						disc_assoc = it_sala->disciplinas_associadas;

						GGroup<int> disc_assoc_FINAL;
						bool modificou_disc_assoc = false;

						for( GGroup<int>::iterator it = disc_assoc.begin();
							it != disc_assoc.end(); it++) {

								if( (*it) == it_replicas_to_do->first ) {
									disc_assoc_FINAL.add(*it);
									modificou_disc_assoc = true;
								}
								else {
									disc_assoc_FINAL.add(*it);
								}
						}

						if(modificou_disc_assoc) {
							it_sala->disciplinas_associadas.clear();
							it_sala->disciplinas_associadas = disc_assoc_FINAL;
						}

						std::cout << std::endl;

					}
				}

				// <<< <<< <<< <<<

				// >>> >>> >>> >>>

				// Alterando os dados da disciplina em <Campi->Professor->disciplinas>:
				ITERA_GGROUP(it_prof,it_cp->professores,Professor) {
					ITERA_GGROUP(it_mag,it_prof->magisterio,Magisterio) {
						if( it_mag->disciplina_id == it_disc->getId()) {

							it_mag->disciplina_id = (*it_replicas_to_do->second.begin());
							break; // Garantido que um mesmo professor nao possui preferencias diferentes em relacao a uma mesma disciplina.
						}
					}
				}

				// <<< <<< <<< <<<

			}

			// <<< <<< <<<

			// >>> >>> >>>

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

			// <<< <<< <<<

			// >>> >>> >>>

			// Alterando os dados da disciplina em <GrupoCurso->curriculos>
			ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
				ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {
					/* 
					FIXME, isto está errado, deveria-se, de algum jeito,
					saber o periodo da disciplina ou, iterar sobre todos os periodos 
					validos de um curso e nao sobre uma estimativa.
					*/

					GGroup<DisciplinaPeriodo> prd_disc;
					GGroup<DisciplinaPeriodo>::iterator it_prd_disc =
						it_curriculo->disciplinas_periodo.begin();

					bool modificou_prd_disc = false;

					for(;it_prd_disc != it_curriculo->disciplinas_periodo.end();it_prd_disc++) {
						if( (*it_prd_disc).second == it_disc->getId() ) {
							DisciplinaPeriodo aux = std::make_pair<int,int>(
								(*it_prd_disc).first,
								(*it_replicas_to_do->second.begin()) );

							prd_disc.add(aux);

							modificou_prd_disc = true;

						}
						else {
							prd_disc.add( (*it_prd_disc) );
						}
					}

					if(modificou_prd_disc) {
						it_curriculo->disciplinas_periodo.clear();
						it_curriculo->disciplinas_periodo = prd_disc;

					}
					std::cout << std::endl;
				}
			}

			// <<< <<< <<<

			// >>> >>> >>>

			// Alterando os dados da disciplina em <Demanda>
			//Demanda *nova_demanda = NULL;
			ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
				//int num_vezes_ecncontrado = 0;

				if( it_dem->disciplina_id == it_disc->getId()) {

					it_dem->disciplina_id = (*it_replicas_to_do->second.begin());
					break;

					/*
					if(num_vezes_ecncontrado > 0) {
					std::cout << "POSSIVEL ERRO EM <divideDisciplinas()> -> " << 
					"Encontrei mais de uma demanda para uma dada disciplina de um " <<
					"dado curso em um determinado campus." << std::endl;
					getchar();
					}
					num_vezes_ecncontrado++;
					*/
				}
			}

			// <<< <<< <<<

			it_disc->setId(*it_replicas_to_do->second.begin());

			// <<< <<<

		}
		else{
			std::cout << "ERRO: ProblemDataLoader::armz_disc_curriculo() metodo." << std::endl;
		}

	}

	// <<<


	ITERA_GGROUP(it_disc,disciplinas_novas,Disciplina) {
		problemData->disciplinas.add(*it_disc);
	}

	// Alterando alguns dados


	// >>>

	// Fazendo a associação das novas disciplinas aos períodos corretos

	std::map<std::pair<int/*id_Curso*/,int/*id_curric*/>,
		GGroup<DisciplinaPeriodo>/*curric atualizado com os novos ids*/> curso_prd_disc;

	ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

		std::map<int/*id do curso*/,
			std::map<int/*novo_id_Disc*/,
			std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > >::iterator

			it_relacao_curso_discs_curric = 

			problemData->relacao_curso_discs_curric.find(
			it_curso->getId());

		if( it_relacao_curso_discs_curric != 
			problemData->relacao_curso_discs_curric.end()){

				std::map<int/*novo_id_Disc*/,
					std::pair<int/*id_Disc*/,int/*id_Curriculo*/> >::iterator

					it_relacao_discs_curric =

					it_relacao_curso_discs_curric->second.begin();

				for(; it_relacao_discs_curric !=
					it_relacao_curso_discs_curric->second.end();
					it_relacao_discs_curric++) {

						// >>>
						// Encontrando o periodo da disc.
						int periodo = -1;

						ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {
							if( it_curriculo->getId() == it_relacao_discs_curric->second.second ) {

								GGroup<DisciplinaPeriodo>::iterator it_prd_disc =
									it_curriculo->disciplinas_periodo.begin();

								for(;it_prd_disc != it_curriculo->disciplinas_periodo.end();it_prd_disc++) {
									if( (*it_prd_disc).second == it_relacao_discs_curric->second.first ){
										periodo = (*it_prd_disc).first;
										break;
									}
								}								

								break;
							}
						}
						// <<<

						curso_prd_disc[
							std::make_pair<int,int>(it_curso->getId(),
								it_relacao_discs_curric->second.second)].add(
							std::make_pair<int,int>(periodo,it_relacao_discs_curric->first));
				}
		}
	}

	// Atualizando os dados

	ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
		ITERA_GGROUP(it_curriculo,it_curso->curriculos,Curriculo) {

			std::map<std::pair<int/*id_Curso*/,int/*id_curric*/>,
				GGroup<DisciplinaPeriodo>/*curric atualizado com os novos ids*/>::iterator

				it_curso_prd_disc = 

				curso_prd_disc.find(
				std::make_pair<int,int>(it_curso->getId(),it_curriculo->getId()));

			if( it_curso_prd_disc != curso_prd_disc.end() ) {

				it_curriculo->disciplinas_periodo.clear();
				it_curriculo->disciplinas_periodo = it_curso_prd_disc->second;
			}
		}
	}

	// <<<

	std::map<int/*id_Sala*/,
		GGroup<int/*lista dos novos ids das disciplinas que
				  estao associada a sala determinada pela chave*/> > sala_disc_assoc;


	// ToDo : Fazer a listagem das salas.

	// ToDo : Trocar os dados antigos com os novos.

	std::cout << std::endl;


	exit(1);
}
// <<< 15/10/2010

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
		//tdemanda += it_disc->demanda_total;
      tdemanda += it_disc->getDemandaTotal();
	}

	FILE *file = fopen("./CSV/PROBLEM_SETTINGS.csv","wt");
	fprintf(file,"Campi:\t%4d,\n",ncampi);
	fprintf(file,"Unidades:\t%4d,\n",nunidades);
	fprintf(file,"Salas:\t%4d,\n",problemData->campi.begin()->totalSalas);
	fprintf(file,"Disciplinas:\t%4d,\n",ndiscs);
	fprintf(file,"Cursos:\t%4d,\n",ncursos);
	fprintf(file,"Professores:\t%4d,\n",nprofs);
	fprintf(file,"Ofertas:\t%4d,\n",nofertas);
	fprintf(file,"Demanda total:\t%4d,\n",tdemanda);

	if(file)
		fclose(file);
}
