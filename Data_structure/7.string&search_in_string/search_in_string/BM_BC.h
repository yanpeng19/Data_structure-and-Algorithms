#pragma once
#include "my_string.h"
/*
BM_GS 算法
-------------------------------------------------------------------------------------------------------
是对 BM_BC 算法的一种改进，主要是针对 当

P: ACDRSGL|SLXSDIWERQO
S: SLOWIEO|SL
i = 0,j = 6;
char c = p[i+j] = L;
bc[c] = 8;

bc[c] > j;  //  最后一次出现的在对比位置之后，那么我们如果要对齐，就必须把S串拖回来，这样
//  对比问题反而扩大了，所以简单的往后移动一个位置就可以了；

这种只能+1的情况进行处理的；

我们的目的： 当这种只能移动1个单位的情况，我们想通过一个合理的办法在可以接受u成本的情况下移动多个

步骤：
第一大步.我们要构造一个 ss 数组；而 ss 的内容： 查找串中 for(i=1;i<size()-2) 的情况下，
将 S[i]~S[0] 和 S[size-1]~s[i] 进行比对，寻找最长的子字符串的长度
例如：

I 后半部分的比较必须从尾部开始：O（n^n) // n = 查找串的长度

                      i          ss
S: S|SLOWIEOSLO       0          0
S: SS|LOWIEOSLO       1          0
S: SSL|OWIEOSLO       2          0
S: SSLO|WIEOSLO       3          3 //SLO
S: SSLOW|IEOSLO       4          3
S: SSLOWI|EOSLO       5          3
S: SSLOWIE|OSLO       6          3
S: SSLOWIEO|SLO       7          3
S: SSLOWIEOS|LO       8          2
S: SSLOWIEOSL|O       9          1
S：SSLOWIEOSLO|       10         10

ss[]存储的内容是： 当 i 在不同位置切割查找串的时候，前半部分和后半部分相同的 字符的 #长度#.
我们可以根据3种情况，来判断移动的距离生成GS


第二大步：生成GS[] 确定移动的数位
A.如果在坏字符判断之后，我们在好字符模式同样没有找到可以匹配的情况，ss[i] = 0;
  这时候，我们可以判断，前面的位置 没有字符和 s[s.size()-1] 相匹配了，所以这种情况的话
  我们可以 直接拖动 s.size()个字符：

  P: ACDRSEL|SLXSDIWERQO
  S: AESLIEO|SL

  i = 6;
  ss[i] = 0;
  无法确认移动的数量，暂定1好了

 B.如果ss[i]!=0 而且 ss[i] = i+1:

  P: AC|DRSELSLXSDIWERQO
  S: SL|DRSELSL

  i = 1;
  ss[i] = 2;
  移动距离为：将S拖过去对齐， s.size()-(ss[i]+1) = 7;

 C.如果ss[i] !0 但是 ss[i]!=i+1;

  P: AC|DRSELSLXSDIWERQO
  S: SS|DRSELSL

  i=1;
  ss[i] = 1;
  移动的距离为： ss[i] = 1 个单位


然后对BM_BC的算法，适当进行修改即可：

s:查找串 p：总串

vector<int> bc(256,-1);
vector<int> ss,gs;
get_bc(s,bc);
get_ss(s,ss);
get_gs(s,gs,ss);

for (int i = 0; i < p.size() - s.size() - 2; )
{
	int j = s.size()-1;
	while (p[i + j] == s[j])
	{
		if (j == 0) return i;
		j--;
	}
	if (bc[p[i + j]] >= j) i = i+gs[j];         // if (bc[p[i + j]] > j) i++;  仅修改这里
	else i = i + (s.size() - bc[p[i + j]] - 1);
}
return -1;
*/

using namespace std;

void get_bc(my_string& s, vector<int>& bc);
void get_ss(my_string& s, vector<int>& ss);
void get_gs(my_string& s, vector<int>& ss, vector<int>& gs);

/*
	get_bc 得到字符位置
	1.思路： 将256个字符存放到一个字符数组中。
	         每个字符存放的内容是：
			 “该字符在查找串P中的最后的位置”


*/

int BM_GS(my_string &p,my_string &s)
{
	// i 对齐位置  j 查找串下标
	int i = 0;
	vector<int> bc(256, -1);
	vector<int> ss;
	vector<int> gs;

	int p_len = p.size();
	int s_len = s.size();
	int j = s_len - 1;
	get_bc(s, bc);
	get_ss(s, ss);
	get_gs(s, ss, gs);

	for (int i = 0; i < p.size() - s.size() - 2;)
	{
		int j = s.size() - 1;
		while (p[i + j] == s[j])
		{
			if (j == 0) return i;
			j--;
		}

		if (bc[s[j]] >= j)
		{
			i = i + gs[j];
			//i++
		}
		else i = i + (s.size() - bc[p[i + j]] - 1);
	}
	return -1;
}

void get_bc(my_string& s, vector<int>& bc)
{
	for (int i = 0; i < s.size(); i++)
	{
		char temp = s[i];
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
	using rank = int;
	ss = vector<int>(s.size(), 0);

	for (rank i = s.size() - 2; i >= 0; i--)//遍历s 复杂度 O(s)
	{
		//same 最后一位
		// baba|ca
		int same = s.size()-1;
		int j = i ;

		while (s[j] == s[same])
		{
			if (j < 0) break;
			j--;
			same--;
		}
		ss[i] = s.size() - 1 - same;

	}
	ss[ss.size()-1]=s.size();
}

void get_gs(my_string &s, vector<int>& ss, vector<int>& gs)
{
  	for (int i = 0; i < s.size()-1; i++)
	{
		//三种情况
		if (ss[i] == 0) gs.push_back(1);
		else if (ss[i]!=0 && ss[i] == i + 1) gs.push_back(s.size()-1-ss[i]);
		else gs.push_back(ss[i]);
	}
	gs.push_back(1);

	for (int i = 0; i < gs.size(); i++)
		cout << "gs_" << i << ":" << gs[i] << endl;
}



