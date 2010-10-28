#pragma once
#include "ofbase.h"
#include "TipoSala.h"
#include "horario.h"
#include "CreditoDisponivel.h"

#include "Disciplina.h"

class Sala :
	public OFBase
{
public:
	Sala(void);
	~Sala(void);
	//private:
	std::string codigo;
	std::string andar;
	int numero;   
	int tipo_sala_id; //alterar para ref, depois
	int capacidade;
	GGroup<Horario*> horarios_disponiveis;
	GGroup<CreditoDisponivel*> creditos_disponiveis;
	GGroup<int> disciplinas_associadas; //alterar para ref, depois

   // >>> Dados para pré processamento.

   /* Esta estrutura vai, futuramente, substituir a 
   estrutura <GGroup<int> disciplinas_associadas> */
   GGroup<Disciplina*> disciplinasAssociadas;
   // <<<

public:
	virtual void le_arvore(ItemSala& elem);

	int max_creds(int dia);

	TipoSala* tipo_sala;

	// >>>
	int id_unidade;
	// <<<

   
   // =========== METODOS SET

   // =========== METODOS GET
   int getTipoSalaId() const { return tipo_sala_id; }

   //unsigned getCapacidade() const { return capacidade; }


   // ToDo : implementar os métodos básicos para todos os membros da classe.

	//GGroup<Disciplina*> disc_assoc_PT;

};
