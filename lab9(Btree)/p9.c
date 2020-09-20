#include <stdio.h>
#include <stdlib.h>

#define ORDER 3

FILE * fw = NULL;

typedef struct B_node
{
    int n_keys;
    struct B_node * child[ORDER];
    int key[ORDER-1];
}B_node;

//fuction decalare
B_node* Create_Node(void);
void Add_Key(int new_key, B_node* new_child, B_node* ptr);
B_node* Split(int new_key , int key_index, B_node * node_child, B_node* ptr);
B_node* Insert(int element, B_node * ptr);
B_node* Root_Insert(int element, B_node * ptr);
void Inorder(B_node *ptr);
void Deallocate_Tree(B_node *ptr);    
int main(void)
{
    FILE * fr = fopen("input.txt","r");
    fw= fopen("output.txt","w");
    int input;
    B_node * Btree = NULL;
    int i =0;

    //when fail to oepn files
    if(fr == NULL)
    {
        printf("failed to open input file\n");
        return -1;
    }
    if(fw ==NULL)
    {
        printf("failed to open output file\n");
        return -1;
    }
    // make Btree root
    Btree = Create_Node();    
    while((input=fgetc(fr))!=EOF) //get file input util meet EndOfFile
    {
        switch(input)
        {
            case 'i':
                fgetc(fr);//eliminate ' '
                fscanf(fr,"%d",&input);
                Btree = Root_Insert(input,Btree);
                break;
            case 'p':
                Inorder(Btree);
                fprintf(fw,"\n");
                break;
        }
    }
    //deallocation
    Deallocate_Tree(Btree);
    fclose(fr);
    fclose(fw);
    return 0;

}

B_node* Create_Node(void) // create new B tree node,initialize it and return node's pointer
{
    int i;
    B_node* new_node = (B_node*)malloc(sizeof(struct B_node));
    new_node-> n_keys = 0;
    for(i =0; i<ORDER ; i++)
    {
        new_node -> child[i]= NULL;  
    }
 
    return new_node;
}

// 'Add_Key'  is used in case that  node is not overflow
void Add_Key(int new_key, B_node * new_child, B_node* ptr) // get new key value and splited child pointer, parent node pointer 'ptr' from input and add them in parent node 'ptr'
{
    int i,j;
    for(i=0;i<ptr->n_keys;i++)
    {
         if(ptr->key[i]>new_key)
         {
             for(j=ptr->n_keys; j>i ; j--) //push origin keys to right side
             {
                ptr->key[j] = ptr->key[j-1];
                ptr->child[j+1] = ptr->child[j];
             }
             break;
          }
    }
    // add new node and splited child 
     ptr->key[i] =new_key;
     ptr->child[i+1] = new_child;
     ptr->n_keys++;  
}


// Split is used in case that node is overflow and add new key and split it into two node
B_node* Split(int new_key , int key_index,B_node * new_child, B_node* ptr) //get key value, index that is location of new_key ,splited child pointer, parent node pointer from input return splited right side node pointer
{
    int tmp[ORDER]; //need tmp array to store overflow keys and child
    B_node * tmp_child[ORDER+1];
    B_node * new_node = NULL;
    int i;
    int middle;
    // make array for key, child that are overflow and sort them 
    for(i=0; i < key_index; i++)
    {
        tmp[i] = ptr->key[i];
        tmp_child[i] = ptr->child[i];
    }
    // add new key 
    tmp[key_index] = new_key;
    tmp_child[key_index] = ptr->child[i];
    tmp_child[key_index+1] = new_child; // add splited children pointer
    
    for(i=key_index+1 ; i<ORDER ; i++)
    {
       tmp[i] = ptr->key[i-1];
       tmp_child[i+1] = ptr->child[i];
    }
    // split into two nodes
       
    new_node = Create_Node(); // when a node is splited into two node , store right side of node's key and child  in new_ndoe
    
    if(new_node == NULL)
    {
        printf("failed to create new node \n");
    }
    middle = (ORDER-1)/2;
    new_node->key[ORDER-2] = tmp[middle];// store middle key in new_node's not used key index
    new_node->n_keys = ORDER - middle -1;
    ptr -> n_keys = middle; // left side node has middle size keys  
    for(i=0; i < ptr->n_keys ; i++) // initialize left side node
    {
        ptr-> key[i] = tmp[i];
        ptr-> child[i] = tmp_child[i];
    }
    ptr->child[i] = tmp_child[i];
    for(i= 0; i< new_node->n_keys ; i++) // initialize right side node
    {  
       new_node-> key[i] = tmp[i+middle+1];
       new_node-> child[i] = tmp_child[i+middle+1]; 
    }
    new_node->child[i] = tmp_child[i+middle+1];
      
    return new_node;// return right side node , left side node is already connected with parent
}

B_node*  Insert(int element, B_node * ptr) // get key value and Btree node pointer from input and insert element at Btree ,return splited child pointer 
{
    int i,j;
    B_node* child_overflow = NULL;
    int key_index;
    if(ptr->child[0] == NULL) //when node is a leaf node
    {
        if(ptr->n_keys < ORDER-1) // when leaf is not overflow
        { 
            Add_Key(element,NULL,ptr);
            return NULL;
        }

       else // when leaf node is overflow
       {
            for(i=0 ; i < ptr->n_keys ; i++)
            {
                if(element < ptr->key[i])
                {
                    break;
                }
            }
             key_index = i;
             return Split(element,key_index,NULL,ptr);
             
       }
    }
    else if(ptr ->child[0]!= NULL) // when node is not a leaf node
    {
        for(i =0; i< ptr->n_keys ; i++) // find child index to insert element
        {
            if(element < ptr->key[i])
            {
                break;
            }
        }
       key_index = i;
       child_overflow = Insert(element,ptr->child[i]);

       if(child_overflow == NULL)// when child is not overflow
       {
           return NULL;
       }
       else //when child is overflow 
       {
            if(ptr->n_keys < ORDER-1) // when this node is not overflow
            {
                 Add_Key(child_overflow->key[ORDER-2],child_overflow,ptr);
                 return NULL;
            }
            else // when this node is overflow
            {
                return Split(child_overflow->key[ORDER-2], key_index, child_overflow, ptr);
            }
       }
    }

}
B_node* Root_Insert(int element, B_node * ptr) // get element key and node pointer from input and insert key in tree ,return root's pointer
{
    B_node*  new_root = NULL;
    B_node*  overflow = NULL;

    overflow = Insert(element,ptr);
    
    if(overflow != NULL) // when root is overflow
    {
        new_root = Create_Node(); // to make new root
        if(new_root == NULL)
         {
             printf("failed to make node");
         }
        new_root->child[0] = ptr;// left child(origin root)
        new_root->child[1] = overflow;//right child
        new_root->key[0] = overflow->key[ORDER-2];
        new_root->n_keys = 1;
        return new_root; // return new root pointer
    }
    
    return ptr; // when root is not over flow, return origin rot pointer
   
}
void Inorder(B_node *ptr)//get B_tree node from input and pirnt it inorder 
{
    int i;
    if(ptr != NULL)
    {
        for(i=0; i< ptr->n_keys ; i++)
        {
            Inorder(ptr->child[i]);
            fprintf(fw,"%d ",ptr->key[i]);
        }
        Inorder(ptr->child[i]);
    } 
}

void Deallocate_Tree(B_node *ptr) // get B tree pointer from input and dellocate all nodes's of tree in postorder way
{
    int i;
    if(ptr != NULL)
    {
        for(i=0; i <= ptr->n_keys ; i++)
        {
           Deallocate_Tree(ptr->child[i]);
        }
           free(ptr);
    }
}
