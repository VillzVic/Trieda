#ifndef _NAO_ATENDIMENTO_H_
#define _NAO_ATENDIMENTO_H_

#include <ostream>
#include <string>
#include <vector>

#include "GGroup.h"

class NaoAtendimento
{
public:
	NaoAtendimento( int id );

	~NaoAtendimento();

    void setAlunoDemandaId( int aValor ) { this->_aluno_demanda_id = aValor; }
    void addMotivo( std::string str ) { this->_motivos.push_back(str); }

    int getAlunoDemandaId() const { return this->_aluno_demanda_id; }
    std::vector< std::string > getMotivos() const { return this->_motivos; }
    
	
	bool operator < ( const NaoAtendimento & right );
	bool operator == ( const NaoAtendimento & right );
	
private:
	int _aluno_demanda_id;
	std::vector< std::string > _motivos;
};


std::ostream & operator << ( std::ostream & out, NaoAtendimento & right );

#endif
