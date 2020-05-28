#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <malloc.h>
#define CALLOC(num, type) ((int*)calloc (num, sizeof(int)))
#define FREE(addr) (free((void*) (addr)))
#define MAX 100
#define MAX_PROCESS 100

//SJF Header
struct process {
    int pid;
    int option;
    int burst_time;
    int waiting_time;
	int turnaround_time;
};

typedef struct process Process;

double average_waiting_time;
int total_waiting_time;

void sort_process_by_burst_time(Process *pro, int n);
void calculate_waiting_time(Process *pro, int n);
void print_gantt_chart(Process *pro, int n);

void sort_process_by_burst_time(Process *pro, int n)
{
    int i, j;
    Process temp;
    for(i=0; i<n-1; i++) {
        for(j=0; j<n-1-i; j++) {
            if(pro[j].burst_time > pro[j+1].burst_time) {
                temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}

void calculate_waiting_time(Process *pro, int n)
{
    int i;
    total_waiting_time = 0;
    pro[0].waiting_time = 0;
    for(i=1; i<n; i++) {
        pro[i].waiting_time = pro[i-1].waiting_time + pro[i-1].burst_time;
        total_waiting_time += pro[i].waiting_time;
    }
}

void print_gantt_chart(Process *pro, int n)
{
    int i, j;
    int last = pro[n-1].burst_time + ( n== 1 ? 0 : pro[n-1].waiting_time);
    // printing top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    // middle position
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time-1; j++) printf(" ");
        printf("p%d", pro[i].pid);
        for(j=0; j<pro[i].burst_time-1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing waiting time
    int minus = 0;
    for(i=0; i<n; i++) {
        if(pro[i].waiting_time>9) printf(" ");
        printf("%d", pro[i].waiting_time);
        if(pro[i+1].waiting_time>9){
          minus = 1;
        }
        if(i+1 == n )  if (last>9) minus = 1;
        for(j=0; j<pro[i].burst_time-minus; j++) printf("  ");

    }
    if(last>9) printf(" ");
    printf("%d\n", last);

}
//End SJF Header

//FCFS Header
typedef struct
{
    int pid;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Process1;

void print_table1(Process1 *pro, int n);

void print_gantt_chart1(Process1 *pro, int n);

void print_gantt_chart1(Process1 *pro, int n)
{
    int i, j;
    // print top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time - 1; j++) printf(" ");
        printf("P%d", pro[i].pid);
        for(j=0; j<pro[i].burst_time - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing the time line
    printf("0");
    for(i=0; i<n; i++) {
        for(j=0; j<pro[i].burst_time; j++) printf("  ");
        if(pro[i].turnaround_time > 9) printf("\b"); // backspace : remove 1 space
        printf("%d", pro[i].turnaround_time);

    }
    printf("\n");

}

//End Of FCFS Header

//Priority Header
struct process2
{
      char process_name;
      int arrival_time, burst_time, ct, waiting_time, turnaround_time, priority;
      int status;
}process2_queue[10];

int limit;

void Arrival_Time_Sorting()
{
      struct process2 temp;
      int i, j;
      for(i = 0; i < limit - 1; i++)
      {
            for(j = i + 1; j < limit; j++)
            {
                  if(process2_queue[i].arrival_time > process2_queue[j].arrival_time)
                  {
                        temp = process2_queue[i];
                        process2_queue[i] = process2_queue[j];
                        process2_queue[j] = temp;
                  }
            }
      }
}
//End of Priority

//start of None-Preemptive Header
typedef struct Job {
  unsigned int id;
  unsigned int arrivalTime5;
  unsigned int priority;
  unsigned int burstTime5;
  unsigned int remainingTime5;
  bool done;

} Job;



unsigned int time_width = 3;

int jobsN /*= 5*/;

Job jobs[100]/*= {
  {0, 8, 3, 8, 8, false},
  {1, 5, 1, 5, 5, false},
  {2, 0, 4, 12, 12, false},
  {3, 3, 5, 9, 9, false},
  {4, 9, 2, 7, 7, false}
}*/;

int total_burst_time5 = 0;

void sortJobsByArrivalTime() {
  int i, j;
  Job tmp5;
  for (i = 1; i < jobsN; i++) {
    tmp5 = jobs[i];
    for (j = i; j > 0 && jobs[j-1].arrivalTime5 <= tmp5.arrivalTime5; j--) {
      jobs[j] = jobs[j-1];
    }
    jobs[j] = tmp5;
  }
}

void printGCStart() {
  unsigned int i;
  printf(" --");
  for (i = 1; i < total_burst_time5 ; i++) {
    printf("---");
  }
  printf("\n|");
}

void printGCEnd() {
  unsigned int i;
  printf("\n");
  for (i = 0; i < total_burst_time5 ; i++) {
    printf("|--");
  }
  printf("|\n");
  for (i = 0; i <= total_burst_time5 ; i++) {
    printf("%-*u", time_width, i);
  }
  printf("\n\n");
}

void scheduleFCFS() {
  unsigned int i, x, time = 0;
  float y,
        avg_waiting_time5 = 0.0,
        avg_turnaround_time5 = 0.0;
  printf("First Come First Serve Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    x = jobs[i].burstTime5 * time_width;
    printf("%*u", x - (x/2), jobs[i].id);
    printf("%*c", x/2, '|' );
    y = (float)(time - jobs[i].arrivalTime5) / jobsN;
    avg_waiting_time5 += y;
    avg_turnaround_time5 += y + (jobs[i].burstTime5 / jobsN);
    time += jobs[i].burstTime5;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time5);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time5);
}

void scheduleSJF() {
  unsigned int time = 0, i, j, x, next = 0, nextMin;
  float y,
        avg_waiting_time5 = 0.0,
        avg_turnaround_time5 = 0.0;
  printf("Shortest Job First Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].done = false;
  }
  for (i = 0; i < jobsN; i++) {
    nextMin = INT_MAX;
    for (j = 0; j < jobsN && jobs[j].arrivalTime5 <= time; j++) {
      if(!jobs[j].done && jobs[j].burstTime5 < nextMin) {
        next = j;
        nextMin = jobs[j].burstTime5;
      }
    }
    x = jobs[next].burstTime5 * time_width;
    printf("%*u", x - (x/2), jobs[next].id);
    printf("%*c", x/2, '|' );
    jobs[next].done = true;
    y = (float)(time - jobs[next].arrivalTime5) / jobsN;
    avg_waiting_time5 += y;
    avg_turnaround_time5 += y + (jobs[next].burstTime5 / jobsN);
    time += jobs[next].burstTime5;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time5);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time5);
}

void schedulePriority() {
  unsigned int time = 0, i, j, x, next = 0, nextMin;
  float y,
        avg_waiting_time5 = 0.0,
        avg_turnaround_time5 = 0.0;
  printf("Priority Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].done = false;
  }
  for (i = 0; i < jobsN; i++) {
    nextMin = INT_MAX;
    for (j = 0; jobs[j].arrivalTime5 <= time && j < jobsN; j++) {
      if(!jobs[j].done && jobs[j].priority < nextMin) {
        next = j;
        nextMin = jobs[next].priority;
      }
    }
    x = jobs[next].burstTime5 * time_width;
    printf("%*u", x - (x/2), jobs[next].id);
    printf("%*c", x/2, '|' );
    jobs[next].done = true;
    y = (float)(time - jobs[next].arrivalTime5) / jobsN;
    avg_waiting_time5 += y;
    avg_turnaround_time5 += y + (jobs[next].burstTime5 / jobsN);
    time += jobs[next].burstTime5;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time5);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time5);
}

void scheduleRR (unsigned int tq) {
  unsigned int time = 0, i, x, z, next = 0;
  float y,
        avg_waiting_time5 = 0.0,
        avg_turnaround_time5 = 0.0;
  printf("Round Robin Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].remainingTime5 = jobs[i].burstTime5;
  }
  i = 0;
  while(time < total_burst_time5) {
    if (jobs[i].arrivalTime5 <= time) {
      if (jobs[i].remainingTime5) {
        z = (tq < jobs[i].remainingTime5) ?tq :jobs[i].remainingTime5;
        x = z * time_width;
        printf("%*u", x - (x/2), jobs[i].id);
        printf("%*c", x/2, '|' );
        jobs[i].done = true;
        y = (float)(time - jobs[i].arrivalTime5) / jobsN;
        avg_waiting_time5 += y;
        avg_turnaround_time5 += y + (jobs[i].burstTime5 / jobsN);
        time += z;
        jobs[i].remainingTime5 -= z;
      }
      i = (i + 1) % jobsN;
    } else {
      i = 0;

    }
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time5);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time5);
}

void scheduleSRTF() {
  int time = 0, i, j, x, next, pre = -1, preStart = 0, nextMin;
  float y,
        avg_waiting_time5 = 0.0,
        avg_turnaround_time5 = 0.0;
  printf("Shortest Remaining Time First Scheduling:\n");
  printGCStart();
  for (i = 0; i < jobsN; i++) {
    jobs[i].remainingTime5 = jobs[i].burstTime5;
  }
  i = 0;
  while(time <= total_burst_time5) {
    next = -1;
    nextMin = INT_MAX;
    for (j = 0; jobs[j].arrivalTime5 <= time && j < jobsN; j++) {
      i = j;
      if(jobs[j].remainingTime5 && jobs[j].remainingTime5 < nextMin) {
        next = j;
        nextMin = jobs[j].remainingTime5;
        if (pre == -1) {
          pre = next;
        }
      }
    }
    i++;
    if(next != pre || time == total_burst_time5) {
      x = (time - preStart);
      y = (float)(preStart - jobs[pre].arrivalTime5) / jobsN;
      avg_waiting_time5 += y;
      avg_turnaround_time5 += y + (x / jobsN);
      x *= time_width;
      printf("%*u", x - (x/2), jobs[pre].id);
      printf("%*c", x/2, '|' );
      jobs[pre].done = true;
      pre = next;
      preStart = time;
      if (time == total_burst_time5) {
        break;
      }
    }
    x = (i < jobsN) ?jobs[i].arrivalTime5 :total_burst_time5;
    x = x - time;
    x = (x < jobs[next].remainingTime5) ?x :jobs[next].remainingTime5;
    jobs[next].remainingTime5 -= x;
    time += x;
  }
  printGCEnd();
  printf("Average Waiting Time: %f\n", avg_waiting_time5);
  printf("Average Turnaround Time: %f\n", avg_turnaround_time5);
}
//End of none-Preemptive Header/////////////////////////////////////////////////////////////////////////////
int main()
{
    Process pro[MAX_PROCESS];
    int n, i, j;
    puts("===============================================================================");
    puts("CPU SCHEDULING");
    puts("===============================================================================");
	puts("|Options: 1:SJFN | 2:FCFS | 3:Priority | 4:RounRobin | 5:None-Preemptive|6:Exit|");
	puts("===============================================================================\n");

	int option;
	printf("Enter option: ");
    scanf("%d", &option);

	switch(option)
	{
	case 1:
		{
    printf("Enter total process: ");
    scanf("%d", &n);
    printf("Enter burst time for each process:\n");
    for(i=0; i<n; i++) {
        printf("P[%d]: ", i+1);
        scanf("%d", &pro[i].burst_time);
        pro[i].pid = i+1;
    }

    sort_process_by_burst_time(pro, n);
    calculate_waiting_time(pro, n);

    average_waiting_time = (double) ( (double)total_waiting_time / (double) n );

    puts("");
    printf("Average Waiting Time: %.2lf\n",average_waiting_time);

    printf("Gantt Chart:\n");
    print_gantt_chart(pro, n);
	printf("Burst Time:\t%.2lf\n",pro[n].burst_time);
	printf("waiting Time:\t%.2lf\n",total_waiting_time);
		}
	break;

	case 2:
		{
		    Process1 p[MAX];
    int i, j, n;
    int sum_waiting_time=0, sum_turnaround_time=0;
    printf("Enter total number of process: ");
    scanf("%d", &n);
    printf("Enter burst time for each process:\n");
    for(i=0; i<n; i++) {
        pro[i].pid = i+1;
        printf("P[%d] : ", i+1);
        scanf("%d", &pro[i].burst_time);
        pro[i].waiting_time = pro[i].turnaround_time = 0;
    }

    // calculate waiting time and turnaround time
    pro[0].turnaround_time = pro[0].burst_time;

    for(i=1; i<n; i++) {
        pro[i].waiting_time = pro[i-1].waiting_time + pro[i-1].burst_time;
        pro[i].turnaround_time = pro[i].waiting_time + pro[i].burst_time;
    }

    // calculate sum of waiting time and sum of turnaround time
    for(i=0; i<n; i++) {
            sum_waiting_time += pro[i].waiting_time;
            sum_turnaround_time += pro[i].turnaround_time;
    }

    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);

    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
	print_gantt_chart1(pro, n);
		}
   break;
   case 3:
	   {
   	int time = 0, burst_time = 0, largest;
      char c;
      float wait_time = 0, turnaround_time = 0, average_waiting_time, average_turnaround_time;
      printf("\nEnter Total Number of Processes:\t");
      scanf("%d", &limit);
      for(i = 0, c = 'A'; i < limit; i++, c++)
      {
            process2_queue[i].process_name = c;
            printf("\nEnter Details For Process[%C]:\n", process2_queue[i].process_name);
            printf("Enter Arrival Time:\t");
            scanf("%d", &process2_queue[i].arrival_time );
            printf("Enter Burst Time:\t");
            scanf("%d", &process2_queue[i].burst_time);
            printf("Enter Priority:\t");
            scanf("%d", &process2_queue[i].priority);
            process2_queue[i].status = 0;
            burst_time = burst_time + process2_queue[i].burst_time;
      }
      Arrival_Time_Sorting();
      process2_queue[9].priority = -9999;
      printf("\nProcess Name\tArrival Time\tBurst Time\tPriority\tWaiting Time");
      for(time = process2_queue[0].arrival_time; time < burst_time;)
      {
            largest = 9;
            for(i = 0; i < limit; i++)
            {
                  if(process2_queue[i].arrival_time <= time && process2_queue[i].status != 1 && process2_queue[i].priority > process2_queue[largest].priority)
                  {
                        largest = i;
                  }
            }
            time = time + process2_queue[largest].burst_time;
            process2_queue[largest].ct = time;
            process2_queue[largest].waiting_time = process2_queue[largest].ct - process2_queue[largest].arrival_time - process2_queue[largest].burst_time;
            process2_queue[largest].turnaround_time = process2_queue[largest].ct - process2_queue[largest].arrival_time;
            process2_queue[largest].status = 1;
            wait_time = wait_time + process2_queue[largest].waiting_time;
            turnaround_time = turnaround_time + process2_queue[largest].turnaround_time;
            printf("\n%c\t\t%d\t\t%d\t\t%d\t\t%d", process2_queue[largest].process_name, process2_queue[largest].arrival_time, process2_queue[largest].burst_time, process2_queue[largest].priority, process2_queue[largest].waiting_time);
      }
      average_waiting_time = wait_time / limit;
      average_turnaround_time = turnaround_time / limit;
      printf("\n\nAverage waiting time:\t%f\n", average_waiting_time);
      printf("Average Turnaround Time:\t%f\n", average_turnaround_time);
	   }

      break;
   case 4:
	   {
		    int i, limit, total = 0, x, counter = 0, time_quantum;
      int wait_time = 0, turnaround_time = 0, arrival_time[10], burst_time[10], temp[10];
      float average_wait_time, average_turnaround_time;
      printf("\nEnter Total Number of Processes:\t");
      scanf("%d", &limit);
      x = limit;
      for(i = 0; i < limit; i++)
      {
            printf("\nEnter Details of Process[%d]\n", i + 1);
            printf("Arrival Time:\t");
            scanf("%d", &arrival_time[i]);
            printf("Burst Time:\t");
            scanf("%d", &burst_time[i]);
            temp[i] = burst_time[i];
      }
      printf("\nEnter Time Quantum:\t");
      scanf("%d", &time_quantum);
      printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");
      for(total = 0, i = 0; x != 0;)
      {
            if(temp[i] <= time_quantum && temp[i] > 0)
            {
                  total = total + temp[i];
                  temp[i] = 0;
                  counter = 1;
            }
            else if(temp[i] > 0)
            {
                  temp[i] = temp[i] - time_quantum;
                  total = total + time_quantum;
            }
            if(temp[i] == 0 && counter == 1)
            {
                  x--;
                  printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", i + 1, burst_time[i], total - arrival_time[i], total - arrival_time[i] - burst_time[i]);
                  wait_time = wait_time + total - arrival_time[i] - burst_time[i];
                  turnaround_time = turnaround_time + total - arrival_time[i];
                  counter = 0;
            }
            if(i == limit - 1)
            {
                  i = 0;
            }
            else if(arrival_time[i + 1] <= total)
            {
                  i++;
            }
            else
            {
                  i = 0;
            }
      }
      average_wait_time = wait_time * 1.0 / limit;
      average_turnaround_time = turnaround_time * 1.0 / limit;
      printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
      printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time);
	   }
	   break;
	   case 5:
	   	{
	   		int i, tq = 3;
  unsigned int ch=0;
  printf("Enter the number of jobs: ");
  scanf("%u", &jobsN);
  printf("\nEnter the details of jobs: ");
  for (i = 0; i < jobsN; i++) {
    printf("Job %u:\n", i+1);
    printf("  ID: ");
    scanf("%u", &jobs[i].id);
    printf("  Arrival Time: ");
    scanf("%u", &jobs[i].arrivalTime5);
    printf("  Priority: ");
    scanf("%u", &jobs[i].priority);
    printf("  Burst Time: ");
    scanf("%u", &jobs[i].burstTime5);
    jobs[i].remainingTime5 = jobs[i].burstTime5;
    jobs[i].done = false;
    total_burst_time5 += jobs[i].burstTime5;
  }
  sortJobsByArrivalTime();
///////////////////////////////////////////////////////////////////////////////////////
  ch = true;
  while(ch) {
    printf("Select a Scheduling:\n");
    printf("1: First Come First Serve\n");
    printf("2: Shortest Job First\n");
    printf("3: Priority Scheduling\n");
    printf("4: Round Robin\n");
    printf("5: Shortest Remaining Time First\n");
    printf("6: stop\n");
    printf("\nEnter your choice: ");
    fflush(stdin);
    scanf("%u", &ch);
    printf("\n");
    if(ch==1)
      {
      	scheduleFCFS();
	  }
      else if(ch==2)
      {
      	scheduleSJF();
	  }
      else if(ch==3)
      {
      	schedulePriority();
	  }
      else if(ch==4)
      {
        printf("Enter the value of time quantum: \n");
        scanf("%u", &tq);
        scheduleRR(tq);
	  }
      else if(ch==5)
      {
      	scheduleSRTF();
	  }
      else if(ch==6)
      {
      	printf("Thank You...\n");
      	break;
	  }
      else
      	{

        printf("Invalid Choice!\n");
        }
	}
    printf("\n\n");

		   }
		   break;
		   case 6:
		   	{
		   		goto ext;
			   }
   default:
	   {
		   printf("Your Have Entered an Invalid Letter\n");
	   }

	}
	ext:
    return 0;

}
