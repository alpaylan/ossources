/* parent /bin/ls | child tr */
#include<unistd.h>
#include<stdio.h>

int main() {

	int p;
	int fd[2];

	pipe(fd);													// Pipe is opened with 1 file descriptor for read and 1 for write
	if (p=fork()) {												// Parent process returns non-zero value
		close(fd[0]);											// Close reading option for pipe
		dup2(fd[1],1);											// Stdout is forwarded to pipe
		close(fd[1]);											// Close writing option for pipe
		execl("/bin/ls","/bin/ls","-al",(char *)0);				// Execute ls -al in the working directory with execl syscall
	} else {													// Child process returns zero 
		close(fd[1]);											// Close writing option for pipe
		dup2(fd[0],0);											// Pipe read is forwarded to stdin
		close(fd[1]);											// Close reading option for pipe
		execl("/usr/bin/tr","tr","/a-z/","/A-Z/",(char *)0);	// stdin(pipe) input is converted uppercase and forwarded to stdout
	}

	return 0;
}


