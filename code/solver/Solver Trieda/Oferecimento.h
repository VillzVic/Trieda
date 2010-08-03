#pragma once
#include "ofbase.h"
#include "Turno.h"

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
   Turno* turno;
   Disciplina* disciplina;
   Sala* sala;
   int dia_da_semana;
   int creditos;
};
