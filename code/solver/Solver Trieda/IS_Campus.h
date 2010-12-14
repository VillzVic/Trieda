#pragma once

#include "Campus.h"

#include "IS_Unidade.h"

class IS_Campus : public OFBase
{
public:
   IS_Campus(Campus * _campus);

   IS_Campus(IS_Campus const & is_campus);

   virtual ~IS_Campus(void);

   Campus * campus;

   GGroup<IS_Unidade*> unidades;
};
