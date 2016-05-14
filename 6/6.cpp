#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
string maxNumber="2147483648";
string minNumber="-2147483648";
vector<long long>numbers;

//return 1-OK
int checkNumber(string num)
{
	if(num[0]=='-')
	{
		if(num.length()>minNumber.length())
			return 0;
		if(num.length()<minNumber.length())
			return 1;
		else
		{
			for(int i=0; i<num.length(); ++i)
			{
				if(num[i]>minNumber[i])
					return 0;
				else
					return 1;
			}
		}
	}
	else
	{
		if(num.length()>maxNumber.length())
			return 0;
		if(num.length()<maxNumber.length())
			return 1;
		else
		{
			for(int i=0; i<num.length(); ++i)
			{
				if(num[i]>maxNumber[i])
					return 0;
				else
					return 1;
			}
		}
	}
}
long long createNumber(string num)
{
	long long resultNumber=0;
	long long mul=1;
	for (int i=num.length()-1; i>=1; i--)
	{
		resultNumber+=((int)num[i]-(int)'0')*mul;
		mul*=10;
	}
	if (num[0]!='-')
		resultNumber+=((int)num[0]-(int)'0')*mul;
	else
		resultNumber*=-1;
	return resultNumber;
}
void findNumber(string str)
{
	string num="";
	int flag=-1;
	for(int i=0; i<str.length(); ++i)
	{
		//cout<<str<<endl;
		if(str[i]=='-' && num=="" && str[i+1]<='9' && str[i+1]>='1')
		{
			num+=str[i];
			//cout<<num<<endl;
		}
		else if(str[i]>='1' && str[i]<='9')
		{
			num+=str[i];
			//cout<<num<<endl;
		}
		else
		{
			//cout<<num<<endl;
			if(num=="")
			{
				continue;
			}
			else
			{
				flag=checkNumber(num);
				if(flag==1)
				{
					long long newNumber=createNumber(num);
					numbers.push_back(newNumber);	
				}
				num="";
			}
		}
		if(num!="" && i==str.length()-1)
		{
			//cout<<num<<endl;
			flag=checkNumber(num);
			if(flag==1)
			{
				long long newNumber=createNumber(num);
				numbers.push_back(newNumber);	
			}
			num="";
		}
	}
}
int main(int argc, char *mas[], char *envp[])
{
	if(argc==1)
	{
		cout<<"Enter the file name"<<endl;
		return 0;
	}
	for(int i=1; i<argc-1; ++i)
	{
		ifstream fd(mas[i]);
		if (!fd.good())
		{
			cout<<"File "<<mas[i]<<" was not open"<<endl;
			continue;
		}
		string str;
		while(fd>>str)
		{
			findNumber(str);
		}
	}
	sort(numbers.begin(),numbers.end());
	ofstream sd(mas[argc-1]);
	if(sd.good())
	{
		for(int i=0; i<numbers.size(); ++i)
			sd<<numbers[i]<<" ";
	}
	else
	{
		cout<<"File "<<mas[argc-1]<<" was not open"<<endl;
		for(int i=0; i<numbers.size(); ++i)
			sd<<numbers[i]<<" ";
		cout<<endl;
	}
}
