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
#include <stdio.h>

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
    
    for (num = 0; num < 1; num++) {
	//printf("*** thread %d looped %d times\n", which, num);
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
              //t->Fork(SimpleThread, (void*)i);
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
     for(int i = 0; i < 10; i++)
         {
              Thread *t = new Thread("forked thread");
              t->Fork(SimpleThread, (void*)i);
         } 
      
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
		    printf("%s     \t %d   \t %d   \t %s  \t \n",threadArray[i]->getName(),threadArray[i]->getUserID(),threadArray[i]->getThreadID(),threadArray[i]->getStatus());
		}
            }
          }
        else
          Exit(0);
       }
}

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
    default:
	printf("No test specified.\n");
	break;
    }
}

