#include "buffer.h"

Buffer myBuffer;

void BufferInit(Buffer *b){
	b->front = b->rear = 0;
	for(int i = 0;i < MAXBUFFERSIZE;i++)
		b->data[i] = '\0';
}

bool EnBuffer(Buffer *b,BufferElemtype e){
	if((b->rear+1)%MAXBUFFERSIZE==b->front){
        //If the queue is full, continue to join the queue and overwrite the previous data.
        b->front =(b->front+1)%MAXBUFFERSIZE;
    }
    b->data[b->rear]=e;
    b->rear=(b->rear+1)%MAXBUFFERSIZE;
		return true;
}

bool DeBuffer(Buffer *b){
	if(b->front==b->rear)
        return false;
    b->data[b->front] = '\0';
    b->front =(b->front+1)%MAXBUFFERSIZE;
    return true;
}

int BufferLength(Buffer *b){
	while(1){
		while(b->data[b->front] != 0x0d && DeBuffer(b));
		uint32_t count = (b->rear - b->front + MAXBUFFERSIZE)%MAXBUFFERSIZE;
		if(count > 1){
			if(b->data[(b->front+1)%MAXBUFFERSIZE] != 0x0a){
				DeBuffer(b);
			}else{
				return count;
			}
		}else{
			return count;
		}
	}
}

bool isEmpty(Buffer *b){
	if(b->rear ==b->front)
		return true;
	return false;
}

bool traverseBuffer(Buffer *b)
{
    if(isEmpty(b)){
        return false;
    }
    while(b->front != b->rear){
        printf("%c",b->data[b->front]);
        DeBuffer(b);
        //b->front = (b->front + 1)%MAXBUFFERSIZE;
    }
    return true;
}
