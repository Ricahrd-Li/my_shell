#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define READ_END 0
#define WRITE_END 1


// pipe的生命周期？？

int Pipe(int count,char* args[],int array[]){
    pid_t pid;
    int **fd=(int **)malloc(sizeof(int[2])*count);
    for(int i=0;i<count;i++) pipe(fd[i]);
    for(int i=0;i<count;i++){
        if((pid=fork())==0){
            if(i){
                close(fd[i-1][WRITE_END]);
                dup2(fd[i-1][READ_END],STDIN_FILENO);  
                close(fd[i-1][READ_END]);
            }
            if(i<count-1){
                close(fd[i][READ_END]);
                dup2(fd[i][WRITE_END],STDOUT_FILENO);
                close(fd[i][WRITE_END]);
            }
            
            //not built-in command
            // if(strcmp(*(args+array[i]),"cd")&& strcmp(*(args+array[i]),"pwd")&& strcmp(*(args+array[i]),"export"))
                execvp(*(args+array[i]),args+array[i]);
                exit(0);
            //built-in command: cd, pwd ,export
            // else if(strcmp(*(args+array[i]),"cd")==0){
                // if (*(args+array[i]+1))
                    // chdir(*(args+array[i]+1));
            // }
            // else if(strcmp(*(args+array[i]),"pwd")==0){
                char wd[4096];
                puts(getcwd(wd, 4096));
            // }
            // else if(strcmp(*(args+array[i]),"export")==0){
                char *name,*value, *a;
                if(*(args+array[i]+1)){
                    name=*(args+array[i]+1);
                    for(a=name; *a &&(*a!='=');a++);
                    *a='\0';   //find '=' and divide
                    if(*(a+1)) value=a+1;
                    else value="\0";
                    setenv(name,value,1);
                }
            // }
        }
        else{
            wait(NULL);
            // printf("parent:%d\n",i);
            
        }
    }
}


int main(){
    int count=2; // 2 | 
    char * args[8]={"ls",NULL,"cat",NULL,NULL,"wc",NULL,NULL};
    int array[3]={0,2,5};
    // printf("%s\n%s\n%s\n",);
    Pipe(count,args,array);
    return 0;
}
