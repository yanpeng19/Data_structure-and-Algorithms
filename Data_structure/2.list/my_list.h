/*
	list �б�ģ���д

		0.list ����ɣ� list��һ���� Node�� Ϊ�ڵ㣬�ṹ��ɵ����ݽṹ
		0.1 Node��:
		{
			T* data;           �洢����
			Node *pre;         ��¼ǰһ������
			Node *next;        ��¼��һ������
		}

		1.list ���ص㣺 list��һ�ֱ���ɾ��/��ӽڵ�����ݽṹ ƽ������ʱ��O(1)
	                    ������ʽ�ṹ��ʹ��ÿ�β��Ҷ���Ҫƽ�� O(n)��ʱ��
						������Ƶ��ɾ�������ĳ���

		2.���캯���Լ��������� �����ݲ���
		{
			public��
			 my_list();
			 my_list(const T&);
			 my_list(const my_list&);
			 my_list(initializer_list<T>);
			 ~my_list()

			private:
			 Node* head;          //���е�һ������ָ��
			 Node* tail;          //�������һ������ָ��
			 int _size;           //����Ԫ�ظ���
		}

		3.����
			3.1  empty()           //�ж�ʱ��Ϊ��
			3.2  remove()          //ɾ��Ԫ��
			3.3  size()            //Ԫ�ظ���
			3.4  insert_before()   //�ڽڵ�ǰ����
			3.5  insert_after()    //�ڽڵ�����
			3.6  find()            //�����������
			3.7  seach()           //�����������
			3.8  sel_sort()        //ѡ������
			3.9  insert_sort()     //��������

		4.���켰ʹ���߼�����
			4.1 ���죺	��ʼ��head,tail,_size;
			4.2 ���ң�  ��head��tail ��һ�������Ƚ�Ԫ��
			4.3 ���룺  ���Ӵ��� �ڵ�X �� X->pre , X->next ���߹�ϵ
			4.4 ɾ����  �����ڵ� - ����X->pre��x->next;
*/

#include "stdafx.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <initializer_list>

using namespace std;

template<typename T>
class Node
{
public:
	typename allocator<T> alloc;
	Node() :data(NULL), pre(NULL), next(NULL) {};

	Node(const Node&t) :data(t.data), pre(t.pre), next(t.next) {};
	Node(const T& t) :pre(NULL), next(NULL)
	{
		data = alloc.allocate(1);
		alloc.construct(data, t);

	};
	~Node()
	{
		cout << "Node ����һ��" << endl;
	}
	void free()
	{
		//pre = next = NULL;
		alloc.destroy(data);
		alloc.deallocate(data, 1);
	}
	T get() { return *data; };
	T * data;
	Node *pre;
	Node *next;
};

template<typename T>
class my_list
{
public:
	class const_iterator
	{
	public:
		const_iterator() :iter(NULL) {};
		const_iterator(const Node<T>*t) :iter(t) {};
		const_iterator(Node<T>*t) :iter(t) {};
		const_iterator(const const_iterator&t) :iter(*t) {};

		const_iterator operator=(const const_iterator&t) const
		{
			iter = t.iter;
			return *this;
		}

		const_iterator operator+(int i) const
		{
			for (int a = 0; a < i; a++)
			{
				iter = iter->next;
			}
			return *this;

		}

		bool operator==(const const_iterator&t)
		{
			return *iter == *t;
		}

		bool operator!=(const const_iterator&t)
		{
			return iter == *t?0 : 1;
		}
		const_iterator operator++() const
		{
			iter = iter->next;
			return iter->pre;
		}
		const_iterator operator++(int)
		{

			iter = iter->next;
			const_iterator a(iter);
			return a;
		}
		const_iterator operator--() const
		{
			iter = iter->pre;
			return iter->next;
		}
		const_iterator operator--(int) const
		{
			iter = iter->pre;
			return *iter;
		}
		Node<T>* operator*() const
		{
			return iter;
		}
		Node<T>* operator->() const
		{
			return iter;
		}
	private:
		Node<T> *iter;
	};

	class iterator
	{

	public:
		iterator() :iter(NULL) {};
		iterator(Node<T> *t) :iter(t) {  };
		iterator(const iterator &t) :iter(t.iter) {  };

		iterator operator=(const iterator&t)
		{
			iter = t.iter;
			return *this;
		}

		iterator operator+(int i)
		{
			for (int a = 0; a < i; a++)
			{
				iter = iter->next;
			}
			return *this;

		}

		bool operator==(iterator t) const
		{
			return iter == *t;
		}

		bool operator!=(iterator t) const
		{
			return iter == *t ? 0 : 1;
		}

		iterator operator++()
		{
			iter = iter->next;
			return iter->pre;
		}
		iterator operator++(int)
		{

			iter = iter->next;
			iterator a(iter);
			return a;
		}
		iterator operator--()
		{
			iter = iter->pre;
			return *this;
		}
		iterator operator--(int)
		{
			iter = iter->pre;
			return *this;
		}
		Node<T>* operator*()
		{
			return iter;
		}
		Node<T>* operator->()
		{
			return iter;
		}

	private:
		Node<T> *iter;

	};

	my_list() :_size(0)
	{
		head = new Node<T>();
		tail = new Node<T>();
		head->next = tail;
		tail->pre = head;
	};
	my_list(const T&);
	my_list(const my_list&);
	my_list(initializer_list<T>);
	~my_list()
	{
		head = head->next;
		while (head != tail)
		{
			auto temp = head->next;
			head->free();
			head = temp;
		}
	}

	T front();
	T back();
	Node<T>* remove(const T&);
	Node<T>* remove(Node<T>* t);

	int unique();
	bool empty(){return head->next == tail ? 1 : 0;}
	void push_back(const T&);
	void push_front(const T& t);
	T pop_back()
	{
		if (empty()) return NULL;
		auto temp = tail->pre->get();
		erase(end()->pre);
		return temp;
	}
	T pop_front()
	{
		if (empty()) return NULL;
		auto temp = head->next->get();
		erase(begin());
		return temp;
	}

	iterator begin(){ return head->next;}
	const_iterator const cbegin() const{ return head->next; }
	const_iterator const cend() const { return tail; }
	iterator end() { return tail; }


	int size() { return _size; };
	void erase(Node<T>* );
	void insert_before(Node<T>* post,Node<T>* e);
	void insert_after(Node<T>* post, Node<T>* e);
	Node<T>* find(const T&);   //�������
	Node<T>* find(const T&, int rank, Node<T>* p);
	Node<T>* seach(const T&);  // �������
	Node<T>* seach(const T&, int rank,const Node<T>* const &post);

	//��������
	void sel_sort();//ѡ������
	void sel_sort(Node<T>* post, int n);
	Node<T>* sel_max(Node<T>* post, int n);
	void insert_sort();// ��������
	void insert_sort(Node<T>* post, int n);

	void out();

private:
	Node<T> *head;
	Node<T> *tail;
	int _size;
};

template<typename T>
void my_list<T>::out()
{
	int i = 1;
	for (auto a = begin(); a != end(); a = a->next)
	{
		cout << i << ". " <<  a << " :" << a->get() << endl;
	}
	cout << endl;
}

template<typename T>
void my_list<T>::insert_sort(Node<T>* p,int n)
{

	//for (int i = 0; i < n; i++) p = p->next; // p ָ�������βԪ��

	for (int rank = 0; rank <= n; rank++)
	{
		auto pos = seach(p->get(), rank, p);
		//out();
		Node<T>* temp = new Node<T>(p->get());
		insert_after(pos,temp);
		//out();
		p = p->next;
		remove(p->pre);
		//out();
	}
}

template<typename T>
void my_list<T>::insert_sort()
{

	//for (int i = 0; i < n; i++) p = p->next; // p ָ�������βԪ��
	Node<T>* p = *(begin());
	int n = _size;
	for (int r = 0; r <= n; r++)
	{
		auto pos = seach(p->get(), r, p);
		//out();
		Node<T>* temp = new Node<T>(p->get());
		insert_after(pos, temp);
		//out();
		p = p->next;
		remove(p->pre);
		//out();
	}
}

template<typename T>
void my_list<T>::sel_sort(Node<T>* post, int n) //�ڵ�p ֮���n��Ԫ�ؽ�������
{
	Node<T>* p;
	Node<T>* tail_ = post->pre;
	for (p = begin(); p != end(); p = p->next)
		if (p == post) break;
	if (p == post) //�ڵ���ɹ�
	{
		for (int i = 0; i < n;i++) //��p ָ����Ҫ��������Ľڵ�
		{
			if (p == end()) i = n;
			p = p->next;
		}

		while(n>0)
		{
			auto max = sel_max(post, n);
			auto resault = remove(max);
			insert_after(tail_,resault);

			/*for (auto a =begin(); a != end(); a = a->next)
			{
				cout << a->get() << " ";
			}*/
			cout << endl;
			n--;
		}
	}
}

template<typename T>
void my_list<T>::sel_sort() //�ڵ�p ֮���n��Ԫ�ؽ�������
{
	Node<T>* post = begin();
	int n = _size;
	Node<T>* p;
	Node<T>* tail_ = post->pre;
	for (p = begin(); p != end(); p = p->next)
		if (p == post) break;
	if (p == post) //�ڵ���ɹ�
	{
		for (int i = 0; i < n; i++) //��p ָ����Ҫ��������Ľڵ�
		{
			if (p == end()) i = n;
			p = p->next;
		}

		while (n>0)
		{
			auto max = sel_max(post, n);
			auto resault = remove(max);
			insert_after(tail_, resault);

			/*for (auto a = begin(); a != end(); a = a->next)
			{
				cout << a->get() << " ";
			}*/

			n--;
		}
	}
}

template<typename T>
Node<T>* my_list<T>::sel_max(Node<T>* post, int n) //��post ��ʼ ������n������ �ҳ�����һ���ڵ�
{
	Node<T>* max = post;
	while (n > 1)
	{
		post = post->next;
		if (max->get() < post->get())
		{
			max = post;
		}
		n--;
	}
	return max;
}

template<typename T>
my_list<T>::my_list(initializer_list<T> ls):my_list()
{
	for (auto a : ls)
	{
		push_back(a);
	}
}

template<typename T>
void my_list<T>::erase(Node<T>* n)
{
	auto temp = begin();
	while (temp != n)
	{
		if (temp == end()) break;
		temp=temp->next;
	}
	if (temp == n)
	{
		temp->pre->next = temp->next;
		temp->next->pre = temp->pre;
		temp->free();
	}
	_size--;
}

template<typename T>
int my_list<T>::unique()
{
	if (_size < 2) return 0;
	int rank = 1;
	auto post = begin()->next;
	int old_size = _size;

	while (post != end())
	{
		auto q = find(post->get(), rank, post);
		if (q != NULL) erase(q);
		else rank++;
		post = post -> next;
	}
	return old_size - _size;
}

template<typename T>
Node<T>* my_list<T>::seach(const T& t, int n,const Node<T>* const &post)
{
	iterator p;
	for (p = --(begin()); p != end(); p++) { if (*p == post) break; };
	if (*p == post)
	{
		while (n > 0)
		{
			p--;
			if (p == head) return head;
			if (p->get() <= t) break;
			n--;
		}
		return *p;
	}
	else
	{
		cout << "node error, post out of list" << endl;
		return NULL;
	}
}

template<typename T>
Node<T>* my_list<T>::seach(const T&t)
{
	Node<T>* temp;
	for (temp = end() -> pre; temp != head->pre; temp=temp->pre)
	{
		if (temp == head) return head;
		if (temp->get() <= t) return temp;
	}
}

template<typename T>
Node<T>* my_list<T>::find(const T&t)
{
	for (auto a = begin(); a != end(); a = a->next)
	{
		if (a->get() == t)
			return a;
	}
	return NULL;
}

template<typename T>
Node<T>* my_list<T>::find(const T&t, int rank, Node<T>* p)
{
	Node<T>* temp;
	for (temp = end(); temp != head; temp = temp->pre)
	{
		if (temp == p) break;
	}
	if (temp == p) //λ�ýڵ�p ��list�еĽڵ㣬��ʼ��������
	{
		auto a = p;
		while (rank != 0)
		{
			a = a->pre;
			rank--;
			if (a == head) return NULL;
			if (a->get() == t) return a;
		}
		return NULL;

	}
	return NULL;
}

template<typename T>
T my_list<T>::front()
{
	return head->next->get();
}

template<typename T>
T my_list<T>::back()
{
	return tail->pre->get();
}

template<typename T>
my_list<T>::my_list(const T&t):my_list()
{
	push_front(t);
}

template<typename T>
void my_list<T>::push_front(const T& t)
{
	Node<T> * temp = new Node<T>(t);
	_size++;
	temp->next = head->next;
	head->next->pre = temp;
	temp->pre = head;
	head->next = temp;
}

template<typename T>
void my_list<T>::push_back(const T&t)
{
	Node<T> *temp = new Node<T>(t);
	_size++;
	temp->next = tail;
	temp->pre = tail->pre;
	tail->pre->next = temp;
	tail->pre =temp;
}

template<typename T>
my_list<T>::my_list(const my_list& m):my_list()
{
	Node<T>* t;
	for (t = m.cbegin(); t != m.cend(); t = t->next)
	{
		push_back(t->get());
	}
}

template<typename T>
Node<T>* my_list<T>::remove(const T& t)
{
	auto a = begin();
	while (a != end())
	{
		if (a->get() == t)
		{
			auto b = a;
			a = a->next;
			Node<T>* resault = new Node<T>(b.get());
			erase(b);
			return resault;
		}
		a = a->next;
	}
	return NULL;
}

template<typename T>
Node<T>* my_list<T>::remove(Node<T>* t)
{
	auto a = begin();
	while (a != end())
	{
		if (a == t)
		{
			auto b = a;
			a = a->next;
			Node<T>* resault = new Node<T>(b->get());
			erase(*b);
			return resault;
		}
		a++;
	}
	return NULL;
}
template<typename T>
void my_list<T>::insert_before(Node<T>* p,Node<T> *t)
{
	auto temp = begin();

	while (temp!=end())
	{
		if (temp == p) break;
		temp = temp->next;
	}
	if (temp == p)
	{
		t->pre = p->pre;
		t->next = p;
		p->pre->next = t;
		p->pre = t;
	}
}

template<typename T>
void my_list<T>::insert_after(Node<T>* p, Node<T> *t)
{
	auto temp = --(begin());

	while (temp != end())
	{
		if (temp == p) break;
		temp = temp->next;
	}
	if (temp == p)
	{
		t->pre = p;
		t->next = p->next;
		p->next->pre = t;
		p->next = t;
	}
}

