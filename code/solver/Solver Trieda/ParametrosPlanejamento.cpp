#include "ParametrosPlanejamento.h"

ParametrosPlanejamento::ParametrosPlanejamento(void)
{
}

ParametrosPlanejamento::~ParametrosPlanejamento(void)
{
}

void ParametrosPlanejamento::le_arvore( ItemParametrosPlanejamento & elem )
{
   modo_otimizacao = elem.modoOtimizacao();

   otimizarPor = elem.otimizarPor();

   ITERA_SEQ( it_niveis_dificuldade_horario,
			  elem.niveisDificuldadeHorario(),
			  NivelDificuldadeHorario )
   {
      NivelDificuldadeHorario * nivel_dif_hor = new NivelDificuldadeHorario();
      nivel_dif_hor->le_arvore( *it_niveis_dificuldade_horario );
      niveis_dificuldade_horario.add( nivel_dif_hor );
   }

   equilibrar_diversidade_disc_dia = elem.equilibrarDiversidadeDiscDia();
   minimizar_desloc_prof = elem.minimizarDeslocProfessor();
   minimizar_desloc_aluno = elem.minimizarDeslocAluno();

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if ( elem.maxDeslocProfessor().present() )
   {
	  maxDeslocProf = elem.maxDeslocProfessor().get();
   }

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if ( elem.maximizarAvaliacaoCursos().present() )
   {
	   ITERA_NSEQ( it_maximizar_avaliacao_cursos,
				   elem.maximizarAvaliacaoCursos().get(),
				   id, Identificador ) 
	   {
		  maximizar_avaliacao_cursos.add( *it_maximizar_avaliacao_cursos );
	   }
   }

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if ( elem.minimizarCustoDocenteCursos().present() )
   {
	   ITERA_NSEQ( it_minimizar_custo_docente_cursos,
				   elem.minimizarCustoDocenteCursos().get(),
				   id, Identificador )
	   {
		  minimizar_custo_docente_cursos.add( *it_minimizar_custo_docente_cursos );
	   }
   }

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if ( elem.permiteCompartilhamentoTurma().present() )
   {
	   xsd::cxx::tree::sequence< GrupoIdentificador >::iterator it = 
		   elem.permiteCompartilhamentoTurma().get().GrupoIdentificador().begin();
	   for(; it != elem.permiteCompartilhamentoTurma().get().GrupoIdentificador().end(); ++it )
	   {
		  GGroup< int > * g = new GGroup< int >;
		  ITERA_NSEQ( it_id, *it, id, Identificador )
		  {
		     g->add( *it_id );
		  }

		  nao_permite_compart_turma.add( g );
	   }
   }

   // ALUNO
   if ( elem.cargaHorariaSemanalAluno().equilibrar() )
   {
      carga_horaria_semanal_aluno = EQUILIBRAR;
   }
   else if ( elem.cargaHorariaSemanalAluno().minimizarDias() )
   {
      carga_horaria_semanal_aluno = MINIMIZAR_DIAS;
   }
   else if ( elem.cargaHorariaSemanalAluno().indiferente() )
   {
      carga_horaria_semanal_aluno = INDIFERENTE;
   }

   // PROFESSOR
   if ( elem.cargaHorariaSemanalProfessor().equilibrar() )
   {
      carga_horaria_semanal_prof = EQUILIBRAR;
   }
   else if ( elem.cargaHorariaSemanalProfessor().minimizarDias() )
   {
      carga_horaria_semanal_prof = MINIMIZAR_DIAS;
   }
   else if ( elem.cargaHorariaSemanalProfessor().indiferente() )
   {
      carga_horaria_semanal_prof = INDIFERENTE;
   }
   
   minimizar_horarios_vazios_professor = elem.minimizarHorariosVaziosProfessor();
   desempenho_prof_disponibilidade = elem.desempenhoProfDisponibilidade();
   custo_prof_disponibilidade = elem.custoProfDisponibilidade();
   evitar_reducao_carga_horaria_prof = elem.evitarReducaoCargaHorariaProf();
   evitar_prof_ultimo_primeiro_hr = elem.evitarProfUltimoPrimeiroHor();
   min_alunos_abertura_turmas = elem.minAlunosAberturaTurmas();
   funcao_objetivo = elem.funcaoObjetivo();
   considerar_equivalencia = elem.considerarEquivalencia();
   permite_compartilhamento_turma_sel = elem.permiteCompartilhamentoTurmaSel();
   permitir_alunos_em_varios_campi = elem.permitirAlunosEmVariosCampi();
   min_doutores = elem.percentuaisMinimoDoutores();
   min_mestres = elem.percentuaisMinimoMestres();
   considerar_preferencia_prof = elem.preferenciaProfessorDisciplina();
   considerar_desempenho_prof = elem.desempenhoProfDisponibilidade();
   violar_min_alunos_turmas_formandos = elem.violarMinAlunosTurmasFormandos();
   utilizarDemandasP2 = elem.utilizarDemandasP2();

   perc_max_reducao_CHP = elem.evitarReducaoCargaHorariaProfValor().get();
   min_alunos_abertura_turmas_value = elem.minAlunosAberturaTurmasValor().get();
      
   if ( min_alunos_abertura_turmas_value <= 0 ) min_alunos_abertura_turmas=false;

   // TODO
  // considerar_equivalencia_por_aluno = false;
   considerar_equivalencia_por_aluno = considerar_equivalencia;
   considerar_equivalencia = false;
   
   considerar_disponibilidade_prof_em_tatico = true;

}
