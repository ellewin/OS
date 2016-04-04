#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <syslog.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
using namespace std;

struct structura
{
	string file_name;
	string * massive;
	int bit;
};

structura * st = new structura [100];

void readConfig()
{
	char s[256];
	ifstream fin("1.txt");
	string ss;
	fin >> st[0].file_name;
	int new_i;
	new_i=st[0].file_name.size()+1;
	//fin.getline(s,sizeof(s));
	//fin.close();
	st[0].massive= new string [100];
	string new_word;
	new_word="";
	int co=0;
	int i=0;
	while (getline(fin,ss))
	{
		for(int i=1; i<ss.length(); ++i)
		{
			if (ss[i]!=' ')
			{
				new_word+=ss[i]; //cout<<1<<endl;
			}
			else
			{
				st[0].massive[co]=new_word; //cout<<new_word<<endl;
				new_word="";
				++co;
			}
		}
		st[0].bit=ss[ss.length()-1]-48;

	}
	
	cout<<st[0].bit<<endl;
}

int main()
{
	readConfig();
}
