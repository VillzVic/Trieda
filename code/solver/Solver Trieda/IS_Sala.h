#ifndef _IS_SALA_H_
#define _IS_SALA_H_

#include <vector>
#include <map>

#include "Sala.h"
#include "Demanda.h"
#include "Disciplina.h"
#include "Oferta.h"
#include "IS_Dia_Semana.h"

class IS_Sala
   : public OFBase
{
public:
   IS_Sala( Sala * );
   IS_Sala( IS_Sala const & );
   virtual ~IS_Sala( void );

   Sala * sala;
   GGroup< IS_Dia_Semana * > is_Dia_Semana;

   void aloca( int,
      Demanda *, // Referência para a Demanda que está sendo atendida (parcialmente ou totalmente)
      int, // Indica a quantidade da demanda que está sendo atendida
      std::vector< std::pair< int /*dia*/, int /*num creds*/ > > & );

   bool regraValida( std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > & );
};

#endif