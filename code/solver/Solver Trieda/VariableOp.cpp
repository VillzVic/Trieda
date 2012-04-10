#include "VariableOp.h"

#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )

VariableOp::VariableOp()
{
   reset();
}

VariableOp::VariableOp( const VariableOp & var )
{
   *this = var;
}

void VariableOp::reset()
{
   this->type = VariableOp::V_ERROR;
   this->value = 0;
   this->curso = NULL;
   this->h = NULL;
   this->aula = NULL;
   this->professor = NULL;
   this->turma = -1;
   this->disciplina = NULL;
   this->horarioDiaD = NULL;
   this->horarioDiaD1 = NULL;
   this->sala = NULL;
   this->horarioAula = NULL;
   this->dia = -1;
   this->h1 = NULL;
   this->h2 = NULL;
   this->unidade = NULL;
   this->campus = NULL;
}

VariableOp::~VariableOp()
{
   reset();
}

VariableOp& VariableOp::operator = ( const VariableOp & var )
{
   this->value = var.getValue();
   this->type = var.getType();

   this->curso = var.curso;
   this->aula = var.getAula();
   this->professor = var.getProfessor();
   this->h = var.getHorario();
   this->turma = var.getTurma();
   this->disciplina = var.getDisciplina();
   this->horarioDiaD = var.getHorarioDiaD();
   this->horarioDiaD1 = var.getHorarioDiaD1();
   this->sala = var.getSala();
   this->horarioAula = var.getHorarioAula();
   this->dia = var.getDia();
   this->h1 = var.getH1();
   this->h2 = var.getH2();
   this->unidade = var.getUnidade();
   this->campus = var.getCampus();

   return ( *this );
}

bool VariableOp::operator < ( const VariableOp & var ) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;

   if( this->getCurso() == NULL && var.getCurso() != NULL )
		return true;
	else if( this->getCurso() != NULL && var.getCurso() == NULL )
		return false;
	else if( this->getCurso() != NULL && var.getCurso() != NULL )
	{
		if( *this->getCurso() < *var.getCurso() )
			return true;
		else if( *var.getCurso() < *this->getCurso() )
			return false;
	}

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

   if( this->getHorarioAula() == NULL && var.getHorarioAula() != NULL )
		return true;
	else if( this->getHorarioAula() != NULL && var.getHorarioAula() == NULL )
		return false;
	else if( this->getHorarioAula() != NULL && var.getHorarioAula() != NULL )
	{
		if( *this->getHorarioAula() < *var.getHorarioAula() )
			return true;
		else if( *var.getHorarioAula() < *this->getHorarioAula() )
			return false;
	}

   if( (int)this->getDia() < (int) var.getDia() )
      return true;
   else if( (int)this->getDia() > (int) var.getDia() )
      return false;

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

   if ( (int)this->getTurma() < (int) var.getTurma() )
      return true;
   else if ( (int)this->getTurma() > (int) var.getTurma() )
      return false;

   if ( E_MENOR( this->getHorarioDiaD(), var.getHorarioDiaD() ) )
   {
      return true;
   }
   else if ( E_MENOR( var.getHorarioDiaD(), this->getHorarioDiaD() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getHorarioDiaD1(), var.getHorarioDiaD1() ) )
   {
      return true;
   }
   else if ( E_MENOR( var.getHorarioDiaD1(), this->getHorarioDiaD1() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getH1(), var.getH1() ) )
   {
      return true;
   }
   else if ( E_MENOR( var.getH1(), this->getH1() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getH2(), var.getH2() ) )
   {
      return true;
   }
   else if ( E_MENOR( var.getH2(), this->getH2() ) )
   {
      return false;
   }

   if( this->getUnidade() == NULL && var.getUnidade() != NULL )
		return true;
	else if( this->getUnidade() != NULL && var.getUnidade() == NULL )
		return false;
	else if( this->getUnidade() != NULL && var.getUnidade() != NULL )
	{
		if( *this->getUnidade() < *var.getUnidade() )
			return true;
		else if( *var.getUnidade() < *this->getUnidade() )
			return false;
	}

   if( this->getCampus() == NULL && var.getCampus() != NULL )
		return true;
	else if( this->getCampus() != NULL && var.getCampus() == NULL )
		return false;
	else if( this->getCampus() != NULL && var.getCampus() != NULL )
	{
		if( *this->getCampus() < *var.getCampus() )
			return true;
		else if( *var.getCampus() < *this->getCampus() )
			return false;
	}

   return false;
}

bool VariableOp::operator == ( const VariableOp & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariableOp::toString()
{
   std::stringstream str( "" );
   std::string output;

   switch ( type )
   {
     case V_X_PROF_AULA_HOR:
        str << "X"; break;
     case V_Y_PROF_DISCIPLINA:
        str << "Y"; break;
     case V_Z_DISCIPLINA_HOR:
        str << "Z"; break;
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
      case V_F_DISC_HOR:
        str << "F_DISC_HOR"; break;
      case V_PROF_CURSO:
        str << "V_PROF_CURSO"; break;
      case V_F_MIN_MEST_CURSO:
        str << "V_F_MIN_MEST_CURSO"; break;
      case V_F_MIN_DOUT_CURSO:
        str << "V_F_MIN_DOUT_CURSO"; break;
      case V_F_CARGA_HOR_MIN_PROF:
        str << "V_F_CARGA_HOR_MIN_PROF"; break;
      case V_F_CARGA_HOR_MIN_PROF_SEMANA:
        str << "V_F_CARGA_HOR_MIN_PROF_SEMANA"; break;
      case V_F_CARGA_HOR_MAX_PROF_SEMANA:
        str << "V_F_CARGA_HOR_MAX_PROF_SEMANA"; break;
      case V_DIAS_PROF_MINISTRA_AULAS:
        str << "V_DIAS_PROF_MINISTRA_AULAS"; break;
      case V_CUSTO_CORPO_DOCENTE:
        str << "V_CUSTO_CORPO_DOCENTE"; break;
      case V_MAX_DISC_PROF_CURSO:
        str << "V_MAX_DISC_PROF_CURSO"; break;
      case V_F_MAX_DISC_PROF_CURSO:
        str << "V_F_MAX_DISC_PROF_CURSO"; break;
      case V_AVALIACAO_CORPO_DOCENTE:
        str << "V_AVALIACAO_CORPO_DOCENTE"; break;
      case V_F_ULTIMA_PRIMEIRA_AULA_PROF:
        str << "V_F_ULTIMA_PRIMEIRA_AULA_PROF"; break;
      case V_GAPS_PROFESSORES:
        str << "V_GAPS_PROFESSORES"; break;		
	  case V_FOLGA_DEMANDA:
        str << "V_FOLGA_DEMANDA"; break;
	  case V_FOLGA_DISCIPLINA_HOR:
        str << "V_FOLGA_DISCIPLINA_HOR"; break;
		
      default:
        str << "!";
   }

   str << "_{";

   if ( professor != NULL )
   {
      str << "_Prof" << professor->getIdOperacional();
   }

   if ( curso != NULL )
   {
      str << "_Curso" << curso->getId();
   }

   if ( aula != NULL )
   {
      str << "_(Turma" << aula->getTurma()
		  << ",Disc" << aula->getDisciplina()->getId()
          << ",Dia" << aula->getDiaSemana()
          << ",Sala" << aula->getSala()->getId()
          << ",NCred" << aula->getTotalCreditos();
	  if ( aula->getDisciplinaSubstituida() != NULL )
		  str  << ",DiscAntiga" << aula->getDisciplinaSubstituida()->getId();
	  str << ")";
   }

   if ( h != NULL )
   {
	   str << "_H" << h->getHorarioAulaId();
	   if ( aula == NULL) str << "," << "Dia" << h->getDia();
	   
   }

   if ( horarioAula != NULL && h == NULL )
   {
      str << "_H" << horarioAula->getId();
   }

   if ( dia >= 0 && aula == NULL )
   {
      str << "_Dia" << dia;
   }

   if ( disciplina != NULL && aula == NULL )
   {
      str << "_Disc" << disciplina->getId();
   }

   if ( turma >= 0 )
   {
      str << "_Turma" << turma;
   }
   
   if ( campus != NULL )
   {
      str << "_Cp:" << campus->getId();
   }

   if ( unidade != NULL )
   {
      str << "_Unid:" << unidade->getId();
   }

   if ( sala != NULL && aula == NULL )
   {
      str << "_Sala" << sala->getId();
   }

   if ( h1 != NULL )
   {
      str << "_H1:" << h1->getId();
   }

   if ( h2 != NULL )
   {
      str << "_H2:" << h2->getId();
   }
   
   str << "}";

   str >> output;
   return output;
}
