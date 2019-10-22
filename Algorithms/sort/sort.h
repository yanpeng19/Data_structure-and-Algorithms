/*
本文件包括各种排序算法：
1.冒泡排序
2.归并排序
3.选择排序
4.插入排序
5.计数  ， 排序
6.堆排序
7.快速排序
8.希尔排序

1.冒泡排序
----------------------------------------------------------------------------
冒泡排序思路比较简单，遍历序列0~size()-1;
每次将 i 与 i+1 元素对比，如果大小不对，则交换二者位置。
这样，每一次遍历，都可以确保最大/最小的元素到达最后的位置，问题规模缩减1单位

复杂度：O（n^n）；
----------------------------------------------------------------------------

2.归并排序
----------------------------------------------------------------------------
归并排序是一种分而治之的思想，将数据不停的分成两部分，然后再合并排序的过程；
分为 分和合两步骤：
分就是分到只有一个单位位置，
合就是分到了最细的程度之后，每个局部的的排序；

总体复杂度 O（nlogn）
----------------------------------------------------------------------------

3.选择排序
----------------------------------------------------------------------------
每次都从数据中选择最大/小的数字，并且放到最后/前，并且循环这个过程。

总体复杂度: O(N^N)， 和冒泡相比，优点是每次只进行一次交换操作
----------------------------------------------------------------------------

4.插入排序
----------------------------------------------------------------------------
将数据排序想象成对有序 序列进行插入的过程。
从0-n 逐个插入数据，并且保证前半部分的数据都已经是有序序列。

总体复杂度: O(n^n) 空间复杂度：1
----------------------------------------------------------------------------

5.计数排序
----------------------------------------------------------------------------
a.建立一个长度为n的散列表
b.通过散列函数 hash处理每个数据，然后将数据逐个取出即可
其本质是通过将数据hash处理，然后通过相应的排解冲突的方法来排序

总体复杂度：O（n)
----------------------------------------------------------------------------

6.堆排序
----------------------------------------------------------------------------
直接使用数据建堆，然后每次都将最大/最小数取出来。

总体复杂度：O（nlogn)
----------------------------------------------------------------------------

7.快速排序
----------------------------------------------------------------------------
快速排序是类似 归并排序的一种排序方法，分而治之，然后合并。

思路：迅速找到一个大小居中的数 x，将数据分成左右两部分。
      大于 x 放在后面，将小于x的放在前面。然后再直接合并即可。

x的选择：一般选取众数或者中位数，众数的定义： 众数的个数大于总个数的一半。
         如果没有众数，则选取中位数；


#在O(n）时间寻找众数的方法：
*定义：如果序列中有众数那么有定义 data[mode] > data.size()/2
*思路：线性扫描 int i = lo, i < hi; i++;
       因为众数占了系列中一半以上的数量，所以 一个众数与一个非众数的数一起从系列中抛弃，
       众数的地位并不会发生改变。

       众数的计数 t = 0;
       假设第一个数 data[i] 为 众数,t=1; 如果 data[i]==data[i+1] ,那么t++,i++;
       否则t--,i++;
       如果t = 0;将二者一同抛弃，尝试下一个数，并假设其为众数 t=1;i++;

       验证步骤：O(n) 扫描步骤O（n)
#寻找众数结束

#寻找中位数k的方法O(n):
a.首先按照每组5个数 将所有数据分成 N 组，然后使用任意方法得到每组的数据中位数 m
b.将所有的中位数集合，再次分为5组，并再次寻找中位数集合。直到最终 中位数集合中的数字个数小于5
c.通过一般方法确定最终的中位数 K ， 此时有  1/8 < K < 1/8
d.将所有数统一和 K 进行对比，分成三组， L（L集合数小于K），E（E集合等于K），R（R集合大于K）
e.然后 得到 rank k = (hi-lo)/2; 表示排序后的第 k 个数，应当为 中位数。
  我们判断 k 是在 L E R中的哪个集合中；
  如果在 L 中，那么缩小问题规模，将 L 的所有数字，重复 a 的步骤。
  如果在 E 中，因为E中数据都是一致的，那么说明就找到了，直接 返回 E 的中的数值即可。
  如果在 R 中，那么缩小问题规模，将 R 的所有数字，重复 a 的步骤。
#寻找中位数结束

整体复杂度：O（nlogn）
-----------------------------------------------------------------------------

8.希尔排序
-----------------------------------------------------------------------------
*步骤：1.将排序向量分别按照 一个序列 N（1，2，3，5，8，9）中，从大到小的顺序，
分别以 N[i] 个数拍成一行，并将下一行与只对齐，如果向量还有数据，则继续排列下去
这样就一共得到 data.size()/n 行数据
2.将每列数据按照从小到大顺序进行排序，排序方法使用 对输入敏感的 — 插入排序法
3.将数据合并
4.循环执行1-3直到完成 N = 1 之后的排序

复杂度：不同的序列会造成不同的复杂度，一般为 O(nlogn)

以下代码适用的是 Hibbard 序列： h[1]=1,h[i]=2∗h[i-1]+1
-----------------------------------------------------------------------------
*/

#pragma once
#include <iostream>
#include <vector>
#include "PQ.h"

using namespace std;

void bubble_sort(int t[],int t_size)
{
	for (int i = 0; i < t_size - 2;i++)
	{
		for(int j = 0;j<t_size-1-i;j++)
		if (t[j] > t[j + 1])
			swap(t[j], t[j + 1]);
	}
}

void merge(int t[], int beg, int mid, int end);
// beg 和 end 都是下标
void merge_sort(int t[], int beg,int end)
{
	if (beg == end) return;

	//分
	int mid = beg + (end-beg) / 2;
	merge_sort(t,beg,mid);
	merge_sort(t,mid+1,end);
	//合 此时左右两边都是有序性的；
	merge(t, beg, mid, end);

	return;
}

void merge(int t[], int beg, int mid, int end)
{
	
	int m = mid+1;
	vector<int> l, r;
	for (int i = beg; i <= mid; i++)
		l.push_back(t[i]);
	for (int i = mid + 1; i <= end; i++)
		r.push_back(t[i]);

	int i = 0;
	int j = 0;
	int k = beg;
	while (i <= mid-beg && j <= end-mid-1)
	{
		if (l[i] < r[j])
			t[k++] = l[i++];
		else
			t[k++] = r[j++]; 
	}

	while (i <= mid - beg)
		t[k++] = l[i++];
	while (j <= end - mid - 1)
		t[k++] = r[j++];

}

void sel_sort(int t[], int beg, int end)
{
	for (int i = beg; i < end; i++)
	{
		int min_index = i;
		for (int j = i+1; j <= end; j++)
		{
			if (t[j] < t[min_index])
				min_index = j;
		}
		if (min_index != i) swap(t[i], t[min_index]);
	}
}

void ins_sort(int t[], int beg, int end)
{
	for (int i = beg + 1; i <= end; i++)
	{
		for (int j = i; j > beg; j--)
		{
			if (t[j] < t[j - 1]) swap(t[j], t[j - 1]);
		}
	}
}

void stack_sort(int t[], int beg, int end)
{
	PQ_ComplHead<int> p;
	for (int i = beg; i <= end; i++)
		p.insert(t[i]);
	for (int i = end; i >= beg; i--)
	{
		t[i] = p.get_Max();
		p.delete_Max();
	}
}

void quick_sort(vector<int> data, int lo, int hi)
{

	if (lo == hi) return;
	int low = lo;
	int high = hi;

	//int m = data[lo];
	//先尝试众数寻找，如果失败，再使用通用算法
	int m = get_mode(data, lo, hi);
	if (m == -1234)
	{
		m = liner_sel(data, lo, hi);
	}

	int p = ++lo;

	while (lo != hi)
	{
		// 6, 1, 2, 7, 9, 3, 4, 5, 10, 8 
		if (p == lo)  //p 在从左向右扫描
		{
			if (data[p] > m) p = hi;
			else
			{
				data[lo - 1] = data[lo];
				lo++;
				p++;
			}
		}
		else      //p从右向左
		{
			if (data[p] < m)
			{
				swap(data[p], data[lo]);
				data[lo - 1] = data[lo];
				lo++;
				p = lo;

			}
			else
			{
				hi--;
				p--;
			}
		}
	}
	data[lo - 1] = m;
	quick_sort(data, low, m - 1);
	quick_sort(data, m, high);
}

/*
*寻找众数
*定义：如果序列中有众数那么有定义 data[mode] > data.size()/2
*思路：线性扫描 int i = lo, i < hi; i++;
因为众数占了系列中一半以上的数量，所以 一个众数与一个非众数的数一起从系列中抛弃，
众数的地位并不会发生改变。

众数的计数 t = 0;
假设第一个数 data[i] 为 众数,t=1; 如果 data[i]==data[i+1] ,那么t++,i++;
否则t--,i++;
如果t = 0;将二者一同抛弃，尝试下一个数，并假设其为众数 t=1;i++;

验证步骤：O(n) 扫描步骤O（n)
*/
int get_mode(vector<int> &data, int lo, int hi)
{
	int t = 0;
	int m = 0;

	for (int i = lo; i <= hi; i++)
	{
		if (t == 0)
		{
			m = data[i];
			t = 1;
			i++;
			continue;
		}
		if (data[i] == m)
		{
			t++;
			i++;
		}
		else
		{
			t--;
			i++;
		}
	}
	if (t == 0) return -1234;

	//二次验证
	t = 0;
	for (int i = 0; i <= hi; i++)
		if (data[i] == m) t++;

	//标准稍微降低一点， 众数 > (size()-1)/2 即可
	if (t > (hi - lo) / 2) return m;
	else return -1234;
}

int bubble_sel(vector<int>&data, int lo, int hi)
{
	for (int i = lo; i <= (hi - lo) / 2; i++)
	{
		for (int j = i; j < hi; j++)
			if (data[j] > data[i]) swap(data[i], data[j]);
	}
	return data[lo + (hi - lo) / 2];
}

/*
*寻找中位数
*思路：
1.将数据用按 Q（一般为5）为一组进行划分，每Q个数据分为一个小组，一共可分为 N/Q 组  O(N)
2.将分好的小组中 用一般方法求得小组的 中位数，得到 N/Q 个中位数
3.递归的将这些小组中位数再次进行 以Q 进行分组 ，直到数据个数小于 Q，并取得其中位数 M   此时，M的区间必定是  1/8 < m < 1/8
4.用 M 作为轴点，将序列划分 为3个部分  L 部分小于M  E 部分等于 M ，G部分大于 M
5.根据 L,E,G三个部分的大小来进行判断：
5.0 rank k = （hi-lo)/2;
5.1 如果 k 在 L 中，说明中位数在 L数据中，将L部分数据重复步骤1.( k<L.size()) 递归调自身 return linear_sel（data,lo,l.size()-1)
5.2 如果 k 在 E 中,说明找到了k ，直接返回。 (k > L.size()&&k<G.size()) return k;
5.3 如果 k 在 G 中,说明中位数k 在M的右侧，将 G 部分的数据再次进行步骤1.  递归调用自身 return liner_sel(data,(hi-lo-L.size()-E.size());

*/
int liner_sel(vector<int>&data, int lo, int hi)
{
	int Q = 5;
	if ((hi - lo) < 5) return bubble_sel(data, lo, hi);

	vector<int> mode_vec;


	//小组中数集
	for (int i = 0; i < (hi - lo) / 5; i++)
		mode_vec.push_back(bubble_sel(data, i*Q, i*Q + 5));

	mode_vec.push_back(bubble_sel(data, ((hi - lo) / 5) * 5, hi));

	int m = liner_sel(mode_vec, 0, mode_vec.size() - 1);

	using rank = int;
	rank L = 0;
	rank E = 0;
	rank G = 0;
	for (int i = lo; i <= hi; i++)
	{
		if (data[i] < m) L++;
		else if (data[i] == m) E++;
		else G++;
	}

	rank k = (hi - lo) / 2;
	if (k < L) return liner_sel(data, lo, lo + L);
	else if (k > L && k < L + E) return m;
	else return liner_sel(data, (hi - lo - L - E), hi);
}

void shell_sort(vector<int>&data, int lo, int hi)
{
	vector<int> hibbard{ 1,3,7,15,31,63,127,255,511,1023,2047,
		4095,8191,16383,32767,65535,131071,262143,524287,
		1048575,2097151,4194303,8388607,16777215,
		33554431,67108863,134217727,268435455,536870911,1073741823 };

	int i = 0;
	while (hibbard[i] < data.size() / 2) i++;
	if (i) i--;

	for (int rank = hibbard[i]; i >= 0; i--)  //最外层循环 排序序列循环
	{
		for (int col = 0; col < hibbard[i]; col++) // 逐列处理数据排序,最大列数为 hibbard[i]
		{
			/*
			* 每个列都使用插入排序,插入排序思路 :
			* int rank = 1; rank < size()-2;rank++
			* 通过循环，每次都将第i个找放置到 data[i] > data[i] 的位置

			*/
			//同一列的数据间隔为space = hibbard[i]
			int space = hibbard[i];
			if (col + space > data.size()) continue; // 说明此列只有一个单位，可以去看下一列了

			for (int col_rank = col + space; col_rank < data.size(); col_rank = col_rank + space)
			{
				int sort_rank = col_rank; // sort_rank 为当前排序的下标，需要移动，所以通过备份来
				while (sort_rank > col && data[sort_rank] < data[sort_rank - space])
				{
					swap(data[sort_rank], data[sort_rank - space]);
					sort_rank -= space;
				}
			}
		}

	}
}

