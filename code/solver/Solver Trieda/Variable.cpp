#include "Variable.h"

#include "HashUtil.h"

#define E_MENOR(a,b) \
   (a == NULL && b != NULL) || \
   (b != NULL && a != NULL && (*a < *b))

Variable::Variable()
{
   reset();
}

Variable::Variable(const Variable& var)
{
   *this = var;
}

void Variable::reset()
{
   value = -1;
   i = -1;
   j = -1;
   t = -1;
   k = -1;

   cp = NULL;
   u = NULL;
   s = NULL;
   tps = NULL;
   c = NULL;
   c_incompat = NULL;
   b = NULL;
   d = NULL;
   o = NULL;
}

Variable::~Variable()
{
   reset();
}

Variable& Variable::operator = ( const Variable & var )
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

   this->t = var.getDia();

   this->o = var.getOferta();

   this->k = var.getK();

   return *this;
}

bool Variable::operator <(const Variable& var) const
{
   if( (int)this->getType() < (int) var.getType() )
      return true;
   else if( (int)this->getType() > (int) var.getType() )
      return false;

   if(E_MENOR(this->getCampus(), var.getCampus())) return true;
   if (E_MENOR(this->getUnidade(),var.getUnidade())) return true;
   if (E_MENOR(this->getSala(),var.getSala())) return true;

   if (E_MENOR(this->getSubCjtSala(),var.getSubCjtSala())) return true;

   if (this->getTurma() < var.getTurma()) return true;
   
   if(E_MENOR(this->getCurso(),var.getCurso())) return true;
   
   if(E_MENOR(this->getCursoIncompat(),var.getCursoIncompat())) return true;


   if (E_MENOR(this->getBloco(),var.getBloco())) return true;
   if (E_MENOR(this->getDisciplina(),var.getDisciplina())) return true;

   if (this->getSubBloco() < var.getSubBloco()) return true;

   if (this->getDia() < var.getDia()) return true;

   if (E_MENOR(this->getOferta(),var.getOferta())) return true;

   if (this->getK() < var.getK()) return true;

   /*
   if( (int)this->getType() < (int) var.getType() )
   return true;
   else if( (int)this->getType() > (int) var.getType() )
   return false;
   if (this->getTurma() < var.getTurma()) return true;

   // ---
   if(E_MENOR(this->getCurso(),var.getCurso())) return true;
   // ---

   if (E_MENOR(this->getDisciplina(),var.getDisciplina())) return true;
   if (E_MENOR(this->getUnidade(),var.getUnidade())) return true;
   if (E_MENOR(this->getSala(),var.getSala())) return true;
   if (E_MENOR(this->getBloco(),var.getBloco())) return true;

   // ---
   if (this->getSubBloco() < var.getSubBloco()) return true;
   // ---

   if (this->getDia() < var.getDia()) return true;
   */

   return false;
}

bool Variable::operator ==(const Variable& var) const
{
   return (!(*this < var) && !(var < *this));
}

std::string Variable::toString()
{
   std::stringstream str("");
   std::string output;

   bool cond_disc = false;

   switch(type) {
     case V_ABERTURA:
        str << "z"; break;
     case V_ALUNOS:
        str << "a"; break;
     case V_CREDITOS:
        str << "x"; break;
     case V_DIAS_CONSECUTIVOS:
        str << "c"; break;
     case V_ERROR:
        str << "?"; break;
     case V_MAX_CRED_SEMANA:
        str << "H"; break;
     case V_MIN_CRED_SEMANA:
        str << "h"; break;
     case V_OFERECIMENTO:
        str << "o"; break;
     case V_N_SUBBLOCOS:
        str << "w"; break;
     case V_ALOC_ALUNO:
        str << "b"; break;
     case V_ALOC_DISCIPLINA:
        str << "y"; break;
     case V_N_ABERT_TURMA_BLOCO:
        str << "v"; break;
     case V_SLACK_DIST_CRED_DIA_SUPERIOR:
        str << "fcp"; cond_disc = true; break;
     case V_SLACK_DIST_CRED_DIA_INFERIOR:
        str << "fcm"; cond_disc = true; break;
     case V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS:
        str << "r"; break;
     case V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT:
        str << "bs"; break;
     case V_SLACK_DEMANDA:
        str << "fd"; cond_disc = true; break;
	case V_COMBINACAO_DIVISAO_CREDITO:
        str << "m"; break;
   case V_SLACK_COMBINACAO_DIVISAO_CREDITO_M:
        str << "fkm"; break;
   case V_SLACK_COMBINACAO_DIVISAO_CREDITO_P:
        str << "fkp"; break;
    case V_CREDITOS_MODF:
      str << "xm"; break;
    case V_ABERTURA_COMPATIVEL:
      str << "zc"; break;
	case V_ABERTURA_BLOCO_MESMO_TPS:
		str <<"n"; break;
	case V_SLACK_ABERTURA_BLOCO_MESMO_TPS:
		str <<"fn"; break;
    default:
        str << "!";
   }
   str << "_";

   bool hb = false;

   if (b != NULL) { str << "{" << b->getId(); hb = true; }

   //if (i != NULL) str << (hb?",":"{") << i->getId();
   if (i >= 0) str << (hb?",":"{") << i;

   if(cond_disc){
      if (d != NULL) str << "{" << d->getId();
   }
   else {
      if (d != NULL) str << "," << d->getId();
   }

   if (u != NULL) str << "," << u->getId();
   if (s != NULL) str << "," << s->getId();
   if (tps) str << "," << tps->getId();


   if (t >= 0) str << "," << t;

   if (j >= 0) str << "," << j;
   if (c) str << "," << c->getId();

   if (c_incompat) str << "," <<c_incompat->getId();

   if (cp) str << "," << cp->getId();

   if (o) str << "," << o->getId();

   if (k >= 0) str << "," << k;

   str << "}";
   str >> output;
   return output;
}

bool VariableHasher::operator()(const Variable& v1, const Variable& v2) const
{
   return (v1 < v2);
}

size_t VariableHasher::operator()(const Variable& v) const
{
   unsigned int sum = 0;

   if(v.getCampus()) {
      sum *= HASH_PRIME; sum+= intHash(v.getCampus()->getId());
   }
   if(v.getUnidade()) {
      sum *= HASH_PRIME; sum+= intHash(v.getUnidade()->getId());
   }
   if(v.getSala()) {
      sum *= HASH_PRIME; sum+= intHash(v.getSala()->getId());
   }
   if(v.getSubCjtSala()){
      sum *= HASH_PRIME; sum += intHash(v.getSubCjtSala()->getId());
   }
   if(v.getTurma() != -1) {
      sum *= HASH_PRIME; sum+= intHash(v.getTurma());
   }
   if(v.getCurso()) {
      sum *= HASH_PRIME; sum+= intHash(v.getCurso()->getId());
   }
   if(v.getCursoIncompat()) {
	   sum *= HASH_PRIME; sum+= intHash(v.getCursoIncompat()->getId());
   }
   if(v.getBloco()) {
      sum *= HASH_PRIME; sum+= intHash(v.getBloco()->getId());
   }
   if(v.getDisciplina()) {
      sum *= HASH_PRIME; sum+= intHash(v.getDisciplina()->getId());

   }
   if(v.getSubBloco() != -1) {
      sum *= HASH_PRIME; sum+= intHash(v.getSubBloco());
   }
   if(v.getDia() != -1) {
      sum *= HASH_PRIME; sum+= intHash(v.getDia());
   }	
   if(v.getOferta()) {
      sum *= HASH_PRIME; sum+= intHash(v.getOferta()->getId());
   }
   if(v.getK() != -1) {
      sum *= HASH_PRIME; sum+= intHash(v.getK());
   }

   return sum;
}

