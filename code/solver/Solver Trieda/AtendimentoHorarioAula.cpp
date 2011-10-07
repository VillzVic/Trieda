#include "AtendimentoHorarioAula.h"

AtendimentoHorarioAula::AtendimentoHorarioAula( int id )
{
   this->setId( id );
   this->atendimentos_ofertas = new GGroup< AtendimentoOferta * >();
   _profVirtual = false;
}

AtendimentoHorarioAula::~AtendimentoHorarioAula( void )
{
   this->setId( -1 );

   if ( atendimentos_ofertas != NULL )
   {
      atendimentos_ofertas->deleteElements();
      delete atendimentos_ofertas;
   }
}

std::ostream& operator << ( std::ostream & out, AtendimentoHorarioAula & horario_aula )
{
   out << "<AtendimentoHorarioAula>" << std::endl;

   out << "<horarioAulaId>" << horario_aula.getHorarioAulaId() << "</horarioAulaId>" << std::endl;
   out << "<professorId>" << horario_aula.getProfessorId() << "</professorId>" << std::endl;
   out << "<virtual>" << ( ( horario_aula.profVirtual() ) ? "true" : "false" ) << "</virtual>" << std::endl;
   out << "<creditoTeorico>" << horario_aula.getCreditoTeorico() << "</creditoTeorico>" << std::endl;

   out << "<atendimentosOfertas>" << std::endl;
   GGroup< AtendimentoOferta * >::GGroupIterator it_oferta
	   = horario_aula.atendimentos_ofertas->begin();

   for (; it_oferta != horario_aula.atendimentos_ofertas->end();
	       it_oferta++ )
   {
      out << "<AtendimentoOferta>" << std::endl;
      out << ( **it_oferta );
      out << "</AtendimentoOferta>" << std::endl;
   }
   out << "</atendimentosOfertas>" << std::endl;

   out << "</AtendimentoHorarioAula>" << std::endl;

   return out;
}
