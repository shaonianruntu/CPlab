#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;


void Print();
void Scanner();
bool Statement();
bool Condition();
bool While_Statement();
bool Expression();
bool Item_expression();
bool Factor();
bool Conditional_statements();
bool Assignment_statement();
bool Compound_statements();

char ch;
int sum;
char token[10];
char program[300] = " \
const x=8,y=7; \
Var a, b;  \
begin  \
	a = x + y; \
	b = a*x;  \
end#";

char *rwtab[9] = { "begin","if","then",
"else","while","do","const"
,"var","end" };
int m;
int syn;
int p;
int t = 1;
char word[10];
char var[10];

bool isDigital(char ch)
{
	if (ch <= '9'&&ch >= '0')
		return true;
	else
		return false;
}

bool isAlpha(char ch)
{
	if (ch >= 'a'&&ch <= 'z' || ch >= 'A'&&ch <= 'Z')
		return true;
	else
		return false;
}




void Scanner()
{
	int num = 0;
	for (m = 0; m<10; m++)
	{
		token[m] = '\0';
	}
	m = 0;
	ch = program[p++];

	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		ch = program[p++];
	}
	if (isAlpha(ch)) {
		do {
			token[m++] = ch;
			ch = program[p++];
		} while (isAlpha(ch) || isDigital(ch));
		p--;
		syn = 10;
		token[m++] = '\0';
		for (int n = 0; n <= 8; n++)
		{
			if (strcmp(token, rwtab[n]) == 0)
			{
				syn = n + 1;
				break;
			}
		}
		return;
	}
	else if (isDigital(ch))
	{
		sum = 0;
		while (isDigital(ch))
		{
			sum = sum * 10 + ch - '0';
			ch = program[p++];
		}
		p--;
		syn = 11;
		if (isAlpha(ch))
			syn = -1;
		return;
	}
	else
	{
		token[0] = ch;
		switch (ch)
		{
		case '<':ch = program[p++];
			if (ch == '>')
			{
				syn = 22;
				token[1] = ch;
			}
			else if (ch == '=')
			{
				syn = 18;
				token[1] = ch;
			}
			else {
				syn = 19;
				p--;
			}
			break;
		case '>':ch = program[p++];
			if (ch == '=')
			{
				syn = 21;
				token[1] = ch;
			}
			else {
				syn = 20;
				p--;
			}
			break;
		case '=':ch = program[p++];
			if (ch == '=')
			{
				syn = 17;
				token[1] = ch;
			}
			else {
				syn = 16;
				p--;
			}
			break;
		case '+':syn = 12; break;
		case '-':syn = 13; break;
		case '*':syn = 14; break;
		case '/':syn = 15; break;
		case ';':syn = 23; break;
		case '(':syn = 24; break;
		case ')':syn = 25; break;
		case ',':syn = 26; break;
		case '#':syn = 0; break;
		default:syn = -1; break;
		}
		return;
	}
}

bool Constan_Defined()
{
	if (syn == 10)
	{
		//cout << "��������" << token << endl;
		Scanner();
		if (syn == 16)
		{
			//cout << "����" << token << endl;
			Scanner();
			if (syn == 11)
			{

				//cout << "�޷�������" << sum << endl;
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool Constan_Description()
{
	Scanner();
	if (syn == 7)
	{
		//cout << "����˵��" << token << endl;
		Scanner();
		while (Constan_Defined())
		{
			Scanner();
			if (syn == 23)
			{
				//cout << "�ֺ�" << token << endl;
				return true;
			}
			else if (syn == 26)
			{
				//cout << "����" << token << endl;
				Scanner();
				continue;
			}
			//cout << "����˵������" << endl;
		}
	}
	else
		return false;
}


bool  Variable_Defined()
{
	if (syn == 10)
	{
		//cout << "��������" << token << endl;
		return true;
	}
	else
		return false;
}

bool Variable_Description()
{
	if (syn == 10 || syn == 2 || syn == 5 || syn == 1 || syn == 0)
	{
		return true;
	}
	Scanner();
	if (syn == 8)
	{
		//cout << "����˵��" << token << endl;
		Scanner();
		while (Variable_Defined())
		{
			Scanner();
			if (syn == 23)
			{
				//cout << "�ֺ�" << token << endl;
				return true;
			}
			else if (syn == 26)
			{
				//cout << "����" << token << endl;
				Scanner();
				continue;
			}

		}
	}
}



bool Condition()
{
	//cout << "����" << endl;
	Expression();
	if (syn == 17 || syn == 18 || syn == 19 || syn == 20 || syn == 21
		|| syn == 22)
	{
		//cout << "��ϵ�����" << token << endl;
		Scanner();
	}
	else
	{
		//cout << "��ϵ���������" << endl;
		return false;
	}
	Expression();
}

bool Expression()
{
	//cout << "���ʽ" << endl;
	//Scanner();
	do {
		if (syn == 12 || syn == 13)
		{
			//cout << "�ӷ������" << token << endl;
			cout << "+";
			Scanner();
			Item_expression();
		}
		else
		{
			Item_expression();
		}
	} while (syn == 12 || syn == 13);
	return true;
}

bool Item_expression()
{
	//cout << "��" << endl;
	while (Factor())
	{
		//Scanner();
		if (syn == 14 || syn == 15)
		{
			//cout << "�˷������" << token << endl;
			cout << "*";
			Scanner();

		}
		else {
			return true;
		}
	}
	return false;
}

bool Factor()
{
	//cout << "����" << endl;
	//Scanner();
	if (syn == 10)
	{
		//cout << "��ʶ��" << token << endl;

		cout << token;
		Scanner();//���� 
		return true;
	}
	else if (syn == 11)
	{
		cout << sum;
		//cout << "�޷�������" << sum << endl;
		Scanner();
		return true;
	}
	else if (syn == 24)
	{
		//cout << "������" << token << endl;
		Scanner();
		Expression();
		if (syn == 25)
		{
			//cout << "������" << token << endl;
			Scanner();
			return true;
		}
		else
		{
			//cout << "û�У�������" << endl;
			return false;
		}
	}
	else
	{
		//cout << "û��������" << endl;
		return false;
	}
	return false;
}

bool Assignment_statement()
{
	//cout << "��ֵ���" << endl;
	//cout << "��ʶ��" << token << endl;

	strcpy(word, token);


	Scanner();
	if (syn == 16)
	{
		//cout << "��ֵ���=" << endl;
		Scanner();
	//	cout << endl;
		cout << "t" << t << "=";
		Expression();
	//	cout << endl;
		return true;
	}
	else
	{
		//cout << "û��=" << endl;
	}
}

bool Compound_statements()
{
	//cout << "�������" << token << endl;
	Scanner();
	while (Statement())
	{
		if (syn == 23)
		{
			//cout << "��������еķָ��" << token << endl;
			cout << endl;
			cout << word << "=" << "t" << t << endl;
			t++;
			Scanner();
			if (syn == 9) {
				//cout<<"�������"<<token<<endl;
				break;
			}
		}
	}
	if (syn == 9)
	{
		//cout << "�������" << token << endl;
		Scanner();
		return true;
	}
	else
	{
		//cout << "�������ȱ��" << endl;
		return false;
	}
	if (syn == 0)
	{
		//cout << "�������" << token << endl;
	}
}


bool Conditional_statements()
{
	if (syn == 2)
	{
		//cout << "�������if" << endl;
		Scanner();
		Condition();
		if (syn == 3)
		{
			//cout << "then" << endl;
			Scanner();
			Statement();
			if (syn == 4)
			{
				Scanner();
				Statement();
			}
			else
			{
				return true;
			}
		}
		else
		{
			//cout << "���������ȱ�� then" << endl;
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool While_Statement()
{
	//cout << "ѭ�����" << token << endl;
	Scanner();

	Condition();
	if (syn == 6)
	{
		//cout << "whileѭ����do" << endl;
		Scanner();
		Statement();
		return true;
	}
	//      else 
	//          return false;


	else
		return false;
}

bool Statement()
{
	if (syn == 10)
	{
		Assignment_statement();
		return true;
	}
	else if (syn == 5)
	{
		While_Statement();
		return true;
	}
	else if (syn == 1)
	{
		Compound_statements();
		return true;
	}
	else if (syn == 2)
	{
		Conditional_statements();
		return true;
	}
	else
	{
		return false;
	}
}



int main()
{
	freopen("result.txt", "w", stdout);
	printf("�м�������ɣ�\n");
	p = 0;
	cout << "********************************************" << endl;
	cout << "����ʼ" << endl;
	Constan_Description();
	Variable_Description();
	do {
		Scanner();
		Statement();
	} while (syn != 0);
	cout << "********************************************" << endl;
	cout << "�������" << endl;
	cout << "��������˳�" << endl;
}

