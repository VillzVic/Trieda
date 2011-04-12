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
   //Professor(void);
	Professor(bool = false);
   ~Professor(void);

   virtual void le_arvore(ItemProfessor &);

   GGroup< Horario * > horarios;

   // Esse eh o disciplinas do XSD de input
   GGroup< Magisterio * > magisterio;

   TipoContrato * tipo_contrato;
   TipoTitulacao * titulacao; 
   AreaTitulacao * area;

   void setCpf(std::string s) { cpf = s; }
   void setNome(std::string s) { nome = s; }
   void setTipoContratoId(int s) { tipo_contrato_id = s; }
   void setChMin(int s) { ch_min = s; }
   void setChMax(int s) { ch_max = s; }
   void setChAnterior(int s) { ch_anterior = s; }
   void setTitulacaoId(int s) { titulacao_id = s; }
   void setAreaId(int s) { area_id = s; }
   void setValorCredito(int s) { valor_credito = s; }
   void setIdOperacional(int s) { id_operacional = s; }
   void setCustoDispProf(int c) { custoDispProf = c; }
   void setIsVirtual(bool value) { is_virtual = value; }

   std::string getCpf() const { return cpf; }
   std::string getNome() const { return nome; }
   int getTipoContratoId() const { return tipo_contrato_id; }
   int getChMin() const { return ch_min; }
   int getChMax() const { return ch_max; }
   int getChAnterior() const { return ch_anterior; }
   int getTitulacaoId() const { return titulacao_id; }
   int getAreaId() const { return area_id; }
   double getValorCredito() const { return valor_credito; }
   int getIdOperacional() const { return id_operacional; }
   int getCustoDispProf() const { return custoDispProf; }
   bool eVirtual() const { return is_virtual; }

private:
   std::string cpf;
   std::string nome;
   int tipo_contrato_id;
   int ch_min;
   int ch_max;
   int ch_anterior;
   int titulacao_id;
   int area_id;
   double valor_credito;
   int id_operacional;

   // Utilizado na função de prioridade para o modelo operacional.
   int custoDispProf;
   bool is_virtual;
};
