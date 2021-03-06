#ifndef _PROBLEM_DATA_LOADER_H_
#define _PROBLEM_DATA_LOADER_H_

// O XSD Code Synthesis gera uma estrutura de dados para listas de
// objetos que � um pouco estranha e contra-intuitiva. A macro abaixo
// gera uma itera��o por ela que pode ser representada por um c�digo
// mais leg�vel e intuitivo, baseada nos padr�es de projeto escolhidos

#ifndef ITERA_SEQ
#define ITERA_SEQ( it, addr, type ) for ( Grupo##type##::##type##_iterator it = \
   ( addr ).##type##().begin(); it != ( addr ).##type##().end(); ++it ) 
#endif

#include "ProblemData.h"
#include "TRIEDA-InputXSD.h"

struct DivCredType;

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
   // seus respectivos hor�rios de aula dispon�veis
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

   // Trata das equival�ncias entre disciplinas
   void referenciaDisciplinasEquivalentes();

   void substituiDisciplinasEquivalentes();
   void atualizaOfertasDemandas();

   /* */
   void divideDisciplinas();

   /* */
   void geraHorariosDia();

   template<class T>
   void find_and_set_lessptr(int, GGroup<T*, LessPtr<T>>&, T*&, bool);
   template<class T>
   void find_and_set(int, GGroup<T*, Less<T*>>&, T*&, bool);

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

   // Cria um vetor ordenado dos hor�rios de
   // aula ( todos os hor�rios de todos os professores )
   void criaListaHorariosOrdenados();

   /* */
   void disciplinasCursosCompativeis();

   // Preenche o atributo 'cursosComp_disc' de problemData
   void preencheDisciplinasDeCursosCompativeis();

   // Preenche o atributo 'oftsComp_disc' de problemData
   void preencheDisciplinasDeOfertasCompativeis();

   // Divide as fixa��es de acordo com seus tipos
   void relacionaFixacoes();

   // Verifica se j� existe uma fixa��o equivalente
   bool contemFixacao( GGroup< Fixacao *, LessPtr< Fixacao > > ,
      Professor *, Disciplina *, Sala *, int, HorarioAula * );

   // Informa se j� n�o existe uma fixa��o contendo exatamente os mesmos dados
   bool contemFixacaoExato( GGroup< Fixacao *, LessPtr< Fixacao > > ,
      Professor *, Disciplina *, Sala *, int, HorarioAula * );

   // Cria uma nova fixa��o com os dados informados
   Fixacao * criaFixacao( int, Professor *,
      Disciplina *, Sala *, int, HorarioAula * );

   // Deleta fixa��es que relacionam disciplinas que foram substitu�das
   void removeFixacoesComDisciplinasSubstituidas();   

   // Associa corretamente fixa��es que relacionam salas a disciplinas
   // que possuem creditos praticos
   void corrigeFixacoesComDisciplinasSalas();

   // Quando houver uma fixa��o de uma disciplina em um dia
   // da semana, esse m�todo remove os demais dias da semana
   // no conjunto de dias letivos da disciplina em quest�o
   void verificaFixacoesDiasLetivosDisciplinas();

   // Quando houver uma disciplina que use laborat�rios e tamb�m
   // existir uma fixa��o dessa disciplina, devemos criar a fixa��o
   // correta para a disciplina dividida (cr�ditos pr�ticos)
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

   // Calcula a demanda m�xima e demanda total da disciplina
   // em quest�o. Esses valores s�o armazenados na pr�pria disciplina.
   void calculaDemandas();

   // Armazena em cada sala, refer�ncias para as disciplinas
   // associadas. Armazena tamb�m, para cada disciplina,
   // ponteiros para as salas compat�veis.
   void associaDisciplinasSalas();

   // Armazena, para cada disciplina, ponteiros para as salas compat�veis.
   void carregaDiscSalas();

   // Armazena refer�ncias para as disciplinas associadas
   // em cada elemento do <ConjuntoSala>. Como o m�todo
   // associaDisciplinasSalas() j� associou as disciplinas
   // �s salas, basta percorrer o map de salas de um dado
   // elemento do <ConjuntoSala> adicionando as salas ao
   // map de disciplinas associadas do <ConjuntoSala>.
   void associaDisciplinasConjuntoSalas();

   // Relaciona cada disciplina com as ofertas em que ela aparece.
   void relacionaDiscOfertas();

   // Relaciona cada disciplina de um campus com as ofertas em que ela aparece.
   //void relacionaDiscCampusOfertas();

   // Inicializando as estruturas presentes em cada Sala, que s�o respons�veis por 
   // informar a quantidade de cr�ditos livres em um dado dia letivo.
   void calculaCredsLivresSalas();

   // Conjunto de combina��es poss�veis de divis�o de cr�ditos de uma uma disciplina
   void combinacaoDivCreditos();
   void checkDivisaoGenerica(Disciplina *disciplina);
   bool checkExisteDivisao(Disciplina *disciplina);
   void criaCombinacoes(Disciplina *disciplina, std::set<DivCredType> &combinacoes);
   void espelhaCombinacoes(Disciplina *disciplina, std::set<DivCredType> &combinacoes);
   void addCombinacoes(Disciplina *disciplina, std::set< DivCredType > const combinacoes);

   // Relaciona cada poss�vel valor de cr�dito com as regras de cr�ditos existentes.
   void relacionaCredsRegras();

   // Relaciona cada curso com o seu respectivo campus
   void relacionaCursosCampus();

   int retornaMaiorIdDemandas();

   int retornaMaiorIdAlunoDemandas();

   // Para o caso em que h� uma solu��o do m�dulo t�tico
   // e deseja-se executar o m�dulo operacional. Deve-se
   // verificar na solu��o informada se existe alguma disciplina
   // que foi dividida est� alocada com id positivo em um laborat�rio.
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
