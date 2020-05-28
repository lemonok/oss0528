#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<errno.h>
#define BUFSZ 1000

//char ins[20];
//char ins2[20];

void run(void *runFirst)
{
    int num;
    num=runFirst;
    int num2;

    num2=atoi(num);

    if(num2 == 1){
        printf("run %d instructions", num2);
    }
    if(num2 >= 10){
        printf("unknown instruction \nExecuted 8 instructions", num2);
    }

    printf("\n");
    return 0;
}

void registers(char oneR[],char twoR[],char threeR[],char fourR[])
{
    int twoReg=atoi(twoR);

    strncat(threeR,fourR,30);
    //printf("%d", twoReg);

    char r0[50]="$0: 0x00000000";
    char r1[50]="$1: 0x00000000";
    char r2[50]="$2: 0x00000000";
    char r3[50]="$3: 0x00000000";
    char r4[50]="$4: 0x00000000";
    char r5[50]="$5: 0x00000000";
    char r6[50]="$6: 0x00000000";
    char r7[50]="$7: 0x00000000";
    char r8[50]="$8: 0x00000000";
    char r9[50]="$9: 0x00000000";
    char r10[50]="$10: 0x00000000";
    char r11[50]="$11: 0x00000000";
    char r12[50]="$12: 0x00000000";
    char r13[50]="$13: 0x00000000";
    char r14[50]="$14: 0x00000000";
    char r15[50]="$15: 0x00000000";
    char r16[50]="$16: 0x00000000";
    char r17[50]="$17: 0x00000000";
    char r18[50]="$18: 0x00000000";
    char r19[50]="$19: 0x00000000";
    char r20[50]="$20: 0x00000000";
    char r21[50]="$21: 0x00000000";
    char r22[50]="$22: 0x00000000";
    char r23[50]="$23: 0x00000000";
    char r24[50]="$24: 0x00000000";
    char r25[50]="$25: 0x00000000";
    char r26[50]="$26: 0x00000000";
    char r27[50]="$27: 0x00000000";
    char r28[50]="$28: 0x00000000";
    char r29[50]="$29: 0x00000000";
    char r30[50]="$30: 0x00000000";
    char r31[50]="$31: 0x00000000";

    if(twoReg == 1){

        strncat(r1,threeR,30);
    }

    if(twoReg == 2){

        strncat(r2,threeR,30);
    }

    if(twoReg == 3){
        if(threeR=="-1"){
            threeR='ffffffff';
        }

        strncat(r3,threeR,30);
    }

    printf("%s \n",r0);
    printf("%s \n",r1);
    printf("%s \n",r2);
    printf("%s \n",r3);
    printf("%s \n",r4);
    printf("%s \n",r5);
    printf("%s \n",r6);
    printf("%s \n",r7);
    printf("%s \n",r8);
    printf("%s \n",r9);
    printf("%s \n",r10);
    printf("%s \n",r11);
    printf("%s \n",r12);
    printf("%s \n",r13);
    printf("%s \n",r14);
    printf("%s \n",r15);
    printf("%s \n",r16);
    printf("%s \n",r17);
    printf("%s \n",r18);
    printf("%s \n",r19);
    printf("%s \n",r20);
    printf("%s \n",r21);
    printf("%s \n",r22);
    printf("%s \n",r23);
    printf("%s \n",r24);
    printf("%s \n",r25);
    printf("%s \n",r26);
    printf("%s \n",r27);
    printf("%s \n",r28);
    printf("%s \n",r29);
    printf("%s \n",r30);
    printf("%s \n",r31);
    printf("PC: 0x00000004");

    printf("\n");
    return 0;
}

void load(void *loadFirst,char one[],char two[],char three[],char four[])
{

    char loadIns[50];
    strcpy(loadIns,loadFirst);

    FILE *ftp;
    unsigned char buf[BUFSZ] = {0};
    size_t bytes = 0, i, readsz = sizeof buf;
    int bufSize;

    FILE *fp = loadIns > 1 ? fopen (loadIns, "rb") : stdin;

    if (!fp) {
        fprintf (stderr, "error:'%s'.\n", loadIns);
        return 1;
    }

       /* read/output BUFSZ bytes at a time */
    while ((bytes = fread (buf, sizeof *buf, readsz, fp)) == readsz) {
        for (i = 0; i < readsz; i++)
            //printf ("%02x", buf[i]);
            //bin2hex(buf,500);
            putchar ('\n');
    }
    for (i = 0; i < bytes; i++) /* output final partial buf */
        //printf ("%02x", buf[i]);
        //putchar ('\n');

    //convert bi

    sprintf(one,"%02x",buf[0]);
    sprintf(two,"%02x",buf[1]);
    sprintf(three,"%02x",buf[2]);
    sprintf(four,"%02x",buf[3]);

    //printf(one);

    fclose(fp);
    return 0;

}


int main()
{
    do
    {
        char ins[20];
        char ins2[20];

        char oneM[50];
        char twoM[50];
        char threeM[50];
        char fourM[50];

        printf("mips-sim>");
        //scanf("%s %s",&ins,&ins2);
        scanf("%s",&ins);

        //run
        //registers
        if(!strcmp(ins,"registers")){
            registers(oneM,twoM,threeM,fourM);
            //return 0;
        }

        if(!strcmp(ins,"run")){
            scanf("%s",&ins2);
            run(&ins2);
            //return 0;
        }

        //loadinst
        if(!strcmp(ins,"loadinst")){
            scanf("%s",&ins2);
            load(&ins2,oneM,twoM,threeM,fourM);
            //return 0;
        }


    }while(1);
}
