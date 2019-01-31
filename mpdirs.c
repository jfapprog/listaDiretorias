/*
** UC: 21111 - Sistemas Operativos
** e-fólio A 2014/15 (mpdirs.c)
**
** Aluno: 1301913 - José Póvoa
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NDIR 2

int main(int argc, char *argv[])
{
	pid_t pidFork;
	int status, i, modoAbrir = 0;
	char *stringAbrir[2] = {"w","a"};
	char nomeProcesso[10] = {'B','C','D','E','F','G','H','I','J','L'};
	FILE *fin;
	
	/*testar se o número de argumentos é correto*/
	if (argc != 1 + NDIR)
	{
		printf("Utilizacao: mpdirs ");
		for (i=1; i <= NDIR; i++)
			printf("dir%d ",i);
		printf("\n");
		exit(1);
	}

	/*testar se existem as diretorias*/
	for (i=1; i <= NDIR; i++)
	{
		if ((fin = fopen(argv[i],"r")) == NULL)
		{
			printf("Diretoria %s inexistente\n", argv[i]);
			exit(1);
		}
		fclose(fin);
	}
	
	/*Apresentar o PID do processo A e do seu processo pai*/
	printf("Processo A: PID=%4d PPID=%4d\n", (int) getpid(), (int) getppid());
	
	for (i = 1; i <= NDIR; i++)
	{
		/*Criar um processo filho*/	
		pidFork = fork();
	
		/*Analisar se ocorreu um erro*/
		if (pidFork == -1)
		{
			printf("Ocorreu um erro ao criar um processo filho\n");
			exit(1);
		}
	
		/*Código para executar pelo processo pai - processo A*/
		if (pidFork)
		{
			/*Bloquear o processo A até que o seu filho termine*/
			wait(&status);
			/*Analisar o estado de término do processo filho*/
			if (WIFEXITED(status))
				printf("Processo A: Lista %s terminada!\n", argv[i]);
			else
				printf("Processo A: Lista %s terminada com possiveis erros! \n", argv[i]);
		}
		/*Código para executar pelo processo filho*/
		else
		{
			/*Apresentar o PID do processo e o do seu processo pai*/
			printf("Processo %c: PID=%4d PPID=%4d\n", nomeProcesso[i-1], (int) getpid(), (int) getppid());
			/*Subtituir a imagem do processo filho pela do comando ls diri*/
			/*Mudar o modo de abrir para "appending" caso não seja a primeira diretoria*/
			if (i>1 && modoAbrir == 0)
				modoAbrir = 1;
			/*Mudar o apontador FILE *stdout, para ser passado para a nova imagem do código filho*/
			if (freopen("dirs.txt",stringAbrir[modoAbrir],stdout) == NULL)
			{
				printf("Impossivel abrir ficheiro para escrita\n");
				exit(1);
			}
			/*invocar a funcao execl*/
			execl("/bin/ls", "ls",argv[i],NULL);
			/*Apresentar mensagem caso ocorra um erro na funcção execl*/
			printf("Ocorreu em erro ao executar o comando ls %s\n", argv[i]);
		}
	}
	
	/*Apresentar mensagem de listas terminadas*/
	printf("Processo A: Listagem total dirs.txt terminada!\n");
	return 0;
}
