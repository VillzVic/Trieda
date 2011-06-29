#include "ConstraintOp.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL ) || \
   ( b != NULL && a != NULL && ( *a < *b ) )

ConstraintOp::ConstraintOp()
{
   reset();
}

ConstraintOp::ConstraintOp( const ConstraintOp & cons )
{
   *this = cons;
}

ConstraintOp::~ConstraintOp()
{
   reset();
}

ConstraintOp & ConstraintOp::operator = ( const ConstraintOp & cons )
{   
   this->type = cons.getType();

   this->curso = cons.curso;
   this->aula = cons.getAula();
   this->professor = cons.getProfessor();
   this->s = cons.getSala();
   this->b = cons.getBloco();
   this->j = cons.getSubBloco();
   this->t = cons.getDia();
   this->h = cons.getHorario();
   this->disciplina = cons.getDisciplina();
   this->turma = cons.getTurma();
   this->horarioAula = cons.getHorarioAula();

   return *this;
}

bool ConstraintOp::operator < ( const ConstraintOp & cons ) const
{
   if( (int)this->getType() < (int) cons.getType() )
      return true;
   else if( (int)this->getType() > (int) cons.getType() )
      return false;

   if( this->getCurso() == NULL && cons.getCurso() != NULL )
		return true;
	else if( this->getCurso() != NULL && cons.getCurso() == NULL )
		return false;
	else if( this->getCurso() != NULL && cons.getCurso() != NULL )
	{
		if( *this->getCurso() < *cons.getCurso() )
			return true;
		else if( *cons.getCurso() < *this->getCurso() )
			return false;
	}

   if( this->getAula() == NULL && cons.getAula() != NULL )
		return true;
	else if( this->getAula() != NULL && cons.getAula() == NULL )
		return false;
	else if( this->getAula() != NULL && cons.getAula() != NULL )
	{
		if( *this->getAula() < *cons.getAula() )
			return true;
		else if( *cons.getAula() < *this->getAula() )
			return false;
	}

   if( this->getProfessor() == NULL && cons.getProfessor() != NULL )
		return true;
	else if( this->getProfessor() != NULL && cons.getProfessor() == NULL )
		return false;
	else if( this->getProfessor() != NULL && cons.getProfessor() != NULL )
	{
		if( *this->getProfessor() < *cons.getProfessor() )
			return true;
		else if( *cons.getProfessor() < *this->getProfessor() )
			return false;
	}

   if( this->getSala() == NULL && cons.getSala() != NULL )
		return true;
	else if( this->getSala() != NULL && cons.getSala() == NULL )
		return false;
	else if( this->getSala() != NULL && cons.getSala() != NULL )
	{
		if( *this->getSala() < *cons.getSala() )
			return true;
		else if( *cons.getSala() < *this->getSala() )
			return false;
	}

   if( this->getBloco() == NULL && cons.getBloco() != NULL )
		return true;
	else if( this->getBloco() != NULL && cons.getBloco() == NULL )
		return false;
	else if( this->getBloco() != NULL && cons.getBloco() != NULL )
	{
		if( *this->getBloco() < *cons.getBloco() )
			return true;
		else if( *cons.getBloco() < *this->getBloco() )
			return false;
	}

   if( this->getHorario() == NULL && cons.getHorario() != NULL )
		return true;
	else if( this->getHorario() != NULL && cons.getHorario() == NULL )
		return false;
	else if( this->getHorario() != NULL && cons.getHorario() != NULL )
	{
		if( *this->getHorario() < *cons.getHorario() )
			return true;
		else if( *cons.getHorario() < *this->getHorario() )
			return false;
	}

   if( this->getHorarioAula() == NULL && cons.getHorarioAula() != NULL )
		return true;
	else if( this->getHorarioAula() != NULL && cons.getHorarioAula() == NULL )
		return false;
	else if( this->getHorarioAula() != NULL && cons.getHorarioAula() != NULL )
	{
		if( *this->getHorarioAula() < *cons.getHorarioAula() )
			return true;
		else if( *cons.getHorarioAula() < *this->getHorarioAula() )
			return false;
	}

   if( this->getDisciplina() == NULL && cons.getDisciplina() != NULL )
		return true;
	else if( this->getDisciplina() != NULL && cons.getDisciplina() == NULL )
		return false;
	else if( this->getDisciplina() != NULL && cons.getDisciplina() != NULL )
	{
		if( *this->getDisciplina() < *cons.getDisciplina() )
			return true;
		else if( *cons.getDisciplina() < *this->getDisciplina() )
			return false;
	}

   if( (int)this->getTurma() < (int) cons.getTurma() )
      return true;
   else if( (int)this->getTurma() > (int) cons.getTurma() )
      return false;

   if( (int)this->getDia() < (int) cons.getDia() )
      return true;
   else if( (int)this->getDia() > (int) cons.getDia() )
      return false;

   if( (int)this->getSubBloco() < (int) cons.getSubBloco() )
      return true;
   else if( (int)this->getSubBloco() > (int) cons.getSubBloco() )
      return false;

   return false;
}

bool ConstraintOp::operator == ( const ConstraintOp & cons ) const
{
   return ( !( *this < cons ) && !( cons < *this ) );
}

void ConstraintOp::reset()
{
   curso = NULL;
   aula = NULL;
   professor = NULL;
   s = NULL;
   b = NULL;
   j = -1;
   t = -1;
   h = NULL;
   disciplina = NULL;
   turma = -1;
   horarioAula = NULL;
   type = C_PROFESSOR_HORARIO;
}

std::string ConstraintOp::toString()
{
   std::stringstream ss;

   switch( type )
   {
   case C_SALA_HORARIO:
      ss << "C_SALA_HORARIO"; break;
   case C_PROFESSOR_HORARIO:
      ss << "C_PROFESSOR_HORARIO"; break;
   case C_BLOCO_HORARIO:
      ss << "C_BLOCO_HORARIO"; break;
   case C_ALOC_AULA:
      ss << "C_ALOC_AULA"; break;
   case C_PROF_DISC:
      ss << "C_PROF_DISC"; break;
   case C_PROF_DISC_UNI:
      ss << "C_PROF_DISC_UNI"; break;
   case C_FIX_PROF_DISC_SALA_DIA_HOR:
      ss << "C_FIX_PROF_DISC_SALA_DIA_HOR"; break;
   case C_FIX_PROF_DISC_DIA_HOR:
      ss << "C_FIX_PROF_DISC_DIA_HOR"; break;
   case C_FIX_PROF_DISC:
      ss << "C_FIX_PROF_DISC"; break;
   case C_FIX_PROF_DISC_SALA:
      ss << "C_FIX_PROF_DISC_SALA"; break;
   case C_FIX_PROF_SALA:
      ss << "C_FIX_PROF_SALA"; break;
   case C_DISC_HORARIO:
      ss << "C_DISC_HORARIO"; break;
   case C_DISC_HORARIO_UNICO:
      ss << "C_DISC_HORARIO_UNICO"; break;
   case C_MIN_MEST_CURSO:
      ss << "C_MIN_MEST_CURSO"; break;
   case C_MIN_DOUT_CURSO:
      ss << "C_MIN_DOUT_CURSO"; break;
   case C_ALOC_PROF_CURSO_LESS:
      ss << "C_ALOC_PROF_CURSO_LESS"; break;
   case C_ALOC_PROF_CURSO_GREATER:
      ss << "C_ALOC_PROF_CURSO_GREATER"; break;
   case C_CARGA_HOR_MIN_PROF:
      ss << "C_CARGA_HOR_MIN_PROF"; break;
   case C_CARGA_HOR_MIN_PROF_SEMANA:
      ss << "C_CARGA_HOR_MIN_PROF_SEMANA"; break;
   case C_CARGA_HOR_MAX_PROF_SEMANA:
      ss << "C_CARGA_HOR_MAX_PROF_SEMANA"; break;
   case C_DIAS_PROF_MINISTRA_AULA:
      ss << "C_DIAS_PROF_MINISTRA_AULA"; break;
   case C_CUSTO_CORPO_DOCENTE:
      ss << "C_CUSTO_CORPO_DOCENTE"; break;
   case C_MAX_DISC_PROF_CURSO:
      ss << "C_MAX_DISC_PROF_CURSO"; break;
   case C_AVALIACAO_CORPO_DOCENTE:
      ss << "C_AVALIACAO_CORPO_DOCENTE"; break;
   case C_PREF_DISCIPLINAS:
      ss << "C_PREF_DISCIPLINAS"; break;
   default:
      ss << "!";
   }

   std::string consName = "";
   ss >> consName;

   return consName;
}

/*
size_t ConstraintOpHasher::operator() ( const ConstraintOp & cons ) const
{
   unsigned int sum = 0;

   if ( cons.getCurso() != NULL) 
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getCurso()->getId() );
   }

   if ( cons.getAula() != NULL )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getAula()->getSala()->getId() );
   }
   
   if ( cons.getProfessor() != NULL )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getProfessor()->getId() );
   }
   
   if ( cons.getSala() != NULL)
   {
      sum *= HASH_PRIME;
      sum+= intHash( cons.getSala()->getId() );
   }

   if ( cons.getBloco() != NULL )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getBloco()->getId() );
   }

   if ( cons.getDisciplina() != NULL )
   {
      sum *= HASH_PRIME;
      sum+= intHash( cons.getDisciplina()->getId() );
   }

   if ( cons.getTurma() >= 0 )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getTurma() );
   }
 
   if ( cons.getSubBloco() >= 0 )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getSubBloco() );
   }

   if ( cons.getDia() >= 0 )
   {
      sum *= HASH_PRIME;
      sum += intHash( cons.getDia() );
   }

   if ( cons.getHorario() != NULL ) 
   {
      sum *= HASH_PRIME; 
      sum += intHash( cons.getHorario()->getId() );
   }

   if ( cons.getHorarioAula()!=NULL )
   {
      sum *= HASH_PRIME; 
      sum += intHash( cons.getHorarioAula()->getId() );
   }

   return sum;
}

bool ConstraintOpHasher::operator() ( const ConstraintOp & cons1, const ConstraintOp & cons2 ) const
{
   return ( cons1 < cons2 );
}
*/


