#include "MIPAlocarAlunoVar.h"
#include "OfertaDisciplina.h"
#include "TurmaHeur.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == nullptr && b != nullptr) || \
   ( b != nullptr && a != nullptr && ( *a < *b ) )

MIPAlocarAlunoVar::MIPAlocarAlunoVar()
{
   reset();
}

MIPAlocarAlunoVar::MIPAlocarAlunoVar( const MIPAlocarAlunoVar & var )
{
   *this = var;
}

void MIPAlocarAlunoVar::reset()
{
   this->value_ = 0;
   this->type_ = MIPAlocarAlunoVar::V_ERROR;
   this->aluno_ = nullptr;
   this->turma_ = nullptr;
   this->turmap_ = nullptr;
   this->turmat_ = nullptr;
   this->sala_ = nullptr;
   this->oferta_ = nullptr;
   this->dia_ = -1;
}

MIPAlocarAlunoVar::~MIPAlocarAlunoVar()
{
   reset();
}

MIPAlocarAlunoVar& MIPAlocarAlunoVar::operator = ( const MIPAlocarAlunoVar & var )
{
   this->value_ = var.getValue();
   this->type_ = var.getType();
   this->aluno_ = var.getAluno();
   this->turma_ = var.getTurma();
   this->turmap_ = var.getTurmaP();
   this->turmat_ = var.getTurmaT();
   this->sala_ = var.getSala();
   this->oferta_ = var.getOfertaDisciplina();
   this->dia_ = var.getDia();

   return ( *this );
}

bool MIPAlocarAlunoVar::operator < ( const MIPAlocarAlunoVar & var ) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;
   
   if( (int)this->getDia() < (int) var.getDia() )
      return true;
   else if( (int)this->getDia() > (int) var.getDia() )
      return false;   
   
   if( E_MENOR(this->getAluno(), var.getAluno()) )
		return true;
	else if( E_MENOR(var.getAluno(), this->getAluno()) )
		return false;

   if( E_MENOR(this->getTurma(), var.getTurma()) )
		return true;
	else if( E_MENOR(var.getTurma(), this->getTurma()) )
		return false;
   
   if( E_MENOR(this->getTurmaP(), var.getTurmaP()) )
		return true;
	else if( E_MENOR(var.getTurmaP(), this->getTurmaP()) )
		return false;
   
   if( E_MENOR(this->getTurmaT(), var.getTurmaT()) )
		return true;
	else if( E_MENOR(var.getTurmaT(), this->getTurmaT()) )
		return false;

    if( E_MENOR(this->getSala(), var.getSala()) )
		return true;
	else if( E_MENOR(var.getSala(), this->getSala()) )
		return false;
  
	if( E_MENOR(this->getOfertaDisciplina(), var.getOfertaDisciplina()) )
		return true;
	else if( E_MENOR(var.getOfertaDisciplina(), this->getOfertaDisciplina()) )
		return false;
  

   return false;
}

bool MIPAlocarAlunoVar::operator == ( const MIPAlocarAlunoVar & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string MIPAlocarAlunoVar::toString(TurmaHeur* turma)
{
   std::stringstream str( "" );
   if ( turma != nullptr )
   {
	   str << "_(Disc" << turma->ofertaDisc->getDisciplina()->getId()
		   << "_i" << turma->id
		   << "_Cp" << turma->ofertaDisc->getCampus()->getId();

	   unordered_map<int, AulaHeur*> aulas;
	   turma->getAulas(aulas);
	   for ( auto itDiaAula = aulas.begin(); itDiaAula != aulas.end(); itDiaAula++ )
	   {
		   DateTime dti, dtf;
		   itDiaAula->second->getPrimeiroHor(dti);
		   itDiaAula->second->getLastHor(dtf);
		   str << "_(dia" << itDiaAula->first << "de" << dti.hourMinToStr() << "a" << dtf.hourMinToStr() << ")";
	   }
	   str << ")";
   }
   return str.str();
}

std::string MIPAlocarAlunoVar::toString()
{
   std::stringstream str( "" );
   std::string output;

   switch ( type_ )
   {
     case V_X_ALUNO_DISC:
        str << "x"; break;		
     case V_W_ASSOC_TURMA_PT:
        str << "w"; break;		
     case V_Y_ALUNO_TURMA:
        str << "y"; break;		
     case V_AT_ABRIR_TURMA:
        str << "at"; break;	
     case V_R_SALA_TURMA:
        str << "r"; break;
		

      default:
        str << "!";
   }

   str << "_{";

   if ( aluno_ != nullptr )
   {
	   str << "_Aluno" << aluno_->getId();
   }

   if ( dia_ >= 0 )
   {
      str << "_Dia" << dia_;
   }

   if ( sala_ != nullptr )
   {
	   str << "_Sala" << sala_->getId();
   }
   
   if ( oferta_ != nullptr )
   {
	   str << "_Disc" << oferta_->getDisciplina()->getId();
   }

   str << toString(turma_);
   str << toString(turmap_);
   str << toString(turmat_);
   
   str << "}";

   str >> output;

   return output;
}
