#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <vector>

using namespace std;
int n=5;
int forks=5;


int main(int argc, char *mas[], char *envp[])
{
	int matrix1[n][n];
	int matrix2[n][n];
	char symbol[1];
	int index1,index2;
	if(argc!=3)
	{
		cout<<"Enter 2 arguments - names of files"<<endl;
		return 0;
	}
	ifstream f(mas[1]);
	for(int i=0; i<n; ++i)
		for(int j=0; j<n; ++j)
			f>>matrix1[i][j];
	ifstream g(mas[2]);
	for(int i=0; i<n; ++i)
		for(int j=0; j<n; ++j)
			g>>matrix2[i][j];	
	int flag=0;
	for(int handler=0; handler<forks; ++handler)
	{
		int canal1[2],canal2[2];
		pipe(canal1);
		pipe(canal2);
		if(flag==0)
		{
			index1=canal1[0];
			index2=canal2[0];
			flag=1;
		}
		//cout<<canal1[0]<<" "<<canal2[0]<<endl;
	}
	int workFork=0;
	vector<char> intermediateArray;
	for(int cell=0; cell<n*n; ++cell)
	{
		int fixI=cell/n;
		int fixJ=cell%n;
		for (int k=0; k<n; ++k)
		{
			intermediateArray.push_back((char)matrix1[fixI][k]);
			intermediateArray.push_back((char)matrix2[k][fixJ]);
		}
		if(cell<5)
		{
			char buf[1];
			workFork=n*n/5;
			if(cell<n*n%5)
			{
				++workFork;
			}
			buf[0]=(char) workFork;
			//cout<<"buf "<<buf[0]<<endl;
			write(index1+1+4*(cell%workFork),buf,1);
		}
		for(int i=0; i<2*n-1; ++i)
		{
			symbol[0]=intermediateArray[i];
			write(index1+4*(cell%workFork)+1,symbol,1);
			++i;
		}
		for(int i=1; i<2*n; ++i)
		{
			symbol[0]=intermediateArray[i];
			write(index1+4*(cell%workFork)+1,symbol,1);
			++i;
		}
	}
	for(int handler=0; handler<5; ++handler)
	{
		if(fork()==0) //child
		{
			//cout<<2<<endl;
			char buf[1];
			read(index1+4*(handler),buf,1);
			int numbers=(int)buf[0];
			//cout<<"!!!"<<numbers<<endl;
			for (int proc=0; proc<numbers; ++proc)
			{
				//cout<<3<<endl;
				for(int i=0; i<2*n-1; ++i)
				{
					symbol[0]=intermediateArray[i];
					read(index1+4*handler,symbol,1);
					++i;
				}
				for(int i=1; i<2*n; ++i)
				{
					symbol[0]=intermediateArray[i];
					read(index1+4*handler,symbol,1);
					++i;
				}
				int result=0;
				for(int i=0; i<2*n; ++i)
				{
					//cout<<"!!!-!!! "<<(int)intermediateArray[i]<<" "<<(int)intermediateArray[i+1]<<endl;
					result+=(int)intermediateArray[i]*(int)intermediateArray[i+1];
					//cout<<result<<" ";
					++i;
				}
				//cout<<"result "<<result<<' ';
				char resultBuf[1];
				resultBuf[0]=(char)result;
				write(index2+1+4*handler,resultBuf,1); 
			}
			//cout<<1<<endl;
			return 1;
		}
	}
	for(int i=0; i<n*n; ++i)
	{
		char buf[1];
		read(index2+4*(i%workFork),buf,1);
		cout<<((int)buf[0]+256)%256<<" ";
		if (i%n==n-1)
			cout<<endl;
	}
}
