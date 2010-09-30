#pragma once
#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

#include "AtendimentoUnidade.h"

class AtendimentoCampus:
	public OFBase
{
public:
	AtendimentoCampus(void);
	~AtendimentoCampus(void);

	std::string campus_id;
	GGroup<AtendimentoUnidade*> atendimentos_unidades;

	//virtual void escreve_arvore(ItemAtendimentoCampus& elem);

	//virtual TriedaOutput& escreve_arvore(TriedaOutput& root);


	// >>>

	// ToDo : adiocionar esse cara ... 
	static GGroup<int/*ids de campus existentes*/> campi_id;

	void addUnidade(int id, std::string unidadeId, int id_campus)
	{
		//if(campi_id.size > 0){
		//if( campi_id.find(id_campus) != GGroup<int>::end() ) {
		AtendimentoUnidade *at_unidade;
		std::cout << ">> > >>" << std::endl;
		if( atendimentos_unidades.size() == 0 ) {
			std::cout << "Ainda nao existe nenhuma UNIDADE do CAMPUS \"" << id_campus << "\" adicionada a base.\n\tUNIDADE \"" <<
				unidadeId << "\" com id \"" << id << "\"  adicionada." << std::endl;
			at_unidade = new AtendimentoUnidade;
			at_unidade->setId(id);
			at_unidade->unidade_id = unidadeId;
			atendimentos_unidades.add(at_unidade);
		}
		else {
			bool addUnidade = true;
			ITERA_GGROUP(it_unidade, atendimentos_unidades,AtendimentoUnidade) {
				if(it_unidade->getId() == id ) {
					std::cout << "O id \"" << id << "\" especificado, da UNIDADE \"" 
						<< unidadeId << "\"  ja existe." << std::endl;
					addUnidade = false;
					break;
				}
			}
			if(addUnidade) {
				std::cout << "O id \"" << id << "\" especificado, da UNIDADE \"" 
					<< unidadeId << "\" nao consta no CAMPUS \"" << id_campus << "\". \n\tAdicionando .. ." << std::endl;
				at_unidade = new AtendimentoUnidade;
				at_unidade->setId(id);
				at_unidade->unidade_id = unidadeId;
				atendimentos_unidades.add(at_unidade);
			}
		}
		//	}
		//std::cout << "O id do CAMPUS \"" << id_campus << "\" informado nao existe." << std::endl;
		//}
	};
	// <<<

};

std::ostream& operator << (std::ostream& out, AtendimentoCampus& campus);