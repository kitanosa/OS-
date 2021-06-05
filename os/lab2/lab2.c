#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main()
{
	int fd_0[2]; // три массива
	int fd_1[2];
	int fd_2[2];
	pipe(fd_0); // три трубы создаем без проверки на ошибку
	pipe(fd_1);
	pipe(fd_2); // треться труба между вторым чайлдом и родителем
	int pid_0;
	int pid_1;


	static char output[50];
	if((pid_0 = fork()) > 0) // после разъединения если ты родитель
	{
		if((pid_1 = fork()) > 0) // если все еще родитель ты. Родитель может быть один только
// то мы делимся еще раз в ребенке  
		{
			close(fd_0[0]);
			close(fd_2[1]);

			static char input[50];
        	printf("Введите сообщение от родителя: ");
        	for(int i=0; (input[i]=getchar())!=EOF && input[i]!='\n' && i<49; i++);
        	write(fd_0[1], input, 50); // для первого ребенка пишем в пайп
			close(fd_0[1]); // закрываем на запись

        
			read(fd_2[0], input, 50); // читает от второго ребенка
			for(int i=0; i<50; i++){
				output[i] = input[i];
			}
            printf("В сообщении от ребенка: %s\n", output);
			close(fd_2[0]);
		}
		else if(pid_1 == 0) // это для второго ребенка так как это после уже одного форка происходит
		{
			close(fd_1[1]);
			close(fd_2[0]);
			static char input[50];
        	//close(fd[1]);
        	read(fd_1[0], input, 50);
        	for(int i = 0; i<49; ++i)
        	{
            	if(input[i] == ' '){
                	input[i] = '_';
            	}
        	}
        	write(fd_2[1], input, 50);
        	close(fd_2[1]);
			close(fd_1[0]);
		}
		else
		{
			perror("fork error\n");
			exit(-1);
		}
	}
	else if (pid_0 == 0) // это для первого ребенка. Так как мы поделили уже один раз форком
	{
		close(fd_0[1]);
		close(fd_1[0]);
		static char input[50];
        //close(fd[1]);
        read(fd_0[0], input, 50);
        for(int i = 0; i<49; ++i)
        {
            input[i] = tolower(input[i]); // tolower это функция из библиотеки <ctype.h> которая переводит в нижний регистр буквы
        }
        write(fd_1[1], input, 50);
        close(fd_0[0]);
        close(fd_1[1]);
	}
	else
	{
		perror("fork error\n");
		exit(-1);
	}
	return 0;
}
