#ifndef _PROFESSOR_VIRTUAL_OUTPUT_H_
#define _PROFESSOR_VIRTUAL_OUTPUT_H_

#include <ostream>

#include "GGroup.h"
#include "ProblemData.h"
#include "SolucaoOperacional.h"
#include "AtendimentoCampus.h"
#include "RestricaoViolada.h"

// Código relacionado à issue TRIEDA-881
class ProfessorVirtualOutput
{
public:
	ProfessorVirtualOutput();
	virtual ~ProfessorVirtualOutput();

	GGroup< int > disciplinas;

	void setId( int value ) { id = value; }
	void setChMin( int value ) { ch_min = value; }
	void setChMax( int value ) { ch_max = value; }
	void setTitulacaoId( int value ) { titulacao_id = value; }
	void setAreaTitulacaoId( int value ) { area_titulacao_id = value; }

	int getId() const { return id; }
	int getChMin() const { return ch_min; }
	int getChMax() const { return ch_max; }
	int getTitulacaoId() const { return titulacao_id; }
	int getAreaTitulacaoId() const { return area_titulacao_id; }

private:
	int id;
	int ch_min;
	int ch_max;
	int titulacao_id;
	int area_titulacao_id;
};

std::ostream & operator << ( std::ostream &, ProfessorVirtualOutput & );

#endif