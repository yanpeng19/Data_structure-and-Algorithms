﻿#pragma once
#include "my_string.h"
/*
BM_BC 算法：
----------------------------------------------------------------------------------------------------------------
思路： 坏字符算法，因为基于这样一种思路形成的 “如果 总串 P 和查找串 S 的从后往前某个位置不符，我们可以迅速拖动
       一个可能符合字母过来对齐”的思路，即1.从后往前对比思想（因为不符合情况居多） 2.对比拖动字符串思想 的结合
	   例如
	   P: ACDRSGL|SDXSDIWERQO
	   S: SLOWIEO|
	   i = 0,j = 6;

	   对比的时候，最后一个字符就不符合，那么我们就可以做以下两步操作：
	   1.在S串最后一位 往前寻找 与 P[i+j] 符合的字母
	   2.将这个字符和 p[i+j] 对齐，并且重新从尾部进行对比

所以按照这个思路，我们需要直到查找串中，各种情况，每个字符出现的位置，方便进行对比。
数组 bc 存储的就是 查找串字符存储的信息；

标准的方法是：仅仅存储该字符 最靠后出现的位置。

为什么不用链表把位置从前到后都存储了？复杂度也是O（N)
因为如果查找串中有大量的重复字符，那么 可能 bc 的某个节点的表长度，也会近似 O（n)
那么在循环查找中，获取 位置信息的操作 每次就从 O（1） 变成了 O（n).而且也可能某一步用了O（n)的操作
也仅仅挪动了 1 个单位。

总体匹配最坏的情况下，就是O(N^N) （不过感觉也还好，值得一试）

BC数组的构建： BC数组存储的是查找串 S 中每个字符出现的最后的一个位置，且能在O（1）情况下获取数据
               所以BC数组的长度，是所有可能是的字符个数：127，下标为 0~126
			   BC数组中每个位置 bc[i] 意味着： 第i个字符，最后一次出现的位置，例如：

			   bc[97] = 3;     97在字符表中是'a' ，就是说 S 串中 a 最后出现的位置是 下标3

              而BC数组的构建，就直接遍历一次查找串，并且用后面的覆盖前面的：
			  for(int i = 0; i < s.size();i++)
			  {
			        char c = s[i];    // 位置的字符
					bc[c] = i;        // 更新字符在表中的位置
			  }

实际查找中，使用bc表获得字符最后出现的位置的时候，可能会出现2种情况：

(1):
P: ACDRSGL|SLXSDIWERQO
S: SLOWIEO|SL
i = 0,j = 6;
char c = p[i+j] = L;
bc[c] = 8;

bc[c] > j;  //  最后一次出现的在对比位置之后，那么我们如果要对齐，就必须把S串拖回来，这样
            //  对比问题反而扩大了，所以简单的往后移动一个位置就可以了；

(2):
P: ACDRSGL|SDXSDIWERQO
S: SLOWIEO|
i = 0,j = 6;
char c = p[i+j] = L;
bc[c] = 1;

bc[c] < i;  //  最后一次出现的位置在前面，这个是我们最乐于见到的，因为可以把 s[1] 拖过来进行重新
            //  对齐和进行对比；

相等情况是不会出现的，因为这个位置出现了失配，所以不可能相等。

（*）伪代码：

bc[] = get(bc,s); //获取bc数组
for(int i = 0;i<p.size()-s.size()-2;)
{
    j = s.size()-1;//字符出最后位置的下标
	while(p[i+j]==s[j])
	{
	    if(j==0) return i;
	    j--;
	}
	//到此表示 i+j 的字符 出现了不匹配情况
	temp = bc[p[i+j]; //找到位置；
	if(temp>i) i++;
	else i = i + (s.size() - 1 - bc[p[i+j]];
}
return -1;
*/


using namespace std;

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
	vector<int> bc(256,-1);
	get_bc(p,bc);

	for (int i = 0; i < p.size() - s.size() - 2; i++)
	{
		int j = s.size()-1;
		while (p[i + j] == s[j])
		{
			if (j == 0) return i;
			j--;
		}
		if (bc[p[i + j]] > j) i++;
		else i = i + (s.size() - bc[p[i + j]] - 1);
	}
	return -1;
}
