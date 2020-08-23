typedef struct {
  int input;
  int output;
  int pid;
} pfds;

pfds pipe_open(char *command);
