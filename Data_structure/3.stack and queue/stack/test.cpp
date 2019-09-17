
/*
	1 postfix 输出计算结果
	2 evaluat 后缀表达式

*/
#include "stdafx.h"
#include <sstream>
#include "my_stack.h"

using namespace std;


void dispose_sym(string::iterator &a, my_stack<float> &num, my_stack<char> &sym);

void dispose_sym(string::iterator &a, my_stack<float> &num, my_stack<char> &sym, string &rpn);

bool is_num(string::iterator &a)
{
	string num("0123456789");
	if (num.find_first_of(*a) != string::npos) return 1;
	else return 0;
}

bool is_num(const char &a)
{
	string num("0123456789");
	if (num.find_first_of(a) != string::npos) return 1;
	else return 0;
}

bool is_num(string &s)
{
	string num("0123456789");
	if (s.find_first_of(num) != string::npos) return 1;
	else return 0;
}

void read_num(string::iterator &a, my_stack<float> &num, string&rpn)
{
	stringstream ss;
	float temp = *a - 48;
	while (is_num(++a))
	{
		temp = temp * 10 + (*a - 48);
	}
	num.push(temp);
	ss << temp;
	rpn.append(ss.str());
	rpn.append(" ");
	--a;
}

void convert(my_stack<char> & s, _int64 n, int base)
{
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (n > 0)
	{
		s.push(digit[n%base]);
		n /= base;
	}
	while (!s.empty()) cout << s.pop();
}

bool paren(const string &s)
{
	my_stack<char> sta;
	for (auto a : s)
	{

		if (a == '(' || a == '[')
		{
			sta.push(a);
		}
		else if (a == ')')
		{
			if (sta.back() == '(');
			else return 0;
		}
		else if (a == ']')
		{
			if (sta.back() == '[');
			else return 0;
		}
	}
	if (sta.empty()) return 1;
	return 0;
}

float evaluat(string &s, string &rpn)
{
	my_stack<float> num;  // 存数字栈
	my_stack<char> sym;  // 存符号栈
	rpn = "";


	sym.push('$');

	for (auto a = s.begin(); a != s.end(); a++) //循环处理字符
	{
		if (is_num(a))
			read_num(a, num, rpn);
		else
			dispose_sym(a, num, sym, rpn);
	}
	string t("$$$");
	while (!sym.empty())
	{
		auto a = t.begin() + 1;
		dispose_sym(a, num, sym, rpn);
	}
	cout << rpn << endl;
	return num.back();
}

void dispose_sym(string::iterator &a, my_stack<float> &num, my_stack<char> &sym)
{
	int sym_stack, sym_s;

	switch (*a)
	{
	case'!':sym_s = 1; break;
	case'^':sym_s = 2; break;
	case'*':sym_s = 3; break;
	case'/':sym_s = 3; break;
	case'+':sym_s = 4; break;
	case '-':sym_s = 4; break;
	case')':sym_s = 5; break;
	case'(':sym_s = 5; break;
	case'$':sym_s = 6; break;
	}
	switch (sym.top())
	{
	case'(':sym_stack = 5; break;
	case'!':sym_stack = 1; break;
	case'^':sym_stack = 2; break;
	case'*':sym_stack = 3; break;
	case'/':sym_stack = 3; break;
	case'+':sym_stack = 4; break;
	case '-':sym_stack = 4; break;
	case')':sym_stack = 5; break;
	case'$':sym_stack = 6;

	}
	if (*a == '(') sym.push(*a);
	else if (*a == ')' && sym.top() == '(') sym.back();
	else if (*a == '$'&& sym.top() == '$') sym.back();
	else if (sym_stack<sym_s) // 栈中符号优先度高，优先度低，该执行运算
	{
		if (sym.top() == '!')
		{
			float temp = num.back();
			float sum = 1;
			for (int i = 1; i <= temp; i++)
			{
				sum *= i;
			}
			sym.pop();
			num.push(sum);
			a--;
		}
		else if (sym.top() == '^')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;


			for (int i = 1; i <= num2; i++)
			{
				sum *= num1;
			}
			num.push(sum);
			sym.pop();
			a--;
		}
		else if (sym.top() == '*')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 * num2;
			sym.pop();
			num.push(sum);
			a--;
		}
		else if (sym.top() == '/')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 / num2;
			sym.pop();
			num.push(sum);
			a--;
		}
		else if (sym.top() == '+')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 + num2;
			num.push(sum);
			sym.pop();
			a--;
		}
		else if (sym.top() == '-')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 - num2;
			num.push(sum);
			sym.pop();
			a--;
		}
	}
	else sym.push(*a);
}

void dispose_sym(string::iterator &a, my_stack<float> &num, my_stack<char> &sym, string &rpn)
{
	int sym_stack, sym_s;

	switch (*a)
	{
	case'!':sym_s = 1; break;
	case'^':sym_s = 2; break;
	case'*':sym_s = 3; break;
	case'/':sym_s = 3; break;
	case'+':sym_s = 4; break;
	case '-':sym_s = 4; break;
	case')':sym_s = 5; break;
	case'(':sym_s = 5; break;
	case'$':sym_s = 6; break;
	}
	switch (sym.top())
	{
	case'(':sym_stack = 5; break;
	case'!':sym_stack = 1; break;
	case'^':sym_stack = 2; break;
	case'*':sym_stack = 3; break;
	case'/':sym_stack = 3; break;
	case'+':sym_stack = 4; break;
	case '-':sym_stack = 4; break;
	case')':sym_stack = 5; break;
	case'$':sym_stack = 6;

	}
	if (*a == '(')
	{
		sym.push(*a);
	}
	else if (*a == ')' && sym.top() == '(') sym.back();
	else if (*a == '$'&& sym.top() == '$') sym.back();
	else if (sym_stack<sym_s) // 栈中符号优先度高，优先度低，该执行运算
	{
		if (sym.top() == '!')
		{
			float temp = num.back();
			float sum = 1;
			for (int i = 1; i <= temp; i++)
			{
				sum *= i;
			}
			sym.pop();
			num.push(sum);
			rpn.append("! ");
			a--;
		}
		else if (sym.top() == '^')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;


			for (int i = 1; i <= num2; i++)
			{
				sum *= num1;
			}
			num.push(sum);
			sym.pop();
			rpn.append("^ ");
			a--;
		}
		else if (sym.top() == '*')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 * num2;
			sym.pop();
			num.push(sum);
			rpn.append("* ");
			a--;
		}
		else if (sym.top() == '/')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 / num2;
			sym.pop();
			num.push(sum);
			rpn.append("/ ");
			a--;
		}
		else if (sym.top() == '+')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 + num2;
			num.push(sum);
			sym.pop();
			rpn.append("+ ");
			a--;
		}
		else if (sym.top() == '-')
		{
			float num2 = num.back();
			float num1 = num.back();
			float sum = num1;
			sum = num1 - num2;
			num.push(sum);
			sym.pop();
			rpn.append("- ");
			a--;
		}
	}
	else sym.push(*a);
}

void postfix(string&s)
{
	stringstream ss(s);
	string temp;
	my_stack<float> st;

	float num1, num2;
	float sum;
	while (ss >> temp)
	{
		if (is_num(temp))
		{
			float f;
			f = stof(temp);
			st.push(f);
		}
		else
		{
			if (temp == "!")
			{
				num1 = st.back();
				float sum = 1;
				for (int i = 1; i <= num1; i++) sum *= i;
				st.push(sum);
			}
			else if (temp == "!")
			{
				num2 = st.back();
				num1 = st.back();
				float sum = num1;
				for (int i = 1; i < num2; i++) sum *= num1;
				st.push(sum);
			}
			else if (temp == "+")
			{
				num2 = st.back();
				num1 = st.back();
				st.push(num1 + num2);
			}
			else if (temp == "-")
			{
				num2 = st.back();
				num1 = st.back();
				st.push(num1 - num2);
			}
			else if (temp == "*")
			{
				num2 = st.back();
				num1 = st.back();
				st.push(num1 * num2);
			}
			else if (temp == "/")
			{
				num2 = st.back();
				num1 = st.back();
				st.push(num1 / num2);
			}
		}
	}
	cout << st.back() << endl;

}



int main()
{
	string s("(3+5*5-5)+3*2/(3*5)");
	string rpn;
	auto a = evaluat(s, rpn);
	postfix(rpn);
	system("pause");
    return 0;
}

