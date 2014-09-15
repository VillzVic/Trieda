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
   this->sala = NULL;
   this->horarioAula = NULL;
   this->dia = -1;
   this->unidade = NULL;
   this->campus = NULL;
   this->aluno = NULL;
   this->oferta = NULL;
   this->contrato = NULL;
   faseDoDia_ = -1;
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
   this->sala = var.getSala();
   this->horarioAula = var.getHorarioAula();
   this->dia = var.getDia();
   this->unidade = var.getUnidade();
   this->campus = var.getCampus();
   this->aluno = var.getAluno();
   this->oferta = var.getOferta();
   this->contrato = var.getContrato();
   this->faseDoDia_ = var.getFaseDoDia();

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
		if( this->getHorario()->getDia() < var.getHorario()->getDia() )
			return true;
		else if( this->getHorario()->getDia() > var.getHorario()->getDia() )
			return false;

		if( this->getHorario()->getHorarioAula()->comparaMenor( *var.getHorario()->getHorarioAula() ) )
			return true;
		else if( var.getHorario()->getHorarioAula()->comparaMenor( *this->getHorario()->getHorarioAula() ) )
			return false;


		//if( *this->getHorario() < *var.getHorario() )
		//	return true;
		//else if( *var.getHorario() < *this->getHorario() )
		//	return false;
	}

   if( this->getHorarioAula() == NULL && var.getHorarioAula() != NULL )
		return true;
	else if( this->getHorarioAula() != NULL && var.getHorarioAula() == NULL )
		return false;
	else if( this->getHorarioAula() != NULL && var.getHorarioAula() != NULL )
	{
		if( this->getHorarioAula()->comparaMenor( *var.getHorarioAula() ) )
			return true;
		else if( var.getHorarioAula()->comparaMenor( *this->getHorarioAula() ) )
			return false;

		//if( *this->getHorarioAula() < *var.getHorarioAula() )
		//	return true;
		//else if( *var.getHorarioAula() < *this->getHorarioAula() )
		//	return false;
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

   if( this->getAluno() == NULL && var.getAluno() != NULL )
		return true;
	else if( this->getAluno() != NULL && var.getAluno() == NULL )
		return false;
	else if( this->getAluno() != NULL && var.getAluno() != NULL )
	{
		if( *this->getAluno() < *var.getAluno() )
			return true;
		else if( *var.getAluno() < *this->getAluno() )
			return false;
	}


   if( this->getOferta() == NULL && var.getOferta() != NULL )
		return true;
	else if( this->getOferta() != NULL && var.getOferta() == NULL )
		return false;
	else if( this->getOferta() != NULL && var.getOferta() != NULL )
	{
		if( *this->getOferta() < *var.getOferta() )
			return true;
		else if( *var.getOferta() < *this->getOferta() )
			return false;
	}
	
	if( this->getContrato() == NULL && var.getContrato() != NULL )
		return true;
	else if( this->getContrato() != NULL && var.getContrato() == NULL )
		return false;
	else if( this->getContrato() != NULL && var.getContrato() != NULL )
	{
		if( *this->getContrato() < *var.getContrato() )
			return true;
		else if( *var.getContrato() < *this->getContrato() )
			return false;
	}

   if (this->getFaseDoDia() < var.getFaseDoDia()) 
	   return true;
   if (this->getFaseDoDia() > var.getFaseDoDia())
	   return false;

   return false;
}

bool VariableOp::operator == ( const VariableOp & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariableOp::toString() const
{
   std::stringstream str( "" );
   std::string output;

   switch ( type )
   {
     case V_X_PROF_AULA_HOR:
        str << "x"; break;
     case V_Y_PROF_DISCIPLINA:
        str << "y"; break;
     case V_Z_DISCIPLINA_HOR:
        str << "z"; break;
      case V_PROF_CURSO:
        str << "w"; break;
      case V_F_MIN_MEST_CURSO:
        str << "fmm"; break;
      case V_F_MIN_DOUT_CURSO:
        str << "fmd"; break;
      case V_F_CARGA_HOR_MIN_PROF:
        str << "fchmp"; break;
      case V_F_CARGA_HOR_MIN_PROF_SEMANA:
        str << "fchip"; break;
      case V_F_CARGA_HOR_MAX_PROF_SEMANA:
        str << "fchsp"; break;
      case V_DIAS_PROF_MINISTRA_AULAS:
        str << "pt"; break;
      case V_CUSTO_CORPO_DOCENTE: // NÃO ESTÁ SENDO MAIS USADA; PODE DELETAR SE FOR SUFICIENTE O CUSTO EM Y
        str << "ccd"; break;
      case V_DISC_PROF_CURSO:
        str << "dpc"; break;
      case V_F_MAX_DISC_PROF_CURSO:
        str << "fmdpc"; break;
      case V_AVALIACAO_CORPO_DOCENTE:// NÃO ESTÁ SENDO MAIS USADA; PODE DELETAR SE FOR SUFICIENTE O CUSTO EM Y
        str << "acd"; break; 
      case V_F_ULTIMA_PRIMEIRA_AULA_PROF:
        str << "fupap"; break;
	  case V_FOLGA_DEMANDA:
        str << "fd"; break;
	  case V_FOLGA_DISCIPLINA_HOR:
        str << "fh"; break;
	  case V_HI_PROFESSORES:
        str << "hip"; break;
	  case V_HF_PROFESSORES:
        str << "hfp"; break;		
  	  case V_NRO_PROFS_CURSO:
        str << "np"; break;		
  	  case V_NRO_PROFS_VIRTUAIS_CURSO:
        str << "npv"; break;					
	  case V_NRO_PROFS_VIRTUAIS_MEST_CURSO:
        str << "npvm"; break;					
	  case V_NRO_PROFS_VIRTUAIS_DOUT_CURSO:
        str << "npvd"; break;					
	  case V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO:
        str << "npvg"; break;					
	  case V_PROF_VIRTUAL:
        str << "pv"; break;											
	  case V_FOLGA_ALUNO_MIN_ATEND:
        str << "fmd"; break;													
	  case V_DISC_PROF_OFERTA:
        str << "dpo"; break;	
	  case V_FOLGA_GAP_PROF:
        str <<"fpgap"; break;												
		

      default:
        str << "!";
   }

   str << "_{";

   if ( professor != NULL )
   {
	   if ( professor->eVirtual() )
		   str << "_ProfVirt" << professor->getId() <<"_Tit"<< professor->titulacao->getId() << "_Cont" << professor->tipo_contrato->getId();
	   else
		   str << "_Prof" << professor->getId();
   }

   if ( curso != NULL )
   {
      str << "_Curso" << curso->getId();
   }

   if ( aula != NULL )
   {
      str << "_(i" << aula->getTurma()
		  << ",Disc" << aula->getDisciplina()->getId()
          << ",Dia" << aula->getDiaSemana()
          << ",Sala" << aula->getSala()->getId()
          << ",NCred" << aula->getTotalCreditos();

	   ITERA_GGROUP_LESSPTR( itOft, aula->ofertas, Oferta )
	   {
		//	str << ",O" << (*itOft)->getId();
			GGroup<Disciplina*, LessPtr<Disciplina>> discsOrig = aula->getDisciplinasSubstituidas(*itOft);
			ITERA_GGROUP_LESSPTR( itDiscOrig, discsOrig, Disciplina )
			{
				Disciplina* original = *itDiscOrig;
				if ( original->getId() != aula->getDisciplina()->getId() )
				{
				//	str << "_D" << original->getId();
				}
			}
	   }

	  str << ")";
   }

   if ( h != NULL )
   {
	   str << "_H" << h->getHorarioAulaId() << "_Dt" << h->getHorarioAula()->getInicio().getHour() << "_" << h->getHorarioAula()->getInicio().getMinute();
	   if ( aula == NULL) str << "," << "Dia" << h->getDia();
	   
   }

   if ( horarioAula != NULL && h == NULL )
   {
      str << "_H" << horarioAula->getId() << "_Dt" << horarioAula->getInicio().getHour() << "_" << horarioAula->getInicio().getMinute();
   }

   if ( dia >= 0 && aula == NULL )
   {
      str << "_Dia" << dia;
   }

   if ( disciplina != NULL && aula == NULL )
   {
      str << "_Disc" << disciplina->getId();
   }

   if ( turma >= 0 && aula == NULL )
   {
      str << "_i" << turma;
   }
   
   if ( campus != NULL )
   {
      str << "_Cp:" << campus->getId();
   }

   if ( unidade != NULL && aula == NULL  )
   {
      str << "_Unid:" << unidade->getId();
   }

   if ( sala != NULL && aula == NULL )
   {
      str << "_Sala" << sala->getId();
   }
   
   if ( aluno != NULL )
   {
      str << "_Aluno" << aluno->getAlunoId();
   }

   if ( oferta != NULL )
   {
      str << "_Oft" << oferta->getId();
   }
   
   if ( contrato != NULL )
   {
	   str << "_" << contrato->getNome();
   }

   str << "}";

   str >> output;
   return output;
}
