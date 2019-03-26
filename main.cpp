#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include "BalancedTreeK.h"

#define MAX_KEY UINT_MAX
#define MIN_KEY 0

#define CHECK_SIZE 100
#define INITIAL_INSERT_NUM 70 
#define OPERATIONS_SIZE 100
#define VALUES_LENGTH 10
#define SEED 15487

#define NUM_OF_OPERATIONS 6
#define INSERT 1
#define DELETE 2
#define SEARCH 3
#define RANK 4
#define SELECT 5
#define GETMAXVALUE 6


using namespace std;

class MyKey :public Key
{
public:
	MyKey(unsigned k) : key(k) {}
	MyKey(MyKey const &mk) :key(mk.key) {}
	~MyKey() {}
	MyKey* clone() const
	{
		return new MyKey(*this);
	}
	unsigned get_key() { return key; }
	void set_key(unsigned k) { key = k; }
	bool operator<(const Key &rhs) const { return key <((MyKey&)rhs).key; }
	void print() const { cout << key; }
private:
	unsigned key;
};


class MyValue : public Value
{
public:
	MyValue(string val) : value(val) {}
	~MyValue() {}
	MyValue(MyValue const &v) { value = v.value; }
	MyValue* clone() const
	{
		return new MyValue(*this);
	}
	string get_value() { return value; }
	void set_value(string val) { value = val; }
	bool operator<(const Value &rhs) const { return value <((MyValue&)rhs).value; }
	void print() const { cout << value; }
private:
	string value;
};


void print(Vertex* ver)
{
	if (ver->get_num_childs() == 0)
		return;
	Vertex* cur_ver = ver;
	cout << endl;
	cout << "New Node:";
	((MyKey*)cur_ver->get_key())->print();
	cout << endl;
	if (((MyValue*)cur_ver->get_max_value()) != NULL)
		((MyValue*)cur_ver->get_max_value())->print();
	cout << endl;
	int i = cur_ver->get_num_childs();
	cout << endl;
	cout << "children=" << i << endl;
	cout << "leaves=" << ver->get_num_leaves() << endl;
	for (int j = 0; j < i; j++)
	{
		cout << endl;
		((MyKey*)cur_ver->get_child_x(j)->get_key())->print();
		cout << endl;
		if (((MyValue*)cur_ver->get_child_x(j)->get_max_value()) != NULL)
			((MyValue*)cur_ver->get_child_x(j)->get_max_value())->print();
	}
	for (int j = 0; j < i; j++)
	{
		//cout << "j=" << j<<endl;
		//((MyKey*)cur_ver->get_child_x(j)->get_key())->print();
		cur_ver = cur_ver->get_child_x(j);
		print(cur_ver);
		cur_ver = cur_ver->get_parent();

	}






}

string generate_random_string(const unsigned len)
/*returns a random string of length len*/
{
	const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string s;
	for (int i = 0; i < len; ++i) {
		s = s + alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return s;
}

unsigned generate_random_index(const unsigned max_ind)
/*generate random number in range [0,max_ind-1]*/
{
	return rand() % max_ind;
}

int main()
{
	srand(SEED);
	MyKey* max_key = new MyKey(MAX_KEY);
	MyKey* min_key = new MyKey(MIN_KEY);
	BalancedTreeK* T = new BalancedTreeK(min_key, max_key);

	delete max_key;
	delete min_key;

	unsigned keys_array[CHECK_SIZE];
	string values_array[CHECK_SIZE];

	MyKey* my_keys_array[CHECK_SIZE];
	MyValue* my_values_array[CHECK_SIZE];

	for (unsigned i = 0; i < CHECK_SIZE; i++)
	{
		keys_array[i] = rand();
		values_array[i] = generate_random_string(VALUES_LENGTH);
		my_keys_array[i] = new MyKey(keys_array[i]);
		my_values_array[i] = new MyValue(values_array[i]);
	}

	unsigned inserted_keys_num = INITIAL_INSERT_NUM;

	for (unsigned i = 0; i < INITIAL_INSERT_NUM; i++)
	{

		T->Insert(my_keys_array[i], my_values_array[i]);
		if (my_keys_array[i] == (MyKey*)15701)
		{
			print(T->get_root());
		}
	}
	cout << "______" << endl;
	print(T->get_root());
	cout << "______" << endl;
	unsigned operations_array[OPERATIONS_SIZE];
	for (unsigned i = 0; i < OPERATIONS_SIZE; i++)
	{

		operations_array[i] = generate_random_index(NUM_OF_OPERATIONS) + 1;
	}

	MyValue* val = NULL;
	MyKey* key = NULL;
	MyKey* key1 = NULL;
	unsigned rank;
	unsigned index;

	for (unsigned i = 0; i < OPERATIONS_SIZE; i++)
	{
		cout << i << endl;
		if (i == 61)
		{
			print(T->get_root());
		}
		switch (operations_array[i])
		{
		case INSERT:
			if (inserted_keys_num < CHECK_SIZE)
			{
				T->Insert(my_keys_array[inserted_keys_num], my_values_array[inserted_keys_num]);
				if (my_keys_array[inserted_keys_num] == (MyKey*)15701)
				{
					print(T->get_root());
				}
				inserted_keys_num = inserted_keys_num + 1;
			}
			break;
		case DELETE:
			T->Delete(my_keys_array[generate_random_index(CHECK_SIZE)]);
			break;
		case SEARCH:
			key = my_keys_array[generate_random_index(CHECK_SIZE)];
			val = (MyValue*)T->Search(key);
			cout << "Search for key: ";
			key->print();
			cout << ", resulted in value: ";
			if (val != NULL)
			{
				val->print();
			}
			else
			{
				cout << "NULL";
			}
			cout << "\n";
			//delete val;
			break;
		case RANK:
			key = my_keys_array[generate_random_index(CHECK_SIZE)];
			rank = T->Rank(key);
			cout << "Rank for key: ";
			key->print();
			cout << ", resulted in: ";
			cout << rank << "\n";

			break;
		case SELECT:
			index = generate_random_index(CHECK_SIZE);
			key = (MyKey*)T->Select(index);
			cout << "Select for index: " << index;
			cout << ", resulted in key: ";
			if (key != NULL)
			{
				key->print();
			}
			else
			{
				cout << "NULL";
			}
			cout << "\n";
			//delete key;
			break;
		case GETMAXVALUE:
			key = my_keys_array[generate_random_index(CHECK_SIZE)];
			key1 = my_keys_array[generate_random_index(CHECK_SIZE)];
			val = (MyValue*)T->GetMaxValue(key, key1);
			cout << "GetMaxValue for interval [";
			key->print();
			cout << ",";
			key1->print();
			cout << "]";
			cout << ", resulted in value: ";
			if (val != NULL)
			{
				val->print();
			}
			else
			{
				cout << "NULL";
			}
			cout << "\n";
			//delete val;
			break;
		}
	}

	for (unsigned i = 0; i < CHECK_SIZE; i++)
	{
		T->Delete(my_keys_array[i]);
	}

	for (unsigned i = 0; i < CHECK_SIZE; i++)
	{
		delete my_keys_array[i];
		delete my_values_array[i];
	}
	delete T;
}