#ifndef _IS_CAMPUS_H_
#define _IS_CAMPUS_H_

#include "Campus.h"
#include "IS_Unidade.h"

class IS_Campus
   : public OFBase
{
public:
   IS_Campus( Campus * );
   IS_Campus( IS_Campus const & );
   virtual ~IS_Campus( void );

   Campus * campus;
   GGroup< IS_Unidade * > unidades;
};

#endif