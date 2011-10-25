#ifndef _ATENDIMENTO_BASE_H_
#define _ATENDIMENTO_BASE_H_

#include "ProblemData.h"

class AtendimentoBase
{
public:

   AtendimentoBase();
   virtual ~AtendimentoBase();

   virtual bool operator < ( AtendimentoBase const & ) const;

   Professor * professor;
   Campus * campus;
   Unidade * unidade;
   Sala * sala;
   Turno * turno;
   HorarioAula * horario_aula;
   Disciplina * disciplina;
   int dia_semana;
   int turma;
   int quantidade;

   int idAtCampus;
   int idAtUnidade;
   int idAtSala;
   int idAtDiaSemana;
   int idAtTurno;
   int idAtHorario;
   int idAtOferta;
   bool swap;

   void reset( void );
};

#endif