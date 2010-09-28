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

	this->i = cons.getTurma();
	this->c = cons.getCurso();

	this->c_incompat = cons.getCursoIncompat();

	this->b = cons.getBloco();
	this->d = cons.getDisciplina();

	this->j = cons.getSubBloco();

	this->t = cons.getDia();

	return *this;
}

bool Constraint::operator< (const Constraint& cons) const
{
	if( (int)this->getType(), (int) cons.getType() )
		return true;
	else if( (int)this->getType() > (int) cons.getType() )
		return false;

	if(E_MENOR(this->getCampus(), cons.getCampus())) return true;
	if (E_MENOR(this->getUnidade(),cons.getUnidade())) return true;
	if (E_MENOR(this->getSala(),cons.getSala())) return true;

	if (this->getTurma() < cons.getTurma()) return true;
	if(E_MENOR(this->getCurso(),cons.getCurso())) return true;

	if(E_MENOR(this->getCursoIncompat(),cons.getCursoIncompat())) return true;

	if (E_MENOR(this->getBloco(),cons.getBloco())) return true;
	if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;

	if (this->getSubBloco() < cons.getSubBloco()) return true;

	if (this->getDia() < cons.getDia()) return true;

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

	i = -1;
	c = NULL;;

	c_incompat = NULL;

	b = NULL;;
	d = NULL;;

	j = -1;

	t = -1;;
}

std::string Constraint::toString()
{
	std::stringstream ss;
	ss << "CType[" << (int) type << "]";
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

	return sum;
}

bool ConstraintHasher::operator() (const Constraint& cons1, const Constraint& cons2) const
{
	return (cons1 < cons2);
}
