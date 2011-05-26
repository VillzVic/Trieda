#ifndef _ATENDIMENTO_HORARIO_AULA_H_
#define _ATENDIMENTO_HORARIO_AULA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoOferta.h"

class AtendimentoHorarioAula :
   public OFBase
{
public:
   AtendimentoHorarioAula( void );
   virtual ~AtendimentoHorarioAula( void );

   GGroup< AtendimentoOferta * > * atendimentos_ofertas;

   void setHorarioAulaId( int value ) { horario_aula_id = value; }
   void setProfessorId( int value ) { professor_id = value; }
   void setCreditoTeorico( bool value ) { credito_teorico = value; }

   int getHorarioAulaId() const { return horario_aula_id; }
   int getProfessorId() const { return professor_id; }
   bool getCreditoTeorico() const { return credito_teorico; }

private:
   int horario_aula_id;
   int professor_id;
   bool credito_teorico;
};

std::ostream & operator << ( std::ostream &, AtendimentoHorarioAula & );

#endif