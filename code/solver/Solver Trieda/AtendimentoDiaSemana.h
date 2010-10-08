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

	std::pair<int/*id_sala*/,int/*dia*/> key;

	std::pair<int,int> getPairId_SalaDia() {return key;}
	int getIdSala() { return key.first; }
	int getIdDia() { return key.second; }

	static std::vector<GGroup<int/*ids das salas que possuem o dia (dado pela posicao do vector)*/> > *__ids_cadastrados;

	/*
	void addTatico(AtendimentoOferta& at_oferta, int creds_teoricos, int creds_praticos, int __id)
	{
	std::cout << ">> > >>" << std::endl;
	if(__ids_cadastrados->size() == 0){
	std::cout << "Ainda nao existe nenhum DIA DA SEMANA registrado. Para registrar um ATENDIMENTO TATICO," <<
	"registre uma DIA DA SEMANA antes." << std::endl;
	}
	else {
	bool encontrou_id_cadastrado = false;
	GGroup<int>::iterator it__ids_cadastrados = __ids_cadastrados->begin();
	for(; it__ids_cadastrados != __ids_cadastrados->end(); it__ids_cadastrados++ ) {
	if( *it__ids_cadastrados == __id ) {
	encontrou_id_cadastrado = true;
	break;
	}
	}

	if(encontrou_id_cadastrado) {
	AtendimentoTatico *__at;
	if( atendimentos_tatico.size() == 0 ) {
	std::cout << "Ainda nao existe nenhum ATENDIMENTO TATICO do DIA DA SEMANA \"" << __id << "\" adicionado a base.\n\tATENDIMENTO TATICO adicionado." << std::endl;
	__at = new AtendimentoTatico;
	__at->atendimento_oferta = &at_oferta;
	__at->qtde_creditos_teoricos = creds_teoricos;
	__at->qtde_creditos_praticos = creds_praticos;
	//__at->setId(id);
	//__at->dia_semana = descricao;
	// >>>
	//__at->__ids_cadastrados->add(id);
	// <<<
	atendimentos_tatico.add(__at);
	}
	else {
	bool __add = true;
	ITERA_GGROUP(it_tatico, atendimentos_tatico,AtendimentoTatico) {
	if(it_tatico->getId() == id ) {
	std::cout << "O id \"" << id << "\" especificado, do ATENDIMENTO TATICO ja existe." << std::endl;
	__add = false;
	break;
	}
	}
	if(__add) {
	std::cout << "O id \"" << id << "\" especificado, do ATENDIMENTO TATICO nao consta no DIA DA SEMANA \"" << __id << "\". \n\tAdicionando .. ." << std::endl;
	__at = new AtendimentoTatico;
	__at->atendimento_oferta = &at_oferta;
	__at->qtde_creditos_teoricos = creds_teoricos;
	__at->qtde_creditos_praticos = creds_praticos;
	//__at->setId(id);
	//__at->sala_id = descricao;
	// >>>
	//__at->__ids_cadastrados->add(id);
	// <<<
	atendimentos_tatico.add(__at);
	}
	}
	}
	else {
	//std::cout << "O id \"" << __id << " informado para adicao da SALA \"" << descricao << "\" a UNIDADE \"" << __id << "\"  nao existe." << std::endl;
	std::cout << "Id \"" << __id << "\" de SALA invalido." << std::endl;
	}
	}
	}
	*/

};

std::ostream& operator << (std::ostream& out, AtendimentoDiaSemana& diaSem);