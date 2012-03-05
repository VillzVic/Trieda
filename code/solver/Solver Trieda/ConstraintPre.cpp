#include "ConstraintPre.h"
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

ConstraintPre::ConstraintPre()
{
   reset();
}

ConstraintPre::ConstraintPre( const ConstraintPre & cons )
{
   *this = cons;
}

ConstraintPre::~ConstraintPre()
{
   reset();
}

ConstraintPre& ConstraintPre::operator = ( const ConstraintPre & cons )
{   
   this->type = cons.getType();

   this->cp = cons.getCampus();
   this->u = cons.getUnidade();
   this->s = cons.getSala();
   this->tps = cons.getSubCjtSala();
   this->i = cons.getTurma();
   this->c = cons.getCurso();
   this->c_incompat = cons.getCursoIncompat();
   this->b = cons.getBloco();
   this->d = cons.getDisciplina();
   this->j = cons.getSubBloco();
   this->o = cons.getOferta();
   this->sl = cons.getSemanaLetiva();
   this->parCursos = cons.getParCursos();
   this->cjtSalaCompart = cons.getSubCjtSalaCompart();
   this->parOfts = cons.getParOfertas();

   return *this;
}

bool ConstraintPre::operator < ( const ConstraintPre & cons ) const
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

   if (this->getTurma() < cons.getTurma()) return true;
   if (this->getTurma() > cons.getTurma()) return false;

   if(E_MENOR(this->getCurso(),cons.getCurso())) return true;
   if(E_MENOR(cons.getCurso(), this->getCurso())) return false;

   if(E_MENOR(this->getCursoIncompat(),cons.getCursoIncompat())) return true;
   if(E_MENOR(cons.getCursoIncompat(), this->getCursoIncompat())) return false;

   if (E_MENOR(this->getBloco(),cons.getBloco())) return true;
   if (E_MENOR(cons.getBloco(), this->getBloco())) return false;

   if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;
   if (E_MENOR(cons.getDisciplina(), this->getDisciplina())) return false;

   if ( this->getSubBloco() < cons.getSubBloco() ) return true;
   if ( cons.getSubBloco() < this->getSubBloco() ) return false;

   if (E_MENOR(this->getOferta(),cons.getOferta())) return true;
   if (E_MENOR(cons.getOferta(), this->getOferta())) return false;

   if (this->getSemanaLetiva() < cons.getSemanaLetiva()) return true;
   if (this->getSemanaLetiva() > cons.getSemanaLetiva()) return false;

   if (E_MENOR(this->getSubCjtSalaCompart(),cons.getSubCjtSalaCompart())) return true;
   if (E_MENOR(cons.getSubCjtSalaCompart(), this->getSubCjtSalaCompart())) return false;

   if ( E_MENOR_PAR( this->getParCursos(), cons.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), cons.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( cons.getParOfertas(), this->getParOfertas() ) ) return false;

   return false;

}

bool ConstraintPre::operator== (const ConstraintPre& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintPre::reset()
{
   cp = NULL;
   u = NULL;
   s = NULL;
   cjtSalaCompart = NULL;
   tps = NULL;
   i = -1;
   c = NULL;
   c_incompat = NULL;
   b = NULL;
   d = NULL;
   j = -1;
   o = NULL;
   sl = NULL;
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOfts.first = NULL;
   parOfts.second = NULL;

}

std::string ConstraintPre::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch( type )
   {
   case C_PRE_CARGA_HORARIA:
      ss << "__(CARGA_HORARIA):"; break;
   case C_PRE_MAX_CRED_SALA:
      ss << "__(C_PRE_MAX_CRED_SALA):"; break;
   case C_PRE_VAR_O:
      ss << "__(C_PRE_VAR_O):"; break;
   case C_EVITA_MUDANCA_DE_SALA:
      ss << "__(C_EVITA_MUDANCA_DE_SALA):"; break;	  
   case C_PRE_CAP_ALOC_DEM_DISC:
      ss << "__(C_PRE_CAP_ALOC_DEM_DISC):"; break;
   case C_ALUNO_OFT_DISC:
      ss << "__(C_ALUNO_OFT_DISC):"; break;
   case C_CAP_SALA:
      ss << "__(C_CAP_SALA):"; break;
   case C_PRE_ALUNOS_CURSOS_INCOMP:
      ss << "__(C_PRE_ALUNOS_CURSOS_INCOMP):"; break;
   case C_PRE_PROIBE_COMPARTILHAMENTO:
      ss << "__(C_PRE_PROIBE_COMPARTILHAMENTO):"; break;
   case C_PRE_ATIVA_Z:
      ss << "__(C_PRE_ATIVA_Z):"; break;
   case C_PRE_EVITA_TURMA_DISC_CAMP_D:
      ss << "__(C_PRE_EVITA_TURMA_DISC_CAMP_D):"; break;
   case C_PRE_LIMITA_ABERTURA_TURMAS:
      ss << "__(C_PRE_LIMITA_ABERTURA_TURMAS):"; break;
   case C_PRE_ABRE_TURMAS_EM_SEQUENCIA:
      ss << "__(C_PRE_ABRE_TURMAS_EM_SEQUENCIA):"; break;
   case C_PRE_TURMA_MESMA_DISC_SALA_DIF:
      ss << "__(C_PRE_TURMA_MESMA_DISC_SALA_DIF):"; break;
   case C_PRE_LIM_SUP_CREDS_SALA:
      ss << "__(C_PRE_LIM_SUP_CREDS_SALA):"; break;	  
   case C_PRE_ATIVA_C:
      ss << "__(C_PRE_ATIVA_C):"; break;	  
   case C_PRE_FIXA_NAO_COMPARTILHAMENTO:
      ss << "__(C_PRE_FIXA_NAO_COMPARTILHAMENTO):"; break;	  
	  
   default:
      ss << "!";
   }

   ss << "__{";

   if ( i >= 0 )
   {
      ss << "_Turma" << i;
   }
   
   if ( d != NULL )
   {
      ss << "_Disc" << d->getId();
   }

   if ( u != NULL )
   {
      ss << "_Unid" << u->getId();
   }

   if ( c != NULL )
   {
      ss << "_Curso" << c->getId();
   }

   if ( c_incompat != NULL )
   {
      ss << "_CursoIncomp" << c_incompat->getId();
   }

   if ( b != NULL )
   {
	   ss << "_Bc" << b->getId();
   }

   if ( j >=0 )
   {
      ss << "_Sbc" << j;
   }

   if ( parCursos.first != NULL && parCursos.second != NULL )
   {
	   ss << "_(Curso" << parCursos.first->getId(); 	
	   ss << ",Curso" << parCursos.second->getId() << ")";
   }

   if ( tps != NULL )
   {
      ss << "_Tps" << tps->getId();
	  if ( tps->salas.size()==1 ) ss << "idS" << tps->salas.begin()->first;
   }

   if ( cjtSalaCompart != NULL )
   {
	   ss << "_TpsCompart" << cjtSalaCompart->getId();
	   if ( tps->salas.size()==1 ) ss << "idS" << cjtSalaCompart->salas.begin()->first;
   }

   if ( s != NULL )
   {
      ss << "_Sala" << s->getId();
   }

   if ( o != NULL )
   {
      ss << "_Oferta" << o->getId();
   }
   
   if ( parOfts.first != NULL && parOfts.second != NULL )
   {
      ss << "_(Oft" << parOfts.first->getId();
      ss << ",Oft" << parOfts.second->getId() << ")";
   }
   
   if ( sl != NULL )
   {
	   ss << "_SL" << sl->getId();
   }

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}
