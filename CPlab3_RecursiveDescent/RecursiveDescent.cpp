#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

#define ID 15   // �ǹؼ��ֵı�ʶ��
#define NUM 16   // ��ֵ��

//��Ŵ������ַ��� 
char tempstr[255] = {};
//�ո��־ 
bool temp = false;
//��ʱ����
char word[255] = {};
//keyword�ؼ��� 
string keyword[9] = {
	"Const", "Var", "if", "else", "then",
	"while", "do", "begin", "end"
};

int keyword_num[9] = {
	1,2,3,4,5,
	6,7,8,9
};
//������������������ 
char symbol[10] = { '+','-','*','/',';','(',')','{','}',',' };
//��Ӧ������ֵ
int symbol_num[10] = { 21,22,23,24,25,26,27,28,29,40 };



int type[1000] = { 0 };
int type_p = 0;
int q = 0;

// int parse_error = 0;   // �﷨��������


bool IsLetter(char ch); //�ж��Ƿ�Ϊ��ĸ 
bool IsDigit(char ch);   //�ж��Ƿ�Ϊ���� 
int IsSymbol(char ch);  //�ж��Ƿ�Ϊ�������
int IsKeyword(string str);  //�ж��Ƿ�Ϊ�ؼ��� 
void HandleSpace(char a[]);   //�ո���
void prePro();  //����"//"ע��
void Scanner(char *str);  //���� �ʷ�ɨ��


void Parse(); // �﷨����
bool Statement();  // ���
bool Assignment_Statement(); // ��ֵ���  
bool Looping_Statement();  // ѭ�����
bool Conditional_Statement(); // �������
bool Condition();  // ����
bool Compound_Statement(); // �������
bool Const_Def();  // ��������
bool Const_Desc();    // ����˵��
bool Var_Def();   // ��������
bool Var_Desc();   // ����˵��
bool Expression();   // ���ʽ
bool Term();  // �Ӽ�����
bool Factor();   // ����

int main()
{
	char instr[255] = {}; //���������ַ��� 
	char *str = NULL;//���ÿ�е��ַ��� 
	char delims[] = " ";//�ָ��־ 
	freopen("test.cpp", "r", stdin);
	freopen("result.txt", "w", stdout);

	printf("�ʷ�����������£�\n");
	printf("******************************\n");

	while ((gets_s(instr)) != NULL)
	{
		HandleSpace(instr);
		prePro();

		str = strtok(tempstr, delims);//�ָ��ַ��� 

		while (str != NULL)
		{
			//ͷ�ļ����궨��
			if (*(str) == '#')
			{
				printf("#\n");
				break;
			}

			Scanner(str);

			str = strtok(NULL, delims);
		}
	}

	Parse(); // �﷨����

	return 0;
}

// �﷨����
void Parse()
{
	printf("\n\n�﷨����������£�\n");
	printf("********************************\n");
	Const_Desc();
	Var_Desc();
	do {
		q++;
	//	Statement();
	} while (Statement() && q <= type_p);
//	printf("%d\n%d\n", q, type_p);

	if (q >= type_p)
	{
		printf("********************************\n");
		printf("Parse Success! Congratulations!\n");
		printf("********************************\n");
	}
}

// ���
bool Statement()
{
	if (*(type + q) == ID)   // ID
	{
		if (Assignment_Statement())  // ��ֵ���
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 6)    // while
	{
		if (Looping_Statement())   // ѭ�����
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 3)   // if
	{
		if (Conditional_Statement())  // �������
			return true;
		else
		{
			return false;
		}
	}
	else if (*(type + q) == 8)  // begin
	{ 
		if(Compound_Statement())    // �������
			return true;
		else
		{
			return false;
		}
	}
	return false;
}

// ��ֵ���
bool Assignment_Statement()
{
	q++;
	if (*(type + q) == 35)  // =
	{
		q++;
		if (Expression())
			return true;
		else
		{
			return false;
		}
	}
	else
	{
		printf("Assignment statement error! missing '=', cursor = %d\n", q);
		return false;
	}
}

// ѭ�����
bool Looping_Statement()
{
	q++;
	if (Condition())
	{
		if (*(type + q) == 7)  // do
		{
			q++;
			if (Statement())  // ���
				return true;
			else
			{
				return false;
			}
		}
		else
		{
			printf("Looping statement error! missing 'do', cursor = %d\n", q);
			return false; 
		}
	}
	else
	{
		return false;
	}
}

// �������
bool Conditional_Statement()
{
	if (*(type + q) == 3)  // if
	{
		q++;
		if (Condition())   // ����
		{
			if (*(type + q) == 5)   // then
			{
				q++;
				if (Statement())   // ���
				{
					if (*(type + q) == 4)   // else
					{
						q++;
						if (Statement())
							return true;
						else
						{
							return false;
						}
					}
					else
						return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				printf("Conditional statement error! missing 'then', cursor = %d\n", q);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

// ����
bool Condition()
{
	Expression();  // ���ʽ
	// ��ϵ�����
	if ((*(type + q) == 30) || (*(type + q) == 31) || (*(type + q) == 32) || (*(type + q) == 33) || (*(type + q) == 34) || (*(type + q) == 36))
	{
		q++;
	}
	else 
	{
		printf("Condition statement error! relational operator error, cursor = %d\n", q);
		return false;
	}
	Expression();  // ���ʽ
}

// �������
bool Compound_Statement()
{
	q++;
	while (Statement())
	{
		if (*(type + q) == 25)   // ;
		{
			q++;
			if (*(type + q) == 9)  // end
			{
				break;
			}
		}
	} 
	if (*(type + q) == 9)  // end
	{
		q++;
		return true;
	}
	else
	{
		printf("Compound statement error! missing 'end', cursor = %d\n", q);
		return false;
	}
}

// ��������
bool Const_Def()
{
	if (*(type + q) == ID)  // ID
	{
		q++;
		if (*(type + q) == 35)  // =
		{
			q++;
			if (*(type + q) == NUM)
			{
				printf("Const define success!\n");
				return true;
			}
			else
			{
				printf("Const define error! missing unsigned integer, cursor = %d\n", q);
				return false;
			}
		}
		else
		{
			printf("Const define error! missing '=', cursor = %d\n", q);
			return false;
		}
	}
	else
	{
		printf("Const define error! missing ID, cursor = %d\n", q);
		return false;
	}
}

// ����˵��
bool Const_Desc()
{
//	q++;
	if (*(type + q) == 1)  // Const
	{
		q++;
		while (Const_Def())
		{
			q++;
			if (*(type + q) == 25)  // ;
			{
				printf("Const description success!\n");
				return true;
			}
			else if (*(type + q) == 40)   // ,
			{
				q++;
				continue;
			}
			else
			{
				printf("Const description error! missing ';', cursor = %d\n", q);
				return false;
			}
		}
	}
	else
	{
		printf("Const description error! missing 'Const', cursor = %d\n", q);
		return false;
	}
}

// ��������
bool Var_Def()
{
	if (*(type + q) == ID)  // ID
	{
		printf("Var define success!\n");
		return true;
	}
	else
	{
		printf("Var define error! cursor = %d\n", q);
		return false;
	}
}

// ����˵��
bool Var_Desc()
{
	if ((*(type + q) == 3) || (*(type + q) == 6) || (*(type + q) == 8) || (*(type + q) == ID)) // if, while, begin, ID
	{
		return true;
	}
	q++;
	if (*(type + q) == 2)   // Var
	{
		q++;
		while (Var_Def()) {
			q++;
			if (*(type + q) == 25)  // ;
			{
				printf("Var description success!\n");
				return true;
			}
			else if (*(type + q) == 40)   // ,
			{
				q++;
				continue;
			}
		}
	}
	else
	{
		printf("Var description error! missing 'Var', cursor = %d\n", q);
		return false;
	}
}

// ���ʽ
bool Expression()
{
	do {
		if ((*(type + q) == 21) || (*(type + q) == 22))  // [+/-]
		{
			q++;
			Term();  // ��
		}
		else
		{
			Term();  //��
		}

	} while ((*(type + q) == 21) || (*(type + q) == 22));  // +, -

	return true;
}

// ��
bool Term()
{
	while (Factor())
	{
		if ((*(type + q) == 23) || (*(type + q) == 24)) // *, /
		{
			q++;
		}
		else
			return true;
	}
	printf("Term error! Cannot find factor, cursor = %d\n", q);
	return false;
}

// ����
bool Factor()
{
	if (*(type + q) == ID)   // ��ʶ��
	{
		q++;
		return true; 
	}
	else if (*(type + q) == NUM)  // �޷�������
	{
		q++;
		return true;
	}
	else if(*(type + q) == 26)  // (
	{
		q++;
		if (Expression())  // ���¼�� С���� �ڲ��ı��ʽ
		{
			if (*(type + q) == 27)  // )
			{
				q++;
				return true;
			}
			else
			{
				printf("Factor error! missing ')', cursor = %d\n", q);
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		printf("Factor error! missing '('!, cursor = %d\n", q);
		return false;
	}
}

/*******************************  �ʷ�����  **************************************/
// ���� �ʷ�ɨ��
void Scanner(char *str) {
	bool flag = false; //����ע�ͱ�־,falseΪδ����ע������ 
	string Token;//����ַ��� 

	for (int i = 0; i<strlen(str); i++)
	{
		if (*(str + i) == '/')
		{
			if (*(str + i + 1) == '*')
			{
				flag = true;
				break;
			}
		}
		//ע�ʹ���: */,ע��������� 
		if (*(str + i) == '*'&&flag)
		{
			if (*(str + i + 1) == '/')
			{
				flag = false;
				i++;
				break;
			}
		}

		//��ʶ�����ؼ��� 
		if (IsLetter(*(str + i)) && (!flag))
		{
			while (IsLetter(*(str + i)) || IsDigit(*(str + i)) || *(str + i) == '_')
			{
				Token += *(str + i);
				i++;
			}
			if (IsKeyword(Token) != ID)
			{
				int syb = keyword_num[IsKeyword(Token)];
				*(type + type_p++) = syb;
			}
			else
			{
				*(type + type_p++) = ID;
			}
			printf("%2d: < %s , %d >\n", type_p, Token.c_str(), *(type + type_p - 1));
			Token = "";
		}

		if (IsDigit(*(str + i)) && (!flag))
		{
			while (IsDigit(*(str + i)))
			{
				Token += *(str + i);
				i++;
			}
			*(type + type_p++) = NUM;
			printf("%2d: < %s , %d >\n", type_p, Token.c_str(), *(type + type_p - 1));
			Token = "";
		}

		//<,<=,<>
		if (*(str + i) == '<' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 32;
				printf("%2d: < %s , %d >\n", type_p, "<=", *(type + type_p - 1));
			}
			if (*(str + i + 1) == '>')
			{
				*(type + type_p++) = 31;
				printf("%2d: < %s , %d >\n", type_p, "<>", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 30;
				printf("%2d: < %s , %d >\n", type_p, "<", *(type + type_p - 1));
			}
		}
		//>,>=
		else if (*(str + i) == '>' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 34;
				printf("%2d: < %s , %d >\n", type_p, ">=", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 33;
				printf("%2d: < %s , %d >\n", type_p, ">", *(type + type_p - 1));
			}
		}
		//=,==
		else  if (*(str + i) == '=' && (!flag))
		{
			if (*(str + i + 1) == '=')
			{
				*(type + type_p++) = 36;
				printf("%2d: < %s , %d >\n", type_p, "==", *(type + type_p - 1));
			}
			else
			{
				*(type + type_p++) = 35;
				printf("%2d: < %s , %d >\n", type_p, "=", *(type + type_p - 1));
			}
		}

		//���¶������
		else if (IsSymbol(*(str + i)) != -1 && (!flag))
		{
			int syb = symbol_num[IsSymbol(*(str + i))];
			*(type + type_p++) = syb;
			printf("%2d: < %c , %d >\n", type_p, *(str + i), syb);
		}
	}
}

//�ж��Ƿ�Ϊ��ĸ 
bool IsLetter(char ch)
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
	return false;
}

//�ж��Ƿ�Ϊ���� 
bool IsDigit(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}

//�ж��Ƿ�Ϊ�������
int IsSymbol(char ch)
{
	for (int i = 0; i<10; i++)
	{
		if (ch == symbol[i])
			return i;
	}
	return -1;
}

//�ж��Ƿ�Ϊ�ؼ��� 
int IsKeyword(string str)
{
	for (int i = 0; i<9; i++)
	{
		if (str == keyword[i])
		{
			return i;
		}
	}
	//���ǹؼ��ּ�ΪID
	return ID;
}

//�ո���
void HandleSpace(char a[])
{
	int j = 0;
	memset(word, 0, 255);//��Ҫ��գ���Ȼ���ܲ����ϴε��ַ���
	temp = false;
	for (int i = 0; i < strlen(a); i++)
	{

		if (a[i] != ' ' && a[i] != '\t')
		{
			word[j++] = a[i];
			temp = false;
		}
		else
		{

			if (!temp && a[i] != '\t')
			{
				word[j++] = a[i];
				temp = true;
			}

		}
	}
}

//����"//"ע��
void prePro()
{
	int j = 0;
	memset(tempstr, 0, 255);
	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '/' && word[i + 1] == '/')
		{
			while (i < strlen(word))
			{
				i++;
			}
		}

		else {
			tempstr[j++] = word[i];
		}
	}
}