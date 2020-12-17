#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/wait.h>

//#define DEBUG

#ifdef DEBUG
	#define PRINTD(fmt, args...) printf(fmt, ## args)
#else
	#define PRINTD(fmt, args...)
#endif

#define __STR(const_num) # const_num
#define STR(const_num) __STR(const_num)

void usage(char * name){
	printf("Usage: %s <filename>\n", name);
	printf("<filename> type:\n");
	printf("<time in seconds> <program to execute>\n");
}

int main(int argc, char *argv[], char *envp[]){
char buf[3] = {};
#define S(arg) # arg
char mas[] = "abc"S(23)"def";
printf ("%s\n", mas);
scanf ("%2s", buf);
pid_t pid;

	if (argc != 2){
		usage(argv[0]); exit(EXIT_FAILURE);
	}

	FILE *fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL){
		usage(argv[0]); exit(EXIT_FAILURE);
	}

	char str[FILENAME_MAX] = {};
	int t = 0;

	while (fscanf(fp, "%d%"STR(FILENAME_MAX)"s", &t, str) == 2){
		PRINTD("%s is executed with delay %d\n", str, t);

		if (t < 0){
			usage(argv[0]); exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1){
			assert(0 && "Fork failure!");
		}else if (pid == 0){
			PRINTD("%s is executed in child\n", str);
			sleep(t);
			execlp(str, str, NULL);
			printf("\"%s\" failed!\n", str);
			exit(EXIT_FAILURE);
		}
	}

	while (wait(NULL) != -1) {};
	if (!feof(fp)){
		usage(argv[0]); exit(EXIT_FAILURE);
	}

	fclose(fp);
	return 0;
}
