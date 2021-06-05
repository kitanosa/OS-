
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>


int main(int argc, int *input[]){
    for(int i = 0; i<49; ++i)
    {
        *input[i] = tolower(*input[i]); // tolower это функция из библиотеки <ctype.h> которая переводит в нижний регистр буквы
    }
}
