#pragma once

#include <string>
#include <hash_map>

class STVariable
{
public:
   enum STVariableType
   {
      V_ASSIGN,
      V_SLACK_T,
      V_SLACK_H,
      V_ERROR
   };

   STVariable(void) { reset(); }
   ~STVariable(void) {}

   inline STVariableType getType() const { return type; }
   inline int getDisciplina1() const { return disciplina1; }
   inline int getDisciplina2() const { return disciplina2; }
   inline int getTurma1() const { return turma1; }
   inline int getTurma2() const { return turma2; }
   inline int getHorarioInicial() const { return hi; }
   inline int getHorarioFinal() const { return hf; }

   inline void setType(STVariableType value) { type = value; }
   inline void setDisciplina1(int value) { disciplina1 = value; }
   inline void setDisciplina2(int value) { disciplina2 = value; }
   inline void setTurma1(int value) { turma1 = value; }
   inline void setTurma2(int value) { turma2 = value; }
   inline void setHorarioInicial(int value) { hi = value; }
   inline void setHorarioFinal(int value) { hf = value; }

   STVariable& operator =(const STVariable& right);
   bool operator <(const STVariable& right) const;
   bool operator ==(const STVariable& right) const;

   void reset();

   std::string toString();

private:
   STVariableType type;
   int disciplina1;
   int disciplina2;
   int turma1;
   int turma2;
   int hi;
   int hf;
};

class STVariableHasher : public stdext::hash_compare<STVariable>
{
public:
   bool operator()(const STVariable& v1, const STVariable& v2) const;
   size_t operator()(const STVariable& v) const;
};

typedef stdext::hash_map<STVariable, int, STVariableHasher> STVariableHash;
