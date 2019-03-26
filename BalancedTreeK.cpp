#include "BalancedTreeK.h"
#include "Key.h"
#include "Value.h"
#include "ParameterK.h"
#include "Vertex.h"
#include "BalancedTreeK.h"

BalancedTreeK::BalancedTreeK(const Key* min, const Key* max)
{
	this->_min = min->clone();
	this->_max = max->clone();
	this->_root = new Vertex(_min, _max);
	this->_root->get_child_x(0)->set_parent(_root);
	this->_root->get_child_x(1)->set_parent(_root);

}
void BalancedTreeK::insert_correct_index(Vertex* newchild, Vertex* node)
{
	unsigned int i = 0;
	i = node->get_num_childs();
	node->set_new_child(i, newchild);
	
	for (; i >0; i--)
	{
		Key* childi = node->get_child_x(i)->get_key();
		Key* childi1 = node->get_child_x(i - 1)->get_key();
		if (*(childi) < *(childi1))
		{
			node->swap(i, i - 1);
			

		}
		else 
		{
			delete childi;
			delete childi1;
			node->Update_Key();
			return;
		}
		delete childi;
		delete childi1;

	}
}

Vertex* BalancedTreeK::Insert_And_Split(Vertex* newchild, Vertex* node)
{
	unsigned int i = 0;
	i = node->get_num_childs();
	Vertex* newNode = new Vertex();
	if (i < 2 * K - 1)
	{
		insert_correct_index(newchild, node);
		node->update_num_leaves();
		delete newNode;
		return NULL;
	}
	else
	{
		
		Key* newChildKey = newchild->get_key();
		Key* nodeKey = node->get_child_x(K - 1)->get_key();
		if (*(newChildKey) < *(nodeKey))
		{

			unsigned int h = K - 1;
			for (unsigned int j = 2 * K - 2; j > K - 2; j--)
			{
				newNode->set_new_child(h, node->get_child_x(j));
				
				node->Delete_node(j);
				node->Update_Key();
				
				h--;
			}
			
			insert_correct_index(newchild, node);
			node->update_num_leaves();
		}
		else
		{
			unsigned int h = K - 2;
			for (unsigned int j = 2 * K - 2; j > K - 1; j--)
			{
				newNode->set_new_child(h, node->get_child_x(j));
				
				node->Delete_node(j);
				node->Update_Key();
				
				h--;
			}
			
			insert_correct_index(newchild, newNode);
			newNode->update_num_leaves();
			newNode->Update_Key();
		}
		delete nodeKey;
		delete newChildKey;
		
		return newNode;
	}
}

void BalancedTreeK::Insert(const Key* nkey, const Value* nval)
{

	Vertex* newNode = new Vertex(nkey, nval);
	newNode->set_parent(this->_root);
	Vertex* curr_vertex = this->_root;
	while (curr_vertex->get_num_childs() != 0)
	{
		bool isLargest = true;
		for (unsigned int i = 0; i<curr_vertex->get_num_childs(); i++)
		{
			Key* KeyChildi = curr_vertex->get_child_x(i)->get_key();
			if (*nkey < *(KeyChildi))
			{
				isLargest = false;
				curr_vertex = curr_vertex->get_child_x(i);
				delete KeyChildi;
				break;
			}
			delete KeyChildi;
		}
		if (isLargest == true)
		{
			curr_vertex = curr_vertex->get_child_x(curr_vertex->get_num_childs() - 1);
		}
	}
	curr_vertex = curr_vertex->get_parent();
	
	newNode = Insert_And_Split(newNode, curr_vertex);



	while (curr_vertex != this->_root)
	{
		
		curr_vertex = curr_vertex->get_parent();
		if (newNode != NULL)
		{
			newNode = Insert_And_Split(newNode, curr_vertex);
			curr_vertex->update_num_leaves();
		}
		else
		{
			curr_vertex->update_num_leaves();
			curr_vertex->Update_Key();
			int i = 0;
			i =
				i + 1;

		}
	}
	if (newNode != NULL)
	{
		Vertex* newRoot = new Vertex();
		Key* newNodeKey = newNode->get_key();
		Key* currKey = curr_vertex->get_key();
		if (*(newNodeKey) < *(currKey))
		{
			newRoot->set_new_child(0, newNode);
			newRoot->set_new_child(1, curr_vertex);
		}
		else
		{
			newRoot->set_new_child(1, newNode);
			newRoot->set_new_child(0, curr_vertex);
		}
		delete currKey;
		delete newNodeKey;

		this->_root = newRoot;
		newNode->set_parent(this->_root);
		curr_vertex->set_parent(this->_root);
		newRoot->update_num_leaves();
		newRoot->Update_Key();
	}
}

const Key* BalancedTreeK::Select(unsigned index) const
{
	Vertex* ver = this->_root;
	if (ver->get_num_leaves() < index || index<1)
	{
		return NULL;
	}

	while (ver->get_num_childs() != 0)
	{
		int passed_leaves = 0;
		int sum = 0;
		int new_index = index;
		unsigned int i = 0;
		while (i <= ver->get_num_childs())
		{
			
			sum += ver->get_child_x(i)->get_num_leaves();
			if (new_index <= sum)
			{
				ver = ver->get_child_x(i);
				if (ver->get_num_childs() == 0)
				{
					
					break;
				}
				i = 0;
				new_index -= passed_leaves;
				sum = 0;
				passed_leaves = 0;
			}
			else
			{
				passed_leaves += ver->get_child_x(i)->get_num_leaves();
				i++;
			}
		}
	}
	return ver->get_key_no_Clone();

}

Value* BalancedTreeK::Search(const Key* key) const
{
	
	Vertex* ver = SearchNode(key);
	if (ver != NULL)
		return ver->get_value_no_Clone();
	else
		return NULL;
}
Vertex* BalancedTreeK::SearchNode(const Key* key) const
{
	if (*(key) < *(this->_min) || *(this->_max) < *(key))
	{
		return NULL;
	}
	Vertex* ver = this->_root;
	while (ver->get_num_childs() != 0)
	{
		unsigned int i = 0;
		for (i = 0; i < ver->get_num_childs(); i++)
		{
			Key* childKey = ver->get_child_x(i)->get_key();
			if (*key < *(childKey))
			{
				ver = ver->get_child_x(i);
				delete childKey;
				break;
			}
			else
			{
				if ((!(*key < *(childKey)) && !(*(childKey) < *(key)))
					&& ver->get_child_x(i)->get_num_childs() == 0)
				{
					delete childKey;
					return ver->get_child_x(i);
				}
				else if (!(*key < *(childKey)) && !(*(childKey) < *(key)))
				{
					
					ver = ver->get_child_x(i);
					i = 0;
				}
			}
			delete childKey;
		}



	}
	return NULL;
}
unsigned BalancedTreeK::Rank(const Key* key) const
{
	Vertex* ver1 = SearchNode(key);
	if (ver1 == NULL)
	{
		delete ver1;
		return 0;
	}
	if (*(key) < *(this->_min) || *(this->_max) < *(key))
	{
		return 0;
	}
	unsigned int rank = 0;
	Vertex* ver = this->_root;
	while (ver->get_num_childs() != 0)
	{
		for (unsigned int i = 0; i < ver->get_num_childs(); i++)
		{
			Key* childKey = ver->get_child_x(i)->get_key();
			if ((!(*key < *(childKey)) && !(*(childKey) < *(key))))
			{
				rank += ver->get_child_x(i)->get_num_leaves();
				delete childKey;
				return rank;
			}
			else if (*key < *(childKey))
			{
				ver = ver->get_child_x(i);
				delete childKey;
				break;
			}
			else
			{
				rank += ver->get_child_x(i)->get_num_leaves();
			}
			delete childKey;
		}
	}
	return 0;
}

const Value* BalancedTreeK::GetMaxValue(const Key* key1, const Key* key2) const
{

	if (*(key2) < *(key1))
	{
		return  NULL;
	}
	if (*(key2) < *(this->_min) || *(this->_max) < *(key1))
	{
		return NULL;
	}
	Vertex* ver = this->_root;
	while (ver->get_num_childs() != 0)
	{
		int left = 0;
		int right = ver->get_num_childs() - 1;
		while (left < right)
		{
			bool enter = false;
			Key* childKeyL = ver->get_child_x(left)->get_key();
			
			if (*(childKeyL) < *(key1))
			{
				left++;
				enter = true;
			}
			delete childKeyL;
			Key* childKeyR = ver->get_child_x(right - 1)->get_key();
			if (*key2 < *(childKeyR))
			{
				right--;
				enter = true;
			}
			delete childKeyR;
			if (enter == false)
				break;
		}
		Vertex* maxVer = ver->get_child_x(left);
		for (int i = left; i <= right; i++)
		{
			Value* maxVal = maxVer->get_max_value();
			Value* childVal = ver->get_child_x(i)->get_max_value();
			if (*(maxVal) < *(childVal))
			{
				maxVer = ver->get_child_x(i);
			}
			delete maxVal;
			delete childVal;
		}
		ver = maxVer;

	}
	return ver->get_max_value_no_Clone();
}


Vertex* BalancedTreeK::Borrow_Or_Merge(Vertex* ver)
{
	Vertex* ver_parent = ver->get_parent();
	if (ver_parent->get_child_x(ver_parent->get_num_childs() - 1) == ver)
	{
		Vertex* curr_ver = ver_parent->get_child_x(ver_parent->get_num_childs() - 2);
		if (curr_ver->get_num_childs() == K)
		{
			int ver_children = ver->get_num_childs();
			for (int i = 0; i < ver_children; i++)
			{
				curr_ver->set_new_child(K + i, ver->get_child_x(i));
				ver->Delete_node(i);
				
			}
			unsigned int ver_to_del_index = 0;
			for (ver_to_del_index = 0; ver_to_del_index < ver_parent->get_num_childs(); ver_to_del_index++)
			{
				if (ver_parent->get_child_x(ver_to_del_index) == ver)
				{

					break;
				}
			}
			ver_parent->swap(ver_to_del_index, ver_parent->get_num_childs());
			for (unsigned int i = ver_to_del_index; i < ver_parent->get_num_childs(); i++)
			{
				ver_parent->swap(i, i + 1);
			}
			ver_parent->Delete_node(ver_parent->get_num_childs() - 1);
			ver_parent->Update_Key();
			delete ver;

			
		}
		else if (curr_ver->get_num_childs() > K)
		{
			for (int i = ver->get_num_childs(); i > 0; i--)
			{
				ver->swap(i - 1, i);
			}
			ver->set_new_child(0, curr_ver->get_child_x(curr_ver->get_num_childs() - 1));
			curr_ver->Delete_node(curr_ver->get_num_childs() - 1);
			curr_ver->Update_Key();
		}
		else
		{
			int i = 0;
			i = i + 1;
		}
		
		return curr_ver;
	}
	else
	{
		Vertex* curr_ver = NULL;
		for (unsigned int i = 0; i < ver_parent->get_num_childs(); i++)
		{
			if (ver_parent->get_child_x(i) == ver)
			{
				curr_ver = ver_parent->get_child_x(i + 1);
				break;
			}
		}
		if (curr_ver->get_num_childs() == K)
		{
			for (int i = curr_ver->get_num_childs() - 1; i >= 0; i--)
			{
				curr_ver->swap(i, i + ver->get_num_childs());
			}
			int sum_childs = ver->get_num_childs();
			for (int i = sum_childs - 1; i >= 0; i--)
			{
				curr_ver->set_new_child(i, ver->get_child_x(i));
				ver->Delete_node(i);
				
			}

			unsigned int ver_to_del_index = 0;
			for (ver_to_del_index = 0; ver_to_del_index < ver_parent->get_num_childs(); ver_to_del_index++)
			{
				if (ver_parent->get_child_x(ver_to_del_index) == ver)
				{
					
					break;
				}
			}
			ver_parent->swap(ver_to_del_index, ver_parent->get_num_childs());
			for (unsigned int i = ver_to_del_index; i < ver_parent->get_num_childs(); i++)
			{
				ver_parent->swap(i, i + 1);
			}
			ver_parent->Delete_node(ver_parent->get_num_childs() - 1);
			ver_parent->Update_Key();
			delete ver;
		}
		else
		{
			ver->set_new_child(K - 1, curr_ver->get_child_x(0));
			ver->Update_Key();
			
			curr_ver->swap(0, curr_ver->get_num_childs());
			for (unsigned int i = 1; i < curr_ver->get_num_childs() + 1; i++)
			{
				curr_ver->swap(i, i - 1);
			}
			curr_ver->Delete_node(curr_ver->get_num_childs() - 1);

		}
		
		return curr_ver;
	}
	return NULL;

}

void BalancedTreeK::Delete(const Key* dkey)
{
	Vertex* ver = SearchNode(dkey);
	if (ver == NULL)
	{
		delete ver;
		return;
	}
	Vertex* ver_parent = ver->get_parent();
	unsigned int ver_to_del_index = 0;
	for (ver_to_del_index = 0; ver_to_del_index < ver_parent->get_num_childs(); ver_to_del_index++)
	{
		if (ver_parent->get_child_x(ver_to_del_index) == ver)
		{
			break;
		}
	}
	if (ver_parent->get_num_childs() > K)
	{
		ver_parent->swap(ver_to_del_index, ver_parent->get_num_childs());
		
		for (unsigned int j = ver_to_del_index; j < ver_parent->get_num_childs(); j++)
		{
			ver_parent->swap(j, j + 1);
		}
		
		Vertex* toDel = ver_parent->get_child_x(ver_parent->get_num_childs() - 1);
		ver_parent->Delete_node(ver_parent->get_num_childs() - 1);
		delete toDel;
		ver_parent->Update_Key();
		
	}
	else
	{
		ver_parent->swap(ver_to_del_index, ver_parent->get_num_childs());
		for (unsigned int i = ver_to_del_index; i < ver_parent->get_num_childs(); i++)
		{
			ver_parent->swap(i, i + 1);
		}
		Vertex* toDel = ver_parent->get_child_x(ver_parent->get_num_childs() - 1);
		ver_parent->Delete_node(ver_parent->get_num_childs() - 1);
		delete toDel;
		ver_parent->Update_Key();
		
		while (ver_parent != NULL)
		{
			if (ver_parent->get_num_childs() <= K - 1)
			{
				if (ver_parent != this->_root)
				{
					ver_parent = this->Borrow_Or_Merge(ver_parent);
				}
				else if (ver_parent->get_num_childs() < 2)
				{
					Vertex* NewRoot = this->_root;
					this->_root = ver_parent->get_child_x(0);
					this->_root->set_parent(NULL);
					
					for (int i = 0; i < NewRoot->get_num_childs(); i++)
					{
						NewRoot->Delete_node(i);
					}
					delete NewRoot;
					return;
				}
				else
				{
					break;
				}
			}
			else
			{
				ver_parent->Update_Key();
				
				ver_parent = ver_parent->get_parent();
			}
		}
	}



}

