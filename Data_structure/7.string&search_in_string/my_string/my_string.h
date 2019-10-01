#pragma once
#define  _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <stdexcept>
#include <sstream>

using namespace std;

static allocator<char> alloc;


class my_string
{
	friend bool operator==(const my_string&, const my_string&);
	friend int brote_force(const my_string &p, const my_string &s);
	friend int BM_BC(my_string &p, my_string &s);
	friend ostream& operator<<(ostream&os, const my_string &s);

public:
	my_string() :elements(NULL), cap(NULL), first_free(NULL) {};
	my_string(const char*);
	my_string(const my_string&);
	my_string(const char*, const char*);
	my_string(initializer_list<char> s);
	my_string& operator=(const my_string &);
	my_string& operator+=(const my_string &);
	//my_string& operator<<(const my_string &);
	
	my_string& operator>>(my_string &);
	char& operator[] (unsigned i)
	{
		return *(elements + i);
	}
	char operator[] (unsigned i) const
	{
		return *(elements + i);
	}


	my_string(my_string && s)noexcept :
		elements(s.elements), first_free(s.first_free), cap(s.cap)
	{
		s.elements = s.first_free = s.cap = NULL;
	}
	my_string & operator =(my_string&& s)
	{
		elements = s.elements;
		first_free = s.first_free;
		cap = s.cap;
		s.elements = s.first_free = s.cap = NULL;
		return *this;
	}


	~my_string();
	void insert( const char);
	int size() const { return first_free - elements; };
	int capcity() const { return cap - elements; };
	bool empty() const { return first_free - elements ? false : true; };

	void check_n();
	void relo();
	void free();
	void out();

	char *begin() const { return elements; }
	char *end() const { return first_free; }
	char *begin() { return elements; }
	char *end() { return first_free; }

private:
	char *elements;
	char *cap;
	char *first_free;

};

my_string::my_string(const char *begin, const char* end)
{
	while (begin != end) insert(*begin++);
}

ostream& operator<<(ostream&os,const my_string &s)
{
	for (auto a : s)
		os << a;
	cout << endl;
	return os;
}

void get_ss(my_string& s, vector<int>& ss);
void get_gs(my_string& s, vector<int>& ss,vector<int>& gs);
/*
	get_bc 得到字符位置
	1.思路： 将256个字符存放到一个字符数组中。
	         每个字符存放的内容是：
			 “该字符在查找串P中的最后的位置”


*/
void get_bc(my_string& p, vector<int>& bc)
{
	for (int i = 0; i < p.size(); i++)
	{
		char temp = p[i];
		bc[temp] = i;  // bc[temp] 就是 p[i]的字符 在 bc中的下标，然后将其出现的位置，存储到bc中，就可以直接通过 字符查找最后出现的位置
	}
}

/*
	BM_BC 坏字符算法
	1.思路： 因为字符串不匹配的情况远远高于匹配的情况，所以 查找串按照从后往前的次序进行。
	         如果S[J]！=P[I+J],则从s[0~i+j]中寻找字符 s[j].
			 1.1 如果存在且小于 j 那么 可以直接移动查找串,调整数值i ,使得 p[i+j]==s[j]
			     并进行下一轮比对
			 1.2 如果存在但是大于 i 那么安全起见，向后移动1
			 1.3 如果不存在， 则可以直接前进到头使得s[0] 与 p[i+j+1] 进入下一轮比对

*/
int BM_BC(my_string &p,my_string &s)
{
	// i 对齐位置  j 查找串下标
	int i = 0;
	vector<int> bc(256,-1);
	int p_len = p.size();
	int s_len = s.size();
	int j = s_len - 1;
	get_bc(p,bc);

	while (i+j < p_len)
	{
		//bacbababad ababac ambabacaddababacasdsd
		//
		if (j < 0) return i;

		if (p[i + j] == s[j])
			j--;
		else
		{
			int temp_i = bc[p[i+j]]; //调整对其位置
			if (temp_i < i + j)
			{
				i = temp_i;
				j = s_len - 1;
			}
			
			else if (temp_i > i + j)
			{
				i++;
				j = s_len - 1;
			}
			else if (temp_i == i + j) return -1;
			else if (temp_i == -1)
			{
				i = i + j + 1;
				j = s_len - 1;
			}
		}
	}
	return -1;
}

/*
	BM_GS 好字符算法
	概述：在坏字符算法的基础上，对于 temp_i>i+j 的情况进行进一步的改进和处理
	思路：当 bc[p[i+j] > i+j的时候，即表示当前比对字符，在查找串中最右的位置，大于现在的i+j比对位置。
		  但是 查找串中 s[j~s.size()-1]中，是已经对齐的，我们可以通过在s[0~j]中寻找一个 子串 ，
		  该字串是 s[0~j] 与 s[j~size()-1] 的最大字串。
		  首先用数组 ss[] 遍历s,（i=1;i<s.size();i++)
		  寻找每个以i切割s,前后两个字串的 最大同子串，将其长度存在ss[i]中

		  ss[]向gs[]转换，分两种情况：
		  1.ss[j] == j+1; 即最大字串为 前字串本身。 那么可以直接移动 [m-ss[j]-1]距离
		  2.ss[j] <= j; 即是 前子串的部分；  意味着最大字串在 [m-ss[j]-1] 距离上失配了，
		                同时，又是最大字串，所以可以将字串 移动 [m-ss[j]-1]距离（暂时不太理解，需要测试）
		  3.所以 gs[j] = [m-ss[j]-1] 
		  3.1 当 ss[j] == -1 说明好字符模式 无法继续下去，即字符不存在，直接可以越过 i+j;

		  4.ss[]的构造方法： 即如何寻找两个串的最大公共子序列：
			4.1从后向前思路，即i = size-2;i>0;i++; j = size()-1;
			                   f = i;
			   if(s[f]==s[j] && j>i && f>0)
			      f--;
				  j--;
			   ss[j] = i-f;

			   花费时间 n*m 时间近似


*/
int BM_GS(my_string &p,my_string &s)
{
	// i 对齐位置  j 查找串下标
	int i = 0;
	vector<int> bc(256, -1);
	vector<int> ss(s.size(), -1);
	vector<int> gs(s.size(), -1);

	int p_len = p.size();
	int s_len = s.size();
	int j = s_len - 1;
	get_bc(p, bc);
	get_ss(s, ss);
	get_gs(s, ss, gs);


	while (i + j < p_len)
	{
		//b acbababad ababac ambabacaddababacasdsd
		//
		if (j < 0) return i;

		if (p[i + j] == s[j])
			j--;
		else
		{
			int temp_i = bc[p[i + j]]; //调整对其位置
			if (temp_i < i + j)
			{
				i = temp_i;
				j = s_len - 1;
			}

			else if (temp_i > i + j)
			{
				//i++;
				i =i+gs[j-1];
				j = s_len - 1;
			}
			else if (temp_i == i + j) return -1;
			else if (temp_i == -1)
			{
				i = i + j + 1;
				j = s_len - 1;  
			}
		}
	}
	return -1;
}

/*
	get_ss 思路： 遍历后串，向前推进，永远和前串最后一个字符开始对比
	* rank 遍历位置
	* j 后串位置, j=s.size()-1; j<1;

	2.找到 rank切分的两个子串的最大子串 字符个数
	3.max_size = 0; 并从后往前遍历 前字串，寻找最大值 并赋予 max_szie
	4.将max_size压入ss[rank] 中

*/
void get_ss(my_string &s, vector<int>& ss)
{
	int rank;
	//合计复杂度O(s²)
	for ( rank = 0; rank < s.size()-1; rank++)//遍历s 复杂度 O(s)
	{
		int j = s.size() - 1;
		int i = rank;
		int max_size = 0;
		while (i>=0&&j>rank)    // 复杂度 O(m)
		{
			//ababaca
			if (s[i] == s[j])
			{
				i--;
				j--;
				max_size = (rank - i) > max_size ? (rank - i) : max_size; //每次相同都进行判定
			}
			else
			{
				j--;
				i = rank;
			}
		}
		ss[rank] = max_size;
	}
	ss[s.size() - 1] = s.size()-1;
}

/*
	get_gs 存储的内容为：
	在通过BM算法遇到 不一致情况的时候，根据好字符匹配原则，#将前半串和后半串对齐，需要移动的距离#
	具体可以描述为：
	s_len = s.size();
	rank = 1;rank < s_len-1;rank++

	根据不同情况处理：
	情况1： ss[rank] = rank;  即 前半段字符都是最长公共子串.那么说明后半穿存在着某个和前半串相同的字串，其可能位置从ss[rank+1]~ss[s.size()-1]
	                          所以移动 距离为rank

	情况2： ss[rank] <= rank; 即 前半串 部分字符为最长公共子串，说明后半穿存在着某个和  前半串 ss[rank]个 相同字符，存在于后半段，其可能位置为rank+ss[rank]开始
	                          所以要把ss[rank]个字符拿到 rank之后，所以移动的距离为 ss[rank];

	情况3： ss[rank] = 0;     即 前半段 没有和后半段相同的字符。说明可以直接把s串挪动过去，即挪动rank 个
*/
void get_gs(my_string &s, vector<int>& ss, vector<int>& gs)
{
	int s_len = s.size();
	gs[0] = 0;
	for (int rank = 0; rank < s_len; rank++)
	{
		if (ss[rank] == rank) gs[rank] = rank;
		if (ss[rank] == 0) gs[rank] = 1;
		else gs[rank] = ss[rank];
	}
}

int string_hash(my_string s);
/*
	KP算法：
	*思路： 
	1.根据查找串长度 s s_len
	2.将 s 进行hash（s) 处理 得到 hash_s;
	3.int rank;   rank = 0 ;rank < (p_len-s_len)遍历
	4.每次分别将 p[rank]~p[rank+s_len]串 进行hash处理 得到 p_hash
	5.如果p_hash == s_hash 进行二次检测，对别字符串 
	5.1 如果相同则 return i ;
	5.2 如果不同 则i++
*/

int KP(my_string &p, my_string &s)
{
	int s_len = s.size() - 1;
	int rank = 0;
	int s_hash = string_hash(s);


	for (rank = 0; rank < p.size() - s_len; rank++)
	{
		my_string temp(p.begin()+rank, p.begin()+rank + s_len + 1);
		int temp_hash = string_hash(temp);
		//cout << temp << endl;
		if (temp_hash == s_hash) //二次验证
		{
			for (int i = 0; i < s_len; i++)
			{
				if (s[i] != p[rank + i]) break;
			}
			return rank;
		}
		
	}
	return -1;
}
/*
	字符串hash方法：
	1.将字符串s 按照26进制转换 获得 longlong s_int;    #问题冲突处理
	2.将 s_int 取%  hash处理 s_int % key    (key是一个合适的素数） #如何寻找合适的素数？
	3.return
*/
int string_hash(my_string s)
{
	stringstream ss;
	vector<int> num;

	// 字母转换成 +48
	for (auto a : s) 
		ss << a + 48;
	int key = 97;
	if (ss.str().size() > 9)
	{
		string s_temp(ss.str());
		s_temp.erase(s_temp.begin() + 2, s_temp.end());
		int temp = stoi(s_temp);
		s_temp = ss.str();
		for (int i = 2; i < s_temp.size(); i++)
		{
			temp = temp % key;
			temp = temp * 10 + (s_temp[i] - 48);
		}
		temp = temp % key;
		return temp;
	}
	else
	{
		int s_int = stoi(ss.str());
		return s_int & key;
	}
}


bool operator== (const my_string &r, const my_string &l)
{
	string sr(r.elements, r.first_free);
	string sl(l.elements, l.first_free);
	//cout << sr << endl;
	//cout << sl << endl;
	return sr == sl;
}

int brote_force(const my_string &p, const my_string &s)//p 中查找s
{
	int p_len = p.size();
	int s_len = s.size();

	int i = 0;
	int j = 0;

	while (i < p_len && j < s_len)
	{
		if (p[i] == s[j])
		{
			i++; j++;
		}
		else
		{
			i=i-j+1; 
			j = 0;
		}
	}

	if (j == s_len) return i-j;
	else return -1;

}

/*
getNext的算法：
1.int i,j两个下标。
2.遍历for(int i= 0；i<strlen(p);i++)
3.next的概念： 即需要移动多少个数值，就可以使得 总串和 对比串 的前半部分依然相同。
4.next[0] = -1 是一个标兵
5.做判断 （j==-1 || p[j] == p[i] ) 就推进一个单位，i++，j++;
next[i] = j;(存储的是 下一个位置，该位置表示 s[0~j] == p[i-1 ~i -j]
如果宜然相等，则继续推进下去。
如果不相等，则寻找下一个 满足的next[j]的位置，重新比较

*/
void getNext(char * p, int * next)
{
	//ababa c a
	//  aba baca
	//    a babaca
	next[0] = -1;
	int i = 0, j = -1;

	while (i < strlen(p))
	{
		if (j == -1 || p[i] == p[j])
		{
			++i;
			++j;
			next[i] = j;
		}
		else
			j = next[j];
	}
}

void get_next(const my_string &p, vector<int> &next)
{
	int i = 0;
	int j = -1;
	next.push_back(-1);
	while (i < p.size())
	{
		if (j == -1 || p[i] == p[j])
		{
			i++;
			j++;
			int temp = (p[j] != p[i]) ? j : next[j];
			next.push_back(temp);
		}
		else j = next[j];
	}
}

int BMP(const my_string &p, const my_string &s)
{
	vector<int> next;
	get_next(p, next);
	int i = 0;
	int j = 0;
	int p_len = p.size();
	int s_len = s.size();

	while (i < p_len&& j < s_len)
	{
		if (j==-1 ||p[i] == s[j])
		{
			i++;
			j++;
		}
		else j = next[j];
	}
	if (j == s_len) return i - j;
	else return -1;
}

void my_string::out()
{
	auto temp_l = elements;
	while (temp_l != first_free)
		cout << *temp_l++;
	cout << endl;
}
void my_string::check_n()
{
	if (!(cap - first_free))
		relo();
}
void my_string::free()
{
	auto temp_fir = first_free;
	while (temp_fir != elements)
		alloc.destroy(--temp_fir);
	alloc.deallocate(elements, capcity());
}
void my_string::relo()
{
	int leth;
	leth = elements ? capcity() * 2 : 1;
	auto temp_elem = alloc.allocate(leth);
	uninitialized_copy(elements, first_free, temp_elem);
	auto temp_first = temp_elem + size();
	auto temp_cap = temp_elem + leth;
	free();
	elements = temp_elem;
	first_free = temp_first;
	cap = temp_cap;
}
void my_string::insert(const char c)
{
	check_n();
	alloc.construct(first_free++, c);
}

my_string::my_string(initializer_list<char> s)
{
	for (auto a : s)
		insert(a);
}

my_string::my_string(const char *c)
{
	for (int a = 0; *(c + a) != '\0'; a++)
		insert(*(c + a));
}
my_string::my_string(const my_string &s)
{
	int size = s.first_free - s.elements;
	elements = alloc.allocate(size);
	cap = first_free = elements + size;
	auto temp_s = s.elements;
	auto temp_ele = elements;
	while (temp_s != s.first_free)
		*temp_ele++ = *temp_s++;
	//cout << " copy " << endl;
}
my_string& my_string::operator=(const my_string &s)
{
	int size = s.first_free - s.elements;
	auto new_elem = alloc.allocate(size);
	auto new_first_free = new_elem;
	auto temp_ele = s.elements;
	for (auto i = s.elements; i != s.first_free; i++)
	{
		alloc.construct(new_first_free++, *(temp_ele++));
	}
	//cout << "run '=' once" << endl;
	free();
	elements = new_elem;
	first_free = new_first_free;
	cap = new_elem + size;
	return *this;
}

my_string& my_string::operator+=(const my_string &s)
{
	int s_size = s.first_free - s.elements;
	int size = first_free - elements;
	if ((cap - first_free) > s_size)
	{
		uninitialized_copy(s.cap, s.first_free, first_free);
		first_free += s_size;
		return *this;
	}
	auto new_elem = alloc.allocate(size + s_size);
	auto new_cap = new_elem + (size + s_size);
	auto new_first_free = new_cap;
	uninitialized_copy(elements, first_free, new_elem);
	uninitialized_copy(s.elements, s.first_free, new_elem + size);
	free();
	elements = new_elem;
	first_free = new_first_free;
	cap = new_cap;
	return *this;
}

//my_string& my_string::operator<<(const my_string &s)
//{
//	my_string temp(" ");
//	*this += temp;
//	*this += s;
//	return *this;
//}

my_string& my_string::operator>>(my_string &s)
{
	my_string temp(" ");
	s += temp;
	s += *this;
	return s;
}

my_string ::~my_string()
{
	free();
}

