#ifndef BALANCEDTREEK_H_
#define BALANCEDTREEK_H_

#include "Vertex.h"
#include "Key.h"
#include "Value.h"
#include <cstddef>

class BalancedTreeK
{
private:
	Vertex* _root;
	const Key* _min;
	const Key* _max;
	Vertex* Insert_And_Split(Vertex* newchild,Vertex* node);
	void insert_correct_index(Vertex* newchild, Vertex* node);
	Vertex* Borrow_Or_Merge(Vertex* ver);
	Vertex* SearchNode(const Key* key) const;
public:
	BalancedTreeK():_root(NULL), _min(NULL), _max(NULL) {}
	BalancedTreeK(const Key* min, const Key* max);
	
	Vertex* get_root() {
		return this->_root;
	}
	void Insert(const Key* nkey,const Value* nval);
	const Key* Select(unsigned index) const;
	Value* Search(const Key* key) const;
	unsigned Rank(const Key* key) const;
	const Value* GetMaxValue(const Key* key1, const Key* key2) const;
	void Delete(const Key* dkey);
	~BalancedTreeK()
	{
		Vertex* ver = this->_root;
		while (ver->get_num_childs() != 0)
		{
			ver = ver->get_child_x(0);
		}
		ver = ver->get_parent();
		Vertex* cur_ver = ver->get_parent();
		int sum = 0;
		while (cur_ver!= this->_root && cur_ver!=NULL)
		{

			for (unsigned int j = 0; j < cur_ver->get_num_childs(); j++)
			{
				ver = cur_ver->get_child_x(j);
				int num_childs = ver->get_num_childs();
				for (int i = 0; i < num_childs; i++)
				{
					sum += 1;
					ver->decrease_num_child();
					delete ver->get_child_x(i);
				}
			}
			cur_ver = cur_ver->get_parent();
			
		}
		int num_childs = this->_root->get_num_childs();
		for (int j = 0; j < num_childs; j++)
		{
			this->_root->decrease_num_child();
			delete this->_root->get_child_x(j);
		}
		delete this->_root;
		delete _min;
		delete _max;
	}
};

#endif
