#include <stdio.h>
#include <stdlib.h>

#define INF 99999

FILE * fw = NULL;
//arrays to use in fuctions
int ** adj_matrix = NULL;
int * previous = NULL;
int * distance = NULL;
typedef struct Node{
    int key;
    int weight;
}Node;

typedef struct Heap{
    int capacity;
    int size;
    Node * element;
}Heap;

//fuction definition
void DeallocateHeap(Heap * MinHeap);
void DecreasePriority(Heap * MinHeap, int vertex, int distance);
int** CreateWeight( int size);
int FindNode(int size, int* arr,int value);
Heap* CreateMinHeap(int heapsize);
void Insert(Heap* MinHeap, int vertex, int distance );
int DeleteMin(Heap* MinHeap);
void PrintShortestPath(int * key , int size ,int vertex);
void Dijkstra(Heap *MinHeap, int * vertex_nodes ,int start_vertex);
void PercUp(Heap * MinHeap, int index);
int main(void)
{


    FILE * fr = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    Heap * MinHeap = NULL;
    int * vertexs = NULL;
    int start_vertex, destin_vertex;
    int  node_number = 0, edge_number =0;
    int input,input_, wt;
    int i;


    //when failed to open file
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
    // get node_number, edge_number from file
    while( (input = fgetc(fr)) != '\n')
    
    {
        if(input == ' ')
        {
            node_number = node_number + 1;
        }
    }
    while((input = fgetc(fr)) != '\n')
    {
        if(input == ' ')
        {
            edge_number = edge_number + 1;
        }
    }
    // node, edge number =  ' ' number +1 ;
    node_number ++;
    edge_number ++;
    fseek(fr,0L,SEEK_SET); // set file cursor to first of file

    // dynamic allocation from input
    vertexs = (int *)malloc(sizeof(int)*node_number); // store key informations
    adj_matrix = CreateWeight(node_number);
    MinHeap = CreateMinHeap(node_number);

    for(i=0; i<node_number;i++) // get keys from file
    {
        fscanf(fr,"%d ",&vertexs[i]); 
    }
    for(i=0; i<edge_number;i++) // get edges from file
    {
        fscanf(fr,"%d-%d-%d",&input,&input_,&wt);
        adj_matrix[FindNode(MinHeap->capacity-1,vertexs,input)][FindNode(MinHeap->capacity-1,vertexs,input_)] = wt;  // Minheap element index start from 1 
    }

    fscanf(fr,"%d %d",&start_vertex,&destin_vertex); //get start, destination vertex from file
   
    Dijkstra(MinHeap,vertexs,start_vertex);
    //print path
    if(distance[FindNode(MinHeap->capacity-1,vertexs,destin_vertex)]==INF) // when there is a no path between start and end
    {
        fprintf(fw,"no path\n");
    }
    else
    {
        PrintShortestPath(vertexs, MinHeap->capacity-1 ,destin_vertex);
        fprintf(fw,"\n");
    }
    // deallocation 
    fclose(fw);
    fclose(fr);
    free(distance);
    free(previous);
    for(i = 0 ; i < MinHeap->capacity-1;i++)
    {
        free(adj_matrix[i]);
    }
    free(adj_matrix);
    DeallocateHeap(MinHeap);
    return 0;
}

void DeallocateHeap(Heap* MinHeap) //get MinHeap pointer from input and deallcate Min heap
{
   free(MinHeap->element);
   free(MinHeap);
}

int** CreateWeight(int size)//get arr size from input, and make size*size weight matrixt then return matrix array pointer
{
    int i,j;
    int** weight_matrix = NULL;
    
    weight_matrix = (int**)malloc(sizeof(int*)*size);
    for(i = 0 ; i < size;  i++)
    {
        weight_matrix[i] = (int*)malloc(sizeof(int)*size);
    }
    
    for(i = 0 ; i < size ; i++)
    {
        for(j = 0 ; j < size ; j++)
        {
            weight_matrix[i][j] = INF; // initialize in INF
        }
    }
    return weight_matrix;
}

int FindNode(int size, int * arr , int value ) // get key size ,nodes key array ,node key value from input  and return index matched with key value 
{
    int index =0, i;
    for(i =0 ; i< size ; i++)
    {
        
        if(arr[i] == value)
        {
            index = i;
        }
        
    }
    return index;
}

Heap* CreateMinHeap(int heapsize) //get heapsize from input and initialize heap  return its pointer
{
    Heap * NewHeap = (Heap*)malloc(sizeof(Heap));
    NewHeap -> capacity = heapsize+1;  // not to use index 0 ,so need +1 space 
    NewHeap -> size = 0;
    NewHeap -> element = (Node*)malloc(sizeof(Node)*(heapsize+1));
    return NewHeap;
}

void Insert(Heap * MinHeap, int vertex, int distance) // get heap pointer, vertex key, distance weight from input,and do insert operation in minheap
{
    int i;

    if(MinHeap->size >= MinHeap->capacity) // when heap is full
    {
        printf("heap is full!\n");
        return;
    }
    // percolating up
    for(i = ++ MinHeap->size ; (MinHeap->element[i/2].weight > distance) && (i/2 != 0) ; i/= 2) // 0 index is not used 
    {
        MinHeap->element[i].weight = MinHeap->element[i/2].weight;
        MinHeap->element[i].key = MinHeap->element[i/2].key;
    }
    MinHeap->element[i].key = vertex;
    MinHeap->element[i].weight = distance;
}


int DeleteMin(Heap* MinHeap) // get minheap pointer from input and delete highest priority node, and return its key value 
{
    int i, child;
    int last, min_key, last_key;

    min_key = MinHeap->element[1].key;
    last_key = MinHeap->element[MinHeap->size].key;
    last = MinHeap->element[MinHeap->size--].weight;

    //percolating down
    for(i = 1; i*2 <= MinHeap->size; i = child)
    {
        child = i*2; // child is left child
        if((child != MinHeap->size) && (MinHeap->element[child+1].weight < MinHeap->element[child].weight )) // when right child is smaller than left child
        {
            child ++; // child is right child
        }
        if(last > MinHeap->element[child].weight) 
        {
            MinHeap->element[i].weight = MinHeap->element[child].weight;
            MinHeap->element[i].key = MinHeap->element[child].key;
        }
        else 
            break;
    }
    MinHeap->element[i].weight = last;
    MinHeap->element[i].key = last_key;
    return min_key;
}

void PrintShortestPath(int * key, int size ,int vertex) // get key array, key size,  vertex from input and , print shortest path to destnation in recursive way
{
    if(previous[FindNode(size,key,vertex)]!= -999) //when vertex is start 
    { 
        PrintShortestPath(key,size,previous[FindNode(size,key,vertex)]);
    }
    fprintf(fw,"%d ",vertex);
}

void PercUp(Heap * MinHeap, int index) // get MinHeap pointer, start node index from input , and do percolating up operation
{
    int i;
    int tmp, tmp_key;

    for(i = index ; (MinHeap->element[i/2].weight > MinHeap->element[i].weight ) && ((i/2) != 0); i/=2) // when parent is bigger than child
    {
        //swap parent and child Node
        tmp = MinHeap->element[i].weight;
        tmp_key = MinHeap->element[i].key;

        MinHeap->element[i].weight = MinHeap->element[i/2].weight;
        MinHeap->element[i].key =  MinHeap->element[i/2].key;
        MinHeap->element[i/2].weight = tmp;
        MinHeap->element[i/2].key = tmp_key;
    }
}
void DecreasePriority(Heap * MinHeap, int vertex ,int distance) // get heap pointer, vertex key, distace weight from input , change node weight mated with vertex , rearrange heap 
{
    int i;
    for(i = 1 ; i <= MinHeap->size ; i++) // change node's weight matched with vertex key
    {
        if(MinHeap->element[i].key == vertex)
        {
            MinHeap->element[i].weight = distance;
            break;
        }
    }
    PercUp(MinHeap,i);

}

void Dijkstra(Heap *MinHeap, int * vertex_nodes ,int start_vertex) // get Minheap pointer , key array, start vertex from input , find shortest path from start to destination
{
    int i;
    int index, d_vertex;
    
    //dynamic allocation
    distance = (int*)malloc(sizeof(int)*MinHeap->capacity);
    previous = (int*)malloc(sizeof(int)*MinHeap->capacity);
    
    // initialize arrays
    for(i = 0 ; i< MinHeap->capacity-1 ; i++)
    {
        distance[i] = INF;   
        previous[i] = -999; // -999 means null
    }
    index = FindNode(MinHeap->capacity-1,vertex_nodes,start_vertex); // start vertex index in vertex_nodes array
    distance[index] = 0; // start vertex distance  is 0

    for(i = 0 ; i < MinHeap->capacity-1 ; i++) // initialize distance adjacent with start vertex and initialize all previous vertex in start vertex
    {
        
        if (i != index) //when vertex is not itself
        {
            if(adj_matrix[index][i]!=INF) // when two vertex is connected 
            {
                distance[i] = adj_matrix[index][i];
            }
            previous[i] = start_vertex;
        }
    }
    for(i = 0 ; i < MinHeap->capacity-1 ; i++) // insert all vertex in heap
    {
        Insert(MinHeap,vertex_nodes[i],distance[i]);
    }
    while (MinHeap->size != 0)
    {
        d_vertex = DeleteMin(MinHeap);
        index = FindNode(MinHeap->capacity-1,vertex_nodes,d_vertex);

        // get shortest path to vertex which is adjacent with d_verte`:sx
        for(i =0 ;i <MinHeap->capacity-1 ; i++)
        {
           if(i != index ) // when vertex is not d_vertex itself
           {

                if(adj_matrix[index][i]!=INF) //when two vertex is connected
                {                 
                    if(distance[index] + adj_matrix[index][i] < distance[i]) // when find short path
                    {
                        //update short path
                        distance[i] = distance[index] + adj_matrix[index][i];
                        previous[i] = vertex_nodes[index];
                        DecreasePriority(MinHeap,vertex_nodes[i],distance[i]);
                    }
                }
           }
        }
       
    }

    
}

