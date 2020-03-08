
/* child1 /bin/ls | child2 tr, parent waits both
   same as /bin/ls -al | /usr/bin/tr /a-z/ /A-Z/
   on shell */
#include<unistd.h>
#include<stdio.h>

int main() {

	int p1, p2;
	int fd[2];

	pipe(fd);
	if (p1=fork()) {
		if (p2 = fork()) {										// Parent Process
			int c;
			close(fd[0]);										// Close reading option for pipe
			close(fd[1]);										// Close writing option for pipe
			wait(&c);											// Wait for the first child to terminate and c is equal to c1_pid
			wait(&c);											// Wait for the second child to terminate and c is equal to c2_pid
		} else {												// Second child
			close(fd[0]);										// Close reading option for pipe
			dup2(fd[1],1);										// Bind pipe write to stdout
			close(fd[1]);										// Close stdout file descriptor
			execl("/bin/ls","ls","-al",(char *)0);				// Execute ls -al in the working directory and write to stdout(pipe)
		}
	} else {													// First child
		close(fd[1]);											// Close writing option for pipe
		dup2(fd[0],0);											// Bind pipe read to stdin
		close(fd[0]);											// Close reading option for pipe
		execl("/usr/bin/tr","tr","/a-z/","/A-Z/",(char *)0);	// Execute tr /a-z/ /A-Z/ to stdin(pipe)
	}

	return 0;
}


