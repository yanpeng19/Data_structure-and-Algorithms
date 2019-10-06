/*
    �����㷨:1.˳����� 2.���ֲ���
	--------------------------------------------------------------------
	1.˳�����    ˳���������   ���Ӷ� O(n)

	2.���ֲ���    ���ֲ��ҽ��������Ѿ����������ݡ�
	              ˼·�Ǹ���ȡ�м�ֵ��Ȼ�󽫲������ݺ��м�ֵ�Աȣ�
				  ��������м�ֵ����ô���뵽�ұ߲������ݣ����С����
				  ���뵽��ߣ�������ڣ�����ҳɹ���
				  ���Ӷ� O��logn)
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