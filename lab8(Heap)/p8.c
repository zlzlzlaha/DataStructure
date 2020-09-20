#include <stdio.h>
#include <stdlib.h>


FILE * fw = NULL;

typedef int ElementType;

typedef struct HeapStruct
{
    int Capacity;
    int Size;
    ElementType * Elements;
}Heap;

typedef Heap* PriorityQueue;

void Insert(ElementType X, PriorityQueue H);
void Find(ElementType X, PriorityQueue H);
void Print(PriorityQueue H);


int main(void)
{
    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    int input,i;
    PriorityQueue PQ = NULL;
    if(fr == NULL)
    {
        printf("failed to open input.txt \n");
        return -1;
    }

    if(fw == NULL)
    {
        printf("failed to open output.txt\n");
        return -1;
    }


    fscanf(fr,"%d ",&input); // get input number

    // dynamic allocate and initailize heap 
    PQ = (PriorityQueue)malloc(sizeof(Heap));

    if(PQ == NULL)
    {
        printf("failed to allocate memory \n");
    }
    PQ->Elements = (ElementType*)malloc(sizeof(ElementType)*input+1); // not use 0 index , need extra 1 array

    if(PQ -> Elements == NULL)
    {
        printf("failed to allocate memory \n");
    }
    PQ->Size = 0;

    for(i=0;i<=input;i++)
    {
        PQ->Elements[i] = -1;
    }
    PQ->Capacity = input;
    while((input=fgetc(fr))!=EOF) // until meet EOF , get input from file
    {
        switch(input)
        {
            case 'i' :  // insert node
                        fgetc(fr); // eliminate white space
                        fscanf(fr," %d",&input);
                        Insert(input,PQ);
                        break;
            case 'f' : // find node get from input
                        fgetc(fr);
                        fscanf(fr," %d",&input);
                        Find(input,PQ);
                        break;
            case 'p' :  // print all node in the head
                        Print(PQ);
                        break;

        }
    }
    // deallocation
    fclose(fr);
    fclose(fw);
    return 0;
}


void Insert(ElementType X, PriorityQueue H) // get Element X , PriorityQueue H from input and insert X in Max heap H
{
    int i;

    for(i =1; i<=H->Size ; i++) // when there is same node in heap
    {
        if(X == H->Elements[i])
        {
            fprintf(fw,"%d is already in the heap.\n",X);
            return; // quit insert 
        }
    }

    if(i == H->Size+1) // when can't find same node
    {
        fprintf(fw,"insert %d\n",X);
    } 

    if(H->Size+1 > H -> Capacity)
    {
        fprintf(fw,"heap is full\n"); // When PriorityQueue is full
        return;
    }

    // percolating up
    for(i = ++H->Size ; (X > H->Elements[i/2]) && (i/2!=0) ; i/=2)  // when element is higher than heap node, down that node 
    {
        H->Elements[i] = H->Elements[i/2]; 
    }
    H->Elements[i] = X;
}
void Find(ElementType X, PriorityQueue H) // get Element X and PriorityQueue H  from input and if find X in H, print node in output file
{
    int i;
    for(i = 1 ; i<= H->Size; i++)
    {
        if(X == H->Elements[i])//when this is node same with X
        {   
            fprintf(fw,"%d is in the heap.\n",X);
            break;
        }
    }

    if(i == H->Size+1) // when there is not node same with X
    {
        fprintf(fw,"%d is not in the heap.\n",X);
    }
}
void Print(PriorityQueue H) // get PrioiryQueue from input and print it's node in output file
{
    int i;
    for(i = 1; i<= H->Size ; i++) // print stored node in the heap
    {
        fprintf(fw,"%d ",H->Elements[i]);
    }
    fprintf(fw,"\n");
}



