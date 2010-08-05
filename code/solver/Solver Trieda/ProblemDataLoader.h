#ifndef PROBLEMDATALOADER_H
#define PROBLEMDATALOADER_H

/* O XSD Code Synthesis gera uma estrutura de dados para listas de
   objetos que é um pouco estranha e contra-intuitiva. A macro abaixo
   gera uma iteração por ela que pode ser representada por um código
   mais legível e intuitivo, baseada nos padrões de projeto escolhidos */
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

private:
   void load_unidades(void);
   void load_cursos(void);
   void load_calendario(void);
   void load_professores(void);
   void load_disciplinas(void);
   void load_regras_credito(void);

   // Input data object of the problem
   ProblemData *problemData;
   char *inputFile;

   // XML parser
	std::auto_ptr<Trieda> root;

};

#endif
