#include "VariablePre.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )

#define E_MENOR_PAR( a, b ) \
   ( ( ( a.first == NULL && b.first != NULL) || \
       ( b.first != NULL && a.first != NULL && ( *a.first < *b.first ) ) ) \
	 || \
	 ( ( a.first == b.first) && \
	   ( ( a.second == NULL && b.second != NULL) || \
		 ( b.second != NULL && a.second != NULL && ( *a.second < *b.second ) ) \
	   ) \
	 ) \
   )

VariablePre::VariablePre()
{
   reset();
}

VariablePre::VariablePre( const VariablePre & var )
{
   *this = var;
}

void VariablePre::reset()
{
   value = -1;
   i = -1;
   j = -1;
   cp = NULL;
   u = NULL;
   s = NULL;
   tps = NULL;
   c = NULL;
   c_incompat = NULL;
   b = NULL;
   d = NULL;
   o = NULL;   
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOft.first = NULL;
   parOft.second = NULL;
   aluno = NULL;
   turma1 = -1;
   turma2 = -1;
   disc1 = NULL;
   disc2 = NULL;
}

VariablePre::~VariablePre()
{
   reset();
}

VariablePre & VariablePre::operator = ( const VariablePre & var )
{
   this->value = var.getValue();
   this->type = var.getType();
   this->cp = var.getCampus();
   this->u = var.getUnidade();
   this->s = var.getSala();
   this->tps = var.getSubCjtSala();
   this->i = var.getTurma();
   this->c = var.getCurso();
   this->c_incompat = var.getCursoIncompat();
   this->b = var.getBloco();
   this->d = var.getDisciplina();
   this->j = var.getSubBloco();
   this->o = var.getOferta();
   this->parCursos = var.getParCursos();
   this->parOft = var.getParOfertas();
   this->aluno = var.getAluno();
   this->turma1 = var.getTurma1();
   this->turma2 = var.getTurma2();
   this->disc1 = var.getDisciplina1();
   this->disc2 = var.getDisciplina2();

   return *this;
}

bool VariablePre::operator < ( const VariablePre & var ) const
{
   if( (int) this->getType() < (int) var.getType() )
   {
      return true;
   }
   else if( (int) this->getType() > (int) var.getType() )
   {
      return false;
   }

   if ( E_MENOR( this->getCampus(), var.getCampus() ) ) return true;
   if ( E_MENOR( var.getCampus(), this->getCampus() ) ) return false;

   if ( E_MENOR( this->getUnidade(), var.getUnidade() ) ) return true;
   if ( E_MENOR( var.getUnidade(), this->getUnidade() ) ) return false;

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

   if ( E_MENOR( this->getOferta(), var.getOferta() ) ) return true;
   if ( E_MENOR( var.getOferta(), this->getOferta() ) ) return false;

   if ( E_MENOR_PAR( this->getParCursos(), var.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( var.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), var.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( var.getParOfertas(), this->getParOfertas() ) ) return false;

   if ( E_MENOR( this->getAluno(), var.getAluno() ) ) return true;
   if ( E_MENOR( var.getAluno(), this->getAluno() ) ) return false;
     
   if ( this->getTurma1() < var.getTurma1() ) return true;
   if ( this->getTurma1() > var.getTurma1() ) return false;
   
   if ( this->getTurma2() < var.getTurma2() ) return true;
   if ( this->getTurma2() > var.getTurma2() ) return false;

   if ( E_MENOR( this->getDisciplina1(),var.getDisciplina1() ) ) return true;
   if ( E_MENOR( var.getDisciplina1(), this->getDisciplina1() ) ) return false;

   if ( E_MENOR( this->getDisciplina2(),var.getDisciplina2() ) ) return true;
   if ( E_MENOR( var.getDisciplina2(), this->getDisciplina2() ) ) return false;

   return false;
}

bool VariablePre::operator == ( const VariablePre & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariablePre::toString()
{
   std::stringstream str( "" );
   std::string output;

   bool cond_disc = false;

   switch( type )
   {
     case V_PRE_ABERTURA:
        str << "z"; break;
     case V_PRE_ALUNOS:
        str << "a"; break;
     case V_PRE_CREDITOS:
        str << "x"; break;
     case V_ERROR:
        str << "?"; break;
     case V_PRE_OFERECIMENTO:
        str << "o"; break;
     case V_PRE_ALOC_ALUNO:
        str << "b"; break;
     case V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT:
        str << "bs"; break;
     case V_PRE_SLACK_DEMANDA:
        str << "fd"; cond_disc = true; break;
 	 case V_PRE_SLACK_COMPARTILHAMENTO:
		str <<"fc"; break;
	 case V_PRE_SLACK_SALA:
		str <<"fs"; break;
	 case V_PRE_LIM_SUP_CREDS_SALA:
		str <<"Hs"; break;
	 case V_PRE_ALOC_ALUNO_OFT:
		str <<"c"; break;
	 case V_PRE_ALOCA_ALUNO_TURMA_DISC:
		str <<"s"; break;
	 case V_PRE_SLACK_PRIOR_INF:
		str <<"fpi"; break;
	 case V_PRE_SLACK_PRIOR_SUP:
		str <<"fps"; break;
	 case V_PRE_FOLGA_ABRE_TURMA_SEQUENCIAL:
		str <<"ft"; break;
	 case V_PRE_TURMAS_COMPART:
		str <<"w"; break;
	 case V_PRE_FOLGA_DISTR_ALUNOS:
		 str <<"fda"; break;
    case V_PRE_CRED_SALA_F1:
       str <<"xcs1"; break;
    case V_PRE_CRED_SALA_F2:
       str <<"xcs2"; break;
    case V_PRE_CRED_SALA_F3:
       str <<"xcs3"; break;
	case V_PRE_SLACK_DISC_SALA:
       str <<"fs"; break;
	case V_PRE_ALUNO_SALA:
       str <<"as"; break;
	   	   	   
    default:
        str << "!";
   }

   str << "_{";

   if ( b != NULL )
   {
      str << "_Bc" << b->getId();
   }

   if ( i >= 0 )
      str << "_Turma" << i;

   if ( cond_disc )
   {
      if ( d != NULL )
      {
         str << "_Disc" << d->getId();
      }
   }
   else
   {
      if ( d != NULL )
      {
         str << "_Disc" << d->getId();
      }
   }

   if ( u != NULL )
   {
      str << "_Unid" << u->getId();
   }

   if ( s != NULL )
   {
      str << "_Sala" << s->getId();
   }

   if ( tps )
   {
      str << "_Tps" << tps->getId();
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

   if ( parCursos.first != NULL && parCursos.second != NULL )
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
   
   if ( parOft.first != NULL && parOft.second != NULL )
   {
	   str << "_(Of" << parOft.first->getId() << ",Of" << parOft.second->getId()<<")";
   }

   if ( aluno )
   {
	   str << "_Aluno" << aluno->getAlunoId();
   }

   if ( turma1 >= 0 )
   {
      str << "_Turma1:" << turma1;
   }

   if ( disc1 != NULL )
   {
      str << "_Disc1:" << disc1->getId();
   }
   
   if ( turma2 >= 0 )
   {
      str << "_Turma2:" << turma2;
   }

   if ( disc2 != NULL )
   {
      str << "_Disc2:" << disc2->getId();
   }

   str << "}";
   str >> output;

   return output;
}
