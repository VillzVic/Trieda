#pragma once
#include "ofbase.h"

#include "AreaTitulacao.h"
#include "Unidade.h"
#include "HorarioDisponivel.h"
#include "Disciplina.h"

class Professor :
   public OFBase
{
public:
   Professor(void);
   ~Professor(void);


   virtual void le_arvore(ItemProfessor& elem);

   //TipoContrato* contrato; 
   //TipoTitulacao* titulacao;
   AreaTitulacao* area_titulacao;
   std::string cpf;
   std::string nome;
   int ch_min;
   int ch_max;
   int ch_anterior;
   double valor_credito;
   GGroup<Unidade*> unidades;
   GGroup<HorarioDisponivel*> horarios;
   //GGroup<Magisterio*> magisterio;
};
