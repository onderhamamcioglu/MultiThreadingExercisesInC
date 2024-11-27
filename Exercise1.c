#include <stdio.h>
#include <process.h>
#include <windows.h>


HANDLE mutex;


void decreaseFunction(void *p) {
    while(1) {
        int *numPtr = (int *) p; // cast pointer to int in order to point address of number created in main
        WaitForSingleObject(mutex, INFINITE);
        *numPtr -= 2;
        printf("DECREASED: %d\n", *numPtr);
        ReleaseMutex(mutex);
        Sleep(1500);
    }
}

void increaseFunction(void *p) {
    while(1) {
        int *numPtr = (int *) p;
        WaitForSingleObject(mutex, INFINITE);
        *numPtr += 4;
        printf("INCREASED: %d\n", *numPtr);
        ReleaseMutex(mutex);
        Sleep(500);
    }
}


int main(void) {
    mutex = CreateMutex(NULL, FALSE, NULL);

    int number = 100; // Could this also be a global variable?

    HANDLE increaser = _beginthread(decreaseFunction, 0, (void *) &number);
    HANDLE decreaser = _beginthread(increaseFunction, 0, (void *) &number);

    // Without this program does not wait for threads' end signal
    // https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
    WaitForSingleObject(increaser, INFINITE);
    WaitForSingleObject(decreaser, INFINITE);

    return 0;
}