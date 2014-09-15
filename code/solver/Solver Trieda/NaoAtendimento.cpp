#include "NaoAtendimento.h"

NaoAtendimento::NaoAtendimento( int id )
{
	_aluno_demanda_id = id;
}

	
NaoAtendimento::~NaoAtendimento()
{}

	
bool NaoAtendimento::operator < ( const NaoAtendimento & right )
{	
	return ( _aluno_demanda_id < right._aluno_demanda_id );	    
}


bool NaoAtendimento::operator == ( const NaoAtendimento & right )
{	
	return ( _aluno_demanda_id == right._aluno_demanda_id ); 
}


std::ostream & operator << ( std::ostream & out, NaoAtendimento & right )
{
	out << "<FolgaAlunoDemanda>\n";
	out << "<id>" << right.getAlunoDemandaId() << "</id>\n";

	std::vector< std::string > motivos = right.getMotivos();
	int n = motivos.size();
	if ( n > 0 )
	{
		out << "<motivos>\n";
		for ( int i = 0; i < n; i++ )
		{
			out << "<motivo>";
			out << motivos[i];
			out << "</motivo>\n";
		}
		out << "</motivos>\n";
	}
	else
	{
		out << "<motivos/>\n";
	}
	out << "</FolgaAlunoDemanda>\n";

	return out;
}