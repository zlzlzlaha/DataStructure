#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * fw = NULL; // To use file output in fuction 

typedef struct StackRecord * Stack;
typedef int ElementType;
struct StackRecord
{
    int Capacity;
    int TopOfStack;
    ElementType * Array;
};


Stack CreateStack(void) // make 100 size of New Array Stack and return its pointer value
{
    Stack NewStack = (Stack)malloc(sizeof(struct StackRecord));
    if(NewStack == NULL) // when fail to allocation
    {
        printf("failed to make new Stack!\n");
    }
    NewStack -> Capacity = 100;
    NewStack -> TopOfStack = -1;
    NewStack -> Array = (int*)malloc(sizeof(int)*100);
    if(NewStack -> Array == NULL) //when fail to allocation
    {
        printf("failed to make Array\n");
    }
    return NewStack;
}


int IsEmpty(Stack s) //get stack pointer from input and  when stack s is empty return 1, or not return 0 
{
    if(s->TopOfStack == -1)
    {
        return 1;
    }
    else
        return 0;
}

int IsFull(Stack s) // get stack pointer from input when stack s is full reuturn 1, or not return 0
{
    if(s->TopOfStack == s->Capacity -1) // when stack is full
    {
        return 1;
    }

    else 
        return 0;
}

void Push(ElementType data, Stack s) // get element, stack pointer from input and Push element data into stack s
{
    if(IsFull(s)==1) // when stack is full
    {
        fprintf(fw,"Full\n");
    }
    else
    {
        s->TopOfStack = s->TopOfStack + 1;
        s->Array[s->TopOfStack] = data;
    }
}
void Pop(Stack s) // get stack pointer from input , Push top data and return its value
{
    if(IsEmpty(s) == 1) // when stack is empty
    {
       fprintf(fw,"Empty\n");
    }
    
    else
    {
       fprintf(fw,"%d\n",s->Array[s->TopOfStack]);
       s->TopOfStack = s-> TopOfStack -1;
    }
}



int main(void)
{
    Stack stack;
    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    char input[10]; // to get input information
    int inputData;
    int size = 0 ;
    int i;
    
    // when fail to open file
    if(fr == NULL)
    {
         printf("failed to open input file!\n");
         return -1;
    }

    if(fw == NULL)
    {
         printf("failed to open output file!\n");
         return -1;
    }
    
    fscanf(fr,"%d",&size); //get input number
    stack =  CreateStack(); // make stack 
    for(i = 0 ; i < size ;i++) 
    {
        fscanf(fr,"%s",input);
        if(strcmp(input,"push")==0) // when file input is push
        {   
            fscanf(fr,"%d",&inputData);
            Push(inputData, stack);
        }

        else if(strcmp(input,"pop")==0) // when file input is pop
        {
            Pop(stack);
        }
    }


    // Deallocation
    free(stack->Array);
    free(stack);
    fclose(fr);
    fclose(fw);
    return 0;

}
