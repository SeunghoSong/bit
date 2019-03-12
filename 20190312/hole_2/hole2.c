#include <stdio.h>
#include <string.h>
int main(void){
char buffer [75]={0,};
FILE* file=fopen("tesla.txt","r+");
fseek(file,33,SEEK_SET);
fread(buffer,75,1,file);
fseek(file,34,SEEK_SET);
fwrite("vehicles",8,1,file);
fwrite(buffer,75,1,file);

fclose(file);
return 0;
}


