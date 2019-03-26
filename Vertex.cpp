#include "Vertex.h"

void Vertex::Update_Max_Val(int index)
{
	if (this == NULL)
		return;
	Vertex* cur_ver = this;
	if (cur_ver->_maxValue != NULL)
		delete cur_ver->_maxValue;
	cur_ver->_maxValue = cur_ver->_vararr[1]->get_max_value();
	while (cur_ver->get_parent() != NULL)
	{
		for (unsigned int i = 0; i < cur_ver->get_num_childs() - 1; i++)
		{
			
			if (i != index)
			{
				Value* cur_val = cur_ver->get_max_value();
				Value* childVal = cur_ver->get_child_x(i)->get_max_value();
				if (cur_val != NULL && childVal != NULL)
				{
					if (*(cur_val) < *(childVal))
					{
						delete cur_ver->_maxValue;
						cur_ver->_maxValue = childVal->clone();
					}
				}
				delete cur_val;
				delete childVal;

			}
		}
		
		cur_ver = cur_ver->get_parent();
	}
}




void Vertex::Delete_node(int i)
{
	Value* ChildIVal = this->_vararr[i]->get_max_value();
	if (ChildIVal != NULL && this->get_num_childs() != 0)
	{
		Value* thisMaxVal = this->get_max_value();
		
		if (thisMaxVal != NULL)
		{
			if (!(*(ChildIVal) < *(thisMaxVal)) && !(*(thisMaxVal) < *(ChildIVal)))
			{
				this->Update_Max_Val(i);
			}
		}
		delete thisMaxVal;
	}
	delete ChildIVal;

	this->_vararr[i] = NULL;
	this->decrease_num_child();
	this->update_num_leaves();
}



void Vertex::set_new_child(unsigned int i, Vertex* vertex_ptr)
{
	this->_vararr[i] = vertex_ptr;
	if (this->_key != NULL)
	{
		if (vertex_ptr != NULL) {
			vertex_ptr->set_parent(this);
			Key* prtKey = vertex_ptr->get_key();
			if (*(this->_key) < *(prtKey))
			{
				delete this->_key;
				this->_key = prtKey->clone();
			}
			delete prtKey;
			Value* prtVal = vertex_ptr->get_max_value();
			if (this->_maxValue != NULL)
			{
				if (prtVal != NULL)
				{
					if (*(this->_maxValue) < *(prtVal))
					{
						delete this->_maxValue;
						this->_maxValue = prtVal->clone();
					}
				}
			}
			else
			{
				this->_maxValue = prtVal->clone();
			}
			delete prtVal;
		}
		else
		{
			
			if (this->get_num_childs() == 2)
			{
				delete this->_key;
				this->_key = this->_vararr[0]->get_key();
			}
			else
			{
				delete this->_key;
				this->_key = this->_vararr[i - 1]->get_key();
			}

		}
	}
	else
	{
		if (vertex_ptr != NULL)
		{
			vertex_ptr->set_parent(this);
			this->_key = vertex_ptr->get_key();
			if (vertex_ptr->get_num_childs() != 0)
				this->_maxValue = vertex_ptr->get_max_value();

		}
		else
		{
			this->_key = this->_vararr[this->get_num_childs() - 2]->get_key();
		}

	}
	this->increase_num_child();
	this->update_num_leaves();
}


void Vertex::update_num_leaves()
{
	Vertex* ver = this;
	unsigned int sum = 0;
	while (ver->get_parent() != NULL)
	{
		sum = 0;
		for (unsigned int i = 0; i < ver->get_num_childs(); i++)
		{
			if (ver->get_child_x(i) != NULL)
				sum += ver->get_child_x(i)->get_num_leaves();
		}

		ver->_num_leaves = sum;
		ver = ver->get_parent();
	}
	sum = 0;
	for (unsigned int i = 0; i < ver->get_num_childs(); i++)
		if (ver->get_child_x(i) != NULL)
			sum += ver->get_child_x(i)->get_num_leaves();
	ver->_num_leaves = sum;
}