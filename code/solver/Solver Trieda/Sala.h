#pragma once
#include "ofbase.h"
#include "TipoSala.h"
#include "Horario.h"
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
   std::string numero;   
   //int numero;   
	int tipo_sala_id; //alterar para ref, depois
	int capacidade;
	GGroup<Horario*> horarios_disponiveis;
	GGroup<CreditoDisponivel*> creditos_disponiveis;
	GGroup<int> disciplinas_associadas; //alterar para ref, depois

   /* Conjunto de disciplinas que foram associadas a sala em questão pelo usuario. */
   GGroup<Disciplina*> disciplinas_Associadas_Usuario;
   
   /* Conjunto de disciplinas que foram associadas a sala em questão
   por um pré processamento realizado. Aqui, as associãções são criadas
   independentemente da unidade em que uma sala se encontra. */
   GGroup<Disciplina*> disciplinas_Associadas_AUTOMATICA;

   /* UNIAO dos conjuntos acima. */
   GGroup<Disciplina*> disciplinasAssociadas;

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


   /* Estrutura utilizada no pós-processamento de dados para auxiliar a alocação dos créditos indicados pelas 
   variáveis x às salas. */
   //std::map<int/*dia*/,int/*credsLivres*/> credsLivres;
   //std::vector<int/*credsLivres*/> credsLivres;

   /* Armazena os dias letivos para uma determinada sala */
   GGroup<int> diasLetivos;

};
