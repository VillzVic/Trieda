#ifndef _AULA_H_
#define _AULA_H_

#include <vector>
#include "GGroup.h"

#include "disciplina.h"
#include "Oferta.h"
#include "sala.h"

class Aula
{
public:
   Aula(bool _aulaVirtual = false);
   virtual ~Aula(void);

   // Ofertas que s�o atendidas por essa aula
   GGroup< Oferta * > ofertas;

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

   // Para cada cr�dito alocado da aula em quest�o armazena-se uma
   // refer�ncia para o hor�rio do professor em que tal aloca��o foi realizada.
   std::vector< std::pair< Professor *, Horario * > > bloco_aula;

   virtual bool operator < (Aula const & right) 
   { 
      if(disciplina < right.getDisciplina() )
         return true;
      else if(disciplina > right.getDisciplina() )
         return false;
      
      if(turma < right.getTurma())
         return true;
      else if(turma > right.getTurma())
         return false;

      if(dia_semana < right.getDiaSemana())
         return true;
      else if(dia_semana > right.getDiaSemana())
         return false;

      if(sala < right.getSala())
         return true;
      else if(sala > right.getSala())
         return false;

      return false;
   }

   virtual bool operator == (Aula const & right)
   { 
      return 
         ((turma == right.getTurma()) &&
         (disciplina == right.getDisciplina()) &&
         (sala == right.getSala()) &&
         (dia_semana == right.getDiaSemana()) &&
         (creditos_teoricos == right.getCreditosTeoricos()) &&
         (creditos_praticos == right.getCreditosPraticos()));
   }

   virtual bool operator != (Aula const & right)
   { 
      return !(*this == right);
   }

   void toSring();

private:
   int turma;
   Disciplina* disciplina;
   Sala* sala;
   int dia_semana;
   int quantidade;
   int creditos_teoricos;
   int creditos_praticos;

   // Indica se uma aula � virtual ou n�o.
   bool aula_virtual;
};

#endif // _AULA_H_