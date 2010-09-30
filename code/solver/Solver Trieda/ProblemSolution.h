#ifndef PROBLEMSOLUTION_H
#define PROBLEMSOLUTION_H

#include <ostream>
#include "GGroup.h"
#include "ProblemData.h"

//#include "Oferecimento.h"
#include "AtendimentoCampus.h"

using namespace std;

//Stores output data
class ProblemSolution
{
public:
	//Constructor
	ProblemSolution();

	//Destructor
	~ProblemSolution() {}

	//==================================================
	// SET METHODS 
	//==================================================

	/**
	ToDo:
	All set methods of the private attributes should be defined here
	*/

	//==================================================
	// GET METHODS 
	//==================================================

	/**
	ToDo:
	All get methods of the private attributes should be defined here
	*/

	//private:

	/**
	ToDo:
	All objects that define the problem output should be declared here
	**/

	//GGroup<Oferecimento*> oferecimentos;

	GGroup<AtendimentoCampus*> atendimento_campus;

	// >>>
	void addCampus(int id, std::string campusId)
	{
		AtendimentoCampus *at_campus;
		std::cout << ">> > >>" << std::endl;
		if( atendimento_campus.size() == 0 ) {
			std::cout << "Ainda nao existe nenhum CAMPUS adicionado a base.\n\tCAMPUS \"" <<
				campusId << "\" com id \"" << id << "\"  adicionado." << std::endl;
			at_campus = new AtendimentoCampus;
			at_campus->setId(id);
			at_campus->campus_id = campusId;
			atendimento_campus.add(at_campus);
		}
		else {
			bool addCampus = true;
			ITERA_GGROUP(it_campus, atendimento_campus,AtendimentoCampus) {
				if(it_campus->getId() == id ) {
					std::cout << "O id \"" << id << "\" especificado, do CAMPUS \"" 
						<< campusId << "\"  ja existe." << std::endl;
					addCampus = false;
					break;
				}
			}
			if(addCampus) {
				std::cout << "O id \"" << id << "\" especificado, do CAMPUS \"" 
						<< campusId << "\" nao consta na base de dados. \n\tAdicionando .. ." << std::endl;
				at_campus = new AtendimentoCampus;
				at_campus->setId(id);
				at_campus->campus_id = campusId;
				atendimento_campus.add(at_campus);
			}
		}
	};
	// <<<

};

std::ostream& operator << (std::ostream& out, ProblemSolution& solution );

#endif
