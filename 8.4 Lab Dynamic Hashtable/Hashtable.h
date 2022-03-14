#pragma once
#include <iostream>
#include <memory>
#include <iomanip>

using namespace std;

template<class Type>
struct Bucket {
	Type data;
	bool empty = true;
};


template<class Type>
class Hashtable;

template<class Type>
ostream& operator<< (ostream &, Hashtable<Type>&);


template<class Type>
class Hashtable
{
public:
	Hashtable();
	Hashtable(int);
	Hashtable(int, double);
	Hashtable(const Hashtable<Type>& other);
	Hashtable& operator=(const Hashtable<Type>& other);

	int getSize() const;
	int getCapacity() const;
	double getLoadFactorThreshold() const;

	static bool isPrime(int);
	static int nextPrime(int);
	int indexOf(int) const;
	void rehash();

	bool empty() const;
	void insert(const Type);
	void remove(int);
	bool contains(int) const;
	void clear();
	friend ostream& operator<< <>(ostream&, Hashtable<Type>&);

	void printtable(); // for debugging

private:
	int hash(Type v) const;
	int capacity;
	int size;
	double loadFactorThreshold;
	unique_ptr<Bucket<Type>[]> htable;
};



template<class Type>
Hashtable<Type>::Hashtable()
{
	size = 0;
	capacity = 17;
	loadFactorThreshold = .65;
	htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
Hashtable<Type>::Hashtable(int capacity) : capacity(capacity)
{
	loadFactorThreshold = .65;
	size = 0;
	htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
Hashtable<Type>::Hashtable(int capacity, double threshold) : capacity(capacity), loadFactorThreshold(threshold)
{
	size = 0;
	htable = make_unique<Bucket<Type>[]>(capacity);
}



template<class Type>
Hashtable<Type>::Hashtable(const Hashtable<Type>& other)
{
	capacity = other.capacity;
	size = other.size;
	loadFactorThreshold = other.loadFactorThreshold;
	htable = make_unique<Bucket<Type>[]>(capacity);
	for (int i = 0; i < capacity; i++) {
		htable[i] = other.htable[i];
	}
}

template<class Type>
Hashtable<Type>& Hashtable<Type>::operator=(const Hashtable<Type>& other)
{
	capacity = other.capacity;
	size = other.size;
	loadFactorThreshold = other.loadFactorThreshold;
	htable = make_unique<Bucket<Type>[]>(capacity);
	for (int i = 0; i < capacity; i++) {
		htable[i] = other.htable[i];
	}
	return *this;
}

template<class Type>
int Hashtable<Type>::hash(Type v) const {
	return fmod(v, capacity);
}

template<class Type>
int Hashtable<Type>::getSize() const
{
	return size;
}

template<class Type>
int Hashtable<Type>::getCapacity() const
{
	return capacity;
}

template <class Type>
double Hashtable<Type>::getLoadFactorThreshold() const
{
	return loadFactorThreshold;
}

template<class Type>
bool Hashtable<Type>::isPrime(int in)
{
	if (in == 0 || in == 1) {
		return false;
	}
	for (int i = 2; i <= in / 2; i++) {
		if (in % i == 0) {
			return false;
		}
	}
	return true;
}

template<class Type>
int Hashtable<Type>::nextPrime(int in)
{
	int i = in + 1;
	while (!isPrime(i)) {
		i++;
	}
	return i;
}

template<class Type>
int Hashtable<Type>::indexOf(int val) const
{
	for (int i = 0; i < capacity; i++) {
		if (htable[i].data == val) {
			return i;
		}
	}
	return -1;
}

template<class Type>
bool Hashtable<Type>::empty() const
{
	if (size == 0) {
		return true;
	}
	return false;
}

template<class Type>
void Hashtable<Type>::insert(Type value)
{
	if (((size + 1) / (float) capacity) >= loadFactorThreshold)
	{
		rehash();
	}
	int i = 0;
	while (!htable[hash(value + (i*i))].empty) {
		i++;
	}

	htable[hash(value + (i*i))] = { value, false };
	size++;
}

template<class Type>
void Hashtable<Type>::rehash()
{
	int tempsize = size;
	int oldCapacity = capacity;
	capacity = nextPrime(capacity * 2);
	Type* test;
	test = new Type[tempsize + 1];
	int j = 0;
	for (int i = 0; i < oldCapacity; i++) {
		if (!htable[i].empty) {
			test[j] = htable[i].data;
			j++;
		}
	}

	clear();
	
	for (int i = 0; i < tempsize; i++) {
		insert(test[i]);
	}
}

/*
	Complete the remove the value from the hashtable
*/
template<class Type>
void Hashtable<Type>::remove(int value)
{
	int i = 0;
		while (htable[hash(value + (i*i))].data != value && i < capacity) {
		i++;

	}
	if (i == capacity) {
		return;
	}
	htable[hash(value + (i*i))] = { 0, true };
	size--;
}

/*
	Complete the contains method to return true of the Hashtable contains the value
*/
template<class Type>
bool Hashtable<Type>::contains(int value) const {

	int i = 0;
	while (htable[hash(value + (i*i))].data != value && i < capacity) {
		i++;
		}
	if (htable[hash(value + (i*i))].data == value){
		return true;
	}
	return false;

	/*if (indexOf(value) == -1) {
		return false;
	}
	return true;*/
}



template<class Type>
void Hashtable<Type>::clear()
{
	size = 0;
	htable = make_unique<Bucket<Type>[]>(capacity);
}



template<class Type>
ostream& operator<< (ostream & out, Hashtable<Type>& h) {
	out << "\nCapacity: " << h.capacity << "\n";
	for (int i = 0; i < h.capacity; i++) {

		if (h.htable[i].empty == false) {
			out << i << ": " << h.htable[i].data << endl;
		}

	}
	return out;
}

//for debugging
template<class Type>
void Hashtable<Type>::printtable() {
	for (int i = 0; i < capacity; i++) {
		cout << setw(5) << i << ": " << htable[i].data << endl;
		cout << setw(5) << "Is empty: " << htable[i].empty << endl << endl;
	}
}