#include "STVariable.h"

#include "GUtil.h"
#include "HashUtil.h"

void STVariable::reset()
{
   type = V_ERROR;
   disciplina1 = -1;
   disciplina2 = -1;
   turma1 = -1;
   turma2 = -1;
   hi = -1;
   hf = -1;
}

STVariable& STVariable::operator =(const STVariable& right)
{
   type = right.type;
   disciplina1 = right.disciplina1;
   disciplina2 = right.disciplina2;
   turma1 = right.turma1;
   turma2 = right.turma2;
   hi = right.hi;
   hf = right.hf;

   return *this;
}

bool STVariable::operator <(const STVariable& right) const
{
   if (type != right.type)
      return ((int)type < (int)right.type);

   if (disciplina1 != right.disciplina1)
      return (disciplina1 < right.disciplina1);

   if (disciplina2 != right.disciplina2)
      return (disciplina2 < right.disciplina2);

   if (turma1 != right.turma1)
      return (turma1 < right.turma1);

   if (turma2 != right.turma2)
      return (turma2 < right.turma2);

   if (hi != right.hi)
      return (hi < right.hi);

   return (hf < right.hf);
}

bool STVariable::operator ==(const STVariable& right) const
{
   return (!(*this < right) && !(right < *this));
}

std::string STVariable::toString()
{
   std::string out;

   switch (type)
   {
   case V_ASSIGN: out = "X"; break;
   case V_SLACK_T: out = "FT"; break;
   case V_SLACK_H: out = "FH"; break;
   default: out = "ERR"; break;
   }

   if (disciplina1 != -1)
      out += "_da" + GUtil::intToString(disciplina1);

   if (turma1 != -1)
      out += "_ia" + GUtil::intToString(turma1);

   if (disciplina2 != -1)
      out += "_db" + GUtil::intToString(disciplina2);

   if (turma2 != -1)
      out += "_ib" + GUtil::intToString(turma2);

   if (hi != -1)
      out += "_hi" + GUtil::intToString(hi);

   if (hf != -1)
      out += "_hf" + GUtil::intToString(hf);

   return out;
}

bool STVariableHasher::operator()(const STVariable& v1, const STVariable& v2) const
{
   return (v1 < v2);
}

size_t STVariableHasher::operator()(const STVariable& v) const
{
   unsigned int sum = 0;

   sum += intHash((int)v.getType());

   sum *= HASH_PRIME;
   sum += intHash(v.getDisciplina1());

   sum *= HASH_PRIME;
   sum += intHash(v.getDisciplina2());

   sum *= HASH_PRIME;
   sum += intHash(v.getTurma1());

   sum *= HASH_PRIME;
   sum += intHash(v.getTurma2());

   sum *= HASH_PRIME;
   sum += intHash(v.getHorarioInicial());

   sum *= HASH_PRIME;
   sum += intHash(v.getHorarioFinal());

   return sum;
}
