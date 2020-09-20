#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * fw = NULL; // to use file output in fucntion

typedef struct threaded_tree* threaded_ptr;
typedef struct threaded_tree{
    short int left_thread;
    threaded_ptr left_child;
    char data;
    threaded_ptr right_child;
    short int right_thread;
}threaded_tree;

threaded_ptr CreateNode(char input) // make thread tree head and return it pointer value
{
    threaded_ptr node = (threaded_ptr)malloc(sizeof(struct threaded_tree));
    node -> left_thread = 0;
    node -> right_child = NULL;
    node -> data = input;
    node -> right_child = NULL;
    node -> right_thread = 0;
    return node;
}


int GetPath(int nodenumber, int* path) // get nodenumber and array from input, convert nodenumber to binary notation and store it in array , return last size of array
{
    int tmp = nodenumber;
    int i;
    int j;

    for(i=0;tmp!=1;i++) //make binary number but  not store first nubmer of it 
    {
        path[i]= tmp%2;
        tmp = (int)(tmp/2);
    }
    return i; // return array size

}

void InsertNode(threaded_ptr head, char * input) // get tree head pointer and input string from input and initialize threaded tree
{
    int size = strlen(input);
    int i, j;
    int leftright[6]; // we can store path about node which number is 2^6
    int manymove;
    threaded_ptr tmp_node = NULL;
    threaded_ptr tmp = NULL;
    if(size >=1) // there is node at least one
    {
        // make node connected with head
        head -> left_child  = CreateNode(input[0]);
        if(head == NULL)
            printf("fail to create head\n");
        head -> left_child->left_thread = 1;
        head -> left_child->right_thread = 1;
        head -> left_child->left_child = head;
        head -> left_child->right_child = head;
        
        for(i=2 ; i<=size ; i++) // make node and connect thread
        {
            tmp_node = head -> left_child;
            manymove = GetPath(i,leftright); // array get nodenumber's binary value , and mamymove get size of array

            for(j = manymove-1 ; j>=0 ; j--)
            {
                // when binary value is 0 go left child, 1 go right child
                if(leftright[j] == 0) 
                {
                    if(j == 0) //when reach node number
                    {
                         tmp_node -> left_thread = 0; // not leaf anymore
                         tmp = tmp_node -> left_child;
                         tmp_node ->left_child = CreateNode(input[i-1]); // create and connect node
                         if(tmp_node ->left_child == NULL)
                         {
                             printf("failed to create node!\n");
                         }
                         tmp_node ->left_child->left_thread = 1; // connect thread
                         tmp_node ->left_child->right_thread = 1;
                         tmp_node ->left_child->left_child = tmp; // left child will be parent left child pointer
                         tmp_node ->left_child->right_child = tmp_node; // right child will be parent node's pointer value

                    }
                    else // not reach node number
                        tmp_node = tmp_node->left_child;
                }
                else if(leftright[j] == 1)
                {
                    if(j ==0)
                    { 
                        tmp_node -> right_thread = 0;
                        tmp = tmp_node -> right_child;
                        tmp_node ->right_child = CreateNode(input[i-1]);
                        if(tmp_node->right_child == NULL)
                        {
                            printf("failed to create node!\n");
                        }
                        tmp_node -> right_child -> left_thread =1;
                        tmp_node -> right_child -> right_thread =1;
                        tmp_node -> right_child -> left_child = tmp_node; // left child will be parent node's pointer value
                        tmp_node -> right_child -> right_child = tmp; // right child will be parent right child pointer
                    }
                    else
                    tmp_node = tmp_node->right_child;
                }
            }
            
        }
        
    }
}

threaded_ptr Insucc(threaded_ptr tree) // get head node poiner from input and return its succ
{
    threaded_ptr tmp;
    tmp = tree->right_child;
    if(!tree->right_thread) // not leaf
    {
        while(!tmp->left_thread)
             tmp = tmp->left_child;
    }
    return tmp;
}

void ThreadInorder(threaded_ptr tree) // get thread pointer from input and print node inorder 
{
    threaded_ptr tmp = tree;
    for(;;)
    {
        tmp = Insucc(tmp);
        if(tmp == tree) // when reach head of tree
        {
            break;
        }
        fprintf(fw,"%c ", tmp->data);
    }
    fprintf(fw,"\n");
}

void DeallocateTree(threaded_ptr tree) // deallocate of all nodes of tree by doing thread inorder travelsal
{
    threaded_ptr tmp1 = NULL;
    threaded_ptr tmp2 = tree;
    for(;;)
    {
        tmp1 = Insucc(tmp2);
        tmp2 = tmp1;
        if( tmp1 == tree)
        {
            free(tmp1);
            break;
        }
        free(tmp1);
    }
}


int main(void)
{

    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    threaded_ptr head = NULL; 
    int size;
    char * input;
    int i=0;
    //when fail to open file

    if(fr == NULL)
    {
       printf("failed to open input.txt\n");
       return -1;
    }

    if(fw == NULL)
    {
        printf("failed to open output.txt\n");
        return -1;
    }
    
    head = CreateNode(0);// make head
    head->right_child =head;

    fscanf(fr,"%d",&size);//get input size from file
    input = (char*)malloc(sizeof(char)*size+1);// dynamic allocation for input
    fgetc(fr); // remove \n
    while((input[i]=fgetc(fr))!=EOF ) // get input from file
    {
            if(input[i]!=' ') // except ' '
            {
                i++; 
            }
    }
    input[size] = '\0';//end of input
    //CreateTree
    InsertNode(head,input);
    ThreadInorder(head);
    // deallocation
    DeallocateTree(head);
    fclose(fw);
    fclose(fr);
    free(input);
    return 0;
}


