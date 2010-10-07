#ifndef PROBLEMDATALOADER_H
#define PROBLEMDATALOADER_H

/* O XSD Code Synthesis gera uma estrutura de dados para listas de
   objetos que � um pouco estranha e contra-intuitiva. A macro abaixo
   gera uma itera��o por ela que pode ser representada por um c�digo
   mais leg�vel e intuitivo, baseada nos padr�es de projeto escolhidos */
#ifndef ITERA_SEQ
#define ITERA_SEQ(it,addr,type) for (Grupo##type##::##type##_iterator it = \
   (addr).##type##().begin(); it != (addr).##type##().end(); ++it) 
#endif

#include "ProblemData.h"
#include "input.h"
#include "TRIEDA-InputXSD.h"

class ProblemDataLoader
{
public:
   // Constructor
   ProblemDataLoader( char *inputFile, ProblemData* data );

   // Destructor
   ~ProblemDataLoader();

   // Load the XML file
   void load();
   void divideDisciplinas();
   void gera_refs();
   template<class T> 
   void find_and_set(int id, GGroup<T*>& haystack, T*& needle, bool print) ;
   void cria_blocos_curriculares();
   void calcula_demandas();
   void estima_turmas();
   void print_stats();
   void cache();
   void print_csv();

private:
   // Input data object of the problem
   ProblemData *problemData;
   char *inputFile;

   // XML parser
   std::auto_ptr<TriedaInput> root;

};

#endif
