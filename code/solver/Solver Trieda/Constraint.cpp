#include "Constraint.h"
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

Constraint::Constraint()
{
   reset();
}

Constraint::Constraint( const Constraint & cons )
{
   *this = cons;
}

Constraint::~Constraint()
{
   reset();
}

Constraint& Constraint::operator = ( const Constraint & cons )
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
   this->t = cons.getDia();
   this->o = cons.getOferta();
   this->sl = cons.getSemanaLetiva();
   this->parCursos = cons.getParCursos();
   this->cjtSalaCompart = cons.getSubCjtSalaCompart();
   this->parOfts = cons.getParOfertas();

   return *this;
}

bool Constraint::operator < ( const Constraint & cons ) const
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

   if (this->getDia() < cons.getDia()) return true;
   if (this->getDia() > cons.getDia()) return false;

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

bool Constraint::operator== (const Constraint& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void Constraint::reset()
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
   t = -1;
   o = NULL;
   sl = NULL;
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOfts.first = NULL;
   parOfts.second = NULL;

}

std::string Constraint::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch( type )
   {
   case C_CARGA_HORARIA:
      ss << "__(CARGA_HORARIA):"; break;
   case C_MAX_TEMPO_S_D_SL:
      ss << "__(MAX_TEMPO_CJTSALA_DIA_SEMANALETIVA):"; break;
   case C_MAX_TEMPO_SD:
      ss << "__(MAX_TEMPO_CJTSALA_DIA):"; break;
   case C_MIN_CREDITOS_DD:
      ss << "__(MIN_CREDITOS_DISC_DIA):"; break;
   case C_VAR_O:
      ss << "__(ATIVACAO_VAR_O):"; break;
   case C_EVITA_SOBREPOSICAO_TD:
      ss << "__(EVITA_SOBREPOSICAO_TURMAS):"; break;
   case C_TURMA_DISCIPLINA_SALA:
      ss << "__(TURMA_DISCIPLINA_SALA):"; break;
   case C_TURMA_SALA:
      ss << "__(TURMA_SALA):"; break;
   case C_EVITA_TURMA_DISC_CAMP_D:
      ss << "__(EVITA_TURMA_DISC_CAMP_DIF):"; break;
   case C_TURMAS_BLOCO:
      ss << "__(ABERTURA_TURMAS_MSM_BLOCO):"; break;
   case C_MAX_CRED_DISC_BLOCO:
      ss << "__(MAX_CRED_DISC_BLOCO):"; break;
   case C_NUM_TUR_BLOC_DIA_DIFUNID:
      ss << "__(NUM_TUR_BLOC_DIA_DIF_UNID):"; break;
   case C_LIM_CRED_DIAR_DISC:
      ss << "__(LIM_CRED_DIAR_DISC):"; break;
   case C_CAP_ALOC_DEM_DISC:
      ss << "__(CAP_ALOC_DEM_DISC):"; break;
   case C_CAP_SALA_COMPATIVEL_TURMA:
      ss << "__(CAP_SALA_COMPATIVEL_TURMA):"; break;
   case C_CAP_SALA_UNIDADE:
      ss << "__(CAP_TOTAL_SALA_UNIDADE):"; break;
   case C_TURMA_DISC_DIAS_CONSEC:
      ss << "__(TURMAS_DISC_DIAS_CONSEC):"; break;
   case C_MIN_CREDS_TURM_BLOCO:
      ss << "__(MIN_CREDS_ALOC_TURM_BLOCO):"; break;
   case C_MAX_CREDS_TURM_BLOCO:
      ss << "__(MAX_CREDS_TURM_BLOCO):"; break;
   case C_ALUNO_CURSO_DISC:
      ss << "__(ALUNO_CURSO_TURMA):"; break;
   case C_ALUNOS_CURSOS_INCOMP:
      ss << "__(C_ALUNOS_CURSOS_INCOMP):"; break;
   case C_SLACK_DIST_CRED_DIA:
      ss << "__(FIX_DIST_CRED_DIA):"; break;
   case C_VAR_R:
      ss << "__(ATIVACAO_VAR_R):"; break;
   case C_LIMITA_ABERTURA_TURMAS:
      ss << "__(LIMITA_ABERTURA_TURMAS):"; break;
   case C_ABRE_TURMAS_EM_SEQUENCIA:
      ss << "__(ABRE_TURMAS_EM_SEQUENCIA):"; break;
   case C_COMBINACAO_DIVISAO_CREDITO:
      ss << "__(C_COMBINACAO_DIVISAO_CREDITO):"; break;
   case C_DIVISAO_CREDITO:
      ss << "__(C_DIVISAO_CREDITO):"; break;
   case C_VAR_Y:
      ss << "__(C_VAR_Y):"; break;
   case C_MAX_CREDS_DISC_DIA:
      ss << "__(C_MAX_CREDS_DISC_DIA):"; break;
   case C_MAX_CREDS_BLOCO_DIA:
      ss << "__(C_MAX_CREDS_BLOCO_DIA):"; break;
   case C_VAR_ZC:
      ss << "__(C_VAR_ZC):"; break;
  case C_DISC_INCOMPATIVEIS:
      ss << "__(C_DISC_INCOMPATIVEIS):"; break;
  case C_EVITA_BLOCO_TPS_D:
      ss << "__(C_EVITA_BLOCO_TPS_D):"; break;
  case C_SLACK_EVITA_BLOCO_TPS_D:
      ss << "__(C_SLACK_EVITA_BLOCO_TPS_D):"; break;
  case C_PROIBE_COMPARTILHAMENTO:
	  ss << "__(C_PROIBE_COMPARTILHAMENTO):"; break;
  case C_EVITA_SOBREPOS_SALA_POR_COMPART:
	  ss << "__(C_EVITA_SOBREPOS_SALA_POR_COMPART):"; break;
  case C_VAR_E:
      ss << "__(C_VAR_E):"; break;
  case C_VAR_OF_1:
      ss << "__(C_VAR_OF_1):"; break;
  case C_VAR_OF_2:
      ss << "__(C_VAR_OF_2):"; break;
  case C_VAR_OF_3:
      ss << "__(C_VAR_OF_3):"; break;
  case C_VAR_P_1:
      ss << "__(C_VAR_P_1):"; break;
  case C_VAR_P_2:
      ss << "__(C_VAR_P_2):"; break;
  case C_VAR_P_3:
      ss << "__(C_VAR_P_3):"; break;
  case C_VAR_G:
      ss << "__(C_VAR_G):"; break;
  case C_EVITA_SOBREPOS_SALA_POR_TURMA:
	  ss << "__(C_EVITA_SOBREPOS_SALA_POR_TURMA):"; break;
  case C_VAR_Q_1:
      ss << "__(C_VAR_Q_1):"; break;
  case C_VAR_Q_2:
      ss << "__(C_VAR_Q_2):"; break;
  case C_VAR_Q_3:
      ss << "__(C_VAR_Q_3):"; break;
  case C_VAR_CS:
      ss << "__(C_VAR_CS):"; break;	
  case C_FIXA_NAO_COMPARTILHAMENTO:
      ss << "__(C_FIXA_NAO_COMPARTILHAMENTO):"; break;
  case C_VAR_CBC:
      ss << "__(C_VAR_CBC):"; break;
	  

   default:
      ss << "!";
   }
   //return "";
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

   if ( t >= 0 )
   {
      ss << "_Dia" << t;
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

/*
size_t ConstraintHasher::operator() ( const Constraint & cons ) const
{
   unsigned int sum = 0;

   if(cons.getCampus() != NULL) {
      sum *= HASH_PRIME; sum+= intHash(cons.getCampus()->getId());
   }
   if(cons.getUnidade()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getUnidade()->getId());
   }
   if(cons.getSala()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getSala()->getId());
   }
   if(cons.getSubCjtSala()){
      sum *= HASH_PRIME; sum += intHash(cons.getSubCjtSala()->getId());
   }
   if(cons.getTurma() != -1) {
      sum *= HASH_PRIME; sum+= intHash(cons.getTurma());
   }
   if(cons.getCurso()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getCurso()->getId());
   }

   if(cons.getCursoIncompat()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getCursoIncompat()->getId());
   }


   if(cons.getBloco()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getBloco()->getId());
   }
   if(cons.getDisciplina()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getDisciplina()->getId());

   }
   if(cons.getSubBloco() != -1) {
      sum *= HASH_PRIME; sum+= intHash(cons.getSubBloco());
   }
   if(cons.getDia() != -1) {
      sum *= HASH_PRIME; sum+= intHash(cons.getDia());
   }
   if(cons.getOferta()) {
      sum *= HASH_PRIME; sum+= intHash(cons.getOferta()->getId());
   }	

   return sum;
}

bool ConstraintHasher::operator() (const Constraint& cons1, const Constraint& cons2) const
{
   return (cons1 < cons2);
}
*/
