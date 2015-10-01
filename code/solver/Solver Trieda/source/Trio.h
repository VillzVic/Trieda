#ifndef TRIO_H
#define TRIO_H

template<class T1, class T2, class T3>
class Trio
{
public:
	T1 first;
	T2 second;
	T3 third;

	Trio() {}
	Trio(T1 t1, T2 t2, T3 t3) { set(t1, t2, t3); }
	Trio(const Trio& right) { set(right.first, right.second, right.third); }

	void set(T1 t1, T2 t2, T3 t3)
	{
		first = t1;
		second = t2;
		third = t3;
	}

	Trio& operator=(const Trio& right)
	{
		set(right.first, right.second, right.third);
		return *this;
	}

	bool operator<(const Trio& right) const
	{
		if (first < right.first && second < right.second && third < right.third)
			return true;

		if (first != right.first) return first < right.first;
		if (second != right.second) return second < right.second;
		if (third != right.third) return third < right.third;

		return false;
	}

	bool operator==(const Trio& right) const
	{
		return (first == right.first && second == right.second && third == right.third);
	}
};

typedef Trio<int, int, int> TrioInt;

#endif
