// TODO
// Implement ITERATORS - Usage: to sort matrix based on a line or a column.
// Implement sortMatrix method based on 'sort' of STL.

// Ver este link
//http://www.parashift.com/c++-faq-lite/operator-overloading.html

// boas dicas em 13.9

// Analisar preferencia de m(1,2) do que m[1][2]

// http://www.parashift.com/c++-faq-lite/operator-overloading.html#faq-13.11

// (...)
// Put it this way: the operator() approach is never worse than, and sometimes
// better than, the [][] approach. 

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <map>

class BadIndex
{
private:
   std::string message;

public:
   BadIndex ( std::string m )
   {
      message = m;
   }
};

template< class T, class N = int >
class Matrix
{
private:
   unsigned rows, cols;
   T * data;

   std::map< N, int > rows_Names;
   std::map< N, int > cols_Names;

public:

   // --------------------------

   Matrix ( const Matrix & m ) // Copy constructor
   {
      rows = m.rows;
      cols = m.cols;

      unsigned int total = rows * cols;
      data = new T [total];

      for ( unsigned int i = 0; i < (total); i++ )
      {
         data [i] = m.data [i];
      }
   }

   // --------------------------

   Matrix ( unsigned _quadratic )
   {
      rows = cols = _quadratic;

      if ( rows == 0 || cols == 0 )
      {
         throw BadIndex( "Matrix constructor has 0 size" );
      }

      data = new T [ rows * cols ];
   }

   // --------------------------

   Matrix ( unsigned _rows, unsigned _cols )
   {
      rows = _rows;
      cols = _cols;

      if ( rows == 0 || cols == 0 )
      {
         throw BadIndex("Matrix constructor has 0 size");
      }

      data = new T [ rows * cols ];
   }

   // --------------------------

   virtual ~Matrix () //Destructor
   {
      delete [] data;
   }

   // --------------------------

   T & operator() ( unsigned row, unsigned col ) // subscript operators often come in pairs
   {
      if ( row >= rows || col >= cols )
      {
         std::cout << "[set Matrix] Pedido de (" << row << "," << col << ")"
                   << " de (" << rows << "," << cols << ")" << std::endl;

         int jkl;
         std::cin >> jkl;

         throw BadIndex( "Matrix subscript out of bounds" );
      }

      return data [ cols * row + col ];
   }

   // --------------------------

   T operator() ( unsigned row, unsigned col ) const // subscript operators often come in pairs
   {
      if ( row >= rows || col >= cols )
      {
         std::cout << "[get Matrix] Pedido de (" << row << "," << col << ")"
                   << " de (" << rows << "," << cols << ")" << std::endl;

         int jkl;
         std::cin >> jkl;

         throw BadIndex( "const Matrix subscript out of bounds" );
      }

      return data [ cols * row + col ];
   }

   // --------------------------

   Matrix& operator = ( const Matrix & m ) // Assignment operator
   {
      if ( &m == this )
      {
         return *this;
      }

      delete [] data;

      rows = m.rows;
      cols = m.cols;

      int total = rows * cols;

      if ( total < 0 )
      {
         std::cerr << "Valor maior do que o suportado pela Matrix! (" << total << ")" << std::endl;
         exit(1);
      }

      data = new T [ total ];

      for ( int i = 0; i < (total); i++ )
      {
         data [ i ] = m.data [ i ];
      }

      return *this;
   }

   // --------------------------

   bool square () const
   {
      return ( rows == cols );
   }

   // --------------------------

   void fill ( T v )
   {
      unsigned int total = rows * cols;

      for (unsigned int i = 0; i < (total); i++)
      {
         data [i] = v;
      }
   }

   // --------------------------

   unsigned getRows () const
   {
      return rows;
   }

   // --------------------------

   unsigned getCols () const
   {
      return cols;
   }

   // --------------------------

   std::vector< T > getRow ( int _row ) const
   {
      std::vector< T > row( cols );

      for ( int i = 0; i < cols; i++ )
      {
         row [ i ] = operator()( _row, i );
      }

      return row;
   }

   // --------------------------

   void setRow ( int p, std::vector< T > & _row )
   {
      int numCol = ( (_row.size() < cols) ? _row.size() : cols );

      for ( int i = 0; i < numCol; i++ )
      {
         operator()( p, i ) = _row [ i ];
      }
   }

   // --------------------------

   std::vector< T > getCol ( int _col ) const
   {
      std::vector< T > col( rows );

      for ( int i = 0; i < (int)rows; i++ )
      {
         col [ i ] = operator()( i, _col );
      }

      return col;
   }

   // --------------------------

   void setCol ( int p, std::vector< T > & _col )
   {
      int numRow = ( (_col.size() < rows) ? _col.size() : rows );

      for ( int i = 0; i < numRow; i++ )
      {
         operator()( i, p ) = _col [ i ];
      }
   }

   // --------------------------

   void setRowName ( unsigned index, N name )
   {
      if ( index < 0 || index >= rows )
      {
         throw BadIndex( "Out of bound." );
      }

      rows_Names [ name ] = index;
   }

   // --------------------------

   void setRowByName ( N name, std::vector< T > & _row )
   {
      int numCol = ( (_row.size() < cols) ? _row.size() : cols );

      if ( rows_Names.find( name ) == rows_Names.end() )
      {
         throw BadIndex( "Row name doesn't match." );
      }

      int r = rows_Names.find( name )->second;

      for ( int c = 0; c < numCol; c++ )
      {
         operator()( r, c ) = _row [ c ];
      }
   }

   // --------------------------

   std::vector< T > getRowByName ( N name ) const
   {
      std::vector< T > row( cols );

      if ( rows_Names.find( name ) == rows_Names.end() )
      {
         throw BadIndex( "Row name doesn't match." );
      }

      int _row = ( rows_Names.find( name )->second );

      for ( int c = 0; c < cols; c++ )
      {
         row [ c ] = operator()( _row, c );
      }

      return row;
   }

   // --------------------------

   unsigned getRowIdByName ( N name ) const
   {
      if ( rows_Names.find( name ) == rows_Names.end() )
      {
         throw BadIndex( "Row name doesn't match." );
      }

      return ( rows_Names.find( name )->second );
   }

   // --------------------------

   void setColName ( unsigned index, N name )
   {
      if ( index < 0 || index >= cols )
      {
         throw BadIndex(" Out of bound." );
      }

      cols_Names [ name ] = index;
   }

   // --------------------------

   void setColByName ( N name, std::vector< T > & _col )
   {
      int numRow = ( (_col.size() < rows) ? _col.size() : rows );

      if ( cols_Names.find( name ) == cols_Names.end() )
      {
         throw BadIndex( "Col name doesn't match." );
      }

      int c = ( cols_Names.find( name )->second );

      for ( int r = 0; r < numRow; r++ )
      {
         operator()( r, c ) = _col [ r ];
      }
   }

   // --------------------------

   std::vector< T > getColByName ( N name) const
   {
      std::vector< T > col( rows );

      if ( cols_Names.find( name ) == cols_Names.end() )
      {
         throw BadIndex( "Col name doesn't match." );
      }

      int _col = ( cols_Names.find(name)->second );

      for ( unsigned r = 0; r < rows; r++ )
      {
         col[ r ] = operator()( r, _col );
      }

      return col;
   }

   // --------------------------

   unsigned getColIdByName ( N name ) const
   {
      if ( cols_Names.find( name ) == cols_Names.end() )
      {
         throw BadIndex( "Col name doesn't match." );
      }

      return cols_Names.find( name )->second;
   }
};

//template< class T >
//ostream& operator<< (ostream &os, const Matrix< T > &obj)
//{
//   os << "Matrix(" << obj.getRows() << "," << obj.getCols() << ")" << endl;
//
//   os << endl;
//
//   for (unsigned int i = 0; i < obj.getRows(); i++)
//   {
//      for (unsigned int j = 0; j < obj.getCols(); j++)
//         os << obj(i, j) << " ";
//      os << endl;
//   }
//   return os;
//}

/*
ostream& operator<< (ostream &os, const Matrix< string > &obj)
{
os << "Matrix(" << obj.getRows() << "," << obj.getCols() << ")" << endl;

for (unsigned int i = 0; i < obj.getRows(); i++)
{
for (unsigned int j = 0; j < obj.getCols(); j++)
os << "\"" << obj(i, j) << "\"" << "\t";
os << endl;
}
return os;
}
*/

#endif
