#ifndef _TURNO_H_
#define _TURNO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "HorarioAula.h"

class TurnoIES :
   public OFBase
{
public:
   TurnoIES( void );
   virtual ~TurnoIES( void );

   GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios_aula;

   void setNome( std::string s ) { nome = s; }

   int getNroDeHorariosAula(int dia);
   void retornaHorariosDisponiveisNoDia( int dia, GGroup<HorarioAula*, LessPtr<HorarioAula>> &horarios) const;

   std::string getNome() const { return nome; }

   void addHorarioAula( HorarioAula* h ){ horarios_aula.add(h); }
   bool possuiHorarioDiaOuCorrespondente( HorarioAula *h, int dia );
   GGroup<HorarioAula*,LessPtr<HorarioAula>> retornaHorarioDiaOuCorrespondente( HorarioAula *h, int dia );
   bool possuiHorarioDiaOuCorrespondente( HorarioAula *hi, HorarioAula *hf, int dia );

   // numero de horarios na semana
   int nrHorariosSemana;

private:
   std::string nome;
};

#endif
