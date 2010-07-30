#ifndef PROBLEMDATALOADER_H
#define PROBLEMDATALOADER_H

#include "ProblemData.h"

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
   // Input data object of the problem
   ProblemData *problemData;
};

#endif
