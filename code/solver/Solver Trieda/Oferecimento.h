#pragma once
#include "ofbase.h"
#include "Turno.h"
//#include "TRIEDA-OutputXSD.h"

class Disciplina;
#include "Disciplina.h"
#include "Sala.h"

class Turma;
#include "Turma.h"

class Oferecimento :
   public OFBase
{
public:
   Oferecimento(void);
   ~Oferecimento(void);
private:
   Turma* turma;
   Turno* turno;
   Disciplina* disciplina;
   Sala* sala;
   int dia_da_semana;
   int creditos;
public:
   virtual void escreve_arvore(ItemOferecimento& elem);
};
