#ifndef VERTEX_H_
#define VERTEX_H_


#include "Key.h"
#include "Value.h"
#include <cstddef>
#include "ParameterK.h"

class Vertex {
private:
	Key* _key;
	Value* _value;
	Vertex** _vararr;
	Vertex* _parent;
	Value* _maxValue;
	unsigned int childs_num;
	unsigned int _num_leaves;
public:
	Vertex() :_key(NULL), _value(NULL), _vararr(NULL), _parent(NULL), _maxValue(NULL), childs_num(0), _num_leaves(1)
	{
		this->_vararr = new Vertex*[2 * K];
		for (unsigned int i = 0; i < 2 * K; i++)
			this->_vararr[i] = NULL;
	}

	Vertex(const Key* min, const Key* max) : _key(NULL), _value(NULL), _parent(NULL), _maxValue(NULL)
	{
		this->childs_num = 0;
		this->_num_leaves = 0;
		this->_vararr = new Vertex*[2 * K];
		for (unsigned int i = 0; i < 2 * K; i++)
			this->_vararr[i] = NULL;
		Value* Temp = NULL;
		this->_vararr[0] = new Vertex(min,Temp);
		this->childs_num++;
		this->_vararr[1] = new Vertex(max,Temp);
		this->childs_num++;

	}
	Vertex(const Key* mykey, const Value* val) : _parent(NULL), childs_num(0)
	{
		this->_vararr = new Vertex*[2 * K];
		for (unsigned int i = 0; i < 2 * K; i++)
			this->_vararr[i] = NULL;
		_key = mykey->clone();
		_num_leaves = 0;
		if (val != NULL)
		{
			_num_leaves = 1;
			_value = val->clone();
			_maxValue = val->clone();
		}	
		else
		{
			_value = NULL;
			_maxValue = NULL;
		}
	}
	~Vertex()
	{
		if (_key != NULL)
			delete _key;
		if (_value != NULL)
			delete _value;
		for (unsigned int i = 0; i < this->childs_num; i++)
		{
			delete _vararr[i];
		}
		delete[] _vararr;
		delete _maxValue;
	}
	void set_parent(Vertex* parent)
	{
		_parent = parent;
	}

	Vertex* get_child_x(unsigned int x) const
	{
		if (x < 2 * K)
		{
			
			return _vararr[x];
		}
		return NULL;
	}
	unsigned int get_num_childs() const
	{
		return childs_num;
	}
	unsigned int get_num_leaves() const
	{
		if (this != NULL)
			return _num_leaves;
		return 0;
	}
	Value* get_max_value() const
	{
		if (this != NULL)
			if (this->_maxValue != NULL)
			{
				return this->_maxValue->clone();
			}
		return NULL;
	}
	Value* get_max_value_no_Clone() const
	{
		if (this != NULL)
			if (this->_maxValue != NULL)
			{
				return this->_maxValue;
			}
		return NULL;
	}
	Key* get_key() const { return this->_key->clone(); }
	Key* get_key_no_Clone() const { return this->_key; }
	Value* get_value() const 
	{
		if (this->_value != NULL)
			return this->_value->clone();
		return NULL;
	}
	Value* get_value_no_Clone() const
	{
		if (this->_value != NULL)
			return this->_value;
		return NULL;
	}
	void Update_Key()
	{
		int i = this->get_num_childs();
		if (this->_key != NULL)
			delete this->_key;
		this->_key = this->get_child_x(i - 1)->get_key();
	}

	void Update_Max_Val(int index);

	void Delete_node(int i);
	void set_new_child(unsigned int i, Vertex* vertex_ptr);

	Vertex* get_parent() const 
	{
		return this->_parent;
	}
	void increase_num_child() 
	{ 
		childs_num++; 
	}
	void decrease_num_child() 
	{
		if (childs_num>0)
			childs_num--;
	}
	void swap(unsigned int i, unsigned int j)
	{
		Vertex* Temp = _vararr[i];
		this->_vararr[i] = this->_vararr[j];
		this->_vararr[j] = Temp;
	}

	void update_num_leaves();
	


};

#endif

