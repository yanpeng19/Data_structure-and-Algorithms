#pragma once
#include "my_string.h"

/*
KP算法
--------------------------------------------------------------------------------------------
思想：KP算法是通过将字符串转换成数字，然后在O(1)的时间进行数字对比的一种思想；

步骤：
1.将查找的内容 s 进行 hash处理，得到 s_hash;
2.遍历总串 p ； p = 0 ; p < p.size() s.size()-1; p++ 所有可能位置，
  分别将 p[i]~p[i+s.size()] 进行 hash 得到 p_hash;
3.对比 p_hash 和 s_hash 如果相同，则做二次验证，如果确认一致，返回下标即可。

问题：如何hash ，为什么要hash?
字符串中每个字符 c ,都可以转化成Int 形式存储。如果不进行hash 处理，那么 字符串转换得到的值长度可能会
非常长，可能的最大长度为 int s_ = s.size() * 3;
很可能超过了 Int 的最大值，甚至超过了longlong 的数位。
所以需要hash 。

hash有各种不同的办法，这次我使用的是简单 素数97 求余hash.

int KP_hash(const string&s)
{
    stringstream ss;
	for(auto a :s)
	  ss << a+48;  //字符转化成数字；
    s_int = 0;

    if(s.szie()>9) //超过9位的长度，可能会对导致hash_s > 18 超过int存储长度,模拟除法
	{
	    string s_temp(ss.str()); //先得到数字
		int temp = 0;
		for(int i = 0;i<s_temp.szie();i++)
		{
		    temp = temp * 10 + (s_temp[i]-48);
			temp = temp % key;
		}
		return temp;
	}
	else
	{
	   int s_int = stoi(ss.str());
	   return s_int % key;
	}
}
--------------------------------------------------------------------------------------------
*/
using namespace std;

int KP(my_string &p, my_string &s)
{
	int s_len = s.size() - 1;
	int rank = 0;
	int s_hash = string_hash(s);

	for (rank = 0; rank < p.size() - s_len; rank++)
	{
		my_string temp(p.begin()+rank, p.begin()+rank + s_len + 1);
		int temp_hash = string_hash(temp);
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
		int temp = 0;
		s_temp = ss.str();
		for (int i = 0; i < s_temp.size(); i++)
		{
			temp = temp * 10 + (s_temp[i] - 48);
			temp = temp % key;
		}
		return temp;
	}
	else
	{
		int s_int = stoi(ss.str());
		return s_int % key;
	}
}
