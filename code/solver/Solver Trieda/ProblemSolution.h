#ifndef PROBLEMSOLUTION_H
#define PROBLEMSOLUTION_H

#include <ostream>
#include "GGroup.h"
#include "ProblemData.h"

//#include "Oferecimento.h"
#include "AtendimentoCampus.h"
#include "RestricaoViolada.h"

using namespace std;

//#define DBG_CP

//Stores output data
class ProblemSolution
{
public:
   //Constructor
   ProblemSolution();

   //Destructor
   ~ProblemSolution(); 

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

   RestricaoVioladaGroup* getFolgas() const { return folgas; }

   /**
   ToDo:
   All get methods of the private attributes should be defined here
   */

   private:
	   RestricaoVioladaGroup* folgas;
   public:

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
#ifdef DBG_CP
      std::cout << ">> > >>" << std::endl;
#endif
      if( atendimento_campus.size() == 0 ) {
#ifdef DBG_CP
         std::cout << "Ainda nao existe nenhum CAMPUS adicionado a base.\n\tCAMPUS \"" <<
            campusId << "\" com id \"" << id << "\"  adicionado." << std::endl;
#endif

         at_campus = new AtendimentoCampus;
         at_campus->setId(id);
         at_campus->campus_id = campusId;
         // >>>
         at_campus->campi_id->add(id);
         // <<<
         atendimento_campus.add(at_campus);
      }
      else {
         bool addCampus = true;
         ITERA_GGROUP(it_campus, atendimento_campus,AtendimentoCampus) {
            if(it_campus->getId() == id ) {
#ifdef DBG_CP
               std::cout << "O id \"" << id << "\" especificado, do CAMPUS \"" 
                  << campusId << "\"  ja existe." << std::endl;
#endif
               addCampus = false;
               break;
            }
         }
         if(addCampus) {
#ifdef DBG_CP
            std::cout << "O id \"" << id << "\" especificado, do CAMPUS \"" 
               << campusId << "\" nao consta na base de dados. \n\tAdicionando .. ." << std::endl;
#endif
            at_campus = new AtendimentoCampus;
            at_campus->setId(id);
            at_campus->campus_id = campusId;
            // >>>
            at_campus->campi_id->add(id);
            // <<<
            atendimento_campus.add(at_campus);
         }
      }
   };
   // <<<

//private:

   /* Armazena todas as salas (sala de aula ou lab) em que uma disciplina pode ser oferecida. Pode ser que
   uma disciplina seja associada à duas salas com capacidades diferentes. Qdo o solver alocar essa disciplina
   a um tipo de sala, a outra sala não deverá constar na lista de salas em que essa disciplina poderá 
   ser alocada. Portanto, a estrutura abaixo, visa armazenar todas as salas em que uma disciplina 
   pode ser alocada levando em conta o resultado do solver. */
   //std::map<int/*Id Disc*/,GGroup<Sala*> > discSalas;

};

std::ostream& operator << (std::ostream& out, ProblemSolution& solution );

#endif
