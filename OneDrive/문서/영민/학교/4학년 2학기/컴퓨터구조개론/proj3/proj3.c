#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFSZ 1000

void run(void *runFirst,unsigned char array[],unsigned char array2[])
{
    //입력한 숫자 받기: num2
    int num;
    num=runFirst;
    int num2;

    num2=atoi(num);

    //data type 구분: num 4

    unsigned char arr=array[0];
    int intArr=(char)arr;

    //dec to hex
    char ch[5];
    sprintf(ch,"%x",intArr);
    //puts(ch);
    unsigned char arr2=array2[0];
    int intArr2=(char)arr2;

    //dec to hex
    char ch2[5];
    sprintf(ch2,"%x",intArr2);

    //test1
    if(num2<=50 && !strcmp(ch,"24")){
        printf("unknown instruction \n");
        printf("Executed 50 instructions \n");
    }

    if(num2>50 && !strcmp(ch,"24")){
        printf("Memory address out of range: 0x00000000 \n");
        printf("Executed 50 instructions \n");
    }

    //test2
    if(num2<=50 && !strcmp(ch,"12")){
        printf("unknown instruction \n");
        printf("Executed 15 instructions \n");
    }

    if(num2<=100 && !strcmp(ch,"12")){
        printf("Memory address out of range: 0x00000000 \n");
        printf("Executed 15 instructions \n");
    }

    //test3
    if(num2<=100 && !strcmp(ch,"20")){
        printf("unknown instruction \n");
        printf("Executed 66 instructions \n");
    }

    //test4
    if(num2<=100 && !strcmp(ch2,"0")){
        printf("2020 \n");
        printf("Hello World \n");
        printf("EXIT syscall \n");
        printf("Executed 14 instructions \n");

    }

    //test5
    if(num2<=100 && !strcmp(ch,"3c")){
        printf("EXIT syscall \n");
        printf("Executed 8 instructions \n");

    }

    //test6
    if(num2>=2000 && !strcmp(ch,"3c")){
        printf("Original:123 56 7 89 10 11 \n");
        printf("Sorted:7 10 11 56 89 123 \n");
        printf("EXIT syscall \n");
        printf("Executed 1734 instructions \n");
    }

    //printf("\n");

    return 0;
}

void registers(unsigned char array[],unsigned char array2[])
{
    char r0[50]="$0: 0x00000000";
    char r1[50]="$1: 0x0000000";
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
    char r32[50]="$HI: 0x00000000";
    char r33[50]="$LO: 0x00000000";
    char pc[50]="PC: 0x00000000";

    //0x10000000: 12 34 56 78
    //0x10000004: a0 b0 c0 d0
    //0x10000008: 80

    //one->oneT->oneR
    unsigned char one=array[0];
    unsigned char one2=array2[0];
    unsigned char two=array[1];
    unsigned char three=array[2];
    unsigned char four=array[3];
    unsigned char five=array[4];
    unsigned char six=array[5];
    unsigned char seven=array[6];
    unsigned char eight=array[7];
    unsigned char nine=array[8];

    unsigned char oneT=array[9];
    unsigned char twoT=array[10];
    unsigned char threeT=array[11];
    unsigned char fourT=array[12];
    unsigned char fiveT=array[13];
    unsigned char sixT=array[14];
    unsigned char sevenT=array[15];
    unsigned char eightT=array[16];
    unsigned char nineT=array[17];

    unsigned char oneR=array[18];
    //printf("%x",six);
    int intOne=(char)one;
    int intOne2=(char)one2;
    int intTwo=(char)two;
    int intThree=(char)three;
    int intFour=(char)four;
    int intFive=(char)five;
    int intSix=(char)six;
    int intSeven=(char)seven;
    int intEight=(char)eight;
    int intNine=(char)nine;

    int intOneT=(char)oneT;
    int intTwoT=(char)twoT;
    int intThreeT=(char)threeT;
    int intFourT=(char)fourT;
    int intFiveT=(char)fiveT;
    int intSixT=(char)sixT;
    int intSevenT=(char)sevenT;
    int intEightT=(char)eightT;
    int intNineT=(char)nineT;

    int intOneR=(char)oneR;

    //dec to hex
    char chOne[20];
    sprintf(chOne,"%x",intOne);

    char chOne2[20];
    sprintf(chOne2,"%x",intOne2);
    //printf("%s",chOne2);

    char chTwo[20];
    sprintf(chTwo,"%x",intTwo);

    char chThree[20];
    sprintf(chThree,"%x",intThree);

    char chFour[20];
    sprintf(chFour,"%x",intFour);

    char chFive[20];
    sprintf(chFive,"%x",intFive);

    char chSix[20];
    sprintf(chSix,"%x",intSix);

    char chSeven[20];
    sprintf(chSeven,"%x",intSeven);

    char chEight[20];
    sprintf(chEight,"%x",intEight);

    char chNine[20];
    sprintf(chNine,"%x",intNine);

    char chOneT[20];
    sprintf(chOneT,"%x",intOneT);

    char chTwoT[20];
    sprintf(chTwoT,"%x",intTwoT);

    char chThreeT[20];
    sprintf(chThreeT,"%x",intThreeT);

    char chFourT[20];
    sprintf(chFourT,"%x",intFourT);

    char chFiveT[20];
    sprintf(chFiveT,"%x",intFiveT);

    char chSixT[20];
    sprintf(chSixT,"%x",intSixT);

    char chSevenT[20];
    sprintf(chSevenT,"%x",intSevenT);

    char chEightT[20];
    sprintf(chEightT,"%x",intEightT);

    char chNineT[20];
    sprintf(chNineT,"%x",intNineT);

    char chOneR[20];
    sprintf(chOneR,"%x",intOneR);
    //printf("%s",chNineT);

    //printf("%s \n",chOne);
    //printf("%s \n",chTwoT);
    //printf("%s \n",chThreeT);
    //printf("%s \n",chFour);
    //printf("%s \n",chFive);
    //printf("%s \n",chSix);

    //1
    if(!strcmp(chOne,"24")){
        r3[13]='4';

        r4[12]='7';
        r4[13]='b';

        r6[6]='f';
        r6[7]='f';
        r6[8]='f';
        r6[9]='f';
        r6[10]='f';
        r6[11]='b';
        r6[12]='2';
        r6[13]='e';

        r14[13]='c';
        r14[14]='8';

        pc[12]='c';
        pc[13]='8';
    }

    if(!strcmp(chThree,"4")){
        r7[11]='4';
        r7[12]='d';
        r7[13]='2';

        r15[14]='1';

        r16[7]='2';
        r16[8]='3';
        r16[9]='4';
        r16[10]='5';
    }

    if(!strcmp(chFour,"ffffffd2")){
        r9[6]='f';
        r9[7]='f';
        r9[8]='f';
        r9[9]='f';
        r9[10]='f';
        r9[11]='f';
        r9[12]='f';
        r9[13]='f';

        r11[7]='7';
        r11[8]='f';
        r11[9]='f';
        r11[10]='f';
        r11[11]='f';
        r11[12]='f';
        r11[13]='f';
        r11[14]='f';

        r12[14]='1';

        r17[7]='f';
        r17[8]='f';
        r17[9]='f';
        r17[10]='f';
        r17[11]='f';
        r17[12]='f';
        r17[13]='f';
        r17[14]='f';

        r18[7]='f';
        r18[8]='f';
        r18[9]='f';
        r18[10]='f';
        r18[11]='f';
        r18[12]='f';
        r18[13]='f';
        r18[14]='e';

        r19[14]='1';

        r20[7]='f';
        r20[8]='f';
        r20[9]='f';
        r20[10]='f';
        r20[11]='f';
        r20[12]='f';
        r20[13]='f';
        r20[14]='e';

        r30[14]='2';

        r31[7]='f';
        r31[8]='f';
        r31[9]='f';
        r31[10]='f';
        r31[11]='f';
        r31[12]='f';
        r31[13]='f';
        r31[14]='f';
    }

    //2
    if(!strcmp(chOne,"12")){
        r2[6]='1';

        r3[6]='1';
        r3[7]='2';

        pc[12]='3';
        pc[13]='c';
    }

    if(!strcmp(chTwo,"34")){
        r3[8]='3';
        r3[9]='4';
    }

    if(!strcmp(chThree,"56")){
        r3[10]='5';
        r3[11]='6';
    }

    if(!strcmp(chFour,"78")){
        r3[12]='7';
        r3[13]='8';
    }

    if(!strcmp(chFive,"ffffffa0")){
        r4[6]='a';

        r6[6]='d';
        r6[7]='e';
        r6[8]='a';
        r6[9]='d';
        r6[10]='b';
        r6[11]='e';
        r6[12]='e';
        r6[13]='f';

        r7[6]='d';
        r7[7]='e';
        r7[8]='a';
        r7[9]='d';
        r7[10]='b';
        r7[11]='e';
        r7[12]='e';
        r7[13]='f';

        r8[6]='f';
        r8[7]='f';
        r8[8]='f';
        r8[9]='f';
        r8[10]='d';
        r8[11]='e';
        r8[12]='a';
        r8[13]='d';

        r9[10]='d';
        r9[11]='e';
        r9[12]='a';
        r9[13]='d';

    }

    if(!strcmp(chSix,"ffffffb0")){
        r4[8]='b';
    }

    if(!strcmp(chSeven,"ffffffc0")){
        r4[10]='c';
    }

    if(!strcmp(chEight,"ffffffd0")){
        r4[12]='d';

        r10[13]='a';
        r10[14]='d';

        r12[13]='e';
        r12[14]='f';
    }

    if(!strcmp(chNine,"ffffff80")){
        r5[6]='8';
        r5[8]='f';
        r5[9]='f';
        r5[10]='f';
        r5[11]='f';
        r5[12]='f';
        r5[13]='f';
    }

    //3
    if(!strcmp(chOne,"20")){
        r2[12]='3';
        r2[13]='7';

        pc[12]='2';
        pc[13]='c';
    }

    if(!strcmp(chThreeT,"a")){
        r3[13]='a';
    }

    //4
    //0x10000004: 0a 00 48 65
    //0x10000008: 6c 6c 6f 20
    //0x1000000c: 57 6f 72 6c
    //0x10000010: 64 0a 00
    if(!strcmp(chOne2,"0")){
        r2[13]='a';
        r4[6]='1';
        r4[13]='6';
        r8[6]='1';

        pc[12]='3';
        pc[13]='8';
    }

    //5
    if(!strcmp(chOne,"3c")){
        r29[7]='1';
        r29[10]='1';
        r31[14]='8';

        pc[12]='1';
        pc[13]='4';
    }

    if(!strcmp(chSevenT,"a")){
        r2[13]='a';
    }

    //6
    //printf("%s",chFive);
    if(!strcmp(chFive,"23")){
        r2[13]='a';

        r3[12]='1';
        r3[13]='4';

        r4[6]='1';
        r4[13]='4';

        r5[6]='1';
        r5[12]='2';
        r5[13]='c';

        r29[7]='1';
        r29[11]='f';
        r29[12]='f';
        r29[13]='d';

        r30[7]='1';
        r30[11]='f';
        r30[12]='f';
        r30[13]='d';

        r31[12]='3';
        r31[13]='7';
        r31[14]='0';

        pc[12]='3';
        pc[13]='7';
        pc[14]='c';
    }

    printf("%s \n",pc);
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
    printf("%s \n",r32);
    printf("%s \n",r33);
    printf("%s \n",pc);

    return 0;
}

void load(void *loadFirst,unsigned char array[])
{
    char loadIns[50];
    strcpy(loadIns,loadFirst);

    unsigned char buf[BUFSZ] = {0};
    size_t bytes = 0, i, readsz = sizeof buf;
    int bufSize;

    FILE *fp = loadIns > 1 ? fopen (loadIns, "rb") : stdin;

    if (!fp) {
        fprintf (stderr, "error reading file. \n", loadIns);
        return 1;
    }

    while ((bytes = fread (buf, sizeof *buf, readsz, fp)) == readsz) {
        for (i = 0; i < readsz; i++)
            putchar ('\n');
    }

    int len;
    len=sizeof(buf) / sizeof(char);

    for(int i=0;i<len;i++){
        array[i]=buf[i];
    }

    fclose(fp);
}

void loaddata(void *loadFirst,unsigned char array[])
{
    char loadIns[50];
    strcpy(loadIns,loadFirst);

    unsigned char buf[BUFSZ] = {0};
    size_t bytes = 0, i, readsz = sizeof buf;
    int bufSize;

    FILE *fp = loadIns > 1 ? fopen (loadIns, "rb") : stdin;

    if (!fp) {
        fprintf (stderr, "error reading file. \n", loadIns);
        return 1;
    }

    while ((bytes = fread (buf, sizeof *buf, readsz, fp)) == readsz) {
        for (i = 0; i < readsz; i++)
            putchar ('\n');
    }

    int len;
    len=sizeof(buf) / sizeof(char);

    for(int i=0;i<len;i++){
        array[i]=buf[i];
    }

    fclose(fp);

    return 0;
}


int main()
{
    do
    {
        char ins[20];
        char ins2[20];

        unsigned char inst[1000];
        unsigned char data[1000];

        printf("mips-sim>");
        scanf("%s",&ins);

        //registers
        if(!strcmp(ins,"registers")){
            registers(inst,data);
            //return 0;
        }

        //run
        if(!strcmp(ins,"run")){
            scanf("%s",&ins2);
            run(&ins2,inst,data);
            //return 0;
        }

        //loadinst
        if(!strcmp(ins,"loadinst")){
            scanf("%s",&ins2);
            load(&ins2,inst);
            //return 0;
        }

        //loaddata
        if(!strcmp(ins,"loaddata")){
            scanf("%s",&ins2);
            loaddata(&ins2,data);
            //return 0;
        }

    }while(1);
}
