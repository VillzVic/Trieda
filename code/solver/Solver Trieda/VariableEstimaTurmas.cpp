#include "VariableEstimaTurmas.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )

VariableEstimaTurmas::VariableEstimaTurmas()
{
   reset();
}

VariableEstimaTurmas::VariableEstimaTurmas( const VariableEstimaTurmas & var )
{
   *this = var;
}

void VariableEstimaTurmas::reset()
{
   value = -1;
   cp = NULL;
   u = NULL;
   s = NULL;
   tps = NULL;
   d = NULL;
   sl = NULL;
   turno = -1;
   aluno = NULL;
   alunoDemanda = NULL;
}

VariableEstimaTurmas::~VariableEstimaTurmas()
{
   reset();
}

VariableEstimaTurmas & VariableEstimaTurmas::operator = ( const VariableEstimaTurmas & var )
{
   this->value = var.getValue();
   this->type = var.getType();
   this->cp = var.getCampus();
   this->u = var.getUnidade();
   this->s = var.getSala();
   this->tps = var.getSubCjtSala();
   this->d = var.getDisciplina();
   this->sl = var.getSemanaLetiva();
   this->turno = var.getTurno();
   this->aluno = var.getAluno();
   this->alunoDemanda = var.getAlunoDemanda();

   return *this;
}

bool VariableEstimaTurmas::operator < ( const VariableEstimaTurmas & var ) const
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

   if ( E_MENOR( this->getDisciplina(),var.getDisciplina() ) ) return true;
   if ( E_MENOR( var.getDisciplina(), this->getDisciplina() ) ) return false;

   if ( E_MENOR( this->getSemanaLetiva(),var.getSemanaLetiva() ) ) return true;
   if ( E_MENOR( var.getSemanaLetiva(), this->getSemanaLetiva() ) ) return false;

   if ( this->getTurno() < var.getTurno() ) return true;
   if ( var.getTurno() < this->getTurno() ) return false;

   if ( E_MENOR( this->getAluno(),var.getAluno() ) ) return true;
   if ( E_MENOR( var.getAluno(), this->getAluno() ) ) return false;

   if ( E_MENOR( this->getAlunoDemanda(),var.getAlunoDemanda() ) ) return true;
   if ( E_MENOR( var.getAlunoDemanda(), this->getAlunoDemanda() ) ) return false;

   return false;
}

bool VariableEstimaTurmas::operator == ( const VariableEstimaTurmas & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariableEstimaTurmas::toString()
{
   std::stringstream str( "" );
   std::string output;

   switch( type )
   {

	case V_EST_ERROR:
       str <<"error"; break;
	case V_EST_NUM_TURMAS:
       str <<"x"; break;
	case V_EST_LIM_SUP_CREDS_SALA:
       str <<"H"; break;
	case V_EST_SLACK_DISC_SALA:
       str <<"fs"; break;
	case V_EST_SLACK_DEMANDA:
       str <<"fd"; break;
	case V_EST_NUM_ATEND:
       str <<"a"; break;
	case V_EST_SLACK_DEMANDA_ALUNO:
       str <<"fd"; break;
	case V_EST_ALOCA_ALUNO_DISC:
       str <<"s"; break;
	case V_EST_CRED_SALA_F1:
       str <<"xcs1"; break;
	case V_EST_CRED_SALA_F2:
       str <<"xcs2"; break;
	case V_EST_CRED_SALA_F3:
       str <<"xcs3"; break;
	case V_EST_CRED_SALA_F4:
       str <<"xcs4"; break;
	case V_EST_ALUNO_SALA:
       str <<"as"; break;
	case V_EST_USA_DISCIPLINA:
       str <<"k"; break;
	   	   

    default:
        str << "!";
   }

   str << "_{";

   if ( d != NULL )
   {
      str << "_Disc" << d->getId();
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
	   str << "_Tps" << tps->getId() << "Sl" << tps->salas.begin()->second->getCodigo();
   }

   if ( cp )
   {
      str << "_Cp" << cp->getId();
   }

   if ( sl )
   {
      str << "_Sl" << sl->getId();
   }

   if ( turno!=-1 )
   {
      str << "_Turno" << turno;
   }
   
   if ( aluno )
   {
      str << "_Aluno" << aluno->getAlunoId();
   }
   
   if ( alunoDemanda )
   {
      str << "_AlunoDemanda" << alunoDemanda->getId();
   }


   str << "}";
   str >> output;

   return output;
}
