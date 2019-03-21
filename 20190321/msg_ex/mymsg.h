#include <stdio.h>                                                          
#include <stdlib.h>
#include <string.h>//IPC
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 
#include <errno.h>
#include <unistd.h>


struct my_data {                                                                                 
long int msg_type;// default 값. 있어야 한다.
char name[20];
int height;
 int weight;
};

