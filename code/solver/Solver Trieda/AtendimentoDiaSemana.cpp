#include "AtendimentoDiaSemana.h"

AtendimentoDiaSemana::AtendimentoDiaSemana(void)
{
	//key = std::make_pair<int,int>(-1,-1);
   atendimentos_tatico = new GGroup<AtendimentoTatico*>();
   atendimentos_turno = new GGroup<AtendimentoTurno*>();

   //atendimentos_tatico = NULL;
   //atendimentos_turno = NULL;

}

AtendimentoDiaSemana::~AtendimentoDiaSemana(void)
{
   if( atendimentos_tatico != NULL )
   {
      atendimentos_tatico->deleteElements();
      delete atendimentos_tatico;
   }

   if( atendimentos_turno != NULL )
   {
      atendimentos_turno->deleteElements();
      delete atendimentos_turno;
   }
}

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem)
{
   out << "<AtendimentoDiaSemana>" << endl;

	out << "<diaSemana>" << diaSem.dia_semana << "</diaSemana>" << endl;

	//if(diaSem.atendimentos_tatico.size() > 0)
   if(diaSem.atendimentos_tatico->size() > 0)
	{
		out << "<atendimentosTatico>" << endl;

		//GGroup<AtendimentoTatico*>::GGroupIterator it_tatico = diaSem.atendimentos_tatico.begin();
      GGroup<AtendimentoTatico*>::GGroupIterator it_tatico = diaSem.atendimentos_tatico->begin();

		//for(; it_tatico != diaSem.atendimentos_tatico.end(); it_tatico++)
      for(; it_tatico != diaSem.atendimentos_tatico->end(); it_tatico++)
		{
			out << **it_tatico;
		}

		out << "</atendimentosTatico>" << endl;
	}
	//else if (diaSem.atendimentos_turno.size() > 0)
   else if (diaSem.atendimentos_turno->size() > 0)
	{
		out << "<atendimentosTurnos>" << endl;

		//GGroup<AtendimentoTurno*>::GGroupIterator it_turno = diaSem.atendimentos_turno.begin();
      GGroup<AtendimentoTurno*>::GGroupIterator it_turno = diaSem.atendimentos_turno->begin();

		//for(; it_turno != diaSem.atendimentos_turno.end(); it_turno++)
      for(; it_turno != diaSem.atendimentos_turno->end(); it_turno++)
		{
			//out << (*it_turno) << endl;
			out << **it_turno;
		}

		out << "</atendimentosTurnos>" << endl;
	}

	out << "</AtendimentoDiaSemana>" << endl;

	return out;
}