#pragma once

#include <hash_map>
#include <string>

class STConstraint
{
public:
   enum STConstraintType
   {
      C_CONFLICT,
      C_ASSIGN,
      C_ATEND,
      C_ERROR
   };

   STConstraint(void) { reset(); }
   ~STConstraint(void) {}

   inline STConstraintType getType() const { return type; }
   inline int getHorario() const { return horario; }
   inline int getDisciplina1() const { return disciplina1; }
   inline int getDisciplina2() const { return disciplina2; }
   inline int getTurma1() const { return turma1; }
   inline int getTurma2() const { return turma2; }

   inline void setType(STConstraintType value) { type = value; }
   inline void setHorario(int value) { horario = value; }
   inline void setDisciplina1(int value) { disciplina1 = value; }
   inline void setDisciplina2(int value) { disciplina2 = value; }
   inline void setTurma1(int value) { turma1 = value; }
   inline void setTurma2(int value) { turma2 = value; }

   STConstraint& operator =(const STConstraint& right);
   bool operator <(const STConstraint& right) const;
   bool operator ==(const STConstraint& right) const;

   inline void reset() { type = C_ERROR; horario = -1; disciplina1 = -1; disciplina2 = -1; turma1 = -1; turma2 = -1; }

   std::string toString();

private:
   STConstraintType type;
   int horario;
   int disciplina1;
   int disciplina2;
   int turma1;
   int turma2;
};

class STConstraintHasher : public stdext::hash_compare<STConstraint>
{
public:
   bool operator() (const STConstraint& cons1, const STConstraint& cons2) const;
   size_t operator() (const STConstraint& cons) const;
};

typedef stdext::hash_map<STConstraint, int, STConstraintHasher> STConstraintHash;
