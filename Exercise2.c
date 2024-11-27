#include <stdio.h>
#include <process.h>
#include <windows.h>

HANDLE loaderEvent, assemblerEvent;
int assemblyLine[5];

void loaderRobot(void* param) {
   int id = *(int*)param;
   while (1) {
       WaitForSingleObject(loaderEvent, INFINITE);
       for (int i = 0; i < 5; i++) {
           if (assemblyLine[i] == 0) {
               int newPart = rand() % 9000 + 1000;
               assemblyLine[i] = newPart;
               int place = i + 1;
               printf("Loader Robot %d: Inserted part %d in %dth place on the line\n", id, newPart, place);
               break;
           }
       }
   }
}

void assemblerRobot(void* param) {
   int id = *(int*)param;
   while (1) {
       WaitForSingleObject(assemblerEvent, INFINITE);
       int i = rand() % 5;
       int attempts = 0;
       while (assemblyLine[i] == 0 && attempts < 5) {
           i = rand() % 5;
           attempts++;
       }
       if (assemblyLine[i] != 0) {
           int place = i + 1;
           printf("Assembler Robot %d: Taking part %d from %dth place to assemble\n", id, assemblyLine[i], place);
           assemblyLine[i] = 0;
       }
   }
}

void printAssemblyLine() {
    printf("Parts on the assembly line: ");
    for(int i = 0; i < 5; i++) {
        printf("%d  ", assemblyLine[i]);
    }
    printf("\n");
}

int main(void) {
   assemblerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
   loaderEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    printf("Parts on the assembly line: ");
    for(int i = 0; i < 5; i++) {
        assemblyLine[i] = rand() % 9000 + 1000;
        printf("%d, ",assemblyLine[i]);
    }
    printf("\n");

   int loaderId1 = 1;
   int loaderId2 = 2;
   int assemblerId1 = 1;
   int assemblerId2 = 2;

   HANDLE assemblerThread1 = _beginthread(assemblerRobot, 0, &assemblerId1);
   HANDLE assemblerThread2 = _beginthread(assemblerRobot, 0, &assemblerId2);
   HANDLE loaderThread1 = _beginthread(loaderRobot, 0, &loaderId1);
   HANDLE loaderThread2 = _beginthread(loaderRobot, 0, &loaderId2);

   while(1) {
       SetEvent(assemblerEvent);
       Sleep(1000);
       ResetEvent(assemblerEvent);
       Sleep(1000);
       printAssemblyLine();
       SetEvent(loaderEvent);
       Sleep(1000);
       ResetEvent(loaderEvent);
       printAssemblyLine();
   }

   HANDLE threadArray[4] = {assemblerThread1, assemblerThread2, loaderThread1, loaderThread2};
   WaitForMultipleObjects(4, threadArray, TRUE, INFINITE);

   return 0;
}