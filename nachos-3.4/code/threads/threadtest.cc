// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

// testnum is set in main.cc
int testnum = 1;


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n",which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, (void*)1);
    SimpleThread(0);
}

///////////////////////// lab1 st //////////////////////////////////////
//----------------------------------------------------------------------
// ThreadTest0
//       To check whether the UID & TID can print out
//----------------------ADD By jea--------------------------------------
void 
ThreadTest0()
{
     DEBUG('t', "Entering ThreadTest0");
     for(int i = 0; i <= 10; i++)
         {
              Thread *t = new Thread("forked thread");
              t->Fork(SimpleThread, (void*)i);
              printf("name:%s, UID:%d, TID:%d \n",t->getName(),t->getUserID(),t->getThreadID());
         }
}


//----------------------------------------------------------------------
// ThreadTest2
//       To check whether the limit works
//----------------------ADD By jea--------------------------------------
void 
ThreadTest2()
{
     DEBUG('t', "Entering ThreadTest2");
     for(int i = 0; i <= MaxThreadNum; i++)
         {
              printf("loops: %d\n",i+1);
              Thread *t = new Thread("forked thread");
              t->Fork(SimpleThread, (void*)i);
              printf("name:%s, UID:%d, TID:%d \n",t->getName(),t->getUserID(),t->getThreadID());
         }
}


//----------------------------------------------------------------------
// ThreadTest3
//       To check whether ts works
//----------------------ADD By jea--------------------------------------
void 
ThreadTest3()
{
     DEBUG('t', "Entering ThreadTest3");
     /*for(int i = 0; i < 10; i++)
         {
              Thread *t = new Thread("forked thread");
              t->Fork(SimpleThread, (void*)i);
         } */
      
      while(1){
        printf(">>");
        char str[10];
        int re;
        scanf("%s",&str);

        if(!strcmp(str,"ts")){
	    printf("Name     \t         UID\t        TID \t    Status\t \n");
            for(int i = 0; i < MaxThreadNum; i++)
            {
               if(threadArray[i] != NULL)
	  	{
		    printf("%s     \t %d   \t %d   \t %s  \t  \n",threadArray[i]->getName(),\
                    threadArray[i]->getUserID(),threadArray[i]->getThreadID(),threadArray[i]->getStatus());
                  
		}
            }
          }
        else
          Exit(0);
       }
}
//////////////////////// lab1 end /////////////////////////////////////


///////////////////////// lab2 st /////////////////////////////////////
//----------------------------------------------------------------------
// SimpleThread1
// 	Loop 3 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread1(int which)
{

    int num;
    int i;
    i = currentThread->getPriority();
    if(which > 0){
       which -= 1;
       printf("create Thread %d\n",which);
       Thread *t = new Thread("forked Thread", 5);
       t->Fork(SimpleThread1,(void*)which);

       for (num = 0; num < 10; num++) {
	    printf("*** thread %d looped %d times, Priority:%d, Used/Total %d/%d \n",\
                    which, num,currentThread->getPriority(),\
                    currentThread->getUsedTime(), currentThread->getTotalTime());
            interrupt->SetLevel(IntOn);
            interrupt->SetLevel(IntOff);
           }
       }

}

//----------------------------------------------------------------------
// ThreadTest4
//       To check whether the UID & TID can print out
//----------------------ADD By jea--------------------------------------
void 
ThreadTest4()
{
     DEBUG('t', "Entering ThreadTest4");
     
     /*
     for(int i = 0; i < 4; i++)
         {
	      //printf("create thread %d  \n", i);
              Thread *t = new Thread("forked thread", 32 - i);	      
              t->Fork(SimpleThread1, (void*)i);
              //printf("TID:%d  Priority: %d\n",t->getThreadID(),t->getPriority());
         }
     */
      
     Thread *t = new Thread("forked thread", 5);
     t->Fork(SimpleThread1,(void *)5);
     interrupt->SetLevel(IntOn);
     interrupt->SetLevel(IntOff);
}


///////////////////////// lab2 end ////////////////////////////////////


/////////////////////////  lab3 semaphore st /////////////////////////////////////
/*
#include"synch.h"
//#include"synch.cc"

#define N 3
//Semaphore * semaphore;
//Semaphore * empty;
//Semaphore * full;
Semaphore* semaphore = new Semaphore("mutex",1);
Semaphore* empty = new Semaphore("empty",N);
Semaphore* full = new Semaphore("full",0);
int count = 0;
int n = 3;

void producer(int ith){
     for(int i = 0; i<n; i++){
          empty->P();
          semaphore->P();
          count += 1;
          printf("%s produced an item, total items in buffer: %d \n",\
                 currentThread->getName(), count);
          semaphore->V();
          full->V(); 
    }
          
}

void consumer(int ith){
     for(int i = 0; i<n; i++){
          full->P();
          semaphore->P();
          count -= 1;
          printf("%s consumed an item, total items in buffer: %d \n",\
                  currentThread->getName(), count);
          semaphore->V();
          empty->V();
     }

}

//----------------------------------------------------------------------
// ThreadTest5
// 	Produce a producer thread and a consumer thread
//      Test semaphore
//----------------------------------------------------------------------
void ThreadTest5(){

     Thread * p1 = new Thread("producer1",3,2);
     Thread * p2 = new Thread("producer2",2,2);
     Thread * p3 = new Thread("producer3",1,2);
     Thread * c1 = new Thread("consumer1",3,2);
     Thread * c2 = new Thread("consumer2",2,2);
     Thread * c3 = new Thread("consumer3",1,2);
     
     p1->Fork(producer,(void*) 1);
     p2->Fork(producer,(void*) 2);
     p3->Fork(producer,(void*) 3);
     c1->Fork(consumer,(void*) 1);
     c2->Fork(consumer,(void*) 2);
     c3->Fork(consumer,(void*) 3);

}
*/
///////////////////////// lab3 semaphore end ////////////////////////////////////

///////////////////////// lab3 lock & conditon variable st//////////////////////

#include"synch.h"
Lock * condLock = new Lock("condLock"); // exclusion entrance
Condition * condc = new Condition("consumer");
Condition * condp = new Condition("producer");
int count = 0;
int n = 3;
#define N 3
void producer(int ith){
     int i;
     for(i = 0; i < ith; i++){    
         condLock->Acquire();
         while(count == N) condp->Wait(condLock);
         count += 1;
         printf("%s produced an item,total items in buffer:%d\n", currentThread->getName(),count);
         if(count == 1)         
            condc->Broadcast(condLock);
         condLock->Release(); 
     } 
     //currentThread->Yield();    
}

void consumer(int ith){
     int i;
     for(i = 0; i < ith; i++){    
         condLock->Acquire();
         while(count == 0) condc->Wait(condLock);
         count -= 1;
         printf("%s consumed an item,total items in buffer:%d\n", currentThread->getName(),count);
         if(count == N-1)
            condp->Broadcast(condLock);
         condLock->Release(); 
     } 
      //currentThread->Yield();     
}

//----------------------------------------------------------------------
// ThreadTest6
// 	Produce a producer thread and a consumer thread
//      Test condition variables
//----------------------------------------------------------------------
ThreadTest6(){
     Thread * p1 = new Thread("producer1");
     Thread * p2 = new Thread("producer2");
     Thread * p3 = new Thread("producer3");
     Thread * c1 = new Thread("consumer1");
     Thread * c2 = new Thread("consumer2");
     Thread * c3 = new Thread("consumer3");
     
     p1->Fork(producer,(void*) 1);
     p2->Fork(producer,(void*) 2);
     p3->Fork(producer,(void*) 3);
     c1->Fork(consumer,(void*) 1);
     c2->Fork(consumer,(void*) 2);
     c3->Fork(consumer,(void*) 3);

}

///////////////////////// lab3 lock & condition variable end //////////////////

///////////////////////////// lab3 barrier st ///////////////////////////////
#define M 3
Barrier* barrier = new Barrier("barrier",M);
void
synThread(int which)
{
    int num;
    //printf("barrier used\n");
    for (num = 0; num < 4; num++) {
	printf("*** thread %d looped %d times\n",which, num);
        barrier->Synch();
        //currentThread->Yield();
    }
}

void
unsynThread(int which)
{
    int num;
    //printf("barrier unused\n");
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n",which, num);
        //currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest7
// 	Produce some threads
//      Test barrier
//----------------------------------------------------------------------
ThreadTest7(){
      int i;
     
      for(i = 0; i < M; i++){
          Thread *t = new Thread("synch thread");
          t->Fork(synThread,(void *)i);
      }
      /*
      for(i = 0; i < M; i++){
          Thread *t = new Thread("synch thread");
          t->Fork(unsynThread,(void *)i);
      }*/
      
}

//////////////////////////// lab3 barrier end //////////////////////////////
////////////////////////////// lab3 readerWriterLock st ///////////////////
#define AM 3
int array[AM];
readerWriterLock * rwLock = new readerWriterLock("rwLock");

void reader(int ith){
     int t;
     rwLock->readLockAcquire();
     for(int i=0; i < AM; i++){
         t = array[i];
         printf("%s %d read the %d th number, the number is %d \n",\
            currentThread->getName(), ith, i, t);
     }
     rwLock->readLockRelease();
}

void writer(int ith){
     int tp;
    rwLock->writeLockAcquire();
     for(int i = 0; i < AM; i++){
        tp = array[i];
        array[i] += 1;
        printf("%s %d write the %d th number, the old number is %d and the new number is %d\n",currentThread->getName(), ith, i, tp, array[i]);
     }

    rwLock->writeLockRelease();
}


//----------------------------------------------------------------------
// ThreadTest8
// 	Produce some threads
//      Test barrier
//----------------------------------------------------------------------
ThreadTest8(){
      int i;
     
      // rrwwrrwwrr 
      Thread *t;
      for(i = 0; i < 2; i++){
	t = new Thread("reader thread", 8-i);
        t->Fork(reader,(void*)i);
      }
      for(i = 0; i < 2; i++){
	t = new Thread("writer thread",8-i);
        t->Fork(writer,(void*)i);
      }

      for(i = 2; i < 4; i++){
	t = new Thread("reader thread",8-i);
        t->Fork(reader,(void*)i);
      }

      for(i = 2; i < 4; i++){
	t = new Thread("writer thread",8-i);
        t->Fork(writer,(void*)i);
      }

      for(i = 4; i < 6; i++){
	t = new Thread("reader thread",8-i);
        t->Fork(reader,(void*)i);
      }

      //ThreadTest3();
      
}

//////////////////////////// lab3 readerWriteLock end /////////////////////


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 0:
	ThreadTest0();
	break;
    case 1:
	ThreadTest1();
	break;
    case 2:
	ThreadTest2();
	break;
    case 3:
	ThreadTest3();
	break;
    case 4:
	ThreadTest4();
	break;
    
    case 5:
         ThreadTest6();
         break;
 
    case 6:
         ThreadTest6();
         break;
    case 7:
         ThreadTest7();
         break;
    case 8:
         ThreadTest8();
         break;
    default:
	printf("No test specified.\n");
	break;
    }
}

