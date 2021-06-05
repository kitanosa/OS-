#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int
main(int argc, char *argv[])
{
    int pipefd[2]; // Первое, что следует заметить, это то, что мы передали массив целых чисел со знаком длиной 2.
    //Это потому, что канал – это не что иное, как массив из двух целых чисел без знака, представляющих два файловых дескриптора. Один для письма, один для чтения
/*
Здесь мы назвали переменную fd. fd [0] – дескриптор входного файла, fd [1] – дескриптор выходного файла.
В этой программе один процесс записывает строку в файловый дескриптор fd [1], а другой процесс читает из файлового дескриптора fd [0].
*/
    pid_t cpid;
    char buf;
    if (argc != 2) {
    fprintf(stderr, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
    }
    if (pipe(pipefd) == -1) { //пайп интовое число. Если оно возвращает -1 то ошибка
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork(); // разъединяем процесс
    if (cpid == -1) { //  проверка на оишбку
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    /* Child reads from pipe ребенок читает из труыбы*/
        close(pipefd[1]);          /* Close unused write end  так как ребенок читает то мы закрываем трубу на запись ребенка*/ 
        while (read(pipefd[0], &buf, 1) > 0) // начинаем читать из трубы 1 это наверн чтение по 1 символу
            write(STDOUT_FILENO, &buf, 1); // она пишет что получила из трубы
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]); // закрываем трубу на чтение для ребенка
        _exit(EXIT_SUCCESS); // заканч процесс
    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1])); // пишем в трубу
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}