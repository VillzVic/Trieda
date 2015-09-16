#ifndef _ALOCACAO_PROF_VIRTUAL_H_
#define _ALOCACAO_PROF_VIRTUAL_H_

#include <ostream>
#include <iostream>
#include <vector>
#include <map>

#include "GGroup.h"

class AlocacaoProfVirtual
{
public:
	
	AlocacaoProfVirtual( int discId, int turmaNr, int cpId, bool ehPrat );
	AlocacaoProfVirtual();
	virtual ~AlocacaoProfVirtual();
		
	void setTurma( int value ) { turma = value; }
	void setDisciplinaId( int value ) { disciplinaId = value; }
	void setCampusId( int value ) { campusId = value; }
	void setEhPratica( bool value ) { pratica = value; }
	
	int getTurma() const { return turma; }
	int getDisciplinaId() const { return disciplinaId; }
	int getCampusId() const { return campusId; }
	bool ehPratica() const { return pratica; }

	void addDescricaoDoMotivo( int profRealId, std::string descricao );
	void addDicaEliminacao( int profRealId, std::string dica );

	bool operator < ( AlocacaoProfVirtual const &right ) const;
	bool operator == ( AlocacaoProfVirtual const &right ) const;
	bool operator != ( AlocacaoProfVirtual const &right ) const;

	std::map< int /*Prof-Real-Id*/, std::vector<std::string> > motivosDeUso;
	std::map< int /*Prof-Real-Id*/, std::vector<std::string> > dicasDeEliminacao;

private:
	int turma;
	int disciplinaId;
	int campusId;
	bool pratica;
};

std::ostream & operator << ( std::ostream &, AlocacaoProfVirtual & );

#endif