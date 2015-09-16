   #ifndef _BLOCO_CURRICULAR_H_
#define _BLOCO_CURRICULAR_H_

#include <map>

#include "ofbase.h"
#include "GGroup.h"
#include "Curso.h"
#include "Disciplina.h"
#include "Demanda.h"
#include "Campus.h"
#include "Curriculo.h"

class BlocoCurricular :
   public OFBase
{
public:
   BlocoCurricular( void );
   virtual ~BlocoCurricular( void );

   Curso * curso;
   Campus * campus;
   Curriculo * curriculo;
   Oferta *oferta;

   GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
   GGroup< int > diasLetivos;

   // Associa às disciplinas do bloco
   // suas respectivas demandas - IGNORANDO TURNO
   std::map< Disciplina *, Demanda * > disciplina_Demanda;

   void setPeriodo( int value ) { this->periodo = value; }
   void setTotalTurmas( int value ) { this->total_turmas = value; }
   void setMaxCredsNoDia( int dia, int value ){ this->max_creds_dia[dia] = value; }
   void setCombinaCredSL( int dia, int k, Calendario* sl , int n );
   void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }

   int getPeriodo() const { return this->periodo; }
   int getTotalTurmas() const { return this->total_turmas; }
   int getMaxCredsNoDia( int dia );
   std::map< int /*dia*/, int /*size*/ > getCombinaCredSLSize() const { return combinaCredSLSize; }
   int getNroMaxCredCombinaSL( int k, Calendario *c, int dia );

   void removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t );
   void preencheMaxCredsPorDia();
   
   GGroup<HorarioAula*> retornaHorariosDisponiveisNoDiaPorSL( int dia, Calendario* sl );

   bool combinaCredSL_eh_dominado( int i, Calendario *sl1, int maxSL2, Calendario *sl2, int dia );
   bool combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   bool combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   std::map< Trio<int, int, Calendario*>, int > retornaCombinaCredSL_Dominados( int dia );
   

   std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ > combinaCredSL;

private:
   int periodo;
   int total_turmas;
   std::map< int, int > max_creds_dia; /* map< dia, maximo de creditos no dia > */
   
   std::map< int /*dia*/, int /*size*/ > combinaCredSLSize; // informa o numero total de combinações existentes para cada dia

};

#endif
