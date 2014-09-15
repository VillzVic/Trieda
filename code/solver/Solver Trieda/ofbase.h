#ifndef _OFBASE_H_
#define _OFBASE_H_

#include "TRIEDA-InputXSD.h"
#include "GGroup.h"
#include <iostream>

// O XSD Code Synthesis gera uma estrutura de dados para listas de
// objetos que é um pouco estranha e contra-intuitiva. A macro abaixo
// gera uma iteração por ela que pode ser representada por um código
// mais legível e intuitivo, baseada nos padrões de projeto escolhidos

#ifndef ITERA_SEQ
#ifndef WIN32
#define ITERA_SEQ(it,addr,type) for (Grupo##type::type##_iterator it = \
   (addr).type().begin(); it != (addr).type().end(); ++it) 
/* A macro ITERA_NSEQ serve quando o nome do tag não segue os padrões
   de projeto */
#define ITERA_NSEQ(it,addr,name,type) for (Grupo##type::name##_iterator \
    it = (addr).name().begin(); it != (addr).name().end(); ++it) 
#else
#define ITERA_SEQ(it,addr,type) for (Grupo##type##::##type##_iterator it = \
   (addr).##type##().begin(); it != (addr).##type##().end(); ++it) 
// A macro ITERA_NSEQ serve quando o nome do tag não segue os padrões de projeto
#define ITERA_NSEQ(it,addr,name,type) for (Grupo##type##::##name##_iterator \
    it = (addr).##name##().begin(); it != (addr).##name##().end(); ++it)
#endif
#endif

#ifndef ITERA_GGROUP
#define ITERA_GGROUP( it, ggroup, type ) for ( GGroup< type * >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_INIC_LESSPTR
#define ITERA_GGROUP_INIC_LESSPTR( it, it_inicio, ggroup, type ) for ( GGroup< type *, LessPtr< type > >::iterator it = it_inicio; it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_LESSPTR
#define ITERA_GGROUP_LESSPTR( it, ggroup, type ) for ( GGroup< type *, LessPtr< type > >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_LESSPTRPTR
#define ITERA_GGROUP_LESSPTRPTR( it, ggroup, type ) for ( GGroup< type *, std::less< type* > >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_GREATERPTR
#define ITERA_GGROUP_GREATERPTR( it, ggroup, type ) for ( GGroup< type *, GreaterPtr< type > >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_N_PT
#define ITERA_GGROUP_N_PT( it, ggroup, type ) for ( GGroup< type >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_GGROUP_INIC_N_PT
#define ITERA_GGROUP_INIC_N_PT( it, it_inicio, ggroup, type ) for ( GGroup< type >::iterator it = it_inicio; it != ( ggroup ).end(); ++it )
#endif

#ifndef ITERA_MAP_LESSPTR
#define ITERA_MAP_LESSPTR( it, mymap, type1, type2 ) for ( std::map< type1 *, type2, LessPtr< type1 > >::iterator it = ( mymap ).begin(); it != ( mymap ).end(); ++it )
#endif

#ifndef ITERA_MAP
#define ITERA_MAP( it, mymap, type1, type2 ) for ( std::map< type1, type2 > >::iterator it = ( mymap ).begin(); it != ( mymap ).end(); ++it )
#endif

// A macro abaixo serve para iterar em vectors da STL
#ifndef ITERA_VECTOR
#define ITERA_VECTOR( it, vt, type ) for ( std::vector< type * >::iterator it = ( vt ).begin(); it != ( vt ).end(); ++it )
#endif

class OFBase
{
public:
   OFBase( void );
   virtual ~OFBase( void );

   virtual int getId() const { return id; }
   virtual void setId( int id ) { this->id = id; }

   virtual void le_arvore( ::xml_schema::type & raiz ) { };

   virtual bool operator < ( const OFBase & right ) const
   { 
      return ( getId() < right.getId() );
   }

   virtual bool operator > ( const OFBase & right ) const
   { 
      return ( getId() > right.getId() );
   }

   virtual bool operator == ( const OFBase & right ) const
   { 
      return ( getId() == right.getId() );
   }

   virtual bool operator != ( const OFBase & right ) const
   { 
      return !( getId() == right.getId() );
   }

private:
   int id;
}; 

#endif
