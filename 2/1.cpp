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
#include <cstring>
using namespace std;

string config;
int countProgram;
struct structureFile
{
	string file_name;
	string * massive;
	int bit;
	int count;	
	pid_t pid;
};

structureFile * st = new structureFile [100];
int count_str;
int pid_count;

//fill structure
void readConfig()
{
	//ifstream fin("1.txt");
	countProgram=0;
	ifstream fin(config.c_str());
	string ss;
	string new_word;
	new_word="";
	int i=0;
	int j=0;
	int flag=0;
	count_str=0;
	while (getline(fin,ss))
	{
		++countProgram;
		delete [] st[j].massive;
		st[j].file_name="";
		st[j].count=0;
		int co=0;
		st[j].massive= new string [100];
		st[j].count = 0;
		for(int i=0; i<ss.length(); ++i)
		{
			if (ss[i]!=' ')
			{
				new_word+=ss[i]; 
			}
			else
			{
				if (flag==0)
				{
					st[j].file_name=new_word;
					flag=1;
					new_word="";
				}
				else
				{
					st[j].massive[co]=new_word; 
					new_word="";
					++co;
					++st[j].count;
				}
				
			}
		}
		new_word="";
		flag=0;
		st[j].bit=ss[ss.length()-1]-48;
		++j;
		++count_str;
	}
	//cout<<st[2].bit<<endl;

}

//conversion to execv
int converter(int index)
{

	char ** mas = new char * [st[index].count+2];
	for (int i = 0; i < st[index].count+2; i++)
		mas[i] = new char [10];
	string new_name;
	new_name=st[index].file_name;
	strcpy(mas[0],new_name.c_str());
	for(int j=0; j < st[index].count; ++j)
	{
		string new_arg;
		new_arg=st[index].massive[j];
		strcpy(mas[j+1],new_arg.c_str());
		//cout << new_arg << endl;
	}
	mas[st[index].count+1]=NULL;
	return execv(st[index].file_name.c_str(), mas);
}

//get name
string onlyName(int index)
{
	string only_name = "";
	int pos;
	for (int i=0; i<st[index].file_name.size(); ++i)
	{
		if (st[index].file_name[i] == '/')
		{
			pos=i;
		}
	}
	for(int i=pos+1; i<st[index].file_name.size(); ++i)
	{
		only_name+=st[index].file_name[i];
	} 
	return only_name;
}

void create (int index)
{
	//chdir("/");
	string filename = "";
	string only_name = onlyName(index);
	filename="tmp/"+only_name+".pid";
	//cout<<filename + " created with pid " << st[index].pid <<endl;
	syslog(LOG_NOTICE, "%s created with %d\n",filename.c_str(),st[index].pid);
	ofstream f1(filename.c_str());
}

void removed (int index)
{
	//chdir("/");
	string filename = "";
	string only_name = onlyName(index);
	filename="tmp/"+only_name+".pid";
	syslog(LOG_NOTICE,"%s removed with %d\n", filename.c_str(),st[index].pid);
	//cout<<filename+" removed"<<endl;
	remove(filename.c_str());
}

void func()
{
	for(int i=0; i<countProgram; ++i)
	{
		pid_t cpid = fork();
		switch (cpid)
		{
			case -1:
			{
				syslog(LOG_NOTICE,"Fork failed");
				break;
			}
			case 0:
			{
				int count_try=0;
				while(converter(i)<0)
				{
					++count_try;
					syslog(LOG_NOTICE,"Program %s don't execute %d time",st[i].file_name.c_str(),count_try);
					if(count_try==60)
					{
						syslog(LOG_NOTICE,"Sleep 60 seconds");
						sleep(3600);
						count_try=0;
					}
					
				}
				//converter(i);
				return;
			}
			default:
			{
				st[i].pid=cpid;
				create(i);
				pid_count++;
			}
		}
	}
}


void create_demon()
{
	struct rlimit flim;
	if (getppid() != 1)
	{
		signal(SIGTTOU, SIG_IGN);	
		signal(SIGTTIN, SIG_IGN);	
		signal(SIGTSTP, SIG_IGN);
		if (fork() != 0)
			exit (0);
		setsid();
	}
	getrlimit(RLIMIT_NOFILE, &flim);
	for (int fd = 0; fd < flim.rlim_max; fd++)
		close(fd);
	chdir("/");
	
}

void hup(int signal)
{
	syslog(LOG_NOTICE,"Signal interception\n");
	for(int i=0; i<countProgram; ++i)
	{
		if(st[i].pid!=0)
		{
			syslog(LOG_NOTICE,"Process %s with pid %d died\n",st[i].file_name.c_str(),st[i].pid);
			removed(i);
			kill(st[i].pid,SIGKILL);
			st[i].pid=0;
		}
	}
	syslog(LOG_NOTICE,"All processes are killed, we start anew\n");
	readConfig();
	func();
}

int main(int argc, char *mas[], char *envp[])
{
	pid_t cpid;
	pid_count=0;
	string cc(mas[1]);
	config=cc;
	char buf[FILENAME_MAX]; 
	getcwd(buf, sizeof(buf));
	string path(buf);
	config=path+"/"+config;
	//chdir("/");
	syslog(LOG_NOTICE, "Start program\n");
	create_demon();
	readConfig();
	func();
	signal(SIGHUP, hup);
	while(pid_count)
	{
		cpid=waitpid(-1, NULL, 0);
		for (int i=0; i<countProgram; ++i)
		{
			if (st[i].pid==cpid)
			{
				if(st[i].bit==1)
				{
					removed(i);
					st[i].pid=fork();
					if(st[i].pid>0)
					{
						create(i);
						
					}
					if(st[i].pid==0)
					{
						converter(i);
						exit(0);
					}
				}
				else
				{
					removed(i);
					st[i].pid=0;
					pid_count--;
				}
			}
		}
	}
return 0;
}
