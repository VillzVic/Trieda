#include "Curso.h"

Curso::Curso( void )
{

}

Curso::~Curso( void )
{

}

void Curso::le_arvore( ItemCurso & elem )
{
   this->setId( elem.id() );
   this->codigo = elem.codigo();
   this->tipo_id = elem.tipoId();
   this->qtd_max_prof_disc = elem.qtdMaxProfDisc();
   this->mais_de_uma = elem.maisDeUmaDiscPeriodo();

   double pm;
   int tit_id;

   pm = elem.regraPercMinMestres().percMinimo();
   tit_id = elem.regraPercMinMestres().tipoTitulacaoId();
   regra_min_mestres = std::make_pair( tit_id, pm );

   pm = elem.regraPercMinDoutores().percMinimo();
   tit_id = elem.regraPercMinDoutores().tipoTitulacaoId();
   regra_min_doutores = std::make_pair( tit_id, pm );

   ITERA_NSEQ( it_areas, elem.areasTitulacao(), id, Identificador )
   {
      area_ids.add( *it_areas );
   }

   ITERA_SEQ( it_curriculo, elem.curriculos(), Curriculo )
   {
      Curriculo * curriculo = new Curriculo();
      curriculo->le_arvore( *it_curriculo );
      curriculos.add( curriculo );
   }

   ITERA_NSEQ( it_equiv, elem.equivalencias(), id, Identificador )
   {
      equiv_ids.add( *it_equiv );
   }

   // Porcentagens mínimas de professores que trabalham em regime
   // de dedicação parcial e/ouo integral, de acordo com o tipo de
   // contrato do professor, que devem ministrar aulas desse curso
   this->minTempoIntegral = elem.minTempoIntegral();
   this->minTempoIntegralParcial = elem.minTempoIntegralParcial();
}


bool Curso::possuiDisciplina( Disciplina *d )
{
   ITERA_GGROUP_LESSPTR( it_curriculo, curriculos, Curriculo )
   {
		if ( it_curriculo->possuiDisciplina( d ) )
			return true;
   }
   return false;
}


int Curso::getNumTotalDisciplinasNoCurso()
{
	int n = 0;
    ITERA_GGROUP_LESSPTR( it_curriculo, curriculos, Curriculo )
    {
		n += it_curriculo->disciplinas_periodo.size();
    }
	return n;
}

int Curso::getQtdMaxDiscNoPeriodoPorProf()
{
	if ( this->getMaisDeUma() )
		return qtd_max_prof_disc;
	else
		return 1;
}

double Curso::getMinPercContrato( TipoContrato *contr )
{ 
	switch ( contr->getContrato() )
	{
	case TipoContrato::Parcial :
		return ( this->getMinTempoIntegralParcial() / 100.0 ); 
	case TipoContrato::Integral :
		return ( this->getMinTempoIntegral() / 100.0 );
	case TipoContrato::Outro :
		return 0;
	}

	return 0;
}

bool Curso::possuiEquiv( int equivId )
{
	if ( this->equiv_ids.find( equivId ) != this->equiv_ids.end() )
		return true;
	return false;
}