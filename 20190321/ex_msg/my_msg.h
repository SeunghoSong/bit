#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//IPC
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define P_MAX 10

struct my_score {
int long type ;
char name[20];
int korean;
int math;
int english;
int sum;
};
