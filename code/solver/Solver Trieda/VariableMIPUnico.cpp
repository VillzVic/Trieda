#include "VariableMIPUnico.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == nullptr && b != nullptr) || \
   ( b != nullptr && a != nullptr && ( *a < *b ) )

#define E_MENOR_PAR( a, b ) \
   ( ( ( a.first == nullptr && b.first != nullptr) || \
       ( b.first != nullptr && a.first != nullptr && ( *a.first < *b.first ) ) ) \
	 || \
	 ( ( a.first == b.first) && \
	   ( ( a.second == nullptr && b.second != nullptr) || \
		 ( b.second != nullptr && a.second != nullptr && ( *a.second < *b.second ) ) \
	   ) \
	 ) \
   )

VariableMIPUnico::VariableMIPUnico()
{
   reset();
}

VariableMIPUnico::VariableMIPUnico( const VariableMIPUnico & var )
{
   *this = var;
}

void VariableMIPUnico::reset()
{
   value = -1;
   i = -1;
   j = -1;
   t = -1;
   k = -1;
   cp = nullptr;
   u = nullptr;
   u1 = nullptr;
   u2 = nullptr;
   s = nullptr;
   tps = nullptr;
   c = nullptr;
   c_incompat = nullptr;
   b = nullptr;
   d = nullptr;
   o = nullptr;   
   parCursos.first = nullptr;
   parCursos.second = nullptr;
   parOft.first = nullptr;
   parOft.second = nullptr;
   horarioAulaI = nullptr;
   horarioAulaF = nullptr;
   aluno = nullptr;
   turma1 = -1;
   turma2 = -1;
   disc1 = nullptr;
   disc2 = nullptr;
   dateTimeF = DateTime();
   dateTimeI = DateTime();
   alunoDemanda = nullptr;
   parAlunos.first = nullptr;
   parAlunos.second = nullptr;
   periodo = -1;
   professor_ = nullptr;
   faseDoDia_ = -1;
}

VariableMIPUnico::~VariableMIPUnico()
{
   reset();
}

VariableMIPUnico & VariableMIPUnico::operator = ( const VariableMIPUnico & var )
{
   this->value = var.getValue();
   this->type = var.getType();
   this->cp = var.getCampus();
   this->u = var.getUnidade();
   this->u1 = var.getUnidade1();
   this->u2 = var.getUnidade2();
   this->s = var.getSala();
   this->tps = var.getSubCjtSala();
   this->i = var.getTurma();
   this->c = var.getCurso();
   this->c_incompat = var.getCursoIncompat();
   this->b = var.getBloco();
   this->d = var.getDisciplina();
   this->j = var.getSubBloco();
   this->t = var.getDia();
   this->o = var.getOferta();
   this->k = var.getK();
   this->parCursos = var.getParCursos();
   this->parOft = var.getParOfertas();
   this->horarioAulaI = var.getHorarioAulaInicial();
   this->horarioAulaF = var.getHorarioAulaFinal();
   this->dateTimeI = var.getDateTimeInicial();
   this->dateTimeF = var.getDateTimeFinal();
   this->aluno = var.getAluno();
   this->turma1 = var.getTurma1();
   this->turma2 = var.getTurma2();
   this->disc1 = var.getDisciplina1();
   this->disc2 = var.getDisciplina2();
   this->alunoDemanda = var.getAlunoDemanda();
   this->parAlunos = var.getParAlunos();
   this->periodo = var.getPeriodo();
   this->professor_ = var.getProfessor();
   this->faseDoDia_ = var.getFaseDoDia();

   return *this;
}

bool VariableMIPUnico::operator < ( const VariableMIPUnico & var ) const
{
   if( (int) this->getType() < (int) var.getType() )
   {
      return true;
   }
   else if( (int) this->getType() > (int) var.getType() )
   {
      return false;
   }
   
   if ( E_MENOR( this->getProfessor(), var.getProfessor() ) ) return true;
   if ( E_MENOR( var.getProfessor(), this->getProfessor() ) ) return false;

   if ( E_MENOR( this->getCampus(), var.getCampus() ) ) return true;
   if ( E_MENOR( var.getCampus(), this->getCampus() ) ) return false;

   if ( E_MENOR( this->getUnidade(), var.getUnidade() ) ) return true;
   if ( E_MENOR( var.getUnidade(), this->getUnidade() ) ) return false;
   
   if ( E_MENOR( this->getUnidade1(), var.getUnidade1() ) ) return true;
   if ( E_MENOR( var.getUnidade1(), this->getUnidade1() ) ) return false;
   
   if ( E_MENOR( this->getUnidade2(), var.getUnidade2() ) ) return true;
   if ( E_MENOR( var.getUnidade2(), this->getUnidade2() ) ) return false;

   if ( E_MENOR( this->getSala(), var.getSala() ) ) return true;
   if ( E_MENOR( var.getSala(), this->getSala() ) ) return false;

   if ( E_MENOR( this->getSubCjtSala(), var.getSubCjtSala() ) ) return true;
   if ( E_MENOR( var.getSubCjtSala(), this->getSubCjtSala() ) ) return false;

   if ( this->getTurma() < var.getTurma() ) return true;
   if ( this->getTurma() > var.getTurma() ) return false;
   
   if ( E_MENOR( this->getCurso(), var.getCurso() ) ) return true;
   if ( E_MENOR( var.getCurso(), this->getCurso() ) ) return false;   

   if ( E_MENOR( this->getCursoIncompat(), var.getCursoIncompat() ) ) return true;
   if ( E_MENOR( var.getCursoIncompat(), this->getCursoIncompat() ) ) return false;

   if ( E_MENOR( this->getBloco(), var.getBloco() ) ) return true;
   if ( E_MENOR( var.getBloco(), this->getBloco() ) ) return false;

   if ( E_MENOR( this->getDisciplina(),var.getDisciplina() ) ) return true;
   if ( E_MENOR( var.getDisciplina(), this->getDisciplina() ) ) return false;

   if ( this->getSubBloco() < var.getSubBloco() ) return true;
   if ( this->getSubBloco() > var.getSubBloco() ) return false;

   if ( this->getDia() < var.getDia() ) return true;
   if ( this->getDia() > var.getDia() ) return false;

   if ( E_MENOR( this->getOferta(), var.getOferta() ) ) return true;
   if ( E_MENOR( var.getOferta(), this->getOferta() ) ) return false;

   if ( this->getK() < var.getK() ) return true;
   if ( this->getK() > var.getK() ) return false;

   if ( E_MENOR_PAR( this->getParCursos(), var.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( var.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), var.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( var.getParOfertas(), this->getParOfertas() ) ) return false;
   
   /*if ( E_MENOR( this->getHorarioAulaInicial(), var.getHorarioAulaInicial() ) ) return true;
   if ( E_MENOR( var.getHorarioAulaInicial(), this->getHorarioAulaInicial() ) ) return false;

   if ( E_MENOR( this->getHorarioAulaFinal(), var.getHorarioAulaFinal() ) ) return true;
   if ( E_MENOR( var.getHorarioAulaFinal(), this->getHorarioAulaFinal() ) ) return false;*/
   
   if ( this->getDateTimeInicial() < var.getDateTimeInicial() ) return true;
   if ( this->getDateTimeInicial() > var.getDateTimeInicial() ) return false;

   if ( this->getDateTimeFinal() < var.getDateTimeFinal() ) return true;
   if ( this->getDateTimeFinal() > var.getDateTimeFinal() ) return false;

   if ( this->getAluno() < var.getAluno() ) return true;
   if ( this->getAluno() > var.getAluno() ) return false;
   
   if ( this->getTurma1() < var.getTurma1() ) return true;
   if ( this->getTurma1() > var.getTurma1() ) return false;
   
   if ( this->getTurma2() < var.getTurma2() ) return true;
   if ( this->getTurma2() > var.getTurma2() ) return false;

   if ( E_MENOR( this->getDisciplina1(),var.getDisciplina1() ) ) return true;
   if ( E_MENOR( var.getDisciplina1(), this->getDisciplina1() ) ) return false;

   if ( E_MENOR( this->getDisciplina2(),var.getDisciplina2() ) ) return true;
   if ( E_MENOR( var.getDisciplina2(), this->getDisciplina2() ) ) return false;
      
   if ( E_MENOR_PAR( this->getParAlunos(), var.getParAlunos() ) ) return true;
   if ( E_MENOR_PAR( var.getParAlunos(), this->getParAlunos() ) ) return false;
   
   if (this->getPeriodo() < var.getPeriodo()) return true;
   if (this->getPeriodo() > var.getPeriodo()) return false;
   
   if (this->getFaseDoDia() < var.getFaseDoDia()) return true;
   if (this->getFaseDoDia() > var.getFaseDoDia()) return false;

   return false;
}

bool VariableMIPUnico::operator == ( const VariableMIPUnico & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariableMIPUnico::toString() const
{
   std::stringstream str( "" );
   std::string output;
   
   switch( type )
   {
	 case V_ERROR:
        str << "?"; break;	
	 case V_ALUNO_CREDITOS:
        str << "v"; break;     
	 case V_CREDITOS:
        str << "x"; break;     
	 case V_OFERECIMENTO:
        str << "o"; break;     
	 case V_ALOCA_ALUNO_TURMA_DISC:
        str << "s"; break; 
     case V_DIAS_CONSECUTIVOS:
        str << "c"; break;
     case V_SLACK_DEMANDA_ALUNO:
        str << "fd"; break;
	 case V_COMBINACAO_DIVISAO_CREDITO:
        str << "m"; break;
     case V_SLACK_COMBINACAO_DIVISAO_CREDITO_M:
        str << "fkm"; break;
     case V_SLACK_COMBINACAO_DIVISAO_CREDITO_P:
        str << "fkp"; break;
     case V_ABERTURA_COMPATIVEL:
		str <<"zc"; break;
  	 case V_TURMA_ATEND_CURSO:
		str <<"b"; break;			
  	 case V_ALUNO_DIA:
		str <<"du"; break;		
  	 case V_SLACK_ABERT_SEQ_TURMA:
		str <<"ft"; break;
  	 case V_SLACK_COMPARTILHAMENTO:
		str <<"fc"; break;
  	 case V_SLACK_PRIOR_INF:
		str <<"fpi"; break;
  	 case V_SLACK_PRIOR_SUP:
		str <<"fps"; break;
	 case V_ABERTURA:
		str <<"z"; break;
	 case V_ALUNOS_MESMA_TURMA_PRAT:
		str <<"ss"; break;
	 case V_FOLGA_ALUNO_MIN_ATEND1:
		str <<"fmd1"; break;
	 case V_FOLGA_ALUNO_MIN_ATEND2:
		str <<"fmd2"; break;
	 case V_FOLGA_ALUNO_MIN_ATEND3:
		str <<"fmd3"; break;
	 case V_SALA:
		str <<"u"; break;
	 case V_PROF_TURMA:
		str <<"y"; break;
	 case V_PROF_AULA:
		str <<"k"; break;
	 case V_PROF_UNID:
		str <<"uu"; break;		
	 case V_PROF_DESLOC:
		str <<"desloc"; break;		
    case V_HI_PROFESSORES:
       str <<"hip"; break;
    case V_HF_PROFESSORES:
       str <<"hfp"; break;
    case V_PROF_FASE_DIA_USADA:
       str <<"ptf"; break;
    case V_PROF_DIA_USADO:
       str <<"pt"; break;	   
    case V_HI_ALUNOS:
       str <<"hia"; break;
    case V_HF_ALUNOS:
       str <<"hfa"; break;
    case V_FOLGA_GAP_ALUNOS:
       str <<"fagap"; break;
    case V_FOLGA_GAP_PROF:
       str <<"fpgap"; break;
    case V_FOLGA_MIN_CRED_DIA_ALUNO:
       str <<"fcad"; break;
    case V_F_CARGA_HOR_ANT_PROF:
       str <<"fch"; break;
	   	   
	case V_LONGO_DIA_ALUNO:
       str <<"l"; break;
	case V_FOLGA_MIN_CRED_DIA_ALUNO_MARRETA:
       str <<"fcadm"; break;	   
	   	      
	case V_INICIO_ALUNOS:
       str <<"inicio"; break;	   	      
	case V_FIM_ALUNOS:
       str <<"fim"; break;	   
	   	      

    default:
        str << "!";
   }

   str << "_{";
   
   if ( professor_ )
   {
	   str << "_Prof" << professor_->getId();
   }
   
   if ( aluno )
   {
	   str << "_Aluno" << aluno->getAlunoId();
   }
   
   if ( b != nullptr )
   {
      str << "_Bc" << b->getId();
   }

   if ( i >= 0 )
   {
      str << "_Turma" << i;
   }

   if ( d != nullptr )
   {
      str << "_Disc" << d->getId();
   }
   
   if ( u != nullptr )
   {
	   str << "_Unid" << u->getId() << "_" << u->getCodigo();
   }
   
   if ( u1 != nullptr )
   {
      str << "_1Unid" << u1->getId() << "_" << u1->getCodigo();
   }
   
   if ( u2 != nullptr )
   {
      str << "_2Unid" << u2->getId() << "_" << u2->getCodigo();
   }

   if ( s != nullptr )
   {
      str << "_Sala" << s->getId();
   }

   if ( tps )
   {
      str << "_Tps" << tps->getId() << "Sl" << tps->salas.begin()->first;
   }

   if ( t >= 0 )
   {
      str << "_Dia" << t;
   }

   if ( j >= 0 )
   {
      str << "_Sbc" << j;
   }

   if ( c )
   {
      str << "_Curso" << c->getId();
   }

   if ( c_incompat )
   {
      str << "_CursoIncomp" << c_incompat->getId();
   }

   if ( parCursos.first != nullptr && parCursos.second != nullptr )
   {
	   str << "_(c" << parCursos.first->getId() << ",c" << parCursos.second->getId()<<")";
   }

   if ( cp )
   {
      str << "_Cp" << cp->getId();
   }

   if ( o )
   {
      str << "_Of" << o->getId();
   }
   
   if ( parOft.first != nullptr && parOft.second != nullptr )
   {
	   str << "_(Of" << parOft.first->getId() << ",Of" << parOft.second->getId()<<")";
   }

   if ( k >= 0 )
   {
      str << "_DivCred" << k;
   }

   if ( horarioAulaI )
   {
      str << "_Hi" << horarioAulaI->getId();
   }

   if ( horarioAulaF )
   {
      str << "_Hf" << horarioAulaF->getId();
   }

   if ( dateTimeI != DateTime() )
   {
      str << "_Dti" << dateTimeI.getHour() << "_" << dateTimeI.getMinute();
   }

   if ( dateTimeF != DateTime() )
   {
      str << "_Dtf" << dateTimeF.getHour() << "_" << dateTimeF.getMinute();
   }

   if ( turma1 >= 0 )
   {
      str << "_Turma1." << turma1;
   }

   if ( disc1 != nullptr )
   {
      str << "_Disc1." << disc1->getId();
   }
   
   if ( turma2 >= 0 )
   {
      str << "_Turma2." << turma2;
   }

   if ( disc2 != nullptr )
   {
      str << "_Disc2." << disc2->getId();
   }

   if ( alunoDemanda != nullptr )
   {
      str << "_AlDem." << alunoDemanda->getId();
   }
      
   if ( parAlunos.first != nullptr && parAlunos.second != nullptr )
   {
	   str << "_(a" << parAlunos.first->getAlunoId() << ",a" << parAlunos.second->getAlunoId()<<")";
   }

   if ( periodo >= 0 )
   {
      str << "_Per" << periodo;
   }
   
   if ( faseDoDia_ >= 0 )
   {
      str << "_Fase" << faseDoDia_;
   }

   str << "}";
   str >> output;

   return output;
}
