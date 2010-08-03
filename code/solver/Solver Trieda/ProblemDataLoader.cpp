#include "ProblemDataLoader.h"
#include "ProblemSolution.h"
#include "TRIEDA-InputXSD.h"
#include <iostream>

using namespace std;

ProblemDataLoader::ProblemDataLoader( char *inputFile, ProblemData* data )
{
   std::cout << "DataLoader" << std::endl;
   this->inputFile = inputFile;
   this->problemData  = data;
}

ProblemDataLoader::~ProblemDataLoader()
{
}

void ProblemDataLoader::load()
{
   std::cout << "Loading" << std::endl;
   root = std::auto_ptr<Trieda>(Trieda_(inputFile, xml_schema::flags::dont_validate));
   std::cout << "ate encontrou o arquivo..." << std::endl;
   problemData->le_arvore(*root);
}
