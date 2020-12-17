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

#define PERROR(name, args...) \
	do{printf(args); usage(name);}while(0)

void usage(char * name){
	assert (name);

	printf("Usage: %s <filename>\n"
		"<filename> type:\n"
		"<time in seconds> <program to execute>\n",
		name);

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[], char *envp[]){

	if (argc != 2) PERROR(argv[0], "Wrong number of arguments!\n\n");

	FILE *fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL) PERROR(argv[0], "Incorrect input file!\n\n");

	char str[FILENAME_MAX] = {};
	int t = 0;
	int ret = 0;

	while ((ret = fscanf(fp, "%d%"STR(FILENAME_MAX)"s", &t, str)) == 2){
		PRINTD("%s is executed with delay %d\n", str, t);

		if (t < 0) PERROR(argv[0], "Incorrect time (%d) for process %s!\n\n", t, str);

		pid_t pid = fork();
		if (pid == -1){
			assert(!"Fork failure!");
		}else if (pid == 0){
			PRINTD("%s is executed in child\n", str);
			sleep(t);
			execlp(str, str, NULL);
			printf("\"%s\" failed!\n", str);
			exit(EXIT_FAILURE);
		}
	}

	while (wait(NULL) != -1) {};
	if (!feof(fp)) PERROR(argv[0], "File format is incorrect! %d parameters was read\n\n", ret);

	fclose(fp);
	return EXIT_SUCCESS;
}
