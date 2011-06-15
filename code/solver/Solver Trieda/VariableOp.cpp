#include "VariableOp.h"

#include "HashUtil.h"

#define E_MENOR(a,b) \
   (a == NULL && b != NULL) || \
   (b != NULL && a != NULL && (*a < *b))

VariableOp::VariableOp()
{
   reset();
}

VariableOp::VariableOp(const VariableOp& var)
{
   *this = var;
}

void VariableOp::reset()
{
   value = 0;
   h = NULL;
   aula = NULL;
   professor = NULL;
   turma = -1;
   disciplina = NULL;
   sala = NULL;
}

VariableOp::~VariableOp()
{
   reset();
}

VariableOp& VariableOp::operator = ( const VariableOp & var )
{
   this->value = var.getValue();
   this->type = var.getType();

   this->aula = var.getAula();
   this->professor = var.getProfessor();
   this->h = var.getHorario();
   this->turma = var.getTurma();
   this->disciplina = var.getDisciplina();
   this->sala = var.getSala();

   return *this;
}

bool VariableOp::operator <(const VariableOp& var) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;

   if( this->getHorario() == NULL && var.getHorario() != NULL )
		return true;
	else if( this->getHorario() != NULL && var.getHorario() == NULL )
		return false;
	else if( this->getHorario() != NULL && var.getHorario() != NULL )
	{
		if( *this->getHorario() < *var.getHorario() )
			return true;
		else if( *var.getHorario() < *this->getHorario() )
			return false;
	}

   if( this->getAula() == NULL && var.getAula() != NULL )
		return true;
	else if( this->getAula() != NULL && var.getAula() == NULL )
		return false;
	else if( this->getAula() != NULL && var.getAula() != NULL )
	{
		if( *this->getAula() < *var.getAula() )
			return true;
		else if( *var.getAula() < *this->getAula() )
			return false;
	}

   if( this->getProfessor() == NULL && var.getProfessor() != NULL )
		return true;
	else if( this->getProfessor() != NULL && var.getProfessor() == NULL )
		return false;
	else if( this->getProfessor() != NULL && var.getProfessor() != NULL )
	{
		if( *this->getProfessor() < *var.getProfessor() )
			return true;
		else if( *var.getProfessor() < *this->getProfessor() )
			return false;
	}

   if( this->getDisciplina() == NULL && var.getDisciplina() != NULL )
		return true;
	else if( this->getDisciplina() != NULL && var.getDisciplina() == NULL )
		return false;
	else if( this->getDisciplina() != NULL && var.getDisciplina() != NULL )
	{
		if( *this->getDisciplina() < *var.getDisciplina() )
			return true;
		else if( *var.getDisciplina() < *this->getDisciplina() )
			return false;
	}

   if( this->getSala() == NULL && var.getSala() != NULL )
		return true;
	else if( this->getSala() != NULL && var.getSala() == NULL )
		return false;
	else if( this->getSala() != NULL && var.getSala() != NULL )
	{
		if( *this->getSala() < *var.getSala() )
			return true;
		else if( *var.getSala() < *this->getSala() )
			return false;
	}

   if( (int)this->getTurma() < (int) var.getTurma() )
      return true;
   else if( (int)this->getTurma() > (int) var.getTurma() )
      return false;

   return false;
}

bool VariableOp::operator ==(const VariableOp& var) const
{
   return (!(*this < var) && !(var < *this));
}

std::string VariableOp::toString()
{
   std::stringstream str("");
   std::string output;

   bool cond_disc = false;

   switch(type) {
     case V_X_PROF_AULA_HOR:
        str << "X"; break;
     case V_Y_PROF_DISCIPLINA:
        str << "Y"; break;
     case V_F_FIX_PROF_DISC_SALA_DIA_HOR:
        str << "F_FIX_PROF_DISC_SALA_DIA_HOR"; break;
     case V_F_FIX_PROF_DISC_DIA_HOR:
        str << "F_FIX_PROF_DISC_DIA_HOR"; break;
      case V_F_FIX_PROF_DISC:
        str << "F_FIX_PROF_DISC"; break;
      case V_F_FIX_PROF_DISC_SALA:
        str << "F_FIX_PROF_DISC_SALA"; break;
      case V_F_FIX_PROF_SALA:
        str << "F_FIX_PROF_SALA"; break;
    default:
        str << "!";
   }

   if ( professor != NULL )
   {
      str << "_" << professor->getIdOperacional();
   }

   if ( aula != NULL )
   {
      str << "_(" << aula->getDisciplina()->getId() <<","<<aula->getDiaSemana() << "," << aula->getSala()->getId() << "," <<aula->getTotalCreditos() << ")";
   }

   if (h != NULL) 
   {
      str << "_(" << h->getDia() <<"," << h->getHorarioAulaId() << ")";
   }

   if ( disciplina != NULL )
   {
      str << "_(" << disciplina->getId() << ")";
   }

   if ( turma >= 0 )
   {
      str << "_(" << turma << ")";
   }

   if ( sala != NULL )
   {
      str << "_(" << sala->getId() << ")";
   }

   str >> output;
   return output;
}

bool VariableOpHasher::operator()(const VariableOp& v1, const VariableOp& v2) const
{
   return (v1 < v2);
}

size_t VariableOpHasher::operator()(const VariableOp& v) const
{
   unsigned int sum = 0;

   if(v.getAula()) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getAula()->getSala()->getId());
   }
   if(v.getHorario()) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getHorario()->getId());
   }
   if(v.getProfessor()) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getProfessor()->getId());
   }

   if(v.getDisciplina()) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getDisciplina()->getId());
   }

   if(v.getSala()) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getSala()->getId());
   }

   if ( v.getTurma() >= 0 ) 
   {
      sum *= HASH_PRIME; sum+= intHash(v.getTurma());
   }

   return sum;
}

