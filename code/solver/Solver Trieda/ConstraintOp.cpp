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
   this->h1 = cons.getH1();
   this->h2 = cons.getH2();
   this->horarioDiaD = cons.horarioDiaD;
   this->horarioDiaD1 = cons.horarioDiaD1;
   this->par1_aula_hor = cons.par1_aula_hor;
   this->par2_aula_hor = cons.par2_aula_hor;
   this->campus = cons.getCampus();
   this->unidade = cons.getUnidade();
   this->duracaoAula = cons.getDuracaoAula();
   this->aluno = cons.getAluno();
   this->cpT = cons.getCampusT();
   this->cpP = cons.getCampusP();
   this->turmaT = cons.getTurmaT();
   this->turmaP = cons.getTurmaP();
   this->dti = cons.getDateTimeInicial();
   this->dtf = cons.getDateTimeFinal();
   this->oferta = cons.getOferta();
   this->periodo = cons.getPeriodo();
   this->contrato = cons.getContrato();
   this->faseDoDia_ = cons.getFaseDoDia();

   return *this;
}

bool ConstraintOp::operator < ( const ConstraintOp & cons ) const
{
   if( (int)this->getType() < (int) cons.getType() )
   {
      return true;
   }
   else if( (int)this->getType() > (int) cons.getType() )
   {
      return false;
   }


   if( this->getCurso() == NULL && cons.getCurso() != NULL )
   {
		return true;
   }
	else if( this->getCurso() != NULL && cons.getCurso() == NULL )
   {
		return false;
   }
	else if( this->getCurso() != NULL && cons.getCurso() != NULL )
	{
		if( *this->getCurso() < *cons.getCurso() )
			return true;
		else if( *cons.getCurso() < *this->getCurso() )
			return false;
	}


   if( this->getAula() == NULL && cons.getAula() != NULL )
   {
		return true;
   }
	else if( this->getAula() != NULL && cons.getAula() == NULL )
   {
		return false;
   }
	else if( this->getAula() != NULL && cons.getAula() != NULL )
	{
		if( *this->getAula() < *cons.getAula() )
			return true;
		else if( *cons.getAula() < *this->getAula() )
			return false;
	}


   if( this->getProfessor() == NULL && cons.getProfessor() != NULL )
   {
		return true;
   }
	else if( this->getProfessor() != NULL && cons.getProfessor() == NULL )
   {
		return false;
   }
	else if( this->getProfessor() != NULL && cons.getProfessor() != NULL )
	{
		if( *this->getProfessor() < *cons.getProfessor() )
			return true;
		else if( *cons.getProfessor() < *this->getProfessor() )
			return false;
	}


   if( this->getSala() == NULL && cons.getSala() != NULL )
   {
		return true;
   }
	else if( this->getSala() != NULL && cons.getSala() == NULL )
   {
		return false;
   }
	else if( this->getSala() != NULL && cons.getSala() != NULL )
	{
		if( *this->getSala() < *cons.getSala() )
			return true;
		else if( *cons.getSala() < *this->getSala() )
			return false;
	}


   if( this->getBloco() == NULL && cons.getBloco() != NULL )
   {
		return true;
   }
	else if( this->getBloco() != NULL && cons.getBloco() == NULL )
   {
		return false;
   }
	else if( this->getBloco() != NULL && cons.getBloco() != NULL )
	{
		if( *this->getBloco() < *cons.getBloco() )
			return true;
		else if( *cons.getBloco() < *this->getBloco() )
			return false;
	}


   if( this->getHorario() == NULL && cons.getHorario() != NULL )
   {
		return true;
   }
	else if( this->getHorario() != NULL && cons.getHorario() == NULL )
   {
		return false;
   }
	else if( this->getHorario() != NULL && cons.getHorario() != NULL )
	{
		if( this->getHorario()->getDia() < cons.getHorario()->getDia() )
			return true;
		else if( this->getHorario()->getDia() > cons.getHorario()->getDia() )
			return false;

		if( this->getHorario()->getHorarioAula()->comparaMenor( *cons.getHorario()->getHorarioAula() ) )
			return true;
		else if( cons.getHorario()->getHorarioAula()->comparaMenor( *this->getHorario()->getHorarioAula() ) )
			return false;		

		//if( *this->getHorario() < *cons.getHorario() )
		//	return true;
		//else if( *cons.getHorario() < *this->getHorario() )
		//	return false;
	}


   if( this->getHorarioAula() == NULL && cons.getHorarioAula() != NULL )
   {
		return true;
   }
	else if( this->getHorarioAula() != NULL && cons.getHorarioAula() == NULL )
   {
		return false;
   }
	else if( this->getHorarioAula() != NULL && cons.getHorarioAula() != NULL )
	{
		if( this->getHorarioAula()->comparaMenor( *cons.getHorarioAula() ) )
			return true;
		else if( cons.getHorarioAula()->comparaMenor( *this->getHorarioAula() ) )
			return false;
		
		//if( *this->getHorarioAula() < *cons.getHorarioAula() )
		//	return true;
		//else if( *cons.getHorarioAula() < *this->getHorarioAula() )
		//	return false;
	}

   if( this->getDisciplina() == NULL && cons.getDisciplina() != NULL )
   {
		return true;
   }
	else if( this->getDisciplina() != NULL && cons.getDisciplina() == NULL )
   {
		return false;
   }
	else if( this->getDisciplina() != NULL && cons.getDisciplina() != NULL )
	{
		if( *this->getDisciplina() < *cons.getDisciplina() )
			return true;
		else if( *cons.getDisciplina() < *this->getDisciplina() )
			return false;
	}


   if( (int)this->getTurma() < (int) cons.getTurma() )
   {
      return true;
   }
   else if( (int)this->getTurma() > (int) cons.getTurma() )
   {
      return false;
   }


   if( this->getHorarioDiaD() == NULL && cons.getHorarioDiaD() != NULL )
   {
		return true;
   }
	else if( this->getHorarioDiaD() != NULL && cons.getHorarioDiaD() == NULL )
   {
		return false;
   }
	else if( this->getHorarioDiaD() != NULL && cons.getHorarioDiaD() != NULL )
	{
		if( this->getHorarioDiaD()->getDia() < cons.getHorarioDiaD()->getDia() )
			return true;
		else if( this->getHorarioDiaD()->getDia() > cons.getHorarioDiaD()->getDia() )
			return false;

		if( this->getHorarioDiaD()->getHorarioAula()->comparaMenor( *cons.getHorarioDiaD()->getHorarioAula() ) )
			return true;
		else if( cons.getHorarioDiaD()->getHorarioAula()->comparaMenor( *this->getHorarioDiaD()->getHorarioAula() ) )
			return false;		
	}
		

   if( this->getHorarioDiaD1() == NULL && cons.getHorarioDiaD1() != NULL )
   {
		return true;
   }
	else if( this->getHorarioDiaD1() != NULL && cons.getHorarioDiaD1() == NULL )
   {
		return false;
   }
	else if( this->getHorarioDiaD1() != NULL && cons.getHorarioDiaD1() != NULL )
	{
		if( this->getHorarioDiaD1()->getDia() < cons.getHorarioDiaD1()->getDia() )
			return true;
		else if( this->getHorarioDiaD1()->getDia() > cons.getHorarioDiaD1()->getDia() )
			return false;

		if( this->getHorarioDiaD1()->getHorarioAula()->comparaMenor( *cons.getHorarioDiaD1()->getHorarioAula() ) )
			return true;
		else if( cons.getHorarioDiaD1()->getHorarioAula()->comparaMenor( *this->getHorarioDiaD1()->getHorarioAula() ) )
			return false;		
	}



   if( (int)this->getDia() < (int) cons.getDia() )
   {
      return true;
   }
   else if( (int)this->getDia() > (int) cons.getDia() )
   {
      return false;
   }


   if( (int)this->getSubBloco() < (int) cons.getSubBloco() )
   {
      return true;
   }
   else if( (int)this->getSubBloco() > (int) cons.getSubBloco() )
   {
      return false;
   }


   if( this->getH1() == NULL && cons.getH1() != NULL )
   {
		return true;
   }
	else if( this->getH1() != NULL && cons.getH1() == NULL )
   {
		return false;
   }
	else if( this->getH1() != NULL && cons.getH1() != NULL )
	{
		if( this->getH1()->comparaMenor( *cons.getH1() ) )
			return true;
		else if( cons.getH1()->comparaMenor( *this->getH1() ) )
			return false;
	}


   if( this->getH2() == NULL && cons.getH2() != NULL )
   {
		return true;
   }
	else if( this->getH2() != NULL && cons.getH2() == NULL )
   {
		return false;
   }
	else if( this->getH2() != NULL && cons.getH2() != NULL )
	{
		if( this->getH2()->comparaMenor( *cons.getH2() ) )
			return true;
		else if( cons.getH2()->comparaMenor( *this->getH2() ) )
			return false;
	}

   
   if ( this->getPar1AulaHor() < cons.getPar1AulaHor() )
   {
      return true;
   }
   else if ( cons.getPar1AulaHor() > this->getPar1AulaHor() )
   {
      return false;
   }
   
   if ( this->getPar2AulaHor() < cons.getPar2AulaHor() )
   {
      return true;
   }
   else if ( cons.getPar2AulaHor() > this->getPar2AulaHor() )
   {
      return false;
   }


   if( this->getCampus() == NULL && cons.getCampus() != NULL )
   {
		return true;
   }
   else if( this->getCampus() != NULL && cons.getCampus() == NULL )
   {
		return false;
   }
	else if( this->getCampus() != NULL && cons.getCampus() != NULL )
	{
		if( *this->getCampus() < *cons.getCampus() )
			return true;
		else if( *cons.getCampus() < *this->getCampus() )
			return false;
	}


   if( this->getUnidade() == NULL && cons.getUnidade() != NULL )
   {
		return true;
   }
   else if( this->getUnidade() != NULL && cons.getUnidade() == NULL )
   {
		return false;
   }
	else if( this->getUnidade() != NULL && cons.getUnidade() != NULL )
	{
		if( *this->getUnidade() < *cons.getUnidade() )
			return true;
		else if( *cons.getUnidade() < *this->getUnidade() )
			return false;
	}


   if( this->getDuracaoAula() < cons.getDuracaoAula() )
   {
      return true;
   }
   else if( this->getDuracaoAula() > cons.getDuracaoAula() )
   {
      return false;
   }


   if( this->getAluno() == NULL && cons.getAluno() != NULL )
   {
		return true;
   }
   else if( this->getAluno() != NULL && cons.getAluno() == NULL )
   {
		return false;
   }
	else if( this->getAluno() != NULL && cons.getAluno() != NULL )
	{
		if( *this->getAluno() < *cons.getAluno() )
			return true;
		else if( *cons.getAluno() < *this->getAluno() )
			return false;
	}

   // ---------------------------------------------------------------
   // Variaveis para restricao de professor unico para disciplinas PT

   if( this->getCampusT() == NULL && cons.getCampusT() != NULL )
   {
		return true;
   }
   else if( this->getCampusT() != NULL && cons.getCampusT() == NULL )
   {
		return false;
   }
	else if( this->getCampusT() != NULL && cons.getCampusT() != NULL )
	{
		if( *this->getCampusT() < *cons.getCampusT() )
			return true;
		else if( *cons.getCampusT() < *this->getCampusT() )
			return false;
	}

   if( this->getCampusP() == NULL && cons.getCampusP() != NULL )
   {
		return true;
   }
   else if( this->getCampusP() != NULL && cons.getCampusP() == NULL )
   {
		return false;
   }
	else if( this->getCampusP() != NULL && cons.getCampusP() != NULL )
	{
		if( *this->getCampusP() < *cons.getCampusP() )
			return true;
		else if( *cons.getCampusP() < *this->getCampusP() )
			return false;
	}

   if( (int)this->getTurmaT() < (int) cons.getTurmaT() )
   {
      return true;
   }
   else if( (int)this->getTurmaT() > (int) cons.getTurmaT() )
   {
      return false;
   }

   if( (int)this->getTurmaP() < (int) cons.getTurmaP() )
   {
      return true;
   }
   else if( (int)this->getTurmaP() > (int) cons.getTurmaP() )
   {
      return false;
   }

   // --------------------------------------------------------------

   if ( E_MENOR( this->getDateTimeInicial(), cons.getDateTimeInicial() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getDateTimeInicial(), this->getDateTimeInicial() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getDateTimeFinal(), cons.getDateTimeFinal() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getDateTimeFinal(), this->getDateTimeFinal() ) )
   {
      return false;
   }
   

   if( this->getOferta() == NULL && cons.getOferta() != NULL )
   {
		return true;
   }
	else if( this->getOferta() != NULL && cons.getOferta() == NULL )
   {
		return false;
   }
	else if( this->getOferta() != NULL && cons.getOferta() != NULL )
	{
		if( *this->getOferta() < *cons.getOferta() )
			return true;
		else if( *cons.getOferta() < *this->getOferta() )
			return false;
	}
	

   if( (int)this->getPeriodo() < (int) cons.getPeriodo() )
   {
      return true;
   }
   else if( (int)this->getPeriodo() > (int) cons.getPeriodo() )
   {
      return false;
   }

   
   if( this->getContrato() == NULL && cons.getContrato() != NULL )
   {
		return true;
   }
	else if( this->getContrato() != NULL && cons.getContrato() == NULL )
   {
		return false;
   }
	else if( this->getContrato() != NULL && cons.getContrato() != NULL )
	{
		if( *this->getContrato() < *cons.getContrato() )
			return true;
		else if( *cons.getContrato() < *this->getContrato() )
			return false;
	}
	
   if (this->getFaseDoDia() < cons.getFaseDoDia()) 
	   return true;
   if (this->getFaseDoDia() > cons.getFaseDoDia())
	   return false;

   return false;
}

bool ConstraintOp::operator == ( const ConstraintOp & cons ) const
{
   return ( !( *this < cons ) && !( cons < *this ) );
}

void ConstraintOp::setPar1AulaHor( Aula* a1, HorarioAula* h1 )
{
	this->par1_aula_hor.first = a1;
	this->par1_aula_hor.second = h1;
}

void ConstraintOp::setPar2AulaHor( Aula* a2, HorarioAula* h2 )
{
	this->par2_aula_hor.first = a2;
	this->par2_aula_hor.second = h2;
}

void ConstraintOp::reset()
{
   this->curso = NULL;
   this->aula = NULL;
   this->professor = NULL;
   this->s = NULL;
   this->b = NULL;
   this->j = -1;
   this->t = -1;
   this->h = NULL;
   this->disciplina = NULL;
   this->turma = -1;
   this->horarioAula = NULL;
   this->type = C_PROFESSOR_HORARIO;
   this->h1 = NULL;
   this->h2 = NULL;
   this->horarioDiaD = NULL;
   this->horarioDiaD1 = NULL;
   this->par1_aula_hor.first=NULL;
   this->par1_aula_hor.second=NULL;
   this->par2_aula_hor.first=NULL;
   this->par2_aula_hor.second=NULL;
   this->campus = NULL;
   this->duracaoAula = 0;
   this->unidade = NULL;
   this->aluno = NULL;
   this->cpT = NULL;
   this->cpP = NULL;
   this->turmaT = -1;
   this->turmaP = -1;
   this->dti = NULL;
   this->dtf = NULL;
   this->oferta = NULL;
   this->periodo = -1;
   this->contrato = NULL;
   this->faseDoDia_ = -1;
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
	   case C_BLOCO_HORARIO_DISC:
		   ss << "C_BLOCO_HORARIO_DISC"; break;
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
	   case C_ALOC_PROF_CURSO1:
		  ss << "C_ALOC_PROF_CURSO1"; break;
	   case C_ALOC_PROF_CURSO2:
		  ss << "C_ALOC_PROF_CURSO2"; break;
	   case C_CARGA_HOR_MIN_PROF:
		  ss << "C_CARGA_HOR_MIN_PROF"; break;
	   case C_CARGA_HOR_MIN_PROF_SEMANA:
		  ss << "C_CARGA_HOR_MIN_PROF_SEMANA"; break;
	   case C_CARGA_HOR_MAX_PROF_SEMANA:
		  ss << "C_CARGA_HOR_MAX_PROF_SEMANA"; break;
	   case C_DIAS_PROF_MINISTRA_AULA_MIN:
		  ss << "C_DIAS_PROF_MINISTRA_AULA_MIN"; break;
	   case C_DIAS_PROF_MINISTRA_AULA_MAX:
		  ss << "C_DIAS_PROF_MINISTRA_AULA_MAX"; break;
	   case C_CUSTO_CORPO_DOCENTE:
		  ss << "C_CUSTO_CORPO_DOCENTE"; break;
	   case C_DISC_PROF_CURSO1:
		  ss << "C_DISC_PROF_CURSO1"; break;		  
	   case C_DISC_PROF_CURSO2:
		  ss << "C_DISC_PROF_CURSO2"; break;		  
	   case C_MAX_DISC_PROF_CURSO:
		  ss << "C_MAX_DISC_PROF_CURSO"; break;
	   case C_AVALIACAO_CORPO_DOCENTE:
		  ss << "C_AVALIACAO_CORPO_DOCENTE"; break;
	   case C_DESLOC_PROF:
		  ss << "C_DESLOC_PROF"; break;
	   case C_DESLOC_VIAVEL:
		  ss << "C_DESLOC_VIAVEL"; break;
	   case C_ULTIMA_PRIMEIRA_AULA_PROF:
		  ss << "C_ULTIMA_PRIMEIRA_AULA_PROF"; break;
	   case C_GAPS_PROFESSORES:
		  ss << "C_GAPS_PROFESSORES"; break;
	   case C_PROF_HORARIO_MULTIUNID:
		   ss << "C_PROF_HORARIO_MULTIUNID"; break;
	   case C_ALUNO_HORARIO:
		   ss <<"C_ALUNO_HORARIO"; break;		   
	   case C_GAPS_PROFESSORES_I:
		   ss <<"C_GAPS_PROFESSORES_I"; break;		   
	   case C_GAPS_PROFESSORES_F:
		   ss <<"C_GAPS_PROFESSORES_F"; break;		   	   
	   case C_GAPS_PROFESSORES_I_F:
		   ss <<"C_GAPS_PROFESSORES_I_F"; break;  
	   case C_CALCULA_NRO_PROFS_CURSO:
		   ss <<"C_CALCULA_NRO_PROFS_CURSO"; break;
	   case C_MAX_NAO_MEST_CURSO:
		   ss <<"C_MAX_NAO_MEST_CURSO"; break;
	   case C_MAX_NAO_DOUT_CURSO:
		   ss <<"C_MAX_NAO_DOUT_CURSO"; break;
	   case C_ESTIMA_NRO_PROFS_VIRTUAIS_CURSO:
		   ss <<"C_ESTIMA_N_PROFS_VIRT_CURSO"; break;		   
	   case C_ESTIMA_NRO_PROFS_VIRTUAIS_MEST_CURSO:
		   ss <<"C_ESTIMA_N_PROFS_VIRT_MEST_CURSO"; break;		   
	   case C_ESTIMA_NRO_PROFS_VIRTUAIS_DOUT_CURSO:
		   ss <<"C_ESTIMA_N_PROFS_VIRT_DOUT_CURSO"; break;		   
	   case C_ESTIMA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO:
		   ss <<"C_ESTIMA_N_PROFS_VIRT_GER_CURSO"; break;		   
	   case C_SOMA_NRO_PROFS_VIRTUAIS_CURSO:
		   ss <<"C_SOMA_N_PROFS_VIRTUAIS_CURSO"; break;  		   	      		   	      		   	      		   	   
	   case C_CALCULA_NRO_PROF_VIRT_MEST_CURSO:
		   ss <<"C_CALCULA_N_PROF_VIRT_MEST_CURSO"; break;  		   	      		   	      		   	      		   	   
	   case C_CALCULA_NRO_PROF_VIRT_DOUT_CURSO:
		   ss <<"C_CALCULA_N_PROF_VIRT_DOUT_CURSO"; break;  		 
	   case C_CALCULA_NRO_PROF_VIRT_GERAIS_CURSO:
		   ss <<"C_CALCULA_N_PROF_VIRT_GER_CURSO"; break;		   
	   case C_CALCULA_NRO_PROF_VIRT_CURSO:
		   ss <<"C_CALCULA_N_PROF_VIRT_CURSO"; break;  		   	      		   	      		   	      		   	   		   
	   case C_USA_PROF_VIRT:
		   ss <<"C_USA_PROF_VIRT"; break;  		   	      		   	      		   	      		   	   		   
	   case C_PROF_DISC_PT_UNICO:
		   ss <<"C_PROF_DISC_PT_UNICO"; break;  		   	      		   	      		   	      		   	   		   
	   case C_AULA_PT_SEQUENCIAL:
		   ss <<"C_AULA_PT_SEQUENCIAL"; break;  		   	      		   	      		   	      		   	   		   
	  case C_MAX_DIAS_SEMANA_PROF:
		   ss <<"C_MAX_DIAS_SEMANA_PROF"; break;  		   	      		   	      		   	      		   	   		   
	  case C_MIN_CREDS_DIARIOS_PROF:
		   ss <<"C_MIN_CREDS_DIARIOS_PROF"; break;  		   	      		   	      		   	      		   	   		   
	  case C_PROF_ATEND_COMPLETO:
		   ss <<"C_PROF_ATEND_COMPLETO"; break;  		   	      		   	      		   	      		   	   		   
	  case C_ALOC_MIN_ALUNO:
		   ss <<"C_ALOC_MIN_ALUNO"; break;  		   	      		   	      		   	      		   	   		   		   
	  case C_PROF_MIN_DESCANSO:
		   ss <<"C_PROF_MIN_DESCANSO"; break;  		
	  case C_GARANTE_MIN_PROFS_CURSO:
		   ss <<"C_GARANTE_MIN_PROFS_CURSO"; break;
	  case C_MAX_DISC_PROF_OFERTA_PERIODO:
		   ss <<"C_MAX_DISC_PROF_OFERTA_PERIODO"; break;
	  case C_DISC_PROF_OFERTA1:
		   ss <<"C_DISC_PROF_OFERTA1"; break;		   
	  case C_DISC_PROF_OFERTA2:
		   ss <<"C_DISC_PROF_OFERTA2"; break;		   
	  case C_ESTIMA_NRO_PROFS_VIRTUAIS_CONTRATO_CURSO:
		   ss <<"C_ESTIMA_N_PROFS_VIRT_CONTRATO_CURSO"; break;		   
	  case C_MAX_SEM_CONTRATO_CURSO:
		   ss <<"C_MAX_SEM_CONTRATO_CURSO"; break;		   
	  case C_CALCULA_NRO_PROF_VIRT_CONTRATO_CURSO:
		   ss <<"C_CALCULA_N_PROF_VIRT_CONTRATO_CURSO"; break;		   
	  case C_MIN_CONTRATO_CURSO:
		   ss <<"C_MIN_CONTRATO_CURSO"; break;		   
		   
      case C_PROF_HOR_INIC_LB:
        ss << "C_PROF_HOR_INIC_LB"; break;
      case C_PROF_HOR_INIC_UB:
        ss << "C_PROF_HOR_INIC_UB"; break;	
      case C_PROF_HOR_FIN_LB:
        ss << "C_PROF_HOR_FIN_LB"; break;
      case C_PROF_HOR_FIN_UB:
        ss << "C_PROF_HOR_FIN_UB"; break;	
      case C_PROF_GAP:
        ss << "C_PROF_GAP"; break;	

	   default:
		  ss << "!";
   }

   ss << "__{";

   if ( professor != NULL )
   {
		if ( professor->eVirtual() )
		   ss << "_ProfVirt" << professor->getId() <<"_Tit"<< professor->titulacao->getId() << "_Cont" << professor->tipo_contrato->getId();
	    else
		   ss << "_Prof" << professor->getId();      
   }

   if ( curso != NULL )
   {
      ss << "_Curso" << curso->getId();
   }

   if ( t >= 0 && aula==NULL && h==NULL )
   {
      ss << "_Dia" << t;
   }

   if ( aula != NULL )
   {
      ss << "_(Disc" << aula->getDisciplina()->getId()
		  << ",Turma" << aula->getTurma()
		  << ",Dia" << aula->getDiaSemana()
          << ",Sala" << aula->getSala()->getId()
          << ",NCred" << aula->getTotalCreditos() << ")";
   }
   
   if ( b != NULL )
   {
	   ss << "_Bc" << b->getId();
   }

   if ( j >=0 )
   {
      ss << "_Sbc" << j;
   }

   if ( par1_aula_hor.first != NULL )
   if ( par1_aula_hor.second != NULL )
   {
	   ss << "_(Aula" << par1_aula_hor.first->toString()
		   << ",Hor" << par1_aula_hor.second->getId()<<")";
   }

   if ( par2_aula_hor.first != NULL )
   if ( par2_aula_hor.second != NULL )
   {
	   ss << "_(Aula" << par2_aula_hor.first->toString()
	      << ",Hor" << par2_aula_hor.second->getId() << ")";
   }

   if ( h != NULL )
   {
	   ss << "_H" << h->getHorarioAulaId();
	   if ( aula == NULL) ss <<"," << "Dia" << h->getDia();
   }

   if ( horarioAula != NULL && h == NULL )
   {
      ss << "_H" << horarioAula->getId()  << "_" << horarioAula->getInicio();
   }

   if ( duracaoAula != 0 )
   {
      ss << "_duracaoAula" << duracaoAula;
   }

   if ( disciplina != NULL && aula == NULL )
   {
      ss << "_Disc" << disciplina->getId();
   }

   if ( turma >= 0 && aula==NULL )
   {
      ss << "_Turma" << turma;
   }

   if ( s != NULL && aula == NULL )
   {
      ss << "_Sala" << s->getId();
   }

   if ( h1 != NULL )
   {
      ss << "_H1." << h1->getId();
   }

   if ( h2 != NULL )
   {
      ss << "_H2." << h2->getId();
   }
   
   if ( unidade != NULL )
   {
      ss << "_Unid" << unidade->getId();
   }

   if ( campus != NULL )
   {
      ss << "_Cp" << campus->getId();
   }

   if ( horarioDiaD != NULL )
   {
	   ss << "_(H" << horarioDiaD->getHorarioAulaId()
		  << ",Dia)" << horarioDiaD->getDia();
   }

   if ( horarioDiaD1 != NULL )
   {
	   ss << "_(H" << horarioDiaD1->getHorarioAulaId()
		  << ",Dia)" << horarioDiaD1->getDia();
   }

   if ( aluno != NULL )
   {
	   ss << "_Aluno" << aluno->getAlunoId();
   }

   if ( turmaT >= 0 )
   {
      ss << "_TurmaT" << turmaT;
   }

   if ( turmaP >= 0 )
   {
      ss << "_TurmaP" << turmaP;
   }

   if ( cpT != NULL )
   {
      ss << "_CpT" << cpT->getId();
   }

   if ( cpP != NULL )
   {
      ss << "_CpP" << cpP->getId();
   }
   
   if ( dti != NULL )
   {
      ss << "_dti" << dti->getHour() <<"_"<<dti->getMinute();
   }

   if ( dtf != NULL )
   {
      ss << "_dtf" << dtf->getHour() <<"_"<<dtf->getMinute();
   }

   if ( oferta != NULL )
   {
      ss << "_Oft" << oferta->getId();
   }

   if ( periodo != -1 )
   {
      ss << "_Per" << periodo;
   }
   
   if ( contrato != NULL )
   {
	   ss << "_Contrato" << contrato->getNome();
   }

   if ( faseDoDia_ >= 0 )
   {
      ss << "_Fase" << faseDoDia_;
   }

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}
