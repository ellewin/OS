#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sstream>
using namespace std;

int main(int argc, char *argv[], char *envp[])
{
	
	string name(argv[1]);
	if(name=="-help" || name=="--help")
	{
		cout<<"Enter file-name the desired character the substitution character"<<endl;
		return 0;
	}
	string word1(argv[2]);
	string word2(argv[3]);
	char a = argv[2][0];
	char b = argv[3][0];
	if(word1.size()!=1 || word2.size()!=1)
	{
		cout<<"1 changeable character and 1 replaces the symbol"<<endl;
		return 0;
	}
	string filename=name+".lck";
	int fd=open(argv[1],O_RDWR);
	//flock(fd,LOCK_EX);
	ofstream flck(filename.c_str());
	pid_t pid = getpid();
	sleep(1);
	flck<<pid<<endl;
	char * symbol=new char[1];
	int fd2=open(filename.c_str(),O_RDWR);
	//flock(fd2, LOCK_EX);
	string filename2=filename+".lck";
	ofstream flck2(filename2.c_str(),ios::app);
	flck2<<pid<<endl;
	while(read(fd,symbol,1))
	{
		if(symbol[0]==a)
		{
			lockf(fd,F_LOCK,1);
			int pos=lseek(fd,0,SEEK_CUR);
			flck2<<pos<<endl;
		}
	}
	flck2<<"EOF"<<endl;
	//flock(fd2, LOCK_UN);
	lseek(fd, 0, SEEK_SET);
	while(read(fd,symbol,1))
	{
		if(symbol[0]==a)
		{
			int pos=lseek(fd,-1,SEEK_CUR);
			symbol[0]=b;
			cout<<a<< " in pos = "<<pos<<" changed to "<<b<<endl; 
			write(fd,symbol,1);
			sleep(1);
		}
		
		
	}
	close(fd);
	//remove(filename.c_str());
	//flock(fd,LOCK_UN);
	return 0;
}
