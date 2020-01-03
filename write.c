#include "semaphore.h"

int shmd, semd, fd;
struct sembuf sb;

int main(){
  sb.sem_num = 0;
  sb.sem_op = -1;
  semwriter();
  return 0;
}

int semwriter(){
  printf("trying to get in\n");
  semd = semget(SEMKEY, 1, 0);
  if(semd < 0){
    printf("Errno: %s\n", strerror(errno));
    return -1;
  }
  semop(semd, &sb, 1);
  semop(semd, &sb, 1);
  shmd = shmget(SHKEY, sizeof(char *), 0);
  if(shmd < 0){
    printf("Errno: %s\n", strerror(errno));
    return -1;
  }
  fd = open("trains.txt", O_WRONLY | O_APPEND);
  char *last = shmat(shmd, 0, 0);
  printf("Last addition: %s\n\n", last);
  char next[1000];
  printf("User addition: ");
  fgets(next, 1000, stdin);
  printf("\n");
  write(fd, next, strlen(next));
  close(fd);
  strcpy(last, next);
  shmdt(last);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}