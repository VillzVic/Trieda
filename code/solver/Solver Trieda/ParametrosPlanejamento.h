#pragma once
#include "ofbase.h"

#include "NivelDificuldadeHorario.h"

class ParametrosPlanejamento:
   public OFBase
{
public:
   ParametrosPlanejamento(void);
   virtual ~ParametrosPlanejamento(void);

   std::string modo_otimizacao;
   GGroup< NivelDificuldadeHorario * > niveis_dificuldade_horario;

   GGroup< int > maximizar_avaliacao_cursos;
   GGroup< int > minimizar_custo_docente_cursos;
   GGroup< GGroup< int > * > permite_compart_turma;
   enum CHSA { EQUILIBRAR, MINIMIZAR_DIAS, INDIFERENTE };
   CHSA carga_horaria_semanal_aluno;

   int maxDeslocProf;
   int funcao_objetivo;

   bool equilibrar_diversidade_disc_dia;
   bool minimizar_desloc_prof;
   bool minimizar_desloc_aluno;
   bool minimizar_horarios_vazios_professor;
   bool desempenho_prof_disponibilidade;
   bool custo_prof_disponibilidade;
   bool evitar_reducao_carga_horaria_prof;
   bool evitar_prof_ultimo_primeiro_hr;
   bool min_alunos_abertura_turmas;
   bool considerar_equivalencia;

   virtual void le_arvore( ItemParametrosPlanejamento & );
};
