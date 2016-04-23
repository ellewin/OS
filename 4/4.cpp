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
	string filename=name+".lck";
	int fd=open(argv[1],O_RDWR);
	flock(fd,LOCK_EX);
	ofstream flck(filename.c_str());
	pid_t pid = getpid();
	sleep(1);
	flck<<pid<<endl;
	char * symbol=new char[1];
	while(read(fd,symbol,1))
	{
		lseek(fd,-1,SEEK_CUR);
		if(symbol[0]==argv[2][0])
		{
			symbol[0]=argv[3][0];
		}
		write(fd,symbol,1);
		sleep(1);
	}
	close(fd);
	remove(filename.c_str());
	flock(fd,LOCK_UN);
	return 0;
}
