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
     case V_PRE_MAX_CRED_SEMANA:
        str << "H"; break;
     case V_PRE_MIN_CRED_SEMANA:
        str << "h"; break;
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

   str << "}";
   str >> output;

   return output;
}
