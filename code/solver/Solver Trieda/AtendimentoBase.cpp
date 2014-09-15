#include "AtendimentoBase.h"

#ifndef E_MENOR
#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )
#endif

AtendimentoBase::AtendimentoBase( void )
{
   this->reset();
}

AtendimentoBase::~AtendimentoBase( void )
{
   this->reset();
}

void AtendimentoBase::reset( void )
{
   this->professor = NULL;
   this->campus = NULL;
   this->unidade = NULL;
   this->sala = -NULL;
   this->turno = NULL;
   this->dia_semana = -1;
   this->horario_aula = NULL;
   this->turma = -1;
   this->disciplina = NULL;
   this->quantidade = -1;

   this->idAtCampus = -1;
   this->idAtUnidade = -1;
   this->idAtSala = -1;
   this->idAtDiaSemana = -1;
   this->idAtTurno = -1;
   this->idAtHorario = -1;
   this->idAtOferta = -1;
   this->swap = false;
}

bool AtendimentoBase::operator < ( AtendimentoBase const & right ) const
{
   if ( E_MENOR( this->campus, right.campus ) ) return true;
   if ( E_MENOR( right.campus, this->campus ) ) return false;

   if ( E_MENOR( this->unidade, right.unidade ) ) return true;
   if ( E_MENOR( right.unidade, this->unidade ) ) return false;

   if ( E_MENOR( this->sala, right.sala ) ) return true;
   if ( E_MENOR( right.sala, this->sala ) ) return false;

   if ( E_MENOR( this->turno, right.turno ) ) return true;
   if ( E_MENOR( right.turno, this->turno ) ) return false;

   if ( this->dia_semana < right.dia_semana ) return true;
   if ( this->dia_semana > right.dia_semana ) return false;

   if ( E_MENOR( this->disciplina, right.disciplina ) ) return true;
   if ( E_MENOR( right.disciplina, this->disciplina ) ) return false;
   
   if ( E_MENOR( this->horario_aula, right.horario_aula ) ) return true;
   if ( E_MENOR( right.horario_aula, this->horario_aula ) ) return false;

   if ( this->turma < right.turma ) return true;
   if ( this->turma > right.turma ) return false;

   if ( this->quantidade < right.quantidade ) return true;
   if ( this->quantidade > right.quantidade ) return false;

   if ( E_MENOR( this->professor, right.professor ) ) return true;
   if ( E_MENOR( right.professor, this->professor ) ) return false;

   return false;
}
