#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
//extern char **environ; // _GNU_SOURCE defines this

typedef struct {
  int input;
  int output;
  int pid;
} pfds;

pfds pipe_open(char *cmd[]) {

  int input_fds[2];
  int output_fds[2];

  if (pipe(input_fds)!=0) {
    perror("Pipe failed");
    exit(-1);
  }

  if (pipe(output_fds)!=0) {
    perror("Pipe failed");
    exit(-1);
  }

  // pipe side
  int pid=fork();
  if (pid==0) {


    printf("Starting pipe: %s\n", cmd[0]);
    close(0);
    close(1);
    close(2);

    dup(input_fds[0]); // stdin
    dup(output_fds[1]); // stdout
    dup(output_fds[1]); // stderr

    close(input_fds[1]);          /* Close unused write end */
    close(output_fds[0]);          /* Close unused read end */

    execve(cmd[0], cmd, environ);

  }

  close(input_fds[0]);
  close(output_fds[1]);

  pfds control;
  control.input = input_fds[1];
  control.output = output_fds[0];
  control.pid = pid;

  return control;

}

int main4(int argc, char *argv[]) {

  char *cmd[] = { "/bin/sleep", "10000", '\0' };

  pfds c = pipe_open( cmd );

  char buf[1024];
  while (read(c.input, buf, 1024)>0) {
    printf("%s\n", buf);
  }

  
}

int main2(int argc, char *argv[]) {

  char *cmd[] = { "/bin/echo", "hello", '\0' };

  pfds c = pipe_open( cmd );

  char buf[1024];

  while (read(c.output, buf, 1024)>0) {
    printf("%s\n", buf);
  }

}


int main(int argc, char *argv[]) {

  char *cmd[] = { "/bin/cat", '\0' };
  pfds c = pipe_open( cmd );

  char *string = "Hello, World\n";
  write(c.input, string, strlen(string));
  close(c.input);

  char buf[1024];
  int count = 1;
  while (count > 0) {
    count = read(c.output, buf, 10);
    write(1, buf, count);
  }

  wait();

}
