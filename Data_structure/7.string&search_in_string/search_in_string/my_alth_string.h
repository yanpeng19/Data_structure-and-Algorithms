#pragma once
#include "my_string.h"

/*
本文件包括字符串查询的几种算法：暴力算法、KMP，以及BM算法；BM包括 BM_BC,BM_GS两种
------------------------------------------------------------------------------
现有字符串 总串p ，和查查找串 s

暴力算法：
----------------------------------------------------------------
1.遍历p[0]~p[p.size()-s.size()-2]： 
2.如果p[rank] == s[0]，那么持续比对下去 ；
  如果一直到 s 尾部都相同，则返回 rank 的数值，即在这个位置找到了
3.否则持续下去，最终返回-1；表示没有找到
*伪代码：
  for(int rank = 0;rank < p.size()-s.size()-2;rank++)
  {
     if(p[rank]==s[rank])
	 { 
	    for(int j = 0;j < s.size();j++)
		{
		   if(p[rank+j]==s[j])
		      if(j==s.size()-1) return rank;
	          else continue;
		   else break;
		}
	 }
  }
4.时间复杂度 O(N^N)
-----------------------------------------------------------------

KMP算法：
-----------------------------------------------------------------
KMP算法的概念和方法：
KMP是根据暴力算法进行改进，例如 有搜索串 S（"ABCDABACDE");
在和P("ABCDABDABCDABACDE")中查找，
在rank = 0，j = 6;即这种状态出现了不一致：
P: ABCDAB|DABCDABACDE
S：ABCDAB|ACDE

如果是暴力算法，我们回到头部，重新rank+1；
但是实际上，我们也可以 将 S 拖动4位置，使得 RANK + 4：
P: ABCDAB|DABCDABACDE
    S：AB|CDABACDE

继续进行对比；

这种“拖动行为的实质”：
在查找串的已经匹配部分，S_matched("ABCDAB")中，寻找一个
0~i == j~size()-1 部分的最大值；
在S_matched("ABCDAB")中， i = 1;j=4;
可以理解成： 已经匹配的 串（"ABCDAB"）如果需要挪动的话，可以将S[1]挪到和P已匹配串进行对齐；

记做 next[5] = 1;  //前5个字符已经匹配了，如果第六个配了，那么就可以把 S 串的第 1 个字符
                   //拖动过来进行下一次对比

这样以来，在查找串S中，从S[1]失陪到S[size()-2]失陪，每个情况，都可以找到这样的一个值；
表示在第 i 个字符如果失陪了，我们可以把查找串的第 next[i] 个字符来过来对齐，继续对比下一个；

next[0]=-1;   因为如果在 S[0] 失配的话，不能从S[0]之前，找到和S[0]一样的字符；就是说
              我们可以把 S 串痛快的拖过去，在下一个位置 rank+1 和 S[0] 重新进行对比；
next[s.size()-1] 是不存在的，因为这种情况已经是完全匹配了；

所以再次总结一下概念；如果在匹配的时候,已经匹配了的串坐标 是 S[0]~S[i] ;然后在S[i+1] 出现了失配
那么我们可以安全的把 S[next[i]] 拖过来和现在的比对位置对齐，然后继续对比下一个字符；

举例：
P: ABCDAB|DABCDABACDE
S：ABCDAB|ACDE
在 S[i] = 5 的情况，即 S[0]~S[i] 已经匹配，在S[i+1] 出现了失陪；那么把 S[next[5]] = S[1] 对齐，继续比较：
P: ABCDAB|DABCDABACDE            i = 5;next[i] = 1;
    S：AB|CDABACDE               在第六个字符失配了，那么把2个字符拖过来对齐，比较第三个即可

伪代码：
for(int rank = 0;rank < p.size()-s.size()-2; )
{
    if(p[rank]==s[rank])
    {
        for(int j = 0;j < s.size();j++)
        {
          if(p[rank+j]==s[j])
             if(j==s.size()-1) return rank;
             else continue;
          else
		  {
		      rank = (rank+(j+1) - (next[j]+1); //rank+(j+1)   j下标，所以需要+1，是表示rank挪到了最新匹配的位置
			                                    //next[j]+1    表示新对齐的S中，已经有 next[j]+1个字符是确认对齐了
												//再把rank拖回来 i 个字符，就可以确定新的rank位置了；
			  j = rank + next[j] + 1;          //新的对齐位置，然后再加上字母个数，就是下一个比对的字母了
            
			 /*图示：
			 P: ABCDAB|DABCDABACDE
			 S：ABCDAB|ACDE

		     	 P: ABCD|ABDABCDABACDE     rank = rank + j + 1;
			     S：AB|CDABACDE

			   P: ABCD|ABDABCDABACDE       rank = rank + j + 1 - (next[j]+1);
				 S：AB|CDABACDE
			/
		  }
        }
    }  
}

那么另外一个问题：如何高效的获取 next[] 数组？

                        i              next[i]
S：A|BCDABACDE          0                -1  
S：AB|CDABACDE          1                -1
S：ABC|DABACDE          2                -1
S：ABCD|ABACDE          3                -1
S：ABCDA|BACDE          4                 0         // S[0] == S[4]
S：ABCDAB|ACDE          5                 1        // S[0~1] == S[5-1]~S[5]
S：ABCDABA|CDE          6                 0        // S[0] == S[6]
S：ABCDABAC|DE          7                -1
S：ABCDABACD|E          8                -1

思路：我们要寻找的这个坐标有以下规则：
1.s[0]~s[next[i] 的内容 要和  s[j]~最后相同的位置  一致
2.s[0]~s[next[i] 的长度，必须 也和后半部分一致

所以这样分析的话，我们因为不能确认相同字符串的长度，所以无法从前到后进行遍历；
只能从后往前遍历，首先从 s[0]~s[i-1] 中找到 和 s[i] 相同的数据看，
将此时下标记作j，如果没有，那么j就是-1，可以直接判断next[i] = -1;
如果有 s[j] == s[i] 的话，那么继续向前对比，这种相同性如果可以达到s[0]，
那么就可以将 j 存到 next[i] 中；
如果不能的话，需要 j 继续向前寻找下一个 和 s[i]相同的数值：

for(int i = 1;i<s.size()-1;i++)   //这一步复杂度 O(N)
{
   int j = i-1;
   if(j==-1) //复1 情况
   {
       next[i] == -1;
	   continue;
   }
   for( j = i-1;j>=0;j--)  // 这一步复杂度O（N）
   {
       if(s[i]==s[j])
       {
           same_i = i;
	       same_j = j;
	       //循环向前比较
	       while(s[same_i]==s[same_j]&&same_j>=0) // 这一步大多数情况下，只有1-2次对比，通常达不到O（N），可以视为O（1）
	       {
	           same_i--;
		       same_j--;
	       }
	       if(same_j==-1) next[i] = j; // 如果到了-1说明 相同性已经到了s[0];
		}
   }
}

上面这段代码，逻辑没有问题，比较清晰，但是时间复杂度到达了O(N^N）；我们可以观察 j 的缩进方式，来进行对他的优化：
for(int i = 1;i<s.size()-1;i++)                  
{
      int j = i-1;
     if(j==-1) //复1 情况
     {
       next[i] == -1;
       continue;
     }
     for( j = i-1;j>=0;j=next[j])    //for( j = i-1;j>=0;j--)## 这里是修改的部分1 ##
     {
         if(s[i]==s[j])
         {
            same_i = i;
            same_j = j;
            //循环向前比较
            while(s[same_i]==s[same_j]&&same_j>=0)
            {
               same_i--;
               same_j--;
            }
            if(same_j==-1) next[i] = j; // 如果到了-1说明 相同性已经到了s[0];
         }
     }
}

说明：这里复习以下 next[i] 的内容，位置 next[i] 的字符  等于位置 i 的字符；并且这种相同性一致会向前推；
比如   S：ABCDAB|ACDE   i = 5; next[i] = 1;
再复习一下概念： S[next[i]] 一个最大的，且可以到达s[0]的 字符串的 尾坐标；

所以我们不需要在对比的过程中，将j一个字符一个字符的向前挪动，而直接找到 next[j] 这个唯一可能到达头部的，且相同的字串的尾部；
并继续向前推进对比；如果依然不同，我们继续从 位置 next[j] 开始，继续往前，寻找下一个可能性的下标；
同样的，我们依然不需要往前挪动一个位置，而是继续寻找 next[j] 对应的 next[] 坐标，直到 -1 或者对比成功为止；

这样，我们就将next构造的过程优化到了 O（N) 的程度；
然后我们就可以在查找开始的时候，构造一个 查找串的NEXT[]数组了；


-----------------------------------------------------------------



*/
using namespace std;

void get_next(const my_string &p, vector<int> &next);
int string_hash(my_string s);
void get_ss(my_string& s, vector<int>& ss);
void get_gs(my_string& s, vector<int>& ss, vector<int>& gs);

int BMP(const my_string &p, const my_string &s)
{
	vector<int> next; //先填满-2 表示这个位置还没 处理过
	get_next(p, next);
	int i = 0;
	int j = 0;
	int p_len = p.size();
	int s_len = s.size();

	while (i < p_len&& j < s_len)
	{
		if (j == -1 || p[i] == s[j])
		{
			i++;
			j++;
		}
		else j = next[j];
	}
	if (j == s_len) return i - j;
	else return -1;
}

void get_next(const my_string &p, vector<int> &next)
{
	int time = 0;
	next.push_back(-1);
	for (int i = 1; i < p.size() - 1; i++) //这里O(n)
	{
		int j = i - 1;
		if (j == -1)
		{
			next[i] = -1;
			time++;
			continue;

		}

		for( ;j>=0;j=next[j])       // 这里可以视为O（1）
		{
			int same_i = i;
			int same_j = j;
			while (p[same_i] == p[same_j] && same_j >= 0) // 这里一般是 O(1)
			{
				time++;
				same_i--;
				same_j--;
			}
			if (same_j == -1)
			{
				//找到了 赶紧溜
				next.push_back(j);
				time++;
				break;
			}
		}
		//遍历完了都没有成功的，只能是 -1； ps:被我提前填满了 -2；
		if (next.size() <= i)
		{
			next.push_back(-1);
			time++;
		}
	}
	cout << "get_next:" << time << endl;
}


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


