#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <cstring>

class Cell
{
	public:
	char* str;
	Cell* link;
	
	Cell (char* x)
	{
		if (x)
		{
			this->str = (char*)calloc(strlen(x)+1, sizeof(char));
			strcpy(this->str, x);

			link = NULL;
		}
	}

	~Cell ()
	{
		if (str)
		{
			delete (str);
		}
		link = NULL;
	}
};

class List
{
	private:
	Cell* head;
	Cell* tail;
	int n;

	public:
	List ()
	{
		this->head = NULL;
		this->tail = NULL;
		this->n = 0;
	}

	~List ()
	{
		
	}

	List* clone (void)
	{
		List* res = new List();
		Cell* ptr = head;

		while (ptr)
		{
			res->insert(ptr->str);
			ptr = ptr->link;
		}
		return (res);
	}

	int getSize (void)
	{
		return(n);
	}

	void insert (char* x)
	{
		if (x)
		{
			if (!head)
			{
				head = new Cell(x);
				tail = head;
			}
			else
			{
				tail->link = new Cell(x);
				tail = tail->link;
			}
			n++;
		}
	}

	char* remove (void)
	{
		char* res = NULL;
		if (head && tail)
		{
			res = (char*)calloc(strlen(head->str)+1, sizeof(char));
			strcpy(res, head->str);

			Cell* tmp = head;
			head = head->link;
			delete(tmp);
		}

		return (res);
	}

	char* get (int p)
	{
		char* res = NULL;
		if (head && tail)
		{
			Cell* ptr = head;
			int i = 0;

			while (ptr && i < p)
			{
				ptr = ptr->link;
				i++;
			}

			if (ptr)
			{
				res = (char*)calloc(strlen(ptr->str)+1, sizeof(char));
				strcpy(res, ptr->str);
			}
		}
		return (res);
	}

	void print (Cell* ptr)
	{
		if (ptr && ptr->str)
		{
			std::cout << "[" << ptr->str << "] ";
			print(ptr->link);
		}
	}

	void print ()
	{
		if (head)
		{
			print(head);
		}
		std::cout << "\n";
	}
};

#endif
