#ifndef PROBLEMDATALOADER_H
#define PROBLEMDATALOADER_H

/* O XSD Code Synthesis gera uma estrutura de dados para listas de
objetos que � um pouco estranha e contra-intuitiva. A macro abaixo
gera uma itera��o por ela que pode ser representada por um c�digo
mais leg�vel e intuitivo, baseada nos padr�es de projeto escolhidos */
#ifndef ITERA_SEQ
#define ITERA_SEQ(it,addr,type) for (Grupo##type##::##type##_iterator it = \
   (addr).##type##().begin(); it != (addr).##type##().end(); ++it) 
#endif

#include "ProblemData.h"
#include "input.h"
#include "TRIEDA-InputXSD.h"

class ProblemDataLoader
{
public:
   // Constructor
   ProblemDataLoader( char *inputFile, ProblemData* data );

   // Destructor
   ~ProblemDataLoader();

   // Load the XML file
   void load();

   void estabeleceDiasLetivosDiscs();

   void criaConjuntoSalas();

   void estabeleceDiasLetivosDiscSala();

   void divideDisciplinas();
   // >>> 14/10/2010
   void armz_disc_curriculo();
   // <<< 14/10/2010
   void gera_refs();
   template<class T> 
   void find_and_set(int id, GGroup<T*>& haystack, T*& needle, bool print) ;
   void cria_blocos_curriculares();
   void estima_turmas();
   void print_stats();
   void cache();
   void print_csv();

private:
   // Input data object of the problem
   ProblemData *problemData;
   char *inputFile;

   // XML parser
   std::auto_ptr<TriedaInput> root;

   // >>> Vari�veis e/ou estruturas de dados para realizar o pr� processamento dos dados.

   // <<<

   // >>> M�todos para realizar o pr� processamento dos dados.

   // =========== METODOS SET

   // =========== METODOS GET

   // =========== METODOS AUXILIARES

   void referenciaCampus();
   void referenciaDisciplinas();
   void referenciaOfertas();

   void calculaTamanhoMedioSalasCampus();

   // Armazena todas as disciplinas ofertadas em um campus.
   void relacionaCampusDiscs();

   /* Calcula a demanda m�xima e demanda total da disciplina em quest�o. Esses valores
   s�o armazenados na pr�pria disciplina. */
   void calculaDemandas();

   /* Armazena em cada sala, refer�ncias para as disciplinas associadas. Armazena tamb�m,
   para cada disciplina, ponteiros para as salas compat�veis.*/
   void carregaDisciplinasAssociadasSalas();

   // Armazena, para cada disciplina, ponteiros para as salas compat�veis.
   void carregaDiscSalas();

   /* Armazena refer�ncias para as disciplinas associadas em cada elemento do <ConjuntoSala>. 
   Como o m�todo carregaDisciplinasAssociadasSalas() j� associou as disciplinas �s salas, basta
   percorrer o map de salas de um dado elemento do <ConjuntoSala> adicionando as salas ao map de
   disciplinas associadas do <ConjuntoSala>.*/
   void carregaDiscAssocConjuntoSalas();

   /* Relaciona cada disciplina com as ofertas em que ela aparece. */
   void relacionaDiscOfertas();

   /* Relaciona cada disciplina de um campus com as ofertas em que ela aparece. */
   //void relacionaDiscCampusOfertas();

   // <<<

};

#endif
