#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include <iostream>

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
   std::string filename = "HAha.xml";
   std::auto_ptr<Trieda> root;
   root = std::auto_ptr<Trieda>(Trieda_(filename.c_str(), xml_schema::flags::dont_validate));
//   std::auto_ptr<cenario> root;
//   root = std::auto_ptr<cenario>(cenario_(filename.c_str(), xml_schema::flags::dont_validate));
//   root->cadeia().get().capacidadesEstocagemEloProduto().get().capacidadeEstocagemEloProduto().begin()
   root->unidades().ItemUnidade().begin()

}

