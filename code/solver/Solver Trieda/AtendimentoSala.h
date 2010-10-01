#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoDiaSemana.h"

using namespace std;

class AtendimentoSala:
	public OFBase
{
public:
	AtendimentoSala(void);
	~AtendimentoSala(void);

	std::string sala_id;
	GGroup<AtendimentoDiaSemana*> atendimentos_dias_semana;

	//virtual void escreve_arvore(ItemAtendimentoUnidade& elem);

	static GGroup<int/*ids de salas existentes*/> *__ids_cadastrados;

	void addDiaSemana(int id, std::string descricao, int __id)
	{
		std::cout << ">> > >>" << std::endl;
		if(__ids_cadastrados->size() == 0){
			std::cout << "Ainda nao existe nenhuma SALA registrada. Para registrar um DIA DA SEMANA," <<
				"registre uma SALA antes." << std::endl;
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
				AtendimentoDiaSemana *__at;
				if( atendimentos_dias_semana.size() == 0 ) {
					std::cout << "Ainda nao existe nenhum DIA DA SEMANA da SALA \"" << __id << "\" adicionada a base.\n\tDIA DA SEMANA \"" <<
						descricao << "\" com id \"" << id << "\"  adicionado." << std::endl;
					__at = new AtendimentoDiaSemana;
					__at->setId(id);
					//__at->dia_semana = descricao;
					// >>>
					__at->__ids_cadastrados->add(id);
					// <<<
					atendimentos_dias_semana.add(__at);
				}
				else {
					bool __add = true;
					ITERA_GGROUP(it_sala, atendimentos_dias_semana,AtendimentoDiaSemana) {
						if(it_sala->getId() == id ) {
							std::cout << "O id \"" << id << "\" especificado, do DIA DA SEMANA \"" 
								<< descricao << "\"  ja existe." << std::endl;
							__add = false;
							break;
						}
					}
					if(__add) {
						std::cout << "O id \"" << id << "\" especificado, do DIA DA SEMANA \"" 
							<< descricao << "\" nao consta na SALA \"" << __id << "\". \n\tAdicionando .. ." << std::endl;
						__at = new AtendimentoDiaSemana;
						__at->setId(id);
						//__at->sala_id = descricao;
						// >>>
						__at->__ids_cadastrados->add(id);
						// <<<
						atendimentos_dias_semana.add(__at);
					}
				}
			}
			else {
				//std::cout << "O id \"" << __id << " informado para adicao da SALA \"" << descricao << "\" a UNIDADE \"" << __id << "\"  nao existe." << std::endl;
				std::cout << "Id \"" << __id << "\" de SALA invalido." << std::endl;
			}
		}
	}

	// <<<


};

std::ostream& operator << (std::ostream& out, AtendimentoSala& sala);