#ifndef _OFERECIMENTO_H_
#define _OFERECIMENTO_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"
#include "Tatico.h"
#include "Operacional.h"

class Oferecimento :
   public OFBase
{
public:
   Oferecimento( void );
   virtual ~Oferecimento( void );

   int oferta_curso_campi_id;
   int disciplina_id;
   std::string turma;
   int quantidade_alunos;
   int sala_id;
   int dia_semana;
   Tatico * alocacao_tatico;
   Operacional * alocacao_operacional;

   // virtual void escreve_arvore( ItemAtendimento & );
   // virtual void escreve_arvore();
};

#endif
