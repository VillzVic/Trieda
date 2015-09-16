#ifndef HORARIODIA_H_
#define HORARIODIA_H_

#include "ofbase.h"
#include "GGroup.h"

class HorarioAula;

class HorarioDia
{
public:
   HorarioDia();
   HorarioDia( const HorarioDia & );
   virtual ~HorarioDia();

   bool operator < ( const HorarioDia & ) const;
   
   bool operator == ( const HorarioDia & ) const;

   bool igual ( const HorarioDia & right ) const;

   int getId() const { return this->id; }
   int getHorarioAulaId() const { return this->idHorarioAula; }
   HorarioAula * getHorarioAula() const { return this->horarioAula; }
   int getDia() const { return this->dia; }

   void setId( int i ) { this->id = i; }
   void setHorarioAulaId( int i ) { this->idHorarioAula = i; }
   void setHorarioAula( HorarioAula * ha ) { this->horarioAula = ha; }
   void setDia( int d ) { this->dia = d; }

private:
   int id;
   int dia;
   int idHorarioAula;
   HorarioAula * horarioAula;
};

typedef GGroup< HorarioDia *, LessPtr< HorarioDia > > HorarioDiaGroup;

namespace std
{
	template<>
	struct less<HorarioDia*>
	{
		bool operator() (HorarioDia* const first, HorarioDia* const second) const
		{
			return (*first) < (*second);
		}
	};
}

#endif