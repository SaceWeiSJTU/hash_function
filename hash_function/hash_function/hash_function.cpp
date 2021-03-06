// hash_function.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

/*        //如果要使用开链哈希，请将此处取消注释并将闭链部分注释 行数为10~120
class hashTable
{
private:
	struct node
   {
	string data;
	int state;
   };
	node *array;
	int size;
public:
	hashTable(int length, int (*f)(const string &x));
	~hashTable()
	{
		delete [] array;
	}
	int crash;          //记录冲突次数
	bool find(const string &x) const;
	bool insert(const string &x);
	bool remove(const string &x);
	void rehash();
	int (*key)(const string &x);
	void print();
};

hashTable::hashTable(int length, int (*f)(const string &x))
{
	size = length;
	array = new node[size];
	key = f;
	crash = 0;
}


bool hashTable::insert(const string &x)
{
	int initPos, pos ;
	initPos = pos = key(x) % size;
	do
	{
		if (array[pos].state != 1)
		{
			array[pos].data = x;
			array[pos].state = 1;
			return true;
		}
		if (array[pos].state == 1 && array[pos].data == x)
			return true;
		pos = (pos+1) % size;
		crash += 1;
	}
	while (pos != initPos);
	return false;
}


bool hashTable::remove(const string &x)
{
	int initPos, pos ;
	initPos = pos = key(x) % size;
	do
	{
		if (array[pos].state == 0) return false;
		if (array[pos].state == 1 && array[pos].data == x)
		{
			array[pos].state = 2;
			return true;
		}
		pos = (pos+1) % size;
	}
	while (pos != initPos);
	return false;
}


bool hashTable::find(const string &x) const
{
	int initPos, pos ;
	initPos = pos = key(x) % size;
	do
	{
		if (array[pos].state == 0) return false;
		if ((array[pos].state == 1) && (array[pos].data) == x)
			return true;
		pos = (pos+1) % size;
	}
	while (pos != initPos);
	return false;
}


void hashTable::rehash()
{
	node *tmp = array;
	array = new node[size];
	for (int i = 0; i < size; ++i)
	{
		if ( tmp[i].state == 1)
			insert(tmp[i].data);
	}
	delete [] tmp;
}


void hashTable::print()
{
	for (int i=0; i < size; i++)
		cout<<i<<":"<<array[i].data<<endl;
}
*/


class hashTable               //如果要使用闭链哈希，请将此处（行数为123~254）取消注释并将开链部分注释（行数为10~120）
{
private:
	struct node
	{
		string data;
		node *next;
		node(const string &d)
		{
			data = d;
			next = NULL;
		}
		node()
		{
			next = NULL;
		}
	};
	node **array;
	int  size;
public:
	hashTable(int length, int(*f)(const string &x));
	virtual ~hashTable();
	bool find(const string &x) const;
	bool insert(const string &x);
	bool remove(const string &x);
	int(*key)(const string &x);
	void print();
	int crash;
};


hashTable::hashTable(int length, int(*f)(const string  &x))
{
	size = length;
	array = new node*[size];
	key = f;
	for (int i = 0; i < size; ++i) array[i] = new node;
	crash = 0;
}


hashTable::~hashTable()
{
	node *p, *q;
	for (int i = 0; i < size; ++i)
	{
		p = array[i];
		do
		{
			q = p->next;
			delete p;
			p = q;
		} while (p != NULL);
	}
	delete[] array;
}


bool hashTable::insert(const string &x)
{
	int pos;
	node *p;
	pos = key(x) % size;
	p = array[pos]->next;
	while (p != NULL && !(p->data == x))
	{
		p = p->next;
		crash++;
	}
	if (p == NULL)
	{
		p = new node(x);
		p->next = array[pos]->next;
		array[pos]->next = p;
		return true;
	}
	return false;
}


bool hashTable::remove(const string &x)
{
	int  pos;
	node *p, *q;
	pos = key(x) % size;
	p = array[pos];
	while (p->next != NULL && !(p->next->data == x))
		p = p->next;
	if (p->next == NULL) return false;
	else
	{
		q = p->next;
		p->next = q->next;
		delete q;
		return true;
	}
}


bool hashTable::find(const string &x) const
{
	int pos;
	node *p;
	pos = key(x) % size;
	p = array[pos];
	while (p->next != NULL && !(p->next->data == x))
		p = p->next;
	if (p->next != NULL) return true;
	else return false;
}

void hashTable::print()
{
	node *p;
	for (int i = 0; i < size; i++)
	{
		cout << i << ":";
		if (array[i]->next != NULL)
		{
			p = array[i]->next;
			while (p != NULL)
			{
				cout << p->data << " ";
				p = p->next;
			}
			cout << endl;
		}
		else
			cout << endl;

	}
}

int funcMod(const string &x)    //取余法
{
	int a = 0;
	for (auto i : x)         //此处使用了C++11的语法，请将编译器勾选这一选项
		a += (i - 96);
	return a % 37;
}



int funcAddr(const string &x)   //地址法
{
	int a = 0;
	for (auto i : x)         //此处使用了C++11的语法，请将编译器勾选这一选项
		a += (i - 96);
	return a / 2;
}



int funcPerfect(const string &x)     //二进制完美哈希
{
	int k = 0, a = 0, counter = 0;
	for (auto i : x)
	{
		k = pow(2, counter);
		counter++;
		a += k * (i - 96);
	}
	return a;
}



int main()
{
	string nameList[30] = {};
	for (int i = 0; i < 30; i++)
	{
		cin >> nameList[i];
	}

	hashTable hashlist(37, *(funcMod));
	for (int i = 0; i < 30; i++)
		hashlist.insert(nameList[i]);
	hashlist.print();
	cout << hashlist.crash;


	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
