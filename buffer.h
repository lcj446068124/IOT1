#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#define MAXBUFFERSIZE 200

typedef char BufferElemtype;
typedef struct{
		BufferElemtype data[MAXBUFFERSIZE];
    int front;
    int rear;
}Buffer;

extern Buffer myBuffer;

void BufferInit(Buffer *q);
bool EnBuffer(Buffer *q,BufferElemtype e);
bool DeBuffer(Buffer *q);
int BufferLength(Buffer *q);
bool isEmpty(Buffer *q);
bool traverseBuffer(Buffer *b);

#endif
