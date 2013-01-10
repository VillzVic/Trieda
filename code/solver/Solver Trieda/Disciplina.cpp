#include "Disciplina.h"

Disciplina::Disciplina(void)
{
   demanda_total = 0;
   max_alunos_t = -1;
   max_alunos_p = -1;

   divisao_creditos.clear();
   tipo_disciplina = NULL;
   nivel_dificuldade = NULL;
   calendario = NULL;

   max_demanda = -1;
   num_turmas = -1;
   min_creds = -1;
   max_creds = -1;
   nSalasAptas = 0;

}

Disciplina::~Disciplina(void)
{

}

void Disciplina::le_arvore( ItemDisciplina & elem )
{
   this->setId( elem.id() );
   codigo = elem.codigo();
   nome = elem.nome();
   cred_teoricos = elem.credTeoricos();
   cred_praticos = elem.credPraticos();
   max_creds = cred_teoricos + cred_praticos;
   e_lab = elem.laboratorio();

   if ( elem.maxAlunosTeorico().present() )
   {
      max_alunos_t = elem.maxAlunosTeorico().get();
   }

   if ( elem.maxAlunosPratico().present() )
   {
      max_alunos_p = elem.maxAlunosPratico().get();
   }

   tipo_disciplina_id = elem.tipoDisciplinaId();
   nivel_dificuldade_id = elem.nivelDificuldadeId();

   divisao_creditos.clear();
   if ( elem.divisaoDeCreditos().present() )
   {
	   DivisaoCreditos * divisao = new DivisaoCreditos();
	   divisao->le_arvore( elem.divisaoDeCreditos().get() );

	   divisao_creditos.add( divisao );
   }

   ITERA_NSEQ( it_contem, elem.disciplinasEquivalentes(), id, Identificador )
   {
      ids_disciplinas_equivalentes.add( *it_contem );
   }

   ITERA_NSEQ( it_inc, elem.disciplinasIncompativeis(), id, Identificador )
   {
      ids_disciplinas_incompativeis.add( *it_inc );
   }

   ITERA_SEQ( it_hora, elem.horariosDisponiveis(), Horario )
   {
      Horario * horario = new Horario();
      horario->le_arvore( *it_hora );
      horarios.add( horario );
   }
}

int Disciplina::getMenorCapacSala( int campusId ) 
{
	if ( this->menorCapacSala.find( campusId ) != this->menorCapacSala.end()  )
		return this->menorCapacSala[campusId];
	else
		return -1;
}


int Disciplina::getCapacMediaSala( int campusId )
{
	if ( this->capacMediaSala.find( campusId ) != this->capacMediaSala.end()  )
		return this->capacMediaSala[campusId];
	else
		return 0;
}

int Disciplina::getTempoTotalSemana()
{
   int tempoDisciplina = 0;
   for (GGroup< int >::iterator itD = diasLetivos.begin();
      itD != diasLetivos.end();
      itD++)
   {
      tempoDisciplina += calendario->getTempoTotal(*itD);
   }

   return tempoDisciplina;
}

int Disciplina::getMaxTempoDiscEquivSubstituta()
{
   int tempoMax = 0;
   
   ITERA_GGROUP_LESSPTR( it, discEquivSubstitutas, Disciplina )
   {
	   int t = (*it)->getTotalCreditos() * (*it)->getTempoCredSemanaLetiva();
	   if ( t > tempoMax )
		   tempoMax = t;
   }

   return tempoMax;
}

bool Disciplina::inicioTerminoValidos( HorarioAula *&hi, HorarioAula *&hf )
{	
	// Procura, caso já tenho sido calculado
	std::pair<HorarioAula*, HorarioAula*> parHorarios( hi, hf );
	if ( this->horarios_hihf_validos.find( parHorarios ) != this->horarios_hihf_validos.end() )
	{
		return this->horarios_hihf_validos[ parHorarios ];
	}	

	// Não foi calculado ainda. Calcula e insere no map horarios_hihf_validos.

	if ( *hf < *hi )
	{
		this->horarios_hihf_validos[ parHorarios ] = false;
		return false;
	}

	if ( *hf == *hi )
	{
		// Não permite que uma disciplina com nro par de creditos
		// tenha uma aula com somente 1 credito, a não ser que 
		// esteja determinado por regra de divisão de créditos
		if ( this->getTotalCreditos() % 2 == 0 )
		{
			bool CRIAR = false;
			std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
				it1 = this->combinacao_divisao_creditos.begin();
			for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
			{
				std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
					it2 = (*it1).begin();
				for ( ; it2 != (*it1).end(); it2++ )
				{	
					if ( (*it2).second == 1 )
					{
						CRIAR = true; break;
					}
				}
				if ( CRIAR ) break;										
			}
			if ( !CRIAR ){ this->horarios_hihf_validos[ parHorarios ] = false; return false; }
		}
	}

	int nIntervalo = this->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
	if ( nIntervalo > this->getTotalCreditos() )
	{
		this->horarios_hihf_validos[ parHorarios ] = false;
		return false;
	}

	if ( this->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf ) == 2 &&
		 this->getCalendario()->intervaloEntreHorarios(hi,hf) )
	{
		this->horarios_hihf_validos[ parHorarios ] = false;
		return false;
	}

	if ( this->combinacao_divisao_creditos.size() > 0 )
	{
		bool CRIAR = false;
		int nCredHor = this->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
		std::vector< std::vector< std::pair< int /*dia*/, int /*nCreds*/ > > >::iterator 
			it1 = this->combinacao_divisao_creditos.begin();
		for ( ; it1 != this->combinacao_divisao_creditos.end(); it1++ )
		{
			std::vector< std::pair< int /*dia*/, int /*nCreds*/ > >::iterator
				it2 = (*it1).begin();
			for ( ; it2 != (*it1).end(); it2++ )
			{	
				if ( (*it2).second == nCredHor )
				{
					CRIAR = true; break;
				}
			}
			if ( CRIAR ) break;										
		}
		if ( !CRIAR ){ this->horarios_hihf_validos[ parHorarios ] = false; return false; }
	}

	this->horarios_hihf_validos[ parHorarios ] = true;

	return true;
}