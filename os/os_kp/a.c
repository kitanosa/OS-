#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdbool.h>

#define MIN_CAP 4
#define STDIN 0

size_t read_string(char **str_, int fd) {
    free(*str_);
    size_t str_size = 0;
    size_t cap = MIN_CAP;
    char *str = (char*) malloc(sizeof(char) * cap);
    if (str == NULL) {
        perror("Malloc error");
        exit(-1);
    }
    char c;
    while (read(fd, &c, sizeof(char)) == 1) {
        if (c == '\n') {
            break;
        }
        str[(str_size)++] = c;
        if (str_size == cap) {
            str = (char*) realloc(str, sizeof(char) * cap * 3 / 2);
            cap = cap * 3 / 2;
            if (str == NULL) {
                perror("Realloc error");
                exit(-2);
            }
        }
    }
    str[str_size] = '\0';

    *str_ = str;
    return str_size;
}

size_t str_length(char *str) {
    size_t length = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        ++length;
    }
    return length;
}

int main() {
    int ab[2];
    int ac[2];
    int ca[2];
    int cb[2];
    
    
    pipe(ab);
    pipe(ac);// два пайпа. Ведь пайпы однонаправлены 
    pipe(ca);
    pipe(cb);

    int id1 = fork();

    if (id1 < 0) {
        perror("Fork error");
        exit(1);
    }
    else if (id1 == 0) { // ребёнок
        close(ac[1]);
        close(ca[0]);
        close(cb[0]);
        close(ab[0]);
        close(ab[1]);

        // 2 - 99 файловые дескрипторы
        // 1, 0, \0

        char pac[3]; //3 чтобы точно вместился файловый дескриптор при значении 2-99. А третий смивол - конец строки
        sprintf(pac, "%d", ac[0]); // a - > c, программа с читает. sprintf аналог принта только он записывает куда надо. В данном случае в pac

        char pca[3];
        sprintf(pca, "%d", ca[1]); // c -> a, с пишет ok

        char pcb[3];
        sprintf(pcb, "%d", cb[1]); // с -> b, с пишет размер строки
        
        execl("./c", "./c", pac, pca, pcb, NULL); // запуск c.c
    }
    else { // родитель 
        int id2 = fork();
        if (id2 < 0) {
            perror("Fork error");
            exit(1);
        }
        else if (id2 == 0) { // второй ребёнок
            // закрываем ненужные стороны пайпа
            close(ac[0]);
            close(ac[1]);
            close(ca[0]);
            close(ca[1]);
            close(cb[1]);
            close(ab[1]);

            char pcb[2];
            sprintf(pcb, "%d", ca[0]);

            char pab[2];
            sprintf(pab, "%d", cb[0]);

            execl("./b", "./b",  pcb, pab, NULL); // запуск b.c
        }
        else { // родитель
            // закрываем ненужные стороны пайпа
            close(ac[0]);
            close(ca[1]);
            close(ab[0]);
            close(cb[1]);
            close(cb[0]);
         
            char *str = NULL; // указатель на строку
            while ((read_string(&str, STDIN)) > 0) { // читаем по строке до ctrl d
                size_t size = str_length(str); // длина строчки
                write(ac[1], &size, sizeof(size_t)); // дескриптор, указатель на size, size_t в байтах
                write(ac[1], str, size); 
                write(ab[1], &size, sizeof(size_t)); 

                int ok; // ждем пока С не пришлёт ок
                read(ca[0], &ok, sizeof(ok));  // останавливается пока туда что-то запишут
                //  удовлетворяет заданию "ДО тех пор пока программа А не примет "соощение о получении строки" 
                // она не может отправлять следующую строку проргамме С
            }

            close(ca[0]);
            close(ac[1]);
            close(ab[1]);
         }
    }

    return 0;
}
