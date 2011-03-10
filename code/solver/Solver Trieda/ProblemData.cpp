#include "ProblemData.h"
#include <iostream>

/* Macro baseada na ITERA_SEQ para facilitar a leitura dos dados no
   ProblemData */
#define LE_SEQ(ggroup,addr,type) \
   for (Grupo##type##::##type##_iterator __##type##_it = \
   (addr).type##().begin(); __##type##_it != (addr).type##().end(); \
   ++ __##type##_it) { \
     type *__obj_##type## = new type; \
     __obj_##type##->le_arvore(*__##type##_it); \
     ggroup.add(__obj_##type); } 


ProblemData::ProblemData()
{
   /*
   ToDo:
   */
}

ProblemData::~ProblemData()
{
   /*
   ToDo:
   */
}

void ProblemData::le_arvore(TriedaInput& raiz)
{
   calendario = new Calendario();
   calendario->le_arvore(raiz.calendario());
   ITERA_SEQ(it_campi,raiz.campi(),Campus) {
      Campus* c = new Campus;
      c->le_arvore(*it_campi);
      campi.add(c);
   }
   ITERA_SEQ(it_tsalas,raiz.tiposSala(),TipoSala) {
      TipoSala* t = new TipoSala;
      t->le_arvore(*it_tsalas);
      tipos_sala.add(t);
   }
   LE_SEQ(tipos_sala,raiz.tiposSala(),TipoSala);
   LE_SEQ(tipos_contrato,raiz.tiposContrato(),TipoContrato);
   LE_SEQ(tipos_titulacao,raiz.tiposTitulacao(),TipoTitulacao);
   LE_SEQ(areas_titulacao,raiz.areasTitulacao(),AreaTitulacao);
   LE_SEQ(tipos_disciplina,raiz.tiposDisciplina(),TipoDisciplina);
   LE_SEQ(niveis_dificuldade,raiz.niveisDificuldade(),NivelDificuldade);
   LE_SEQ(tipos_curso,raiz.tiposCurso(),TipoCurso);
   LE_SEQ(regras_div,raiz.regrasDivisaoCredito(),DivisaoCreditos);
   LE_SEQ(tempo_campi,raiz.temposDeslocamentosCampi(),Deslocamento);
   LE_SEQ(tempo_unidades,raiz.temposDeslocamentosUnidades(),Deslocamento);
   LE_SEQ(disciplinas,raiz.disciplinas(),Disciplina);
   LE_SEQ(cursos,raiz.cursos(),Curso);
   LE_SEQ(demandas,raiz.demandas(),Demanda);
   ITERA_SEQ(it_of,raiz.ofertaCursosCampi(),OfertaCurso) {
      Oferta* o = new Oferta;
      o->le_arvore(*it_of);
      ofertas.add(o);
   }

   parametros = new ParametrosPlanejamento;
   parametros->le_arvore(raiz.parametrosPlanejamento());

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if (raiz.atendimentosTatico().present())
   {
		atendimentosTatico = &(raiz.atendimentosTatico().get());
   }

   LE_SEQ(fixacoes, raiz.fixacoes(), Fixacao);
}

/*
Disciplina * ProblemData::discPresenteOferta(Disciplina & disc, Oferta & oferta)
{
   GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
      oferta.curriculo->disciplinas_periodo.begin();

   for(; itPrdDisc != oferta.curriculo->disciplinas_periodo.end(); itPrdDisc++)
   { 
      Disciplina * ptDisc = refDisciplinas[(*itPrdDisc).second];

      if(ptDisc->getId() == disc.getId())
      { return ptDisc; }
   }

   return NULL; // caso a oferta não possua a disc.
}
*/