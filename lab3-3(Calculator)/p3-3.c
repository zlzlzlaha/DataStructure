#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STACK_SIZE 100
FILE * fw = NULL;

typedef struct StackRecord * Stack;
typedef int ElementType;
struct StackRecord
{
    int Capacity;
    int TopOfStack;
    ElementType * Array;
};

int GetPrecedence(ElementType operator) // get operator from input and distingush operator's precedence and return its precedence 
{
    switch(operator) // precedence + - :1 , * % / : 2
    {
        case '+':
        case '-':
            return 1;
            break;
        case '*':
        case '%':
        case '/':
            return 2;
            break;
        case '(':
        case ')':
            return 0;    
            break;
        default: // when they are operand
            return -1;

    }
}
Stack CreateStack(void) // make n size of New Stack and return its pointer
{
    Stack NewStack = (Stack)malloc(sizeof(struct StackRecord));
    if(NewStack == NULL)
    {
        printf("failed to make new Stack!\n");
    }
    NewStack -> Capacity = STACK_SIZE;
    NewStack -> TopOfStack = -1;
    NewStack -> Array = (ElementType*)malloc(sizeof(ElementType)*STACK_SIZE);
    if(NewStack -> Array == NULL)
    {
        printf("failed to make Array\n");
    }
    return NewStack;
}


int IsEmpty(Stack s) // get stack pointer value from input and when stack s is empty return 1, or not return 0 
{
    if(s->TopOfStack == -1)
    {
        return 1;
    }
    else
        return 0;
}

int IsFull(Stack s) //get stack pointer value from input and  when stack s is full reuturn 1, or not return 0
{
    if(s->TopOfStack == s->Capacity -1) // when stack is full
    {
        return 1;
    }

    else 
        return 0;
}

void Push(ElementType data, Stack s) // get element and stack pointer vlaue from input ,  Push element data into stack s
{
    if(IsFull(s)==1)
    {
        printf("Full\n");
    }
    else
    {
        s->TopOfStack = s->TopOfStack + 1;
        s->Array[s->TopOfStack] = data;
    }
}
void Pop(Stack s) // get stack pointer value from input, Push top data ,print it and return its value
{
    if(IsEmpty(s) == 1) // when stack is empty
    {
       fprintf(fw,"Empty\n");
    }
    
    else 
    {
       fprintf(fw,"%c",s->Array[s->TopOfStack]);
       s->TopOfStack = s-> TopOfStack -1;
    }
}

ElementType Top(Stack s) // return value of Top of stack S
{
    if(!IsEmpty(s)) // when stack is not empty
        return s->Array[s->TopOfStack];
    else
        printf("Empty stack!\n");
        return 0;
}

int Calculate(ElementType operator, ElementType operand1, ElementType operand2) // get 2 operand 1 operator form input and return value of calculation between operand1 and operand 2 with opertor
{
    switch(operator) 
    {
        case '+': 
            return operand1+operand2;
        case '-':
            return operand2-operand1;
        case '*':
            return operand1*operand2;
        case '/':
            return operand2/operand1;
        case '%':   
            return operand2%operand1;
    }
}
void PostfixPush(ElementType data, Stack s) //get element and stack pointer from input,  Push element data in to Stack s and Calculate Postfix notation 
{
   int tmp1, tmp2;
   if(GetPrecedence(data) > 0) //when data is operator , calculate it with two operand 
   {
        tmp1 = s->Array[s->TopOfStack--]; // Pop from stack but not print it 
        tmp2 = s->Array[s->TopOfStack--];
        Push(Calculate(data,tmp1,tmp2),s); // calculate operands and push to stack
   }
   else // when data is operand
   {    
       data = data -48; // ASCII Code '0' means 48 
       Push(data,s);// push operand to postfix stack
   }
}




void InfixToPostfix(char * infix, Stack postfix) // get infix string and stack pointer from input, print infix string into postfix notaiton and store in postfix stack which calculate postfix notation
{
    int operator;
    int index = 0;
    Stack tmp = CreateStack();
 
    fprintf(fw,"Post Form : ");
    while(index <= strlen(infix)-1)  // until meet \0
    {   
       operator = (int)infix[index];
        switch(operator)
        {
            case '-':
            case '+':
                if(IsEmpty(tmp) ==0) // when stack is not empty
                {
                    if(GetPrecedence(operator) <= GetPrecedence(Top(tmp))) //when top of stack have high or same precedence 
                    {
                        while(!IsEmpty(tmp)&&Top(tmp)!='(')  //until they meet ( or empty , pop all operator
                        {
                            PostfixPush(Top(tmp),postfix); //when operand or operator are printed, push this element to postfix stack 
                            Pop(tmp);
                        }
                    }
                }
                Push(operator,tmp);
                break;
            case '*':
            case '%':
            case '/':
                if(IsEmpty(tmp) == 0) 
                { 
                   if(GetPrecedence(operator) == GetPrecedence(Top(tmp))) // because * % / have highest precedence , they dont have to compare with + -
                   {
                         PostfixPush(Top(tmp),postfix);
                         Pop(tmp);
                   }
                }
                Push(operator,tmp);
                break;
                 
            case '(': // ( always can be pushed in top of stack
                Push(operator,tmp);
                break;

            case ')':
                while(Top(tmp) != '(') // until meet ( pop operator
                {
                   PostfixPush(Top(tmp),postfix); 
                   Pop(tmp);
                }
                tmp->TopOfStack --;// not print (
                break;

            default : // when 0~9
               PostfixPush(operator,postfix); // operands have to be push to postfix stack directly
               fprintf(fw,"%c",operator); // operands are printed directly

        }
        index = index +1;
    }
    while(!IsEmpty(tmp)) //when all numbers are printed pop all operator from stack
    {
        PostfixPush(Top(tmp),postfix);
        Pop(tmp);
    }
    fprintf(fw,"\n");
    //deallocation
    free(tmp->Array);
    free(tmp);
}

int main(void)
{
    Stack PostFix;
    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    char infix[101]; // to get input information
    int index=0;
    
    
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
    PostFix =  CreateStack();
    // get  infix from file
    while(1)
    {
        if((infix[index] = fgetc(fr)) != '#') //until meet #, get input
        {
            index++;
        }
        else 
        {
            infix[index] = '\0'; //change # to \0
            break;
        }
    }

    fprintf(fw,"Infix Form : %s\n",infix); // printf Infix form into file
    InfixToPostfix(infix,PostFix); // print infix notation to postfix notation and calculate postfix notation
    fprintf(fw,"Evaluation Result : %d\n", PostFix->Array[PostFix->TopOfStack]);
    // Deallocation
    free(PostFix->Array);
    free(PostFix);
    fclose(fr);
    fclose(fw);
    return 0;

}
