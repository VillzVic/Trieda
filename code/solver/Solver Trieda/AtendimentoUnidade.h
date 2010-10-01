#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoSala.h"

using namespace std;

class AtendimentoUnidade:
	public OFBase
{
public:
	AtendimentoUnidade(void);
	~AtendimentoUnidade(void);

	std::string unidade_id;
	GGroup<AtendimentoSala*> atendimentos_salas;

	//virtual void escreve_arvore(ItemAtendimentoUnidade& elem);

	// >>>

	static GGroup<int/*ids de unidades existentes*/> *__ids_cadastrados;

	void addSala(int id, std::string descricao, int __id)
	{
		std::cout << ">> > >>" << std::endl;
		if(__ids_cadastrados->size() == 0){
			std::cout << "Ainda nao existe nenhuma UNIDADE registrada. Para registrar uma SALA," <<
				"registre uma UNIDADE antes." << std::endl;
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
				AtendimentoSala *__at;
				if( atendimentos_salas.size() == 0 ) {
					std::cout << "Ainda nao existe nenhuma SALA da UNIDADE \"" << __id << "\" adicionada a base.\n\tSALA \"" <<
						descricao << "\" com id \"" << id << "\"  adicionada." << std::endl;
					__at = new AtendimentoSala;
					__at->setId(id);
					__at->sala_id = descricao;
					// >>>
					__at->__ids_cadastrados->add(id);
					// <<<
					atendimentos_salas.add(__at);
				}
				else {
					bool __add = true;
					ITERA_GGROUP(it_sala, atendimentos_salas,AtendimentoSala) {
						if(it_sala->getId() == id ) {
							std::cout << "O id \"" << id << "\" especificado, da SALA \"" 
								<< descricao << "\"  ja existe." << std::endl;
							__add = false;
							break;
						}
					}
					if(__add) {
						std::cout << "O id \"" << id << "\" especificado, da SALA \"" 
							<< descricao << "\" nao consta na UNIDADE \"" << __id << "\". \n\tAdicionando .. ." << std::endl;
						__at = new AtendimentoSala;
						__at->setId(id);
						__at->sala_id = descricao;
						// >>>
						__at->__ids_cadastrados->add(id);
						// <<<
						atendimentos_salas.add(__at);
					}
				}
			}
			else {
				//std::cout << "O id \"" << __id << " informado para adicao da SALA \"" << descricao << "\" a UNIDADE \"" << __id << "\"  nao existe." << std::endl;
				std::cout << "Id \"" << __id << "\" de UNIDADE invalido." << std::endl;
			}
		}
	}

	// <<<

};

std::ostream& operator << (std::ostream& out, AtendimentoUnidade& unidade);