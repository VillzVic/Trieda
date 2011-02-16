#include "Constraint.h"
#include "HashUtil.h"

#define E_MENOR(a,b) \
   (a == NULL && b != NULL) || \
   (b != NULL && a != NULL && (*a < *b))

Constraint::Constraint()
{
   reset();

   /*
   ToDo:
   Attributes should be initiated
   */

}

Constraint::Constraint(const Constraint &cons)
{
   *this = cons;
}

Constraint::~Constraint()
{
   reset();
}

Constraint& Constraint::operator= (const Constraint& cons)
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

   return *this;
}

bool Constraint::operator< (const Constraint& cons) const
{
   if( (int)this->getType() < (int) cons.getType() )
      return true;
   else if( (int)this->getType() > (int) cons.getType() )
      return false;

   if(E_MENOR(this->getCampus(), cons.getCampus())) return true;
   if (E_MENOR(this->getUnidade(),cons.getUnidade())) return true;
   if (E_MENOR(this->getSala(),cons.getSala())) return true;

   if (E_MENOR(this->getSubCjtSala(),cons.getSubCjtSala())) return true;

   if (this->getTurma() < cons.getTurma()) return true;
   if(E_MENOR(this->getCurso(),cons.getCurso())) return true;

   if(E_MENOR(this->getCursoIncompat(),cons.getCursoIncompat())) return true;

   if (E_MENOR(this->getBloco(),cons.getBloco())) return true;
   if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;

   if (this->getSubBloco() < cons.getSubBloco()) return true;

   if (this->getDia() < cons.getDia()) return true;

   if (E_MENOR(this->getOferta(),cons.getOferta())) return true;

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

   tps = NULL;

   i = -1;
   c = NULL;

   c_incompat = NULL;

   b = NULL;
   d = NULL;

   j = -1;

   t = -1;

   o = NULL;
}

std::string Constraint::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch(type)
   {
   case C_CARGA_HORARIA:
      ss << "__(CARGA_HORARIA):"; break;
   case C_MAX_CREDITOS_SD:
      ss << "__(MAX_CREDITOS_CJTSALA_DIA):"; break;
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
      ss << "__(CAP_SALA_COMPATIVEL_TURMA):";
      ss << "[";
      ss << "_i:" << i;
      ss << "_d:" << d->getId();
      ss << "_cp:" << cp->getId();
      ss << "]:";
      break;
   case C_CAP_SALA_UNIDADE:
      ss << "__(CAP_TOTAL_SALA_UNIDADE):"; break;
   case C_TURMA_DISC_DIAS_CONSEC:
      ss << "__(TURMAS_DISC_DIAS_CONSEC):"; break;
   case C_MIN_CREDS_TURM_BLOCO:
      ss << "__(MIN_CREDS_ALOC_TURM_BLOCO):"; break;
   case C_MAX_CREDS_TURM_BLOCO:
      ss << "__(MAX_CREDS_TURM_BLOCO):"; break;
   case C_ALUNO_CURSO_DISC:
      ss << "__(ALUNO_CURSO_TURMA):";
      ss << "[";
      ss << "_i:" << i;
      ss << "_d:" << d->getId();
      ss << "_c:" << c->getId();
      ss << "_cp:" << cp->getId();
      ss << "]:";
      break;
   case C_ALUNOS_CURSOS_DIF:
      ss << "__(ALUNOS_CURSOS_DIF):"; break;
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
   default:
      ss << "!";
   }

   std::string consName = "";
   ss >> consName;

   return consName;
}

size_t ConstraintHasher::operator() (const Constraint& cons) const
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
