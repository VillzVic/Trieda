#include "AtendimentoTatico.h"

AtendimentoTatico::AtendimentoTatico( int idAtTatico, int idAtOferta )
{
   this->setId( idAtTatico );
   this->atendimento_oferta = new AtendimentoOferta( idAtOferta );

   this->qtde_creditos_teoricos = 0;
   this->qtde_creditos_praticos = 0;
}

AtendimentoTatico::~AtendimentoTatico( void )
{
   this->setId( -1 );

   if ( atendimento_oferta != NULL )
   {
       delete atendimento_oferta;
   }
}

std::ostream & operator << ( std::ostream& out, AtendimentoTatico & tatico )
{
   out << "<AtendimentoTatico>" << std::endl;

   if ( tatico.atendimento_oferta )
   {
      out << "<atendimentoOferta>" << std::endl
		    << ( *tatico.atendimento_oferta )
		    << "</atendimentoOferta>" << std::endl;
   }
   else
   {
      out << "<atendimentoOferta />" << std::endl;
   }

   out << "<qtdeCreditosTeoricos>" << tatico.getQtdCreditosTeoricos()
	    << "</qtdeCreditosTeoricos>" << std::endl;

   out << "<qtdeCreditosPraticos>" << tatico.getQtdCreditosPraticos()
	    << "</qtdeCreditosPraticos>" << std::endl;
   
   GGroup<int> hors = tatico.getHorariosAula();   

   if ( hors.size() > 0 )
   {
	   out << "<horariosAula>" << std::endl;  
	   ITERA_GGROUP_N_PT( it, hors, int )
	   {
		   out << "<horarioAulaId>" << *it << "</horarioAulaId>" << std::endl;
	   }
	   out << "</horariosAula>" << std::endl;
   }

   out << "</AtendimentoTatico>" << std::endl;

   return out;
}