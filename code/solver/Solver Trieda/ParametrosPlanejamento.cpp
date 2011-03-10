#include "ParametrosPlanejamento.h"

ParametrosPlanejamento::ParametrosPlanejamento(void)
{
}

ParametrosPlanejamento::~ParametrosPlanejamento(void)
{
}

void ParametrosPlanejamento::le_arvore(ItemParametrosPlanejamento& elem)
{
   modo_otimizacao = elem.modoOtimizacao();

   ITERA_SEQ(it_niveis_dificuldade_horario,elem.niveisDificuldadeHorario(),NivelDificuldadeHorario) {
      NivelDificuldadeHorario* nivel_dif_hor = new NivelDificuldadeHorario();
      nivel_dif_hor->le_arvore(*it_niveis_dificuldade_horario);
      niveis_dificuldade_horario.add(nivel_dif_hor);
   }

   equilibrar_diversidade_disc_dia = elem.equilibrarDiversidadeDiscDia();
   minimizar_desloc_prof = elem.minimizarDeslocProfessor();
   minimizar_desloc_aluno = elem.minimizarDeslocAluno();
   maxDeslocProf = elem.maxDeslocProfessor();

   ITERA_NSEQ(it_maximizar_avaliacao_cursos,
              elem.maximizarAvaliacaoCursos(),id,Identificador) 
   {
      maximizar_avaliacao_cursos.add(*it_maximizar_avaliacao_cursos);
   }

   ITERA_NSEQ(it_minimizar_custo_docente_cursos,
             elem.minimizarCustoDocenteCursos(),id,Identificador) {
      minimizar_custo_docente_cursos.add(*it_minimizar_custo_docente_cursos);
   }

   xsd::cxx::tree::sequence<GrupoIdentificador>::iterator it = 
      elem.permiteCompartilhamentoTurma().GrupoIdentificador().begin();
   for(;it!=elem.permiteCompartilhamentoTurma().GrupoIdentificador().end();++it) {
      GGroup<int>* g = new GGroup<int>;
      ITERA_NSEQ(it_id,*it,id,Identificador) {
        g->add(*it_id);
      }
      permite_compart_turma.add(g);
   }

   if (elem.cargaHorariaSemanalAluno().equilibrar()) {
      carga_horaria_semanal_aluno = EQUILIBRAR;
   }
   else if (elem.cargaHorariaSemanalAluno().minimizarDias()) {
      carga_horaria_semanal_aluno = MINIMIZAR_DIAS;
   }
   else if (elem.cargaHorariaSemanalAluno().indiferente()) {
      carga_horaria_semanal_aluno = INDIFERENTE;
   }

   minimizar_horarios_vazios_professor = elem.minimizarHorariosVaziosProfessor();
   desempenho_prof_disponibilidade = elem.desempenhoProfDisponibilidade();
   custo_prof_disponibilidade = elem.custoProfDisponibilidade();
   evitar_reducao_carga_horaria_prof = elem.evitarReducaoCargaHorariaProf();
   evitar_prof_ultimo_primeiro_hr = elem.evitarProfUltimoPrimeiroHor();
   min_alunos_abertura_turmas = elem.minAlunosAberturaTurmas();
}
