#pragma once
#include "ofbase.h"
#include "Magisterio.h"
#include "TipoContrato.h"
#include "TipoTitulacao.h"
#include "AreaTitulacao.h"

class Professor :
   public OFBase
{
public:
   Professor(void);
   ~Professor(void);


   virtual void le_arvore(ItemProfessor& elem);

   std::string cpf;
   std::string nome;
   int tipo_contrato_id;
   int ch_min;
   int ch_max;
   int ch_anterior;
   int titulacao_id;
   int area_id;
   double valor_credito;
   GGroup<Horario*> horarios;
   GGroup<Magisterio*> magisterio;
   TipoContrato* tipo_contrato;
   TipoTitulacao* titulacao; 
   AreaTitulacao* area;
};
