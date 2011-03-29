#include "ProblemData.h"
#include <iostream>

// Macro baseada na ITERA_SEQ para facilitar a leitura dos dados no
// ProblemData
#define LE_SEQ(ggroup,addr,type) \
   for (Grupo##type##::##type##_iterator __##type##_it = \
   (addr).type##().begin(); __##type##_it != (addr).type##().end(); \
   ++ __##type##_it) { \
   type *__obj_##type## = new type; \
   __obj_##type##->le_arvore(*__##type##_it); \
   ggroup.add(__obj_##type); } 

ProblemData::ProblemData()
{
   //atendimentosTatico = new GGroup<AtendimentoCampusSolucao*>(); 
   atendimentosTatico = NULL;
}

ProblemData::~ProblemData()
{
}

void ProblemData::le_arvore(TriedaInput& raiz)
{
   calendario = new Calendario();
   calendario->le_arvore(raiz.calendario());

   ITERA_SEQ(it_campi, raiz.campi(), Campus)
   {
      Campus* c = new Campus;
      c->le_arvore(*it_campi);
      campi.add(c);
   }

   ITERA_SEQ(it_tsalas, raiz.tiposSala(), TipoSala)
   {
      TipoSala* t = new TipoSala;
      t->le_arvore(*it_tsalas);
      tipos_sala.add(t);
   }

   LE_SEQ(tipos_sala, raiz.tiposSala(), TipoSala);
   LE_SEQ(tipos_contrato, raiz.tiposContrato(), TipoContrato);
   LE_SEQ(tipos_titulacao, raiz.tiposTitulacao(), TipoTitulacao);
   LE_SEQ(areas_titulacao, raiz.areasTitulacao(), AreaTitulacao);
   LE_SEQ(tipos_disciplina, raiz.tiposDisciplina(), TipoDisciplina);
   LE_SEQ(niveis_dificuldade, raiz.niveisDificuldade(), NivelDificuldade);
   LE_SEQ(tipos_curso, raiz.tiposCurso(), TipoCurso);
   LE_SEQ(regras_div, raiz.regrasDivisaoCredito(), DivisaoCreditos);
   LE_SEQ(tempo_campi, raiz.temposDeslocamentosCampi(), Deslocamento);
   LE_SEQ(tempo_unidades, raiz.temposDeslocamentosUnidades(), Deslocamento);
   LE_SEQ(disciplinas, raiz.disciplinas(), Disciplina);
   LE_SEQ(cursos, raiz.cursos(), Curso);
   LE_SEQ(demandas, raiz.demandas(), Demanda);

   ITERA_SEQ(it_of, raiz.ofertaCursosCampi(), OfertaCurso)
   {
      Oferta* o = new Oferta;
      o->le_arvore(*it_of);
      ofertas.add(o);
   }

   parametros = new ParametrosPlanejamento;
   parametros->le_arvore(raiz.parametrosPlanejamento());

   // Se a tag existir (mesmo que esteja em branco) no xml de entrada
   if (raiz.atendimentosTatico().present())
   {
      atendimentosTatico = new GGroup<AtendimentoCampusSolucao*> ();

      for (unsigned int i = 0; i < raiz.atendimentosTatico().get().AtendimentoCampus().size(); i++)
      {
         ItemAtendimentoCampusSolucao* it_atendimento
            = &(raiz.atendimentosTatico().get().AtendimentoCampus().at(i));

         AtendimentoCampusSolucao* item = new AtendimentoCampusSolucao();
         item->le_arvore(*it_atendimento);
         atendimentosTatico->add(item);
      }
   }

   LE_SEQ(fixacoes, raiz.fixacoes(), Fixacao);

   // Monta um 'map' para recuperar cada 'ItemSala'
   // do campus a partir de seu respectivo id de sala
   map<int, ItemSala*> mapItemSala;
   ITERA_SEQ(it_campi, raiz.campi(), Campus)
   {
      ITERA_SEQ(it_unidade, it_campi->unidades(), Unidade)
      {
         ITERA_SEQ(it_sala, it_unidade->salas(), Sala)
         {
            mapItemSala[ it_sala->id() ] = &(*(it_sala));
         }
      }
   }

   // Prencher os horários e/ou créditos das salas
   ITERA_GGROUP(it_campi, campi, Campus)
   {
      ITERA_GGROUP(it_unidade, it_campi->unidades, Unidade)
      {
         ITERA_GGROUP(it_sala, it_unidade->salas, Sala)
         {
            map<int, ItemSala*>::iterator it
               = mapItemSala.find(it_sala->getId());

            if (it != mapItemSala.end())
            {
               // Objeto de entrada do XML
               ItemSala* elem = it->second;

               it_sala->construirCreditosHorarios(
                  *elem, parametros->modo_otimizacao,
                  raiz.atendimentosTatico().present());
            }
         }
      }
   }
   ///
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