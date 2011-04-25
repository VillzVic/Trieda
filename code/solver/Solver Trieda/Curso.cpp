#include "Curso.h"

Curso::Curso(void)
{
}

Curso::~Curso(void)
{
}

void Curso::le_arvore(ItemCurso& elem)
{
   this->setId( elem.id() );
   codigo = elem.codigo();
   tipo_id = elem.tipoId();
   qtd_max_prof_disc = elem.qtdMaxProfDisc();
   mais_de_uma = elem.maisDeUmaDiscPeriodo();

   double pm;
   int tit_id;
   pm = elem.regraPercMinMestres().percMinimo();
   tit_id = elem.regraPercMinMestres().tipoTitulacaoId();
   regra_min_mestres = std::make_pair(tit_id, pm);

   pm = elem.regraPercMinDoutores().percMinimo();
   tit_id = elem.regraPercMinDoutores().tipoTitulacaoId();
   regra_min_doutores = std::make_pair(tit_id, pm);

   ITERA_NSEQ( it_areas, elem.areasTitulacao(), id,
			   Identificador )
   {
      area_ids.add( *it_areas );
   }

   ITERA_SEQ( it_curriculo, elem.curriculos(),
			  Curriculo )
   {
      Curriculo * curriculo = new Curriculo();
      curriculo->le_arvore( *it_curriculo );
      curriculos.add(curriculo);
   }

   // Porcentagens mínimas de professores que trabalham em regime
   // de dedicação parcial e/ouo integral, de acordo com o tipo de
   // contrato do professor, que devem ministrar aulas desse curso
   this->minTempoIntegral = elem.minTempoIntegral();
   this->minTempoIntegralParcial = elem.minTempoIntegralParcial();
}
