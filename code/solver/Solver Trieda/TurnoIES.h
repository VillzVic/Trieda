#ifndef _TURNO_H_
#define _TURNO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "HorarioAula.h"

class Calendario;

class TurnoIES :
   public OFBase
{
public:
   TurnoIES( void );
   virtual ~TurnoIES( void );

   GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios_aula;
   
   std::map< int/*dia*/, std::map< DateTime, std::set<HorarioAula*> > > mapDiaDateTime;

   void setNome( std::string s ) { nome = s; }

   int getNroDeHorariosAula(int dia) const;
   void retornaHorariosDisponiveisNoDia( int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> &horarios) const;
   void retornaHorariosDisponiveisNoDia( int dia, std::map<DateTime, std::set<HorarioAula*>> *horarios);

   std::string getNome() const { return nome; }

   void addHorarioAula( HorarioAula* h );
   bool possuiHorarioDiaOuCorrespondente( HorarioAula *h, int dia ) const;
   bool possuiHorarioDia( int dia, DateTime dti, DateTime dtf ) const;
   GGroup<HorarioAula*,LessPtr<HorarioAula>> retornaHorarioDiaOuCorrespondente( HorarioAula *h, int dia ) const;
   bool possuiHorarioDiaOuCorrespondente( HorarioAula *hi, HorarioAula *hf, int dia );
   bool possuiHorarioDiaOuCorrespondente( Calendario* const calendario, HorarioAula* const h, int dia );
   HorarioAula* getHorarioDiaCorrespondente( Calendario* const calendario, HorarioAula* const h, int dia );
   
   // numero de horarios na semana
   int nrHorariosSemana;

private:
   std::string nome;
};

#endif
