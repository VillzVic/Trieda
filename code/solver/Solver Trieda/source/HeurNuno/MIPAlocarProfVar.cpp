#include "MIPAlocarProfVar.h"
#include "OfertaDisciplina.h"
#include "TurmaHeur.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == nullptr && b != nullptr) || \
   ( b != nullptr && a != nullptr && ( *a < *b ) )

MIPAlocarProfVar::MIPAlocarProfVar()
{
   reset();
}

MIPAlocarProfVar::MIPAlocarProfVar( const MIPAlocarProfVar & var )
{
   *this = var;
}

void MIPAlocarProfVar::reset()
{
   this->value_ = 0;
   this->type_ = MIPAlocarProfVar::V_ERROR;
   this->professor_ = nullptr;
   this->turma_ = nullptr;
   this->dia_ = -1;
   this->fase_ = -1;
}

MIPAlocarProfVar::~MIPAlocarProfVar()
{
   reset();
}

MIPAlocarProfVar& MIPAlocarProfVar::operator = ( const MIPAlocarProfVar & var )
{
   this->value_ = var.getValue();
   this->type_ = var.getType();
   this->professor_ = var.getProfessor();
   this->turma_ = var.getTurma();
   this->dia_ = var.getDia();
   this->fase_ = var.getFase();

   return ( *this );
}

bool MIPAlocarProfVar::operator < ( const MIPAlocarProfVar & var ) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;


   if( (int)this->getDia() < (int) var.getDia() )
      return true;
   else if( (int)this->getDia() > (int) var.getDia() )
      return false;
   

   if( (int)this->getFase() < (int) var.getFase() )
      return true;
   else if( (int)this->getFase() > (int) var.getFase() )
      return false;


   if( this->getProfessor() == nullptr && var.getProfessor() != nullptr )
		return true;
	else if( this->getProfessor() != nullptr && var.getProfessor() == nullptr )
		return false;
	else if( this->getProfessor() != nullptr && var.getProfessor() != nullptr )
	{
		if( *this->getProfessor() < *var.getProfessor() )
			return true;
		else if( *var.getProfessor() < *this->getProfessor() )
			return false;
	}


   if( this->getTurma() == nullptr && var.getTurma() != nullptr )
		return true;
	else if( this->getTurma() != nullptr && var.getTurma() == nullptr )
		return false;
	else if( this->getTurma() != nullptr && var.getTurma() != nullptr )
	{
		if( *this->getTurma() < *var.getTurma() )
			return true;
		else if( *var.getTurma() < *this->getTurma() )
			return false;
	}
   

   return false;
}

bool MIPAlocarProfVar::operator == ( const MIPAlocarProfVar & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string MIPAlocarProfVar::toString()
{
   std::stringstream str( "" );
   std::string output;

   switch ( type_ )
   {
     case V_X_PROF_TURMA:
        str << "x"; break;
     case V_Y_PROF_USADO:
        str << "y"; break;
     case V_Z_PROF_DIA:
        str << "z"; break;
      case V_CH_PROF_MIN_CH:
        str << "ch"; break;
      case V_CHA_PROF_CH_ANT:
        str << "cha"; break;
      case V_HIP_PROF_HOR_INIC:
        str << "hip"; break;
      case V_HFP_PROF_HOR_FIN:
        str << "hfp"; break;	

      default:
        str << "!";
   }

   str << "_{";

   if ( professor_ != nullptr )
   {
	   if ( professor_->ehVirtual() )
	   {
		   str << "_ProfVirt" << professor_->getId();

		   if ( professor_->tipoTitulacao() != nullptr )
			   str <<"_Tit"<< professor_->tipoTitulacao()->getId();
		   if ( professor_->tipoContrato() != nullptr )
			   str << "_Cont" << professor_->tipoContrato()->getId();
	   }
	   else
		   str << "_Prof" << professor_->getId();
   }

   if ( dia_ >= 0 )
   {
      str << "_Dia" << dia_;
   }

   if ( fase_ >= 0 )
   {
      str << "_f" << fase_;
   }

   if ( turma_ != nullptr )
   {
	   str << "_(Disc" << turma_->ofertaDisc->getDisciplina()->getId()
		   << "_i" << turma_->id
		   << "_Cp" << turma_->ofertaDisc->getCampus()->getId();

	   unordered_map<int, AulaHeur*> aulas;
	   turma_->getAulas(aulas);
	   for ( auto itDiaAula = aulas.begin(); itDiaAula != aulas.end(); itDiaAula++ )
	   {
		   DateTime dti, dtf;
		   itDiaAula->second->getPrimeiroHor(dti);
		   itDiaAula->second->getLastHor(dtf);
		   str << "_(dia" << itDiaAula->first << "de" << dti.hourMinToStr() << "a" << dtf.hourMinToStr() << ")";
	   }
	   str << ")";
   }
   
   str << "}";

   str >> output;

   return output;
}
