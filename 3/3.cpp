#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main(int argc, char *argv[], char *envp[])
{
	if (!argv[1])
	{
		cout<<"Enter 'file-name1 < file-name2'"<<endl;
		cout<<"file-name1 - empty file"<<endl;
		cout<<"file-name2 - unzip file"<<endl;
		return 0;
	}
	int fd=open(argv[1], O_WRONLY | O_CREAT);
	char symbol;
	long int countSymbol=0;
	int empty=1;
	char buf[1];
	while(cin >> symbol)
	{
		if(symbol!='\0')
		{
			buf[0]=symbol;
			write(fd,buf,1);
			empty=0;
		}
		else
		{
			lseek(fd,1,SEEK_CUR);
		}
		++countSymbol;
	}
	ftruncate(fd,countSymbol);
	if(empty==0)
	{
		buf[0]='\x0A';
		write(fd,buf,1);
	}
	close(fd);
}
