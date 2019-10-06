/*
    搜素算法:1.顺序查找 2.二分查找
	--------------------------------------------------------------------
	1.顺序查找    顺序遍历即可   复杂度 O(n)

	2.二分查找    二分查找仅适用于已经排序后的数据。
	              思路是根据取中间值，然后将查找数据和中间值对比，
				  如果大于中间值，那么进入到右边部分数据；如果小于则
				  进入到左边，如果等于，则查找成功。
				  复杂度 O（logn)
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int sequential_search(int t[], int n)
{
	int i = 0;
	while (t[i] != EOF)
	{
		if (t[i] == n) return i;
		else i++;
	}
	return -1;
}

int binary_search(int t[], int t_size, int n)
{
	int beg = 0;
	int end = t_size;
	int mid = 0;

	while (beg <= end)
	{
		mid = beg + (end - beg) / 2;
		if (t[mid] == n) return mid;
		if (t[mid] > n)
			end = mid - 1;	
		else beg = mid + 1;
	}

	return -1;
}