#ifndef _ATENDIMENTO_HORARIO_AULA_H_
#define _ATENDIMENTO_HORARIO_AULA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "AtendimentoOferta.h"

class AtendimentoHorarioAula :
   public OFBase
{
public:
   AtendimentoHorarioAula( int );
   virtual ~AtendimentoHorarioAula( void );

   GGroup< AtendimentoOferta * > * atendimentos_ofertas;

   void setHorarioAulaId( int value ) { this->horario_aula_id = value; }
   void setProfessorId( int value ) { this->professor_id = value; }
   void setCreditoTeorico( bool value ) { this->credito_teorico = value; }
   void setProfVirtual( bool value ) { this->_profVirtual = value; }

   int getHorarioAulaId() const { return this->horario_aula_id; }
   int getProfessorId() const { return this->professor_id; }
   bool getCreditoTeorico() const { return this->credito_teorico; }
   bool profVirtual() const { return this->_profVirtual; }

private:
   int horario_aula_id;
   int professor_id;
   bool _profVirtual;
   bool credito_teorico;
};

std::ostream & operator << ( std::ostream &, AtendimentoHorarioAula & );

#endif
