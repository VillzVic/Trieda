#include "MIPAlocarProfCons.h"

#include "OfertaDisciplina.h"
#include "TurmaHeur.h"
#include "../CentroDados.h"
#include "../Campus.h"

#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == nullptr && b != nullptr) || \
   ( b != nullptr && a != nullptr && ( *a < *b ) )

#define E_MENOR_PAR( a, b ) \
   ( ( ( a.first == nullptr && b.first != nullptr ) || \
       ( a.first != nullptr && b.first != nullptr && ( *a.first < *b.first ) ) ) \
	 || \
	 ( ( ( a.first == nullptr && b.first == nullptr ) || \
	     ( a.first != nullptr && b.first != nullptr && a.first == b.first ) ) && \
	   ( ( a.second == nullptr && b.second != nullptr) || \
		 ( b.second != nullptr && a.second != nullptr && ( *a.second < *b.second ) ) \
	   ) \
	 ) \
   )

MIPAlocarProfCons::MIPAlocarProfCons()
{
   reset();
}

MIPAlocarProfCons::MIPAlocarProfCons( const MIPAlocarProfCons & var )
{
   *this = var;
}

void MIPAlocarProfCons::reset()
{
   this->value_ = 0;
   this->type_ = MIPAlocarProfCons::C_ERROR;
   this->professor_ = nullptr;
   this->turma_ = nullptr;
   this->dia_ = -1;
   this->fase_ = -1;
   this->parTurmas_.first = nullptr;
   this->parTurmas_.second = nullptr;
   this->curso_ = nullptr;
   this->titulacaoId_ = -1;
   this->contratoId_ = -1;
   this->dateTime_ = DateTime();
}

MIPAlocarProfCons::~MIPAlocarProfCons()
{
   reset();
}

MIPAlocarProfCons& MIPAlocarProfCons::operator = ( const MIPAlocarProfCons & var )
{
   this->value_ = var.getValue();
   this->type_ = var.getType();
   this->professor_ = var.getProfessor();
   this->turma_ = var.getTurma();
   this->dia_ = var.getDia();
   this->fase_ = var.getFaseDoDia();
   this->parTurmas_ = var.getParTurmas();
   this->curso_ = var.getCurso();
   this->titulacaoId_ = var.getTitulacaoId();
   this->contratoId_ = var.getContratoId();
   this->dateTime_ = var.getDateTime();

   return ( *this );
}

bool MIPAlocarProfCons::operator < ( const MIPAlocarProfCons & var ) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;


   if( (int)this->getDia() < (int) var.getDia() )
      return true;
   else if( (int)this->getDia() > (int) var.getDia() )
      return false;
   

   if( (int)this->getFaseDoDia() < (int) var.getFaseDoDia() )
      return true;
   else if( (int)this->getFaseDoDia() > (int) var.getFaseDoDia() )
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
 

   if( E_MENOR_PAR(this->getParTurmas(), var.getParTurmas()) )
		return true;
   else if( E_MENOR_PAR(var.getParTurmas(), this->getParTurmas()) )
	   return true;


   if( this->getCurso() == nullptr && var.getCurso() != nullptr )
		return true;
	else if( this->getCurso() != nullptr && var.getCurso() == nullptr )
		return false;
	else if( this->getCurso() != nullptr && var.getCurso() != nullptr )
	{
		if( *this->getCurso() < *var.getCurso() )
			return true;
		else if( *var.getCurso() < *this->getCurso() )
			return false;
	}
	

	if( this->getTitulacaoId() < var.getTitulacaoId() )
      return true;
    else if( var.getTitulacaoId() < this->getTitulacaoId() )
      return false;


	if( this->getContratoId() < var.getContratoId() )
      return true;
    else if( var.getContratoId() < this->getContratoId() )
      return false;


   if( this->getDateTime() < var.getDateTime() )
      return true;
   else if( this->getDateTime() > var.getDateTime() )
      return false;


   return false;
}

bool MIPAlocarProfCons::operator == ( const MIPAlocarProfCons & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

string MIPAlocarProfCons::toString()
{
   std::stringstream str;
   str << "CType[" << (int) type_ << "]__";

   switch ( type_ )
   {
     case C_ERROR:
        str << "C_ERROR"; break;
     case C_TURMA_PROF_ASSIGN:
        str << "C_TURMA_PROF_ASSIGN"; break;
     case C_PROF_USADO:
        str << "C_PROF_USADO"; break;
     case C_PROF_DIA:
        str << "C_PROF_DIA"; break;
      case C_PROF_MIN_CH:
        str << "C_PROF_MIN_CH"; break;
      case C_PROF_CH_ANT:
        str << "C_PROF_CH_ANT"; break;
      case C_PROF_HOR_INIC_LB:
        str << "C_PROF_HOR_INIC_LB"; break;
      case C_PROF_HOR_INIC_UB:
        str << "C_PROF_HOR_INIC_UB"; break;	
      case C_PROF_HOR_FIN_LB:
        str << "C_PROF_HOR_FIN_LB"; break;
      case C_PROF_HOR_FIN_UB:
        str << "C_PROF_HOR_FIN_UB"; break;	
      case C_PROF_GAP:
        str << "C_PROF_GAP"; break;	
      case C_TURMAS_INCOMP:
        str << "C_TURMAS_INCOMP"; break;
      case C_PROF_MAX_DIAS:
        str << "C_PROF_MAX_DIAS"; break;
      case C_PROF_MAX_CH:
        str << "C_PROF_MAX_CH"; break;
      case C_PROF_MIN_CH_DIA:
        str << "C_PROF_MIN_CH_DIA"; break;
      case C_PROF_VIRT_SEQ:
        str << "C_PROF_VIRT_SEQ"; break;
      case C_PROF_MIN_CONTRATO:
        str << "C_PROF_MIN_CONTRATO"; break;
      case C_PROF_MIN_TITULO:
        str << "C_PROF_MIN_TITULO"; break;
		
		
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

   if ( parTurmas_.first != nullptr && parTurmas_.second != nullptr )
   {
	   TurmaHeur *turma1 = parTurmas_.first;
	   TurmaHeur *turma2 = parTurmas_.second;

	   str << "_(Disc" << turma1->ofertaDisc->getDisciplina()->getId()
		   << "_i" << turma1->id
		   << "_Cp" << turma1->ofertaDisc->getCampus()->getId();
	   unordered_map<int, AulaHeur*> aulas;
	   turma1->getAulas(aulas);
	   for ( auto itDiaAula = aulas.begin(); itDiaAula != aulas.end(); itDiaAula++ )
	   {
		   DateTime dti, dtf;
		   itDiaAula->second->getPrimeiroHor(dti);
		   itDiaAula->second->getLastHor(dtf);
		   str << "_(dia" << itDiaAula->first << "de" << dti.hourMinToStr() << "a" << dtf.hourMinToStr() << ")";
	   }
	   str << ")";

	   str << "_(Disc" << turma2->ofertaDisc->getDisciplina()->getId()
		   << "_i" << turma2->id
		   << "_Cp" << turma2->ofertaDisc->getCampus()->getId();
	   aulas.clear();
	   turma2->getAulas(aulas);
	   for ( auto itDiaAula = aulas.begin(); itDiaAula != aulas.end(); itDiaAula++ )
	   {
		   DateTime dti, dtf;
		   itDiaAula->second->getPrimeiroHor(dti);
		   itDiaAula->second->getLastHor(dtf);
		   str << "_(dia" << itDiaAula->first << "de" << dti.hourMinToStr() << "a" << dtf.hourMinToStr() << ")";
	   }
	   str << ")";
   } 

   if ( titulacaoId_ >= 0 )
   {
      str << "_tit" << titulacaoId_;
   }

   if ( contratoId_ >= 0 )
   {
      str << "_contr" << contratoId_;
   }

   if ( dateTime_ != DateTime() )
   {
	   str << "_" << dateTime_.hourMinToStr();
   }


   str << "}";
   
   return (str.str());
}
