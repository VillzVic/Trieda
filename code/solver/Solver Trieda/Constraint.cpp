#include "Constraint.h"
#include "HashUtil.h"

#define E_MENOR(a,b) \
   (a == NULL && b != NULL) || \
   (b != NULL && a != NULL && (*a < *b))

Constraint::Constraint()
{
   reset();

   /*
   ToDo:
   Attributes should be initiated
   */
}

Constraint::Constraint(const Constraint &cons)
{
   *this = cons;
}

Constraint::~Constraint()
{
   reset();
}

Constraint& Constraint::operator= (const Constraint& cons)
{   
   this->type = cons.getType();
   this->b = cons.getBloco();
   this->i = cons.getTurma();
   this->d = cons.getDisciplina();
   this->u = cons.getUnidade();
   this->s = cons.getSala();
   this->t = cons.getDia();

   return *this;
}

bool Constraint::operator< (const Constraint& cons) const
{
   if( (int)this->getType() < (int) cons.getType() )
      return true;
   else if( (int)this->getType() > (int) cons.getType() )
      return false;
   if (E_MENOR(this->getTurma(),cons.getTurma())) return true;
   if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;
   if (E_MENOR(this->getUnidade(),cons.getUnidade())) return true;
   if (E_MENOR(this->getSala(),cons.getSala())) return true;
   if (E_MENOR(this->getBloco(),cons.getBloco())) return true;
   if (this->getDia() < cons.getDia()) return true;
   return false;
}

bool Constraint::operator== (const Constraint& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void Constraint::reset()
{   
   b = NULL;
   i = NULL;
   d = NULL;
   u = NULL;
   s = NULL;
   t = -1;
}

std::string Constraint::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";
   std::string consName = "";
   ss >> consName;
   return consName;
}

size_t ConstraintHasher::operator() (const Constraint& cons) const
{
   unsigned int sum = 0;
   if (cons.getBloco() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getBloco()->getId());
   }
   if (cons.getTurma() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getTurma()->getId());
   }
   if (cons.getDisciplina() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getDisciplina()->getId());
   }
   if (cons.getUnidade() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getUnidade()->getId());
   }
   if (cons.getSala() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getSala()->getId());
   }
   if(cons.getDia() >= 0) {
      sum *= HASH_PRIME; sum+= intHash(cons.getDia());
   }
   return sum;
}

bool ConstraintHasher::operator() (const Constraint& cons1, const Constraint& cons2) const
{
   return (cons1 < cons2);
}
