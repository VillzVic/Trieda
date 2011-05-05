#include "AtendimentoOfertaSolucao.h"

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao(void)
{
}

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao( AtendimentoOferta & at_Oft )
{
	ofertaCursoCampiId = at_Oft.oferta->getCursoId();
    disciplinaId= at_Oft.getDisciplinaId();
    quantidade = at_Oft.getQuantidade();

	std::stringstream str;
    str << at_Oft.getTurma();

    turma = str.str();
}

AtendimentoOfertaSolucao::~AtendimentoOfertaSolucao(void)
{
}

void AtendimentoOfertaSolucao::le_arvore(ItemAtendimentoOfertaSolucao& elem)
{
	ofertaCursoCampiId = elem.ofertaCursoCampiId();
	disciplinaId = elem.disciplinaId();
	quantidade = elem.quantidade();
	turma = elem.turma();
}
