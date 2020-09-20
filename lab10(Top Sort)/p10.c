#include <stdio.h>
#include <stdlib.h>

FILE * fw = NULL;
typedef struct _Graph
{
    int size;
    int* node;
    int** matrix;
}_Graph;

typedef struct _Queue
{
    int* key;
    int first;
    int rear;
    int qsize;
    int max_queue_size;
}_Queue;
typedef _Graph * Graph;
typedef _Queue * Queue;

// fuction declaration
int ** CopyEdge(Graph G);
int CheckInDegree(Graph G,int ** edge ,int vertex);
int FindNode(int nodes[],int number, int arr_size);
Graph CreateGraph(int arr_size);
void InsertEdge(Graph G, int a, int b);
void Topsort(Graph G, int ** edge);
Queue MakeNewQueue(int x);
void Enqueue(Queue Q, int x);
int Dequeue(Queue Q);
void PrintMatrix(Graph G);
void DeallocateGraph(Graph G);
void DeallocateQueue(Queue Q);

int main(void)
{

    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    int input_size = 0;
    int edge_size = 0;
    int input,input_;
    int i; 
    int ** edge = NULL; // store graph matrix not to change origin of graph matrix
    Graph  graph = NULL;
    //when fail to open files
    if(fr == NULL)
    {
        printf("failed to open input.txt\n");
        return -1;
    }

    if(fw == NULL)
    {
        printf("failed to pen output.txt\n");
        return -1;
    }
    // get input number size
    while((input = fgetc(fr))!= '\n')
    {
        if(input == ' ')
        {
            input_size++;
        }
    }
    // get edge number size
    while((input = fgetc(fr))!= EOF)
    {
        if(input == '-')
        {
            edge_size++;
        }
    }
    input_size++; // size of input number = size of ' ' +1 

    graph = CreateGraph(input_size); //create graph
    fseek(fr,0L,SEEK_SET);// set cursor in first of file
    //store input numbers in graph
    for(i=0; i<input_size;i++)
    {
        fscanf(fr,"%d ",&graph->node[i]);
    }
    // store edge in graph
    for(i=0; i < edge_size ; i++)
    {
        fscanf(fr,"%d-%d ",&input,&input_);
        InsertEdge(graph,input,input_);
    }
    edge = CopyEdge(graph); 
    PrintMatrix(graph);
    Topsort(graph,edge);
    // deallocation

    for(i =0 ; i< graph->size ; i++)
    {
        free(edge[i]);
    }
    free(edge);
    DeallocateGraph(graph);
    fclose(fr);
    fclose(fw);
    return 0;
}

void DeallocateQueue(Queue Q) // get Queue pointer from input and dealloate Queue 
{
    free(Q->key);
    free(Q);
}
int FindNode(int nodes[] , int number ,int arr_size) // get graph nodes pointer , node number , node array size from input , and return node index which is matched with number
{
    int index ,i;
    
    for(i=0; i<arr_size;i++)
    {
        if(nodes[i] == number) // when find same number in nodes
        {
            index = i;
        }
    }
    return index ;
}

Graph CreateGraph(int arr_size) // get node size from input and create new graph with node size , return its pointer
{
    int i;
    Graph New_Graph = (Graph)malloc(sizeof(struct _Graph));
    if(New_Graph == NULL)
    {
        printf("failed to create Graph\n");
        return NULL;
    }
    New_Graph->size = arr_size;
    New_Graph->node = (int*)malloc(sizeof(int)*arr_size);
    if(New_Graph->node == NULL)
    {
        printf("failed to create nodes\n");
        return NULL;
    }

    // matrix dynamic allocation 
    New_Graph->matrix = (int**)malloc(sizeof(int*)*arr_size);
    if(New_Graph->matrix == NULL)
    {
        printf("faield to create Matrix\n");
        return NULL;
    }
    for(i=0; i<arr_size ; i++)
    {
        New_Graph->matrix[i] = (int*)calloc(arr_size,sizeof(int));// '0' means that edge does not exist between nodes 
    }
    return New_Graph;

}

void InsertEdge(Graph G, int a, int b) // get Graph pointer , two node numbers from input , and insert edge between a and b
{
    
    G->matrix[FindNode(G->node,a,G->size)][FindNode(G->node,b,G->size)] = 1; // '1' means edge exists between two nodes
}


Queue MakeNewQueue(int x) //  max queue size from input and create new queue and return its pointer
{
   Queue NewQueue = (Queue)malloc(sizeof(struct _Queue));
   if(NewQueue == NULL)
   {
       printf("failed to create NewQueue \n");
       return NULL;
   }
   NewQueue -> key = (int*)malloc(sizeof(int)*x);
   if(NewQueue -> key == NULL)
   {
       printf("faield to create key array\n");
       return NULL;
   }
   NewQueue->first = 0;
   NewQueue->rear = -1;
   NewQueue->qsize = 0;
   NewQueue->max_queue_size = x;
   return NewQueue;
}

void Enqueue(Queue Q, int x) // Queue pointer and element X from input, do enqueue operation 
{
    if(Q->qsize < Q->max_queue_size)// when Queue is not full
    {
        Q->key[++Q->rear] = x;
        Q->qsize ++;
     }
}

int Dequeue(Queue Q) // Queue pointer from input , do dequeue operation and return its value
{
    if(Q->qsize > 0) // when Queue is not empty
    {
        Q->qsize --;
        return Q->key[Q->first++];
    }

    printf("Queue is empty return -999\n"); 
    return -999;
}

void PrintMatrix(Graph G) // Graph pointer from input and print matrix edge information
{  
   int i,j;

   fprintf(fw,"Adjacency matrix\n");
   fprintf(fw,"  ");
   for(i = 0; i< G->size ;i++) 
   {
        fprintf(fw,"%d ",G->node[i]);
   }
   fprintf(fw,"\n");
   for(i = 0 ; i < G->size ; i++)
   {
       fprintf(fw,"%d ",G->node[i]);
       for(j = 0 ; j < G->size ; j++)
       {
            fprintf(fw,"%d ",G->matrix[i][j]); //print edges information 
       }
       fprintf(fw,"\n");
   }
}


int CheckInDegree(Graph G,int ** edge, int vertex) //Graph pointer, edge matrix array ,vertex number from input then return sum of vertex's indegree
{
    int i;
    int sum =0;
    for(i=0; i < G->size; i++) // get sum of vertex's indegree
    {
       sum = sum + edge[i][vertex];
      
    }
    return sum;
}

int** CopyEdge(Graph G) // Graph pointer from input and copy graph matrix in edge array , then return edge array pointer
{
    int i ,j;
    int ** edge = (int**)malloc(sizeof(int*)*G->size);
    for(i = 0 ; i < G->size ; i++)
    {
        edge[i] = (int*)malloc(sizeof(int)*G->size);
    }

    for(i =0; i< G->size ; i++)
    {
        for(j = 0; j < G->size ; j++)
        {
            edge[i][j] = G->matrix[i][j];
        }
    }

    return edge;
}
void Topsort(Graph G,int ** edge)// graph pointer, edge matrix from input and do topological sort
{
    Queue Q  = NULL;
    // create tmp arrays
    int * zero_indegree = (int*)malloc(sizeof(int)*G->size); // to distinguish wich vertex should be enqueued
    int * indegree_sum = (int*)malloc(sizeof(int)*G->size); // to store vertex's indegree sum 
    int zero_indegree_size = 0;
    int i,j,tmp,min,vertex;

    Q = MakeNewQueue(G->size);
    fprintf(fw,"\n\nTopSort Result :");
    
    for(i=0; i<G->size;i++) // find vertex which indegree  is zero 
    {
        indegree_sum[i] = CheckInDegree(G,edge,i); // get sum of vertex's indegree
        if(indegree_sum[i] == 0)
        {
            zero_indegree[zero_indegree_size++] = G->node[i]; // store zero indegree vertex in zero_indegree array 
        }
    }
    
    for(i =0; i<zero_indegree_size; i++) // sort same prioirty vertexs in smallest order
    {
        min = i;      
        for(j = i+1; j<zero_indegree_size; j++)
        {
            if(zero_indegree[min] > zero_indegree[j])
            {
                min = j;
            }
        }
        tmp = zero_indegree[i];
        zero_indegree[i] = zero_indegree[min]; 
        zero_indegree[min] = tmp;
    }
   
    for(i = 0 ; i<zero_indegree_size ; i++) // enqueue vertexs which sum of indegree is zero 
    {
        Enqueue(Q,zero_indegree[i]);
        indegree_sum[FindNode(G->node,zero_indegree[i],G->size)] = -1; // this is for already enqueued vertex not being enqueued anymore
    }
   
    while(Q->qsize != 0) // until all nodes are sorted
    {
        zero_indegree_size = 0; 
        vertex = FindNode(G->node,Dequeue(Q),G->size); //dequeue vertex
        fprintf(fw," %d",G->node[vertex]);
        for(i = 0; i < G->size ; i++)
        {
            edge[vertex][i] = 0;
        }
      
        for(i=0; i<G->size;i++) //find vertex that sum of indegree is zero
        {
            if(indegree_sum[i] != -1) // when vertex is not already enqueued
            {
                indegree_sum[i] = CheckInDegree(G,edge,i);
                if(indegree_sum[i] == 0)
                {
                    zero_indegree[zero_indegree_size++] = G->node[i];
                }
            }
        }
       
        for(i =0; i<zero_indegree_size; i++) // sort zero indegree vertexs in smallest order
        { 
            min = i;
            for(j = i+1; j<zero_indegree_size; j++)
            {
                if(zero_indegree[min] > zero_indegree[j])
                {
                    min = j;
                }
            }
            tmp = zero_indegree[i];
            zero_indegree[i] = zero_indegree[min]; 
            zero_indegree[min] = tmp;
        }

        for(i = 0 ; i<zero_indegree_size;i++) // enqueue zero indegree vertexs
        {          
            Enqueue(Q,zero_indegree[i]);
            indegree_sum[FindNode(G->node,zero_indegree[i],G->size)] = -1;
        }
    }
    fprintf(fw,"\n");
    //deallocation
    free(zero_indegree);
    free(indegree_sum);
    DeallocateQueue(Q);
}

void DeallocateGraph(Graph G) //Graph pointer from input and deallocate graph
{
    int i;
    free(G->node);        
    for(i = 0 ; i< G->size ; i++)
    {
        free(G->matrix[i]);
    }
    free(G->matrix);
    free(G);
    
}
