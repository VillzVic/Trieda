#ifndef _AULA_H_
#define _AULA_H_

#include <vector>
#include "GGroup.h"

#include "disciplina.h"
#include "Oferta.h"
#include "sala.h"
#include "AlocacaoAula.h"

class Aula
{
public:
   Aula(bool _aulaVirtual = false);
   virtual ~Aula(void);

   GGroup<Oferta*> ofertas;

   void setTurma(int);
   void setDisciplina(Disciplina*);
   void setSala(Sala*);
   void setDiaSemana(int);
   void setCreditosTeoricos(int);
   void setCreditosPraticos(int);
   void setAulaVirtual(bool value);

   int getTurma() const;
   Disciplina* getDisciplina() const;
   Sala* getSala() const;
   int getDiaSemana() const;
   int getCreditosTeoricos() const;
   int getCreditosPraticos() const;
   int getTotalCreditos() const;

   bool eVirtual() const;

   std::vector<AlocacaoAula> alocacao_aula;

   virtual bool operator < (Aula const & right) 
   { 
      return 
         ((turma < right.getTurma()) &&
         (disciplina < right.getDisciplina()) &&
         (sala < right.getSala()) &&
         (diaSemana < right.getDiaSemana()) &&
         (creditos_teoricos < right.getCreditosTeoricos()) &&
         (creditos_praticos < right.getCreditosPraticos()));
   }

   virtual bool operator == (Aula const & right)
   { 
      return 
         ((turma == right.getTurma()) &&
         (disciplina == right.getDisciplina()) &&
         (sala == right.getSala()) &&
         (diaSemana == right.getDiaSemana()) &&
         (creditos_teoricos == right.getCreditosTeoricos()) &&
         (creditos_praticos == right.getCreditosPraticos()));
   }

   void toSring();

private:
   int turma;
   Disciplina* disciplina;
   Sala* sala;
   int diaSemana;
   int quantidade;
   int creditos_teoricos;
   int creditos_praticos;

   // Indica se uma aula � virtual ou n�o.
   bool aula_virtual;
};

#endif // _AULA_H_