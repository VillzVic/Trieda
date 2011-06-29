#include "IS_Atendimento_Tatico.h"

IS_Atendimento_Tatico::IS_Atendimento_Tatico(void)
{
   credito = 0;
   turma = -1;
   demanda_Atendida = 0;
}

IS_Atendimento_Tatico::IS_Atendimento_Tatico( IS_Atendimento_Tatico const & is_Atendimento_Tatico )
{
   std::cerr << "COPY CONSTRUCTOR OF <IS_Atendimento_Tatico> NOT IMPLEMENTED YET" << std::endl;
   exit(1);
}

IS_Atendimento_Tatico::IS_Atendimento_Tatico( int _credito )
{
   credito = _credito;
   turma = -1;
   demanda_Atendida = 0;
}
IS_Atendimento_Tatico::~IS_Atendimento_Tatico( void )
{

}

bool IS_Atendimento_Tatico::operator < ( IS_Atendimento_Tatico const & right )
{
   return ( credito <= right.credito );
}

bool IS_Atendimento_Tatico::operator == ( IS_Atendimento_Tatico const & right )
{
   return ( credito == right.credito );
}
