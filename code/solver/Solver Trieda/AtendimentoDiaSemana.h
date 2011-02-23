#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h"

using namespace std;

class AtendimentoDiaSemana:
   public OFBase
{
public:
   AtendimentoDiaSemana(void);
   ~AtendimentoDiaSemana(void);

   int dia_semana;
   GGroup<AtendimentoTatico*> atendimentos_tatico;
   GGroup<AtendimentoTurno*> atendimentos_turno;

   //virtual void escreve_arvore(ItemAtendimentoUnidade& elem);

   //std::pair<int/*id_sala*/,int/*dia*/> key;

   //std::pair<int,int> getPairId_SalaDia() {return key;}
   //int getIdSala() { return key.first; }
   //int getIdDia() { return key.second; }

   //static std::vector<GGroup<int/*ids das salas que possuem o dia (dado pela posicao do vector)*/> > *__ids_cadastrados;

};

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem);