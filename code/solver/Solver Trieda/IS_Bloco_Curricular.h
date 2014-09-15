#ifndef _IS_BLOCO_CURRICULAR_H_
#define _IS_BLOCO_CURRICULAR_H_

#include <iostream>
#include <map>

#include "BlocoCurricular.h"
#include "Demanda.h"
#include "Disciplina.h"

class IS_Bloco_Curricular
{
public:
   IS_Bloco_Curricular( BlocoCurricular * );
   IS_Bloco_Curricular( IS_Bloco_Curricular const & );
   virtual ~IS_Bloco_Curricular( void );

   BlocoCurricular * bloco;

   // Informa a demanda ainda não atendida para cada disciplina do bloco em questão.
   std::map< Disciplina *, int /*Demanda nao atendida*/ > disciplina_Dem_Nao_Atendida;

   // FLAG que indica se o is_Bloco em questão já foi atendido. Mais especificamente,
   // esse flag informa se o is_Bloco em questão já teve a sua chance de ser atendido.
   // bool atendido;

   // Informa a demanda total de um dado IS_Bloco
   int demanda_Total;

   // Operadores
   bool operator < ( IS_Bloco_Curricular const & );
   bool operator == ( IS_Bloco_Curricular const & );
};

#endif