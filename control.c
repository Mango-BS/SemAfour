#include "semaphore.h"

int semd, shmd, fd;
union semun s;
struct sembuf sb;

int semcreator(){
  semd = semget(SEMKEY, 1, IPC_CREAT | 0644);
  if (semd < 0){
    printf("Err no: %s\n", strerror(errno));
    return -1;
  }
  printf("created :)\n");

  semctl(semd, 0, SETVAL, s);
  shmd = shmget(SHKEY, sizeof(char *),IPC_CREAT | 0644);
  if (shmd < 0){
    printf("Err no: %s\n", strerror(errno));
    return -1;
  }
  printf("shared mem created\n");

  fd = open("trains.txt", O_CREAT | O_RDWR, 0644);
  if (fd < 0){
    printf("Err no: %s\n", strerror(errno));
    return -1;
  }
  printf("file was created\n");
  close(fd);
  return 0;
}


int semremover(){
  semd = semget(SEMKEY, 1, 0);
  if (semd < 0){
    printf("Err no: %s\n", strerror(errno));
    return -1;
  }
  printf("Trying to get in\n");
  semop(semd,&sb, 1);
  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0){
    printf("Err no: %s\n", strerror(errno));
    return -1;
  }
  semviewer();

  shmctl(shmd, IPC_RMID, 0);
  printf("Shared mem rm\n");
  remove("trains.txt");
  printf("File rm\n");
  semctl(semd, IPC_RMID, 0);
  printf("Semaphore rm\n");
  return 0;
}

int semviewer(){
  fd = open("trains.txt", O_RDONLY);
  if (fd < 0){
    printf("%s\n", strerror(errno));
    return -1;
  }
  char buff[1000];
  buff[0] = '\0';
  read(fd, buff, 1000);
  if (strlen(buff) != 0){
    *(strrchr(buff, '\n') + 1) = '\0';
  }
  printf("The story so far:\n");
  printf("%s\n", buff);
  close(fd);
  return 0;
}

int main(int argc, char* argv[]){
  sb.sem_num = 0;
  sb.sem_op = -1;
  s.val = 1;
  if (argc > 1){
    if (strcmp(argv[1], "-c") == 0){
      semcreator();
    }
    else if (strcmp(argv[1], "-r") == 0){
      semremover();
    }
    else if (strcmp(argv[1], "-v") == 0){
      semviewer();
    } else {
    printf("Invalid flag\n");
  }
  return 0;
}
}