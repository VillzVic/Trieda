#include "ConstraintEstimaTurmas.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )


ConstraintEstimaTurmas::ConstraintEstimaTurmas()
{
   reset();
}

ConstraintEstimaTurmas::ConstraintEstimaTurmas( const ConstraintEstimaTurmas & cons )
{
   *this = cons;
}

ConstraintEstimaTurmas::~ConstraintEstimaTurmas()
{
   reset();
}

ConstraintEstimaTurmas& ConstraintEstimaTurmas::operator = ( const ConstraintEstimaTurmas & cons )
{   
   this->type = cons.getType();

   this->cp = cons.getCampus();
   this->u = cons.getUnidade();
   this->s = cons.getSala();
   this->tps = cons.getSubCjtSala();
   this->d = cons.getDisciplina();
   this->sl = cons.getSemanaLetiva();
   this->turno = cons.getTurno();
   this->aluno = cons.getAluno();

   return *this;
}

bool ConstraintEstimaTurmas::operator < ( const ConstraintEstimaTurmas & cons ) const
{

   if( (int)this->getType() < (int) cons.getType() )
      return true;
   else if( (int)this->getType() > (int) cons.getType() )
      return false;

   if(E_MENOR(this->getCampus(), cons.getCampus())) return true;
   if(E_MENOR(cons.getCampus(), this->getCampus())) return false;

   if (E_MENOR(this->getUnidade(),cons.getUnidade())) return true;
   if (E_MENOR(cons.getUnidade(), this->getUnidade())) return false;

   if (E_MENOR(this->getSala(),cons.getSala())) return true;
   if (E_MENOR(cons.getSala(), this->getSala())) return false;

   if (E_MENOR(this->getSubCjtSala(),cons.getSubCjtSala())) return true;
   if (E_MENOR(cons.getSubCjtSala(), this->getSubCjtSala())) return false;

   if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;
   if (E_MENOR(cons.getDisciplina(), this->getDisciplina())) return false;
   
   if ( E_MENOR( this->getSemanaLetiva(),cons.getSemanaLetiva() ) ) return true;
   if ( E_MENOR( cons.getSemanaLetiva(), this->getSemanaLetiva() ) ) return false;

   if ( this->getTurno() < cons.getTurno() ) return true;
   if ( cons.getTurno() < this->getTurno() ) return false;
   
   if ( E_MENOR( this->getAluno(),cons.getAluno() ) ) return true;
   if ( E_MENOR( cons.getAluno(), this->getAluno() ) ) return false;

   return false;

}

bool ConstraintEstimaTurmas::operator== (const ConstraintEstimaTurmas& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintEstimaTurmas::reset()
{
   cp = NULL;
   u = NULL;
   s = NULL;
   tps = NULL;
   d = NULL;
   sl = NULL;
   turno = -1;
   aluno = NULL;
}

std::string ConstraintEstimaTurmas::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch( type )
   {
   case C_ERROR:
      ss << "__(ERROR):"; break;
   case C_EST_TEMPO_MAX_SALA:
      ss << "__(C_EST_TEMPO_MAX_SALA):"; break;
   case C_EST_TEMPO_MAX_SALA_SAB:
      ss << "__(C_EST_TEMPO_MAX_SALA_SAB):"; break;
   case C_EST_ATEND_DEMANDA:
      ss << "__(C_EST_ATEND_DEMANDA):"; break;
   case C_EST_CAPACIDADE_SALAS:
      ss << "__(C_EST_CAPACIDADE_SALAS):"; break;
   case C_EST_DISTRIBUI_ENTRE_SALAS:
      ss << "__(C_EST_DISTRIBUI_ENTRE_SALAS):"; break;
   case C_EST_TURMA_DIF_MESMA_DISC_SALA_DIF:
      ss << "__(C_EST_TURMA_DIF_MESMA_DISC_SALA_DIF):"; break;
   case C_EST_DISC_PRATICA_TEORICA:
      ss << "__(C_EST_DISC_PRATICA_TEORICA):"; break;
   case C_EST_DEMANDA_EQUIV_ALUNO:
      ss << "__(C_EST_DEMANDA_EQUIV_ALUNO):"; break;	  	  
   case C_EST_SOMA_CRED_SALA:
      ss << "__(C_EST_SOMA_CRED_SALA):"; break;	  	   	  
   case C_EST_ALUNO_SALA:
      ss << "__(C_EST_ALUNO_SALA):"; break;	  	  
   case C_EST_NRO_TURMAS_DISC_PRAT_TEOR_IGUAL:
      ss << "__(C_EST_NRO_TURMAS_DISC_PRAT_TEOR_IGUAL):"; break;
   case C_EST_TEMPO_MAX_ALUNO_SAB:
      ss << "__(C_EST_TEMPO_MAX_ALUNO_SAB):"; break;
   case C_EST_USA_DISC:
      ss << "__(C_EST_USA_DISC):"; break;	  
	  

   default:
      ss << "!";
   }

   ss << "__{";
   
   if ( d != NULL )
   {
      ss << "_Disc" << d->getId();
   }
   
   if ( cp != NULL )
   {
      ss << "_Cp" << cp->getId();
   }

   if ( u != NULL )
   {
      ss << "_Unid" << u->getId();
   }

   if ( tps != NULL )
   {
      ss << "_Tps" << tps->getId();
	  if ( tps->salas.size()==1 ) ss << "idS" << tps->salas.begin()->first;
   }

   if ( s != NULL )
   {
      ss << "_Sala" << s->getId();
   }

   if ( sl )
   {
      ss << "_Sl" << sl->getId();
   }

   if ( turno!=-1 )
   {
      ss << "_Turno" << turno;
   }
   
   if ( aluno )
   {
      ss << "_Aluno" << aluno->getAlunoId();
   }

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}
