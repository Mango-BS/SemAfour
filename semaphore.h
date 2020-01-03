#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>

#define SEMKEY 24601
#define SHKEY 24602


union semun {
	int val;		
	struct semid_ds *buf;		
	unsigned short *array;	
};

int semcreator();
int semremover();
int semviewer();
int semwriter();