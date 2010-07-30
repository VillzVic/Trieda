#include "Variable.h"

#include "HashUtil.h"

Variable::Variable()
{
   reset();
}

Variable::Variable(const Variable& var)
{
   *this = var;
}

void Variable::reset()
{
   /* ToDo:
   All pointers that define a variable should be addressed to NULL
   */
}

Variable::~Variable()
{
   reset();
}

Variable& Variable::operator=(const Variable& var)
{

   return *this;
}

bool Variable::operator <(const Variable& var) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;

   /*
   ToDo:
   */

   return false;
}

bool Variable::operator ==(const Variable& var) const
{
   return (!(*this < var) && !(var < *this));
}

std::string Variable::toString()
{
   std::string str;

   /*
   ToDo:
   */

   return str;
}

bool VariableHasher::operator()(const Variable& v1, const Variable& v2) const
{
   return (v1 < v2);
}

size_t VariableHasher::operator()(const Variable& v) const
{
   unsigned int sum = 0;

   /**
   ToDo:
   All pointers different from NULL must be considered in the hash function
   **/

   return sum;
}

