/* parent master -> child slave (bc -q, a simple calculator)
   two pipes one for parent to child, other from child to parent
   parent -> 3+4, 5*6 * ... child bc -q results -> parent
*/
#include<unistd.h>
#include<stdio.h>

int main() {

	int p,i;
	int fd1[2],fd2[2];
	char *messages[]={"3+4\n","5*6\n","10+100-(4*4)\n"};	// Comma separated arithmetic operation string array
	char result[100];

	pipe(fd1);												// Initalize first pipe
	pipe(fd2);												// Initalize second pipe
	if (p=fork()) {											// Parent process
		close(fd1[1]);										// Close write option for pipe1
		close(fd2[0]);										// Close read option for pipe2
		dup2(fd2[1],2);										// Bind pipe2 write to stderr
		dup2(fd1[0],0);										// Bind pipe1 read to stdin
		close(fd2[1]);										// Close pipe2 write option
		close(fd1[0]);										// Close pipe1 read option
		for (i=0;i<3;i++) {									// 
			fprintf(stderr,messages[i]);					// Write messages to stderr
			fflush(stderr);									// Flush stderr buffer
			if (fgets(result,100,stdin)) {					// Read result from stdin 
				printf("%s\n",result);						// Print the result
			}
		}

	} else {												// Child process
		close(fd1[0]);										// Close read option for pipe1
		close(fd2[1]);										// Close write option for pipe2
		dup2(fd1[1],1);										// Bind pipe1 write to stdout
		dup2(fd2[0],0);										// Bind pipe2 read to stdin
		close(fd1[1]);										// Close write option for pipe1
		close(fd2[0]);										// Close read option for pipe2
		execl("/usr/bin/bc","bc","-q",(char *)0);			// Execute Bash Calculator : bc -q with stdin(pipe2)
	}

	return 0;
}


