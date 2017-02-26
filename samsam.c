#include <pthread.h> 
#include <stdio.h> 
#define BSIZE 4 
#define NUMITEMS 200
#include <unistd.h>
#include<time.h>
#include<string.h>
#include<sys/timeb.h>



typedef struct { 
  int buf[BSIZE]; 
 int buff[BSIZE];
//double avgwaiting;
//time_t bufftime[BSIZE];
//int bufftime[BSIZE];
int jobtakerfinish;
int cpu1,cpu2,cpu3;
int starttimetotal;
int endttimetotal;
  int occupied; 
  int nextin, nextout; 
  pthread_mutex_t mutex; 
  pthread_cond_t more; 
  pthread_cond_t less; 
} buffer_t; 
buffer_t buffer; 


void * jobtaker(void *); 
void * cpuone(void *); 
void * cputwo(void *);
void * cputhree(void *);
void * displayt(void *);

#define NUM_THREADS 5

pthread_t tid[NUM_THREADS];      /* array of thread IDs */ 

main( int argc, char *argv[] )  
{ 
//--open file to set jobs
int a;
FILE *fp;
fp=fopen("job_file.txt","w");
if(fp == NULL)
{
//error opening file
}
//create job list
time_t t;
srand((unsigned)time(&t));

for(a=0;a<200;a++)
{
fprintf(fp,"%d %d \n",a,rand()%51);
}
fprintf(fp,"Finish");
fclose(fp);


//--

FILE *fpp;
fpp=fopen("simulation_log.txt","w");
fprintf(fpp,"simulation Log\n");
fclose(fpp);
//--closed file

buffer.starttimetotal=0;
buffer.endttimetotal=0;


  int i; 
  pthread_cond_init(&(buffer.more), NULL); 
  pthread_cond_init(&(buffer.less), NULL); 

  pthread_create(&tid[3],NULL,cputhree,NULL);
  pthread_create(&tid[2],NULL,cputwo,NULL);
  pthread_create(&tid[1], NULL,cpuone, NULL); 
  pthread_create(&tid[0], NULL,jobtaker, NULL); 

  pthread_create(&tid[0], NULL,displayt, NULL); 
  for ( i = 0; i < NUM_THREADS; i++) 
    pthread_join(tid[i], NULL); 

}  /* main */ 




void * jobtaker(void * parm) 
{
/*
time_t rawtime;
struct tm * timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
printf("%s",asctime(timeinfo));
*/
//---read
int getfile[200];
FILE *fp;
int b,c;
fp=fopen("job_file.txt","r");
if(fp == NULL){
//error opening 
}

while(fscanf(fp,"%d %d\n",&b,&c)==2)
{

printf("%d %d\n",b,c);
getfile[b]=c;
}
printf("these are the jobs we are going to test in here \n");

fclose(fp);


//---read end
 
// char item[NUMITEMS]="1111222233334444555566667770"; 
  int i; 
      printf("jobtaker started--------------------.\n"); 
  for(i=0;i<NUMITEMS;i++) 
    {  
       
//printf("-%d \n",getfile[i]);

  // if (i == 10) break;  
/* Quit if at end of string. */ 
      pthread_mutex_lock(&(buffer.mutex)); 
      if (buffer.occupied >= BSIZE)           
      printf("jobtaker waiting-------------------.\n"); 
      while (buffer.occupied >= BSIZE) 
          pthread_cond_wait(&(buffer.less),  
      &(buffer.mutex) ); 
      printf("jobtaker executing-----------------.\n"); 

      buffer.buf[buffer.nextin++] = getfile[i]; 
      buffer.buff[buffer.nextin++] = i;



struct timeb  tmb;
ftime(&tmb);
buffer.starttimetotal=buffer.starttimetotal+tmb.millitm;
   //  buffer.bufftime[buffer.nextin++] =tmb.millitm;
printf("%d\n",getfile[i]);


//-----print
time_t rawtime;
struct tm * timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
//printf("%s",asctime(timeinfo));
 
//--open file to set jobs

FILE *fp;
fp=fopen("simulation_log.txt","a+");
if(fp == NULL)
{
//error opening file
}
////create job list

fprintf(fp,"-------------------------------------\n");
fprintf(fp,"job# %d cpu_burst# %d \n",i,getfile[i]);
fprintf(fp,"Arrival Time :%s",asctime(timeinfo));
fprintf(fp,"-------------------------------------\n\n");
fclose(fp);

////--closed file
//



//--print
     buffer.nextin %= BSIZE; 
      buffer.occupied++; 

	  
      pthread_cond_signal(&(buffer.more)); 
      pthread_mutex_unlock(&(buffer.mutex)); 
    

} 
	buffer.jobtakerfinish=1;
  printf("jobtaker exiting  %d  .\n",i); 
  pthread_exit(0); 
}







     
void * cpuone(void * parm) 
{ 
//--

  int item,itemid; 
  int i;
//	 int takeArTime;
	//double diff_t;
  printf("cpuone started.\n"); 

  for(i=0;i<NUMITEMS;i++){ 



  
  pthread_mutex_lock(&(buffer.mutex) ); 
  if (buffer.occupied <= 0) 
{

buffer.cpu1=1;
/*if(buffer.jobtakerfinish==1 && buffer.cpu1==1 && buffer.cpu2==1 &&buffer.cpu3==1)
{

break;
}*/
//else{
printf("cpu one waiting.\n"); 
//}
}

//--
 while(buffer.occupied <= 0) 
     pthread_cond_wait(&(buffer.more),  
        &(buffer.mutex) ); 

         printf("cpuone executing. "); 

  item = buffer.buf[buffer.nextout++];
  itemid = buffer.buff[buffer.nextout++];
 // takeArTime =buffer.bufftime[buffer.nextout++];

  printf("%d\n",item); 
  buffer.nextout %= BSIZE; 
  buffer.occupied--; 

  
 pthread_cond_signal(&(buffer.less)); 


 pthread_mutex_unlock(&(buffer.mutex)); 

/*
time_t start_t;
time(&start_t);
printf(" start  %If",start_t);
*/


time_t mytime;
mytime=time(NULL);
char getsettime[25];
strcpy(getsettime,ctime(&mytime));
//printf("%s of %d\n",ctime(&mytime),item);
sleep(item);







struct timeb end_t;
ftime(&end_t);
buffer.endttimetotal=buffer.endttimetotal+end_t.millitm;
//int diff_t = (int)(1000.0*(takeArTime-end_t.millitm));
//printf("value en1 %d \n",takeArTime);
//printf("value en2 %d \n",end_t.millitm);

//printf("value start %d\n",buffer.starttimetotal);
//printf("value end %d\n",buffer.endttimetotal);

/*
buffer.avgwaiting=buffer.avgwaiting+diff_t;

int sec=(buffer.avgwaiting / 1000/1000/1000);

*/

//printf("time %d",sec);
//printf("time %d",buffer.avgwaiting);





//-----print
time_t rawtime;
struct tm * timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
//printf("%s",asctime(timeinfo));

//--open file to set jobs

FILE *fp;
fp=fopen("simulation_log.txt","a+");
if(fp == NULL)
{
//error opening file
}
////create job list

fprintf(fp,"-------------------------------------\n");
fprintf(fp,"Statistics for CPU-1\n");
fprintf(fp,"Process id :%d  time :%d \n",itemid,item);
fprintf(fp,"Arrival Time    :%s \n",getsettime);
fprintf(fp,"Completion Time :%s \n",asctime(timeinfo));
fprintf(fp,"-------------------------------------\n\n");
fclose(fp);
//
//////--closed file
////
//







//break; 
 } 
  printf("cpuone exiting.\n"); 
 pthread_exit(0); 
} 





void * cputwo(void * parm)
{
  int item,itemid;
  int i;
  printf("cputwo started.\n");

  for(i=0;i<NUMITEMS;i++){
 
 pthread_mutex_lock(&(buffer.mutex) );
  
  if (buffer.occupied <= 0) 
{

buffer.cpu2=1;
/*if(buffer.jobtakerfinish==1 && buffer.cpu1==1 && buffer.cpu2==1 &&buffer.cpu3==1)
{

break;
}*/
//else{
printf("cpu two waiting.\n"); 
//}
}

 //
  while(buffer.occupied <= 0)
      pthread_cond_wait(&(buffer.more),
        &(buffer.mutex) );
         printf("cputwo executing. ");
  item = buffer.buf[buffer.nextout++];
itemid = buffer.buff[buffer.nextout++];
 
 printf("%d \n",item);
  buffer.nextout %= BSIZE;
  buffer.occupied--;




 pthread_cond_signal(&(buffer.less));

 pthread_mutex_unlock(&(buffer.mutex));

  
  
  
time_t mytime;
mytime=time(NULL);
char getsettime[25];
strcpy(getsettime,ctime(&mytime));
//printf("%s of %d\n",ctime(&mytime),item);
sleep(item);




struct timeb end_t;
ftime(&end_t);
buffer.endttimetotal=buffer.endttimetotal+end_t.millitm;
//-----print
time_t rawtime;
struct tm * timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
//printf("%s",asctime(timeinfo));

//--open file to set jobs

FILE *fp;
fp=fopen("simulation_log.txt","a+");
if(fp == NULL)
{
//error opening file
}
////create job list

fprintf(fp,"-------------------------------------\n");
fprintf(fp,"Statistics for CPU-2\n");
fprintf(fp,"Process id :%d  time :%d \n",itemid,item);
fprintf(fp,"Arrival Time    :%s \n",getsettime);
fprintf(fp,"Completion Time :%s \n",asctime(timeinfo));
fprintf(fp,"-------------------------------------\n\n");
fclose(fp);
//
//////--closed file
////
//



  
  
  

//break; 
 }
  printf("cputwo exiting.\n");
  pthread_exit(0);
}








void * cputhree(void * parm)
{
  int item,itemid;
  int i;
  printf("cputhree started.\n");

  for(i=0;i<NUMITEMS;i++){
  
 pthread_mutex_lock(&(buffer.mutex) );
 
  if (buffer.occupied <= 0) 
{

buffer.cpu3=1;
/*if(buffer.jobtakerfinish==1 && buffer.cpu1==1 && buffer.cpu2==1 && buffer.cpu3==1)
{

break;
}
*/

//else{
printf("cpu three waiting.\n"); 
//}
}

 //
  while(buffer.occupied <= 0)
      pthread_cond_wait(&(buffer.more),
        &(buffer.mutex) );
         printf("cputhree executing. ");
  item = buffer.buf[buffer.nextout++];
itemid = buffer.buff[buffer.nextout++];
 
 printf("%d\n",item);
  buffer.nextout %= BSIZE;
  buffer.occupied--;




 pthread_cond_signal(&(buffer.less));

 pthread_mutex_unlock(&(buffer.mutex));

  
  
  
time_t mytime;
mytime=time(NULL);
char getsettime[25];
strcpy(getsettime,ctime(&mytime));
//printf("%s of %d\n",ctime(&mytime),item);
sleep(item);


struct timeb end_t;
ftime(&end_t);
buffer.endttimetotal=buffer.endttimetotal+end_t.millitm;

//-----print
time_t rawtime;
struct tm * timeinfo;
time(&rawtime);
timeinfo=localtime(&rawtime);
//printf("%s",asctime(timeinfo));

//--open file to set jobs

FILE *fp;
fp=fopen("simulation_log.txt","a+");
if(fp == NULL)
{
//error opening file
}
////create job list

fprintf(fp,"-------------------------------------\n");
fprintf(fp,"Statistics for CPU-3\n");
fprintf(fp,"Process id :%d  time :%d \n",itemid,item);
fprintf(fp,"Arrival Time    :%s \n",getsettime);
fprintf(fp,"Completion Time :%s \n",asctime(timeinfo));
fprintf(fp,"-------------------------------------\n\n");
fclose(fp);
//
//////--closed file
////
//



  
  

//break; 
 }
  printf("cputhree exiting.\n");
  pthread_exit(0);
}





void * displayt(void * parm)
{


while(buffer.jobtakerfinish!=2)
{
 if(buffer.jobtakerfinish==1 && buffer.cpu1==1 && buffer.cpu2==1 && buffer.cpu3==1)
{

printf("finish\n");

//create job list


FILE *fppz;
fppz=fopen("simulation_log.txt","a+");
if(fppz == NULL)
{
//error opening file
}



double diff=buffer.endttimetotal- buffer.starttimetotal;
double difffinal=diff/200.0;
double difff=buffer.endttimetotal/200.0;

fprintf(fppz,"-------------------------------------\n");
fprintf(fppz,"Number of processes      # %d processes\n",NUMITEMS);
fprintf(fppz,"Average waiting time     # %If seconds\n",difffinal);
fprintf(fppz,"Average turn around time # %If seconds\n",difff);
fprintf(fppz,"-------------------------------------\n\n");
fclose(fppz);


//--closed file





 pthread_exit(0);
break;
}

}
}
