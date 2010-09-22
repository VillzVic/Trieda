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
	/* ToDo:
	All pointers that define a variable should be addressed to NULL
	*/
	value = -1;

	cp = NULL;
	u = NULL;
	s = NULL;

	i = -1;
	c = NULL;;
	b = NULL;;
	d = NULL;;

	j = -1;

	t = -1;

	/*
	i = -1;
	b = NULL;
	d = NULL;
	u = NULL;
	s = NULL;
	t = -1;
	c = NULL;
	j = -1;
	cp = NULL;   
	*/

}

Variable::~Variable()
{
	reset();
}

Variable& Variable::operator=(const Variable& var)
{
	this->value = var.getValue();
	this->type = var.getType();

	this->cp = var.getCampus();
	this->u = var.getUnidade();
	this->s = var.getSala();

	this->i = var.getTurma();
	this->c = var.getCurso();
	this->b = var.getBloco();
	this->d = var.getDisciplina();

	this->j = var.getSubBloco();

	this->t = var.getDia();

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

	if (this->getTurma() < var.getTurma()) return true;
	if(E_MENOR(this->getCurso(),var.getCurso())) return true;
	if (E_MENOR(this->getBloco(),var.getBloco())) return true;
	if (E_MENOR(this->getDisciplina(),var.getDisciplina())) return true;

	if (this->getSubBloco() < var.getSubBloco()) return true;

	if (this->getDia() < var.getDia()) return true;

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
	  default:
		  str << "!";
	}
	str << "_";
	bool hb = false;
	if (b != NULL) { str << "{" << b->getId(); hb = true; }

	//if (i != NULL) str << (hb?",":"{") << i->getId();
	if (i >= 0) str << (hb?",":"{") << i;

	if (d != NULL) str << "," << d->getId();
	if (u != NULL) str << "," << u->getId();
	if (s != NULL) str << "," << s->getId();
	if (t >= 0) str << "," << t;

	if (j >= 0) str << "," << j;
	if (c) str << "," << c->getId();

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

	/**
	ToDo:
	All pointers different from NULL must be considered in the hash function
	**/

	if(v.getCampus()) {
		sum *= HASH_PRIME; sum+= intHash(v.getCampus()->getId());
	}
	if(v.getUnidade()) {
		sum *= HASH_PRIME; sum+= intHash(v.getUnidade()->getId());
	}
	if(v.getSala()) {
		sum *= HASH_PRIME; sum+= intHash(v.getSala()->getId());

	}
	if(v.getTurma() != -1) {
		sum *= HASH_PRIME; sum+= intHash(v.getTurma());
	}
	if(v.getCurso()) {
		sum *= HASH_PRIME; sum+= intHash(v.getCurso()->getId());
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

	/*
	if (v.getBloco() != NULL) {

	sum *= HASH_PRIME; sum+= intHash(v.getBloco()->getId());
	}
	if (v.getTurma() != NULL) {
	sum *= HASH_PRIME; sum+= intHash(v.getTurma());
	}
	if (v.getDisciplina() != NULL) {
	sum *= HASH_PRIME; sum+= intHash(v.getDisciplina()->getId());
	}
	if (v.getUnidade() != NULL) {
	sum *= HASH_PRIME; sum+= intHash(v.getUnidade()->getId());
	}
	if (v.getSala() != NULL) {
	sum *= HASH_PRIME; sum+= intHash(v.getSala()->getId());
	}
	if(v.getDia() >= 0) {
	sum *= HASH_PRIME; sum+= intHash(v.getDia());
	}

	if(v.getSubBloco() >= 0) {
	sum *= HASH_PRIME; sum+= intHash(v.getSubBloco());
	}
	if(v.getCurso()) {
	sum *= HASH_PRIME; sum+= intHash(v.getCurso()->getId());
	}
	if(v.getCampus() != NULL) {
	sum *= HASH_PRIME; sum+= intHash(v.getCampus()->getId());
	}
	*/

	return sum;
}

