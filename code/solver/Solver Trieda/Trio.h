#pragma once

template<class T1, class T2, class T3 > 
class Trio
{
public:

	T1 first;
	T2 second;
	T3 third;

	Trio(void)
	{
	}

	Trio( T1 t1, T2 t2, T3 t3 )
	{
		first = t1;
		second = t2;
		third = t3;
	}

	~Trio(void)
	{
	}

	void set( T1 t1, T2 t2, T3 t3 ){		
		first = t1;
		second = t2;
		third = t3;
	}
	
   //==================================================
   // OPERATORS 
   //==================================================
   
   Trio & operator = ( const Trio & t )
   {
		this->first = t.first;
		this->second = t.second;
		this->second = t.third;
   }

   bool operator < ( Trio const & right ) const
   {
	  if ( first < right.first &&
		   second < right.second &&
		   third < right.third )
		   return true;

	  if ( first != right.first )
		  return first < right.first;
	  if ( second != right.second )
		  return second < right.second;
	  if ( third != right.third )
		  return third < right.third;

	   return false;
   }
   
   bool operator == ( Trio & right ) const
   { 
		return (  first == right.first &&
				  second == right.second &&
				  third == right.third  );
   }

};

