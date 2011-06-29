#ifndef _IS_UNIDADE_H_
#define _IS_UNIDADE_H_

#include "Unidade.h"
#include "IS_Sala.h"
#include "ConjuntoSala.h"

class IS_Unidade
   : public OFBase
{
public:
   IS_Unidade( Unidade * );
   IS_Unidade( IS_Unidade const & );
   virtual ~IS_Unidade( void );

   Unidade * unidade;
   GGroup< IS_Sala * > salas;
};

#endif