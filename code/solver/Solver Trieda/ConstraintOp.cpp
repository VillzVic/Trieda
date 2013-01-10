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
   this->par_disc_turma = cons.par_disc_turma;
   this->campus = cons.getCampus();
   this->unidade = cons.getUnidade();
   this->duracaoAula = cons.getDuracaoAula();
   this->aluno = cons.getAluno();
   
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
		if( *this->getHorario() < *cons.getHorario() )
			return true;
		else if( *cons.getHorario() < *this->getHorario() )
			return false;
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
		if( *this->getHorarioAula() < *cons.getHorarioAula() )
			return true;
		else if( *cons.getHorarioAula() < *this->getHorarioAula() )
			return false;
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

   if ( E_MENOR( this->getHorarioDiaD(), cons.getHorarioDiaD() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getHorarioDiaD(), this->getHorarioDiaD() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getHorarioDiaD1(), cons.getHorarioDiaD1() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getHorarioDiaD1(), this->getHorarioDiaD1() ) )
   {
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

   if ( E_MENOR( this->getH1(), cons.getH1() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getH1(), this->getH1() ) )
   {
      return false;
   }

   if ( E_MENOR( this->getH2(), cons.getH2() ) )
   {
      return true;
   }
   else if ( E_MENOR( cons.getH2(), this->getH2() ) )
   {
      return false;
   }
   
   if ( this->getParDiscTurma() < cons.getParDiscTurma() )
   {
      return true;
   }
   else if ( cons.getParDiscTurma() > this->getParDiscTurma() )
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

   return false;
}

bool ConstraintOp::operator == ( const ConstraintOp & cons ) const
{
   return ( !( *this < cons ) && !( cons < *this ) );
}

void ConstraintOp::setParDiscTurma( Disciplina* d1, int turma1, Disciplina* d2, int turma2 )
{
	this->par_disc_turma[d1] = turma1;
	this->par_disc_turma[d2] = turma2;
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
   this->par_disc_turma.clear();
   this->campus = NULL;
   this->duracaoAula = 0;
   this->unidade = NULL;
   this->aluno = NULL;
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
	   case C_DIAS_PROF_MINISTRA_AULA:
		  ss << "C_DIAS_PROF_MINISTRA_AULA"; break;
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
	   case C_CALCULA_NRO_PROFS_VIRTUAIS_CURSO:
		   ss <<"C_CALCULA_NRO_PROFS_VIRTUAIS_CURSO"; break;		   
	   case C_CALCULA_NRO_PROFS_VIRTUAIS_MEST_CURSO:
		   ss <<"C_CALCULA_NRO_PROFS_VIRTUAIS_MEST_CURSO"; break;		   
	   case C_CALCULA_NRO_PROFS_VIRTUAIS_DOUT_CURSO:
		   ss <<"C_CALCULA_NRO_PROFS_VIRTUAIS_DOUT_CURSO"; break;		   
	   case C_CALCULA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO:
		   ss <<"C_CALCULA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO"; break;		   
	   case C_SOMA_NRO_PROFS_VIRTUAIS_CURSO:
		   ss <<"C_SOMA_NRO_PROFS_VIRTUAIS_CURSO"; break;  		   	      		   	      		   	      		   	   
		   
	   default:
		  ss << "!";
   }

   ss << "__{";

   if ( professor != NULL )
   {
		if ( professor->eVirtual() )
		   ss << "_ProfVirtual" << professor->titulacao->toString();
	    else
		   ss << "_Prof" << professor->getId();      
   }

   if ( curso != NULL )
   {
      ss << "_Curso" << curso->getId();
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

   if ( par_disc_turma.size() == 2 )
   {
	   std::map<Disciplina*, int>::iterator it = par_disc_turma.begin();

	   ss << "_(Disc" << it->first->getId(); 	
	   ss << ",Turma" << it->second;
	   ++it;
	   ss << ";Disc" << it->first->getId();
	   ss << ",Turma" << it->second << ")";
   }

   if ( h != NULL )
   {
	   ss << "_H" << h->getHorarioAulaId();
	   if ( aula == NULL) ss <<"," << "Dia" << h->getDia();
   }

   if ( horarioAula != NULL && h == NULL )
   {
      ss << "_H" << horarioAula->getId();
   }

   if ( duracaoAula != 0 )
   {
      ss << "_duracaoAula" << duracaoAula;
   }

   if ( t >= 0 && aula==NULL && h==NULL )
   {
      ss << "_Dia" << t;
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


   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}
