#include "AtendimentoOfertaSolucao.h"

#include <string>

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao(void)
{
}

AtendimentoOfertaSolucao::AtendimentoOfertaSolucao( AtendimentoOferta & at_Oft )
{
   ofertaCursoCampiId = at_Oft.oferta->getId();
   quantidade = at_Oft.getQuantidade();

   if ( at_Oft.getDisciplinaSubstitutaId() != NULL )
   {
	   disciplinaId = at_Oft.getDisciplinaSubstitutaId();
	   disciplinaSubstituidaId = at_Oft.getDisciplinaId();
   }
   else
   {
	   disciplinaId = at_Oft.getDisciplinaId();
	   disciplinaSubstituidaId = NULL;
   }

   std::stringstream str;
   str << at_Oft.getTurma();

   turma = str.str();
}

AtendimentoOfertaSolucao::~AtendimentoOfertaSolucao(void)
{
}

void AtendimentoOfertaSolucao::le_arvore(ItemAtendimentoOfertaSolucao& elem)
{
	std::ostringstream s;
	s << elem.disciplinaSubstitutaId();
	std::string str( s.str() );
	
	if ( elem.disciplinaSubstitutaId().present() ) // se tiver o campo disciplinaSubstitutaId
    {
	   disciplinaId = elem.disciplinaSubstitutaId().get();
	   disciplinaSubstituidaId = elem.disciplinaId();
    }
    else
    {
 	   disciplinaId = elem.disciplinaId();
	   disciplinaSubstituidaId = NULL;
    }

	ITERA_NSEQ( itAlunoDemId,elem.alunosDemanda(), id, Identificador )
    {
		alunosDemandasAtendidas.add( *itAlunoDemId );
    }

	ofertaCursoCampiId = elem.ofertaCursoCampiId();
	quantidade = elem.quantidade();
	turma = elem.turma();
}
