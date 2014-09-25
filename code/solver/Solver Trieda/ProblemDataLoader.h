#ifndef _PROBLEM_DATA_LOADER_H_
#define _PROBLEM_DATA_LOADER_H_

// O XSD Code Synthesis gera uma estrutura de dados para listas de
// objetos que é um pouco estranha e contra-intuitiva. A macro abaixo
// gera uma iteração por ela que pode ser representada por um código
// mais legível e intuitivo, baseada nos padrões de projeto escolhidos

#ifndef ITERA_SEQ
#define ITERA_SEQ( it, addr, type ) for ( Grupo##type##::##type##_iterator it = \
   ( addr ).##type##().begin(); it != ( addr ).##type##().end(); ++it ) 
#endif

#include "ProblemData.h"
#include "TRIEDA-InputXSD.h"

class ProblemDataLoader
{
public:
   // Constructor
   ProblemDataLoader( char *, ProblemData * );

   // Destructor
   virtual ~ProblemDataLoader();

   // Load the XML file
   void load();

   void printInputDataLog();

   void referenciaTurnos();
   void referenciaHorariosAula();
   void relacionaCalendarioHorariosAula();
   void relacionaDiscCalendariosOrig();
   void geraRefsTipoSala();
   void geraRefsDemandas();
   void geraRefsOfertas();
   void associaAlunoComAlunoDemanda();
   void referenciaDisciplinasEquivalentesIncompativeis();
   void referenciaDisciplinasCurriculos();
   void preencheTempoAulaHorarios();

   // Para cada dia da semana, relaciona os
   // seus respectivos horários de aula disponíveis
   void relacionaHorariosAulaDiaSemana();

   // Carrega os dias letivos dos Campus, Unidades e Salas.
   void carregaDiasLetivosCampusUnidadeSala();

   // Carrega os dias letivos de cada disciplina.
   void carregaDiasLetivosDiscs();

   // Cria os Conjuntos de Salas para cada Unidade.
   void criaConjuntoSalasUnidade();
   
   // Establece os dias letivos comuns entre as salas e suas disciplinas associadas.
   void estabeleceDiasLetivosDisciplinasSalas();
   
   // Establece os dias letivos comuns entre os professores e suas disciplinas.
   void estabeleceDiasLetivosProfessorDisciplina();
   
   void referenciaCursos_CurricCalendarios();

   void referenciaCalendariosCurriculos();

   // Trata das equivalências entre disciplinas
   void referenciaDisciplinasEquivalentes();

   void substituiDisciplinasEquivalentes();
   void atualizaOfertasDemandas();

   /* */
   void divideDisciplinas();

   /* */
   void geraHorariosDia();

   /* */
   //template< class T > 
   //void find_and_set( int, GGroup< T *, LessPtr< T > > &, T * &, bool );

   template< class T > 
   void find_and_set_lessptr( int, GGroup< T *, LessPtr< T > > &, T * &, bool );

   /* */
   void gera_refs();

   /* */
   void cria_blocos_curriculares();

   /* */
   void calculaMenorCapacSalaPorDisc();

   /* */
   void calculaCapacMediaSalaPorDisc();

   /* */
   void estima_turmas();
   
   /* */
   void estima_turmas_sem_compart();

   void estima_turmas_com_compart();

   void estima_turmas_com_compart_ordena_cap_salas();

   /* */
   void print_stats();

   /* */
   void relacionaDemandaAlunos();

   /* */
   void cache();

   /* */
#ifndef HEURISTICA
   void criaAulas();
#endif

   /* */
   void calculaMaxHorariosProfessor();

   // Cria um vetor ordenado dos horários de
   // aula ( todos os horários de todos os professores )
   void criaListaHorariosOrdenados();

   /* */
   void disciplinasCursosCompativeis();

   // Preenche o atributo 'cursosComp_disc' de problemData
   void preencheDisciplinasDeCursosCompativeis();

   // Preenche o atributo 'oftsComp_disc' de problemData
   void preencheDisciplinasDeOfertasCompativeis();

   // Divide as fixações de acordo com seus tipos
   void relacionaFixacoes();

   // Verifica se já existe uma fixação equivalente
   bool contemFixacao( GGroup< Fixacao *, LessPtr< Fixacao > > ,
      Professor *, Disciplina *, Sala *, int, HorarioAula * );

   // Informa se já não existe uma fixação contendo exatamente os mesmos dados
   bool contemFixacaoExato( GGroup< Fixacao *, LessPtr< Fixacao > > ,
      Professor *, Disciplina *, Sala *, int, HorarioAula * );

   // Cria uma nova fixação com os dados informados
   Fixacao * criaFixacao( int, Professor *,
      Disciplina *, Sala *, int, HorarioAula * );

   // Deleta fixações que relacionam disciplinas que foram substituídas
   void removeFixacoesComDisciplinasSubstituidas();   

   // Associa corretamente fixações que relacionam salas a disciplinas
   // que possuem creditos praticos
   void corrigeFixacoesComDisciplinasSalas();

   // Quando houver uma fixação de uma disciplina em um dia
   // da semana, esse método remove os demais dias da semana
   // no conjunto de dias letivos da disciplina em questão
   void verificaFixacoesDiasLetivosDisciplinas();

   // Quando houver uma disciplina que use laboratórios e também
   // existir uma fixação dessa disciplina, devemos criar a fixação
   // correta para a disciplina dividida (créditos práticos)
   void criaFixacoesDisciplinasDivididas();

   GGroup< int > retorna_foxacoes_dias_letivos( Disciplina * );

   bool existe_conjunto_sala__fixacao( Unidade *, Disciplina *, Sala * );

private:
   ProblemData * problemData;
   char * inputFile;
   std::auto_ptr< TriedaInput > root;

   void relacionaEquivalenciasDisciplinasPraticas();
   void addEquivDisciplinasPraticasEmTeoricas();

   void referenciaCampusUnidadesSalas();
   void referenciaDisciplinas();
   void referenciaOfertas();
   void referenciaProfessores();
   void calculaTamanhoMedioSalasCampus();

   // Armazena todas as disciplinas ofertadas em um campus.
   void relacionaCampusDiscs();

   // Calcula a demanda máxima e demanda total da disciplina
   // em questão. Esses valores são armazenados na própria disciplina.
   void calculaDemandas();

   // Armazena em cada sala, referências para as disciplinas
   // associadas. Armazena também, para cada disciplina,
   // ponteiros para as salas compatíveis.
   void associaDisciplinasSalas();

   // Armazena, para cada disciplina, ponteiros para as salas compatíveis.
   void carregaDiscSalas();

   // Armazena referências para as disciplinas associadas
   // em cada elemento do <ConjuntoSala>. Como o método
   // associaDisciplinasSalas() já associou as disciplinas
   // às salas, basta percorrer o map de salas de um dado
   // elemento do <ConjuntoSala> adicionando as salas ao
   // map de disciplinas associadas do <ConjuntoSala>.
   void associaDisciplinasConjuntoSalas();

   // Relaciona cada disciplina com as ofertas em que ela aparece.
   void relacionaDiscOfertas();

   // Relaciona cada disciplina de um campus com as ofertas em que ela aparece.
   //void relacionaDiscCampusOfertas();

   // Inicializando as estruturas presentes em cada Sala, que são responsáveis por 
   // informar a quantidade de créditos livres em um dado dia letivo.
   void calculaCredsLivresSalas();

   // Conjunto de combinações possíveis de divisão de créditos de uma uma disciplina
   void combinacaoDivCreditos();

   // Relaciona cada possível valor de crédito com as regras de créditos existentes.
   void relacionaCredsRegras();

   // Relaciona cada curso com o seu respectivo campus
   void relacionaCursosCampus();

   int retornaMaiorIdDemandas();

   int retornaMaiorIdAlunoDemandas();

   // Para o caso em que há uma solução do módulo tático
   // e deseja-se executar o módulo operacional. Deve-se
   // verificar na solução informada se existe alguma disciplina
   // que foi dividida está alocada com id positivo em um laboratório.
   // Se isso acontecer, tem que converter o id da disciplina.

#ifndef HEURISTICA

   void validaInputSolucaoTatico();

#endif

   void relacionaProfessorDisciplinasAssociadas( void );

   void calculaMaxTempoDisponivelPorSala();
   void calculaMaxTempoDisponivelPorSalaPorSL();
      
   void preencheHashHorarioAulaDateTime();

   void calculaCHOriginalPorAluno();

};



struct DivCredType
{
	std::map< int, int > diaNCred;

	DivCredType(){}

	DivCredType( std::map< int, int > &otherMap )
	{
		for ( auto itOther = otherMap.begin();
			  itOther != otherMap.end();
			  itOther++ )
		{
			diaNCred[itOther->first] = itOther->second;
		}
	}

	bool operator<( const DivCredType& other ) const
    {
		if ( this->diaNCred.size() < other.diaNCred.size() )
			return true;
		if ( this->diaNCred.size() > other.diaNCred.size() )
			return false;
				
		for ( auto itThis = diaNCred.begin(), itOther = other.diaNCred.begin();
			  itThis != diaNCred.end();
			  itThis++, itOther++ )
		{
			if ( itThis->first < itOther->first )
				return true;
			if ( itThis->first > itOther->first )
				return false;
			if ( itThis->second < itOther->second )
				return true;
			if ( itThis->second > itOther->second )
				return false;
		}

		return false;
    }

	DivCredType& operator=( const DivCredType& other )
    {
		diaNCred.clear();

		for ( auto itOther = other.diaNCred.begin();
			  itOther != other.diaNCred.end();
			  itOther++ )
		{
			diaNCred[itOther->first] = itOther->second;
		}

		return *this;
	}

};

// Override templates
namespace std
{
	template<>
	struct less<DivCredType>
	{
		bool operator() (DivCredType const first, DivCredType const second) const
		{
			return first < second;
		}
	};
};


#endif
