#ifndef _PROFESSOR_VIRTUAL_OUTPUT_H_
#define _PROFESSOR_VIRTUAL_OUTPUT_H_

#include <ostream>

#include "OFBase.h"

class AlocacaoProfVirtual;

// Código relacionado à issue TRIEDA-881
class ProfessorVirtualOutput
{
public:
	ProfessorVirtualOutput( int valueId );
	ProfessorVirtualOutput();
	virtual ~ProfessorVirtualOutput();

	GGroup< int > disciplinas; // Se a disciplina tiver sido substituida, é o id da substituta que aparece aqui

	GGroup< AlocacaoProfVirtual*, LessPtr<AlocacaoProfVirtual> > alocacoes;

	void setId( int value ) { id = value; }
	void setChMin( int value ) { ch_min = value; }
	void setChMax( int value ) { ch_max = value; }
	void setTitulacaoId( int value ) { titulacao_id = value; }
	void setAreaTitulacaoId( int value ) { area_titulacao_id = value; }
	void setCursoAssociado( int value ) { curso_id = value; }
	void setContratoId( int value ) { contrato_id = value; }

	int getId() const { return id; }
	int getChMin() const { return ch_min; }
	int getChMax() const { return ch_max; }
	int getTitulacaoId() const { return titulacao_id; }
	int getAreaTitulacaoId() const { return area_titulacao_id; }
	int getCursoAssociadoId() const { return curso_id; }
	int getContratoId() const { return contrato_id; }
	

	AlocacaoProfVirtual* getAlocacao( int discId, int turmaNr, int cpId, bool ehPrat );
	
	bool operator < ( const ProfessorVirtualOutput &right ) const;
	bool operator == ( const ProfessorVirtualOutput &right ) const;
	bool operator != ( const ProfessorVirtualOutput &right ) const;


private:
	int id;
	int ch_min;
	int ch_max;
	int titulacao_id;
	int area_titulacao_id;
	int curso_id;
	int contrato_id;
};

std::ostream & operator << ( std::ostream &, ProfessorVirtualOutput & );

// NAO TERMINADO!
std::istream & operator >> ( std::istream &file, ProfessorVirtualOutput* const &ptrProfVirtual );

#endif