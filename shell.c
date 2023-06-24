#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>

int bg=1,pid_index=0,ctrlz=0;
pid_t pid_stack[100];
jmp_buf background;

void signal_handler(int signum)
{
        //int status;
        //printf("Caught signal %d\n",signum);
        //waitpid(pid_stack[pid_index],&status,0);
	//ctrlz=1;
	ctrlz=1;
	//longjmp(background,1);
	//continue;
}

void initialize_stack()
{
	int i;
	for(i=0;i<100;i++)
		pid_stack[i]=-1;
	return;
}

void put_process(pid_t pid)
{
	int i;
	for(i=0;i<100;i++)
	{
		if(pid_stack[i]==-1)
		{
			pid_stack[i]=pid;
			break;
		}
	}
}

void repair(int thesi)
{
	int i;
        for (i=thesi;i+1<10 ;i++){
                pid_stack[i]=pid_stack[i+1];
        }
        pid_stack[i]=-1;
}

void check_n_repair()
{
	int i;
	for(i=0;i<100;i++)
	{
		if(pid_stack[i]==0)
		{
			repair(i);
		}
	}
}

void remove_process(pid_t rem)
{
	int i;
	for(i=0;i<100;i++)
	{
		if(pid_stack[i]==rem)
		{
			pid_stack[i]=0;
			break;
		}
	}
}

void check_processes()
{
	int i,status;
	for(i=0;i<100;i++)
	{
		if((pid_stack[i]!=0 || pid_stack[i]!=-1)&&(waitpid(pid_stack[i],&status,WNOHANG)!=0))
		{
			pid_stack[i]=0;
		}
	}
}

pid_t last_process()
{
	int i;
	pid_t last=0;
	for(i=0;i<100;i++)
	{
		if(pid_stack[i]!=-1 && pid_stack[i]!=0)
			last=pid_stack[i];
	}
	remove_process(last);
	check_n_repair();
	return last;
}

char* return_prompt()
{
	int i=0;
	char *whole,*login,*shell="@cs345sh",*end="$ ",cwd[1024],*final;
	whole=malloc(1024*sizeof(char));
	login=getlogin();
	getcwd(cwd,sizeof(cwd));
	//printf("%s@cs345sh%s$ ",login,cwd);
	strcat(whole,login);
	strcat(whole,"@cs345sh");
	strcat(whole,cwd);
	strcat(whole,end);
	return whole;

}

int starts_with(char *big, char *small)
{
        int i=0;
        while(small[i]!='\0')
        {
                if(small[i]!=big[i])return 0;
                i++;
        }
	//printf("starts with ended\n");
        return 1;
}

int ends_with(char *big,char *small)
{
        int i=0;
	//printf("\nBIG:%s\n",big);
	//printf("\nSMALL:%s\n",small);
        while(small[i]!='\0')
        {
                if(big[i+strlen(big)-strlen(small)]!=small[i])return 0;
                i++;
        }
	//printf("ends with ended\n");
        return 1;
}

void middle_sep(char*str,char *begin,char *end)
{
	begin=malloc(sizeof(char)*1024);
	end=malloc(sizeof(char)*1024);
	strcpy(begin,strsep(&str,"*"));
	strcpy(end,strsep(&str,"*"));
}

int middle_word(char* peirama,char *begin,char *end)
{
        int i=0;
        char *test;
	test=malloc(sizeof(char)*1024);
	strcpy(test,peirama);
        //printf("STRSEP DONE!\n");
	if(starts_with(test,begin) && ends_with(test,end))
        {
                //free(begin);
                //free(end);
                return 1;
	}
        else
        {
                //free(begin);
                //free(end);
                return 0;
        }
}

int wildcard(char* command,char **in_pieces,int flag,char *ster,int index)
{

	int len=0,i=0,counter=0,loop,j,path_flag=0;
        char *star,*begin,*end,*pth,*path,*str,*rest;
	struct dirent *de;
	DIR *d;
	pth=malloc(sizeof(char)*1024);
	path=malloc(sizeof(char)*1024);
	star=malloc(sizeof(char)*1024);
	str=malloc(sizeof(char)*1024);
        str[strlen(str)]='\0';
	if(strstr(ster,"/")!=NULL)
	{
		int count,thesi;
		path_flag=1;
		//pth=malloc(sizeof(char)*1024);
		for(i=0;i<strlen(ster);i++)
		{
			if(ster[i]=='/')
				thesi=i;
		}
		for(i=0;i<=thesi;i++)
		{
			pth[i]=ster[i];
		}
		int z=0;
		for(i=thesi+1;i<strlen(ster);i++)
		{
			str[z]=ster[i];
			star[z]=ster[i];
			z++;
		}
		//printf("seg\n");
		pth[i]='\0';
		strcpy(path,"./");
 	        strcat(path,pth);
	}
	else
	{
		//printf("Ok\n");
		strcpy(star,ster);
        	strcpy(str,ster);
		path=".";
	}
	d=opendir(path);
        free(in_pieces[index]);
	if (d == NULL)  // opendir returns NULL if couldn't open directory
        {
                printf("Could not open current directory" );
                return;
        }
	if(flag==1)
	{
		int j=0;
		rest=malloc(sizeof(char)*strlen(str));
		while(str[j]!='\0')
		{
			rest[j]=str[j+1];
			j++;
		}
		rest[j]='\0';
	}
	else if(flag==2)
	{
		begin=malloc(sizeof(char)*1024);
                end=malloc(sizeof(char)*1024);
//                printf("STR:%s",star);
                strcpy(begin,strsep(&star,"*"));
                strcpy(end,strsep(&star,"*"));
 //               printf("SEPERATED!\n");
  //              printf("BEGIN:%s\n",begin);
    //            printf("END:%s\n",end);
	}
	else if(flag==3)
	{
		int j=0;
                rest=malloc(sizeof(char)*strlen(str));
                while(str[j]!='*')
                {
                        rest[j]=str[j];
                        j++;
                }
		rest[j]='\0';
	}
	while ((de = readdir(d)) != NULL)
        {
		if(flag==3)
		{
			if(starts_with(de->d_name,rest))
			{
				if(path_flag==0)
				{
					//printf("mpika starts with\n");
					in_pieces[index]=malloc(sizeof(char)*1024);
					strcpy(in_pieces[index],de->d_name);
					index++;
				}
				else
				{
					//printf("mpika starts with\n");
                                        in_pieces[index]=malloc(sizeof(char)*1024);
                                        strcpy(in_pieces[index],path);
					strcat(in_pieces[index],de->d_name);
                                        index++;
				}
			}
		}
		else if(flag==2)
		{
			if(middle_word(de->d_name,begin,end))
			{
				//printf("mpika edw\n");
				if(path_flag==0)
				{
					in_pieces[index]=malloc(sizeof(char)*1024);
					strcpy(in_pieces[index],de->d_name);
					index++;
				}
				else
                                {
                                        //printf("mpika starts with\n");
                                        in_pieces[index]=malloc(sizeof(char)*1024);
                                        strcpy(in_pieces[index],path);
                                        strcat(in_pieces[index],de->d_name);
                                        index++;
                                }
			}
		}
		else if(flag==1)							// && (strlen(de->d_name) >= (strlen(str)-1)))
		{
			if(ends_with(de->d_name,rest))
			{
				if(path_flag==0)
				{	//printf("mpika ends with\n");
					in_pieces[index]=malloc(sizeof(char)*1024);
					strcpy(in_pieces[index],de->d_name);
                        		index++;
				}
				else
                                {
                                        //printf("mpika starts with\n");
                                        in_pieces[index]=malloc(sizeof(char)*1024);
                                        strcpy(in_pieces[index],path);
                                        strcat(in_pieces[index],de->d_name);
                                        index++;
                                }
			}
                }
	}
	//counter=i;
	//loop=counter+spaces;
	return index;
}

int num_of_words(char **in_pieces)
{
 	int i=0;
        while(in_pieces[i]!=NULL)
        {
                i++;
        }
	return i;

}

void pipe_parse(char *command,char **in_pieces)
{
	int i=0;
	char *temp;
	while(1)
	{
		in_pieces[i]=malloc(sizeof(char)*1024);
		temp=strsep(&command,"|");
		if(temp==NULL)
			break;
		strcpy(in_pieces[i],temp);
		i++;
	}
	in_pieces[i]=NULL;
}

int star_position(char *str)
{
        if(*(str+0)=='*')return 1;
        else if(*(str+(strlen(str)-1))=='*')return 3;
        else return 2;
}

void parse_command(char* command,char **in_pieces,int *flag)
{
	int i=0,z=0,x=0,num=0,spaces=0,wild=0,temp=0;
	char *post;
	post=malloc(sizeof(char)*1024);
	while(command[i]!='\0')
	{
		if(command[i]==' ')
		{
			spaces++;
		}
		else if(command[i]=='|')
		{
			flag[0]++;
			//pipe_parse(command,in_pieces);
		}
		else if(command[i]=='<')
		{
			flag[1]=1;
		}
		else if(command[i]=='>')
		{
			flag[2]=1;
		}
		else if(command[i]=='>' && command[i+1]=='>')
		{
			flag[3]=1;
		}
		else if(command[i]=='&')
		{
			flag[4]=1;
		}
		i++;
	}
	if(flag[0]>=1)
	{
		//printf("FLAG[0]=%d",flag[0]);
		return;
	}
	else
	{
		int stoixeia=0;
		//for(i=0;i<=spaces+1;i++)
                //	in_pieces[i]=malloc(sizeof(char)*1024);
		for(i=0;i<(spaces+1);i++)
		{
			in_pieces[i]=malloc(sizeof(char)*1024);
			strcpy(in_pieces[i],strsep(&command," "));
			if(strlen(in_pieces[i])==0)
			{
				i--;
				spaces--;
			}
			else if(strstr(in_pieces[i],"*")!=NULL)
			{
				wild=star_position(in_pieces[i]);
				temp=wildcard(command,in_pieces,wild,in_pieces[i],i);
				stoixeia=temp-i;
				spaces=spaces+stoixeia;
				i=temp;
			}
		}
		in_pieces[i]=NULL;
	}
	return;
}

void aristero_velos(char *input, char *output)
{
        char *temp,*args[]={output,NULL};
        int fd0 = open(input,O_RDONLY);
        dup2(fd0, STDIN_FILENO);
        close(fd0);
	return;
}

void deksi_velos(char *input, char *output)
{
	//printf("%s\n",output);
    	int fd1=creat(output,0644);
    	char *args[]={input,NULL};
   	dup2(fd1,STDOUT_FILENO);
        close(fd1);
	return;
}

void append(char *input,char *output)
{
	FILE *f=fopen(output,"a+");
    	int fd=fileno(f);
	char *args[]={input,NULL};
	dup2(fd,1);
	close(fd);
	return;
}

void redirecting(char **command)
{
	signal(SIGTSTP, SIG_IGN);
	int i=0,flag=0;
	char *arguments[1024];
	while(strcmp(command[i],">")!=0 && strcmp(command[i],"<")!=0 &&strcmp(command[i],">>")!=0)
	{
		arguments[i]=malloc(sizeof(char)*1024);
		arguments[i]=command[i];
		i++;
	}
	arguments[i]=NULL;
	pid_t pid;
        pid=fork();
        if(pid==-1)
        {
                perror("fork");
                exit(EXIT_FAILURE);
        }
	else if(pid==0)
	{
		while(command[i]!=NULL)
		{
			if(!strcmp(command[i],"<"))
			{
				//flag=1;
				aristero_velos(command[i+1],command[0]);
			}
			else if(!strcmp(command[i],">"))
			{
				deksi_velos(command[0],command[i+1]);
			}
			else if(!strcmp(command[i],">>"))
			{
				append(command[0],command[i+1]);
			}
			i++;
			//printf("I:%d",i);
		}
		execvp(arguments[0],arguments);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}

void pipe_redirecting(char **command)
{
        int i=0,flag=0;
        while(command[i]!=NULL)
        {
		//printf("%s\n",command[i]);
                if(!strcmp(command[i],"<"))
                {
                                //flag=1;
                        aristero_velos(command[i+1],command[0]);
                }
                else if(!strcmp(command[i],">"))
                {
                                //flag=1;
                                //printf("input:%s\noutput:%s\n",command[0],command[i+1]);
                        deksi_velos(command[0],command[i+1]);
                }
                else if(!strcmp(command[i],">>"))
                {
                        append(command[0],command[i+1]);
                }
                i++;
        }
}

void change_directory(char *path)
{
    if (chdir (path) == -1)
    {
        printf ("change directory failed -");
    }
}

void clear_strcommand(char* command,char **in_pieces)
{
	int i,j;
	for(i=0;in_pieces[i]!=NULL;i++)
	{
		for(j=0;j<strlen(in_pieces[i]);j++)
		{
			in_pieces[i][j]='\0';
		}
	}
	for(i=0;i<strlen(command);i++)
	{
		command[i]='\0';
	}
}

void foreground()
{
	int status;
	pid_t pid=last_process();
        if(pid!=0 && pid!=-1)
        {	printf("Brought process with pid:%d to the foreground\n",pid);
		waitpid(pid,&status,0);
	}else
		printf("Nothing in the background!\n");
}

void build_in_execute(char * str,char **command)
{
	//printf("ola kala!\n");
	if(!strcmp(command[0],"cd"))
	{
//		printf("mpika edw!\n");
		change_directory(command[1]);
	}
	else if(!strcmp(command[0],"exit"))
	{
		printf("Exiting now...\n");
		exit(1);
	}
	else if(!strcmp(command[0],"fg"))
	{
		//printf("Foreground!\n");
		foreground();
	}
	clear_strcommand(str,command);
}


void simple_pipe(char **command,int *flag)
{
        int pipefd[2],flag1[5]={0,0,0,0,0},flag2[5]={0,0,0,0,0};
        pid_t pid1,pid2;
        char str;
	char *prin[1024];
	char *meta[1024];
	signal(SIGTSTP, SIG_IGN);
        pipe(pipefd);
	parse_command(command[0],prin,flag1);
        parse_command(command[1],meta,flag2);
	pid1=fork();
	if(pid1<0)
	{
		printf("fork failed\n");
		exit(-1);
	}
	if(pid1==0)
        {
		if(flag1[1] || flag1[2] || flag1[3])
        	{
                	pipe_redirecting(prin);
        	}
		close(pipefd[0]);
                dup2(pipefd[1],STDOUT_FILENO);
                close(pipefd[1]);
                execvp(prin[0],prin);
		exit(1);
        }
        else
        {
		//wait(NULL);
		pid2=fork();
		if(pid2<0)
		{
			printf("fork failed\n");
                	exit(-1);
		}
		if(pid2==0)
		{
			if(flag2[1] || flag2[2] || flag2[3])
        		{
                		pipe_redirecting(meta);
        		}
			close(pipefd[1]);
                	dup2(pipefd[0],STDIN_FILENO);
                	close(pipefd[0]);
                	//if(flag1[2]==0)
			execvp(meta[0],meta);
			exit(1);
		}
		else
        	{
			close(pipefd[1]);
			close(pipefd[0]);
			wait(NULL);
			wait(NULL);
        	}
        }
        return;
}

void make_zeros(int *flag)
{
	int i=0;
	for(i=0;i<5;i++)flag[i]=0;
	return;
}

void multi_pipe(char ***);

void multiple_piping(char*command ,int plithos)
{
	int i,j,le3eis=plithos+1,**flag;
	char **commands[1024],*pre_space[1024];
	flag=malloc(sizeof(int*)*(plithos+1));
	pipe_parse(command,pre_space);
	for(i=0;i<le3eis;i++)
	{
		flag[i]=malloc(sizeof(int)*5);
		commands[i]=malloc(sizeof(char*)*(1024));
		make_zeros(flag[i]);
		parse_command(pre_space[i],commands[i],flag[i]);
	}
	commands[le3eis]=malloc(sizeof(char*)*(1024));
	commands[le3eis]=NULL;
	//printf("PLITHOS:%d\n",plithos);
	multi_pipe(commands);
}

void multi_pipe(char ***command_pieces)
{
	int   f_pipe[2];
  	int   input = 0;
	pid_t pid;
	signal(SIGTSTP, SIG_IGN);
  	while (*command_pieces != NULL)
    	{
     		pipe(f_pipe);
		pid=fork();
      		if (pid == -1)
        	{
          		exit(-1);
        	}
      		else if (pid == 0)
        	{
			pipe_redirecting(command_pieces[0]);
			dup2(input,0);
          		if (*(command_pieces+1) != NULL)
            		{
				dup2(f_pipe[1],1);
          		}
			close(f_pipe[0]);
          		execvp((*command_pieces)[0],*command_pieces);
          		exit(-1);
        	}
      		else
        	{
          		wait(NULL);
          		close(f_pipe[1]);
          		input = f_pipe[0];
          		command_pieces++;
        	}
    	}
	return;
}

void non_build(char **command,int no_cols)
{
	int i,z=0;
	pid_t pid;
	pid=fork();
	signal(SIGTSTP, SIG_IGN);
	if (pid == 0)
	{
		execvp(command[0], command);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}


int main()
{
	int i,status,wordsnum,flag[5]={0,0,0,0,0};
	char *command[1024],*str,*inst;
        char *login,cwd[1024];
	pid_t pid;
	initialize_stack();
	str=malloc(sizeof(char)*1024);
	//signal(SIGTSTP,SIG_IGN);
	signal(SIGTSTP, signal_handler);
	//signal(SIGINT, SIG_IGN);
	while(1)
	{
		str=readline(return_prompt());
		if(str==NULL || str=="\n" || str=="\0" || str=="" || str=="" || strlen(str)==0)continue;
                parse_command(str,command,flag);
                if(command[0]!=NULL && ( !strcmp(command[0],"cd")||!strcmp(command[0],"fg")||!strcmp(command[0],"exit")))
                {
                        //printf("paw gia execute\n");
                        build_in_execute(str,command);
                	continue;
		}
		pid=fork();
		if(pid<0)
		{
			printf("fork error\n");
			exit(-1);
		}
		if(pid==0)
		{
			if(flag[0]!=0)
			{
				if(flag[0]==1)
				{
					//printf("Piping...\n");
					pipe_parse(str,command);
					//printf("Done parsing paw simple\n");
					simple_pipe(command,flag);
				}
				else
				{
					//printf("Multiple piping...\n");
					multiple_piping(str,flag[0]);
				}
			}
			else if(flag[1] || flag[2] || flag[3])
			{
				//printf("redirecting\n");
				redirecting(command);
			}
			else
			{
				non_build(command,wordsnum);
			}
			exit(1);
		}
		else
		{
			if(flag[4]==1)
			{
				printf("PID in the background:%d\n",pid);
				put_process(pid);
			}
			else
			{
				do
				{
					if(ctrlz==1)
					{
						printf("PID in the background:%d\n",pid);
						put_process(pid);
					}
				}
				while(ctrlz==0 && waitpid(pid,&status,WNOHANG)==0);
			}
		}
		check_processes();
                check_n_repair();
		clear_strcommand(str,command);
		for(i=0;i<5;i++)flag[i]=0;
		ctrlz=0;
	}
	free(str);
	return 1;
}
