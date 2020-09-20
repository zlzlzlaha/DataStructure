#include <stdio.h>
#include <stdlib.h>

struct AVLNode;
typedef struct AVLNode * Position;
typedef struct AVLNode * AVLTree;
typedef int ElementType;
struct AVLNode
{
    ElementType element;
    AVLTree left;
    AVLTree right;
    int height;
};

FILE * fw = NULL;
int Height(Position P) // get node pointer from input and return node's height
{
    if(P == NULL) // end of leaf
    {
        return -1;
    }
    
    else
        return P->height;
}
int Max(int a, int b) // get two integer from input and return higher one
{
    if(a>=b)
        return a;
    else 
        return b;
}
Position FindNode(ElementType X,AVLTree T) // get tree's pointer and element X from input and find node return its pointer
{
    if(T == NULL)
    {
        return NULL;
    }
    else if(X > T->element) // if element is higer than node go to right node
    {
       return FindNode(X,T->right);
    }
    else if(X < T->element) // if element is lower than node go to left node
    {
        return FindNode(X,T->left);
    }
    else //when find node, return it's value
    {
        return T;
    }
}
Position SingleRotateWithLeft(Position K) // get node pointer from input and do right roatation in left child's subtree return node pointer which replaces K
{
    AVLTree K1 = K-> left;
    K->left = K1-> right;
    K1->right = K;

    // adjust nodes' height
    K->height = Max(Height(K->left),Height(K->right))+1;// get higher height of subtree and +1
    K1->height = Max(Height(K1->left),Height(K1->right))+1;
    return K1;
}
Position SingleRotateWithRight(Position K) // get node pointer from input and do left rotation in right child's subtree return node pointer which replaces K
{
    AVLTree K1 = K -> right;
    K->right = K1->left;
    K1->left =K;

    //adjust node's height
    K->height = Max(Height(K->right),Height(K->left))+1;
    K1->height = Max(Height(K1->right),Height(K1->left))+1;
    return K1;
}
Position DoubleRotateWithLeft(Position K) // get node pointer from input and do double rotation in left child's subtree return node pointer which replaces K
{
    AVLTree K1 = K->left;

    K->left = SingleRotateWithRight(K1);

    return SingleRotateWithLeft(K);
}
Position DoubleRotateWithRight(Position K) // get node pointer from input and do double rotation in right child's subtree return rotated node pointer which replaces K
{
    AVLTree K1 = K->right;

    K->right = SingleRotateWithLeft(K1);

    return SingleRotateWithRight(K);
}
AVLTree Insert(ElementType X, AVLTree T) // get element and tree pointer from input and insert element and check balance, return its node address to parent
{
   
    if(T == NULL) // insert new node
    {
        T = (AVLTree)malloc(sizeof(struct AVLNode));
        if(T == NULL) // when failed to allocate
        {
            printf("failed to make Node \n");
        }
        else 
        {
            T -> left = NULL;
            T -> right = NULL;
            T -> element = X;
            T -> height = 0;
        }
    }
    else if(X > T->element) //when data is higher one , go right subtree
    {
        T->right = Insert(X,T->right);// insert node first then check balance
        if(Height(T->right)-Height(T->left)==2) // when need rotation
        {
            if(T->right->element > X) //when RL
                T  = DoubleRotateWithRight(T); 
            else if(T->right->element < X) // when R
                T  = SingleRotateWithRight(T);
        }
    }
    else if(X< T->element) // when data is lower one, go left subtree
    {
        T->left =Insert(X,T->left);
        if(Height(T->left)-Height(T->right)==2) 
        {
            if(T->left->element > X) // when L
                T = SingleRotateWithLeft(T);
            else if(T->left->element < X) // when LR
                T = DoubleRotateWithLeft(T);
        }
    }
    T->height = Max(Height(T->left),Height(T->right))+1; //if height is changed, updade node's height 
    return T;
}
void PrintInorder(AVLTree T) // get Tree pointer from input and print node in inorder 
{
    if(T != NULL)
    {
        PrintInorder(T->left);
        fprintf(fw,"%d(%d) ",T->element,T->height);
        PrintInorder(T->right);
    }
}
void DeallocateTree(AVLTree T) // get Tree pointer and deallocate all nodes of tree
{
    if(T !=NULL)
    {
        DeallocateTree(T->left);
        DeallocateTree(T->right);
        free(T);
    }
}


int main(void)
{

    FILE * fr = fopen("input.txt","r");
    fw = fopen("output.txt","w");
    int input = 0;
    AVLTree root = NULL;

    //when fail to open file
    if(fr == NULL)
    {
        printf("failed to open input file\n");
        return -1;
    }
    
    if(fw == NULL)
    {
        printf("failed to open output file\n");
        return -1;
    }


    while(fscanf(fr,"%d ",&input)!=EOF) // until meet EOF, read file
    {
        if(FindNode(input,root)==NULL) // if data does not exist in tree, insert node
        {
            root = Insert(input,root);
            PrintInorder(root);
            fprintf(fw,"\n");
        }
        else // data already exists in tree
        {
            fprintf(fw,"%d already in the tree!\n",input);
        }
    }
    //deallocation
    DeallocateTree(root);
    fclose(fr);
    fclose(fw);
    return 0;
}
