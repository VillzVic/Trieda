#include "STConstraint.h"

#include "GUtil.h"
#include "HashUtil.h"

std::string STConstraint::toString()
{
   std::string out;

   switch (type)
   {
   case C_CONFLICT: out = "C_CONFLICT"; break;
   case C_ASSIGN: out = "C_ASSIGN"; break;
   case C_ATEND: out = "C_ATEND"; break;
   default: out = "C_ERR"; break;
   }

   if (horario != -1)
      out += "_h" + GUtil::intToString(horario);

   if (disciplina1 != -1)
      out += "_da" + GUtil::intToString(disciplina1);

   if (turma1 != -1)
      out += "_ia" + GUtil::intToString(turma1);

   if (disciplina2 != -1)
      out += "_db" + GUtil::intToString(disciplina2);

   if (turma2 != -1)
      out += "_ib" + GUtil::intToString(turma2);

   return out;
}

STConstraint& STConstraint::operator =(const STConstraint& right)
{
   type = right.type;
   horario = right.horario;
   disciplina1 = right.disciplina1;
   disciplina2 = right.disciplina2;
   turma1 = right.turma1;
   turma2 = right.turma2;
   return *this;
}

bool STConstraint::operator <(const STConstraint& right) const
{
   if (horario != right.horario)
      return (horario < right.horario);
   if (disciplina1 != right.disciplina1)
      return (disciplina1 < right.disciplina1);
   if (disciplina2 != right.disciplina2)
      return (disciplina2 < right.disciplina2);
   if (turma1 != right.turma1)
      return (turma1 < right.turma1);
   return (turma2 < right.turma2);
}

bool STConstraint::operator ==(const STConstraint& right) const 
{
   return (!(*this < right) && !(right < *this));
}

bool STConstraintHasher::operator()(const STConstraint& c1, const STConstraint& c2) const
{
   return (c1 < c2);
}

size_t STConstraintHasher::operator()(const STConstraint& c) const
{
   unsigned int sum = 0;
   
   sum += intHash((int)c.getType());

   sum *= HASH_PRIME;
   sum += intHash(c.getHorario());

   sum *= HASH_PRIME;
   sum += intHash(c.getDisciplina1());

   sum *= HASH_PRIME;
   sum += intHash(c.getTurma1());

   sum *= HASH_PRIME;
   sum += intHash(c.getDisciplina2());

   sum *= HASH_PRIME;
   sum += intHash(c.getTurma2());

   return sum;
}