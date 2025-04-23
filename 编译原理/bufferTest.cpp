#include "Lexer/doublebuffer.h"
#include<iostream>
#include<string>
using namespace std;

int main()
{
	Lexer::DoubleBuffer<5> buffer("test.txt");
	int state = 0;
	while (true)
	{
		std::string str = "";
		char c = buffer.next();
		if (c == EOF)
		{
			if (state != 2)
			{
				cout << "¶ÁÈ¡´íÎó" << endl;
			}
			break;
		}
		switch (state)
		{
		case 0:
			if (c == 'a')
				state = 1;
			break;
		case 1:
			if (c == 'b')
				state = 2;
			break;
		case 2:
			if (c == 'c')
				str = buffer.getToken();
			cout << str << endl;
			break;
		default:
			break;
		}
	}
	return 0;
}