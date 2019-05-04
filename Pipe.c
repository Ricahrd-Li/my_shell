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
            if(i){  //如果i==0，那么输入应该从STDIN中来
                close(fd[i-1][WRITE_END]);
                dup2(fd[i-1][READ_END],STDIN_FILENO);  
                close(fd[i-1][READ_END]);
            }
            if(i<count-1){  //如果i==count-1,那么应该输出到STDOUT中去
                close(fd[i][READ_END]);
                dup2(fd[i][WRITE_END],STDOUT_FILENO);
                close(fd[i][WRITE_END]);
            }
            execvp(*(args+array[i]),args+array[i]);
   
        else{
            wait(NULL);           
        }
    }
}
int Pipe(int count,char* args[],int array[]){
    pid_t pid;
    int fd[2];
    pipe(fd);
    for(int i=0;i<count;i++){
        if((pid=fork())==0){
            if(i){  //如果i==0，那么输入应该从STDIN中来
                close(fd[WRITE_END]);
                dup2(fd[READ_END],STDIN_FILENO);  
                close(fd[READ_END]);
            }
            if(i<count-1){  //如果i==count-1,那么应该输出到STDOUT中去
                close(fd[READ_END]);
                dup2(fd[WRITE_END],STDOUT_FILENO);
                close(fd[WRITE_END]);
            }
            execvp(*(args+array[i]),args+array[i]);
   
        else{
            wait(NULL);           
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
