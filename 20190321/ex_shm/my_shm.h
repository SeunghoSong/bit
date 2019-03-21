#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//IPC
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#define P_MAX 10
#define M_MAX 100
struct my_score {
char name[20];
int korean;
int math;
int english;
int sum;
};
