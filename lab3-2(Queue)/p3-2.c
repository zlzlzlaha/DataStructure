#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

FILE * fw = NULL;// to use file output in fuction


typedef struct Queue{
    int key[MAX_QUEUE_SIZE];
    int rear;
    int front;
}Queue;

Queue* CreateQueue(void) // allocate memoery of queue and return Queue pointer value
{
    Queue * NewQueue = (Queue*)malloc(sizeof(Queue));
    if(NewQueue ==NULL) // when failed to open file
    {
        printf("failed to create Queue\n");
    }
    NewQueue->rear = -1;
    NewQueue->front = 0;
    return NewQueue;
}

int IsEmpty(Queue * q) // get Queue pointer value from input and  when Queue q is empty return 1 or not return 0
{
    if(q->rear < q->front) //when rear is less then front, it means empty
        return 1;
    else 
        return 0;
    
}
int IsFull(Queue *q) // get Queue pointer value from input and when Queue q is full, return 1 or not return 0
{
    if(q->rear == MAX_QUEUE_SIZE -1)
        return 1;
    else
        return 0;
}

void enQ(int number, Queue *q) //get element and Queue pointer value from input and  insert input number to Queue q
{
    if(!IsFull(q)) // whe queue is not full
    {
        q->key[++(q->rear)] = number;
    }
    else // when queue is full
    {
        fprintf(fw,"Full\n");
    }

}

void deQ(Queue * q) // get Queue pointer value from input and Delete front element of q
{
    if(!IsEmpty(q)) // when queue is not empty 
    {
        fprintf(fw,"%d\n",q->key[q->front]);
        q->front = q->front +1;
    }
    else // when queue is empty
    {
        fprintf(fw,"Empty\n"); 
    }
}



int main(void)
{
    FILE * fr= fopen("input.txt","rt");
    fw= fopen("output.txt","wt");
    int i;
    char input[10]; //to get enQ or deQ from file
    int number;
    int size;
    Queue * queue = NULL;

    //when fail to open files
    if(fr == NULL)
    {
        printf("failed to open input file! \n");
        return -1;
    }
    if(fw==NULL)
    {  
        printf("failed to open output file!\n");
        return -1;
    }
    //make new queue
    queue = CreateQueue();

    fscanf(fr,"%d",&size);//get input size from file

    for(i=0 ; i<size ; i++) // make loop according to input size
    {
       fscanf(fr,"%s",input); //get operation from file
       if(strcmp(input,"enQ") == 0) // when input is enQ
       {
           fscanf(fr,"%d",&number);
           enQ(number,queue);
       }

       else if(strcmp(input,"deQ") == 0)//when input is deQ
       {
           deQ(queue);
       }

    }
    //deallocation
    free(queue);
    fclose(fr);
    fclose(fw);
    return 0;
}
