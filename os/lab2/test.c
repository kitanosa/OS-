// pipe.c
/*
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

extern int errno;

int main(){
    pid_t pid; // это значение будет принимать -1 0 1 для перехода ребенок - родитель
    static char input[50];
    static char buf[50];
    char c;
    char name[] = "database.txt";
    FILE *fp ;
    fp = fopen(name, "r");

    signed int fd[2]; // создали массив для  0 1 на запись или чтение
    pipe(fd);// провели трубу

    if((pid=fork())==-1){
        int err=errno;
        perror("ответвление не сработало");
        exit(err);
    }

    if(pid != 0){
        close(fd[1]);
        while(scanf("%c", &c) > 0)
            read(fd[0], &c, sizeof(char)); // верно
        printf("В сообщении от ребенка: %s\n", buf);
    } else {
        close(fd[0]);
        printf("Введите сообщение от родителя: ");
        while(scanf("%c", &c) > 0){
            write(fd[1], &c, sizeof(char)); // верно
        }
        exit(0);
    }
    return 0;
}
*/ 
/*
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

extern int errno;

int main(){
    signed int fd[2];
    pid_t pid;
    static char input[50];
    static char buf[50];

    pipe(fd);

    if((pid=fork())==-1){
        int err=errno;
        perror("ответвление не сработало");
        exit(err);
    }

    if(pid != 0){
        close(fd[1]);
        read(fd[0], buf, 50);
        for(int i = 0; i<49; ++i)
        {
            buf[i] = tolower(buf[i]); // tolower это функция из библиотеки <ctype.h> которая переводит в нижний регистр буквы
        }
        printf("В сообщении от ребенка: %s\n", buf);
    } else {
        close(fd[0]);
        printf("Введите сообщение от родителя: ");
        for(int i=0; (input[i]=getchar())!=EOF && input[i]!='\n' && i<49; i++);
        write(fd[1], input, 50);
        exit(0);
    }
    return 0;
}
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

extern int errno;

int main(){
    signed int fd[2];
    pid_t pid;

    pipe(fd);

    if((pid=fork())==-1){
        int err=errno;
        perror("ответвление не сработало");
        exit(err);
    }

    if(pid != 0){
        static char input[50];
        close(fd[1]);
        read(fd[0], input, 50);
        for(int i = 0; i<49; ++i)
        {
            input[i] = tolower(input[i]); // tolower это функция из библиотеки <ctype.h> которая переводит в нижний регистр буквы
            if(input[i] == ' '){
                input[i] = '_';
            }
        }
        close(fd[0]);
        printf("В сообщении от ребенка: %s\n", input);
    } else {
        static char input[50];
        close(fd[0]);
        printf("Введите сообщение от родителя: ");
        for(int i=0; (input[i]=getchar())!=EOF && input[i]!='\n' && i<49; i++);
        write(fd[1], input, 50);
        close(fd[1]);
        exit(0);
    }
    return 0;
}