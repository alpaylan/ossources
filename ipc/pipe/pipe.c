/* child: hello, i am child -> parent writes */
#include<unistd.h>
#include<stdio.h>

int main() {

	int p;
	int fd[2];
	char message[80];

	pipe(fd);									// Pipe is opened with 1 file descriptor for read and 1 for write
	if (p=fork()) {  							// Parent process returns non-zero value
		close(fd[1]); 							// Close writing option for pipe
		dup2(fd[0],0); 							// Bind pipe read to stdin
		close(fd[0]);  							// Close reading option for pipe
		while (fgets(message,10,stdin)!=NULL) {
			printf("CHILD: %s",message);  
		}
	} else {									// Child process returns 0(zero)
		close(fd[0]);							// Close reading option for pipe
		dup2(fd[1],1);							// Bind pipe write to stdout
		close(fd[1]);							// Close writing option for pipe
		//write(1,"Hello\n",6);					// Write"Hello" to stdout(pipe) via file descriptor 1(stdout)
		printf("Hello\n");						// Write "Hello" to stdout(pipe)
		printf("Hello\n");						// Write "Hello" to stdout(pipe)
		printf("I'am child %d\n",getpid());		// Write process id to stdout(pipe)
		fflush(stdout);							// Flush stdout(flush the buffer immediately)
		close(1);								// Close stdout
	}

	return 0;
}


