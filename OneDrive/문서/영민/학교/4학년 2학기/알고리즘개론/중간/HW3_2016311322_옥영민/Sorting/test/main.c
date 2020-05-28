#include "ds.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

# ifdef sun
# define const
# endif

extern const char *
okfail(int val)
{
    return val ? "[ OK ]" : "[FAIL]";
}

//prototype
void read_input(char *, char *);
void convert_PARRAY(char *, PARRAY);
void convert_STACK(char *, STACK);
void convert_QUEUE(char *, QUEUE);
void convert_HEAP(char *, HEAP);

//sorting function


HEAP Heap_h(HEAP);

int main(int argc, char *argv[])
{
	printf("Algorithm - Homework # 2 - <2016311322>\n");

	char *filename, *buf;
	int n, m;
	//int x,y;

	for (n=0; n<1000001; n+=200000)
		for (m=0; m<10; m++)
		{
			char *c="_";
			char *d=".txt";
			char *file="input_";

			//read the file input_x_y.txt into a buffer
			asprintf(&filename,"%s%d%s%d%s",file,n,c,m,d);
			//printf("%s\n",filename);
			buf="r";

			read_input(filename, buf);
			//printf("%s\n",filename);

			//free(filename);

			//convert the buffer to data structure

			//get the current time (time.h)

			//sort the data with sorting functions

			//get the current time (time.h)

		}
	return 0;
}

void read_input(char *filename, char *buf)
{
	FILE *ptr_file;
	ptr_file = fopen(filename,buf);

	if (ptr_file==NULL)
	{
		printf("File is not exist.\n");
		//exit(1);
	}
	if(ptr_file!=NULL)
	{
		printf("%s\n",filename);
	}

    return 0;
}

void convert_PARRAY(char *buf, PARRAY parray)
{

}

void convert_STACK(char *buf, STACK stack)
{

}

void convert_QUEUE(char *buf, QUEUE queue)
{

}

void convert_HEAP(char *buf, HEAP heap)
{

}

