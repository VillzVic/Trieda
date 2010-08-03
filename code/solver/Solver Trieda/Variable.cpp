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
   value = -1;
   b = NULL;
   i = NULL;
   d = NULL;
   u = NULL;
   s = NULL;
   t = -1;

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
   if (v.getBloco() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(v.getBloco()->getId());
   }
   if (v.getTurma() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(v.getTurma()->getId());
   }
   if (v.getDisciplina() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(v.getDisciplina()->getId());
   }
   if (v.getUnidade() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(v.getUnidade()->getId());
   }
   if (v.getSala() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(v.getSala()->getId());
   }
   if(v.getDia() >= 0) {
      sum *= HASH_PRIME; sum+= intHash(v.getDia());
   }
   return sum;
}

