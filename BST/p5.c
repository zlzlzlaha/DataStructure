#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode * Treeptr;
typedef struct TreeNode
{
    int value;
    Treeptr left;
    Treeptr right;
}TreeNode;


FILE * fw = NULL;
Treeptr FindMax(Treeptr Node) // get node pointer from input and find max value and return its pointer
{
    Treeptr tmp = Node; 
    if(tmp == NULL) // if there is not node ,return NULL
    {
        return NULL;
    }
    else     
    {
        while(tmp->right != NULL)  // right subtree is bigger 
        {
            tmp = tmp -> right;
        }
    }
    return tmp;
}


Treeptr InsertNode(int data, Treeptr root) // get data and tree root pointer from input and append node to tree
{
    Treeptr tmp = root;
    if (tmp  == NULL) // create new node
    {
        tmp = (Treeptr)malloc(sizeof(TreeNode));
        if(tmp == NULL) // failed to make node
        {
            printf("failed to make Node !\n");
        }
        tmp -> value = data;
        tmp -> left = NULL;
        tmp -> right = NULL;
    }

    else if(data > tmp->value)  // if data is bigger than node , add it right subtree
    {
        tmp->right = InsertNode(data, tmp->right);
    }
    else if(data < tmp-> value) // if data is smaller than node, add it left subtree
    {
        tmp->left =  InsertNode(data, tmp->left);
    }
    return tmp;
}

Treeptr DeleteNode(int data, Treeptr root) // get input from node data and node pointer from input and delete  data in tree , and return connected tree node pointer 
{
    Treeptr tmp = root;
    Treeptr delete_tmp = NULL;
    if(tmp == NULL)
    {
       fprintf(fw,"Deletion failed. %d does not exist.\n",data);
    }

    else if(data > root -> value) // if data is big , go to right subtree
    {
        tmp -> right = DeleteNode(data, tmp->right);
    }
    else if(data < root -> value) // if data is small go to left subtree
    {
        tmp -> left = DeleteNode(data, tmp->left);
    }
    else if(tmp->left && tmp->right) // when node has 2 children
    {
        delete_tmp = FindMax(tmp->left);
        tmp -> value = delete_tmp->value;
        tmp -> left = DeleteNode(tmp->value,tmp->left);
    }
    else // when node has 0,1 child
    {
        delete_tmp  = tmp;
        if(tmp->right == NULL)
        {
           tmp  = tmp -> left;
        }
        else if(tmp -> left == NULL)
        {
            tmp = tmp-> right;
        }
        free(delete_tmp);
    }

    return tmp;

}
Treeptr FindNode(int data, Treeptr root) // get data and root pointer from input and return found node pointer
{
    Treeptr tmp = root;
    if(tmp == NULL) // if there is not node , return null
    {
        return NULL;
    }
    else if(data < tmp -> value) // when data is samll, go to left subtree 
    {
        return FindNode(data, tmp->left);
    }
    else if(data > tmp -> value) // when data is big, go to right subtree
    {
        return FindNode(data, tmp->right);
    }
    else // when there is node return its pointer
        return tmp;
}
void PrintInorder(Treeptr root)//get tree root pointer from input and print its dat in inorder
{
    if(root !=NULL)
    {
        PrintInorder(root->left);
        fprintf(fw,"%d ",root->value);
        PrintInorder(root->right);
    }
}
void PrintPreorder(Treeptr root) // get tree root pointer from input and print its data in preorder
{
    if(root != NULL)
    {
        fprintf(fw,"%d ",root->value);
        PrintPreorder(root->left);
        PrintPreorder(root->right);
    }
}
void PrintPostorder(Treeptr root) // get tree root pointer from input and print its data in postorder
{
    if(root != NULL)
    {
        PrintPostorder(root->left);
        PrintPostorder(root->right);
        fprintf(fw,"%d ",root->value);
    }
}

void DeallocateTree(Treeptr root) // get root pointer from input and deallocate all nodes in tree
{
    if(root != NULL)
    {
        DeallocateTree(root -> left);
        DeallocateTree(root-> right);
        free(root);
    }
}





int main(void)
{   
    FILE* fr  = fopen("input.txt","r");
    fw  = fopen("output.txt","w");
    Treeptr root = NULL;
    char operator;
    int  input;
    //when failed to open files
    if(fr == NULL)
    {
        printf("failed to open input.txt\n");
    }

    if(fw == NULL)
    {
        printf("failed to open output.txt\n");
    }

    while((operator =fgetc(fr)) != EOF)
    {
        switch(operator)
        {
            case 'i': // insert node
                fgetc(fr); // remove ' ' 
                fscanf(fr,"%d",&input);
                if(FindNode(input,root)!=NULL) //when there is a same node in tree
                {
                    fprintf(fw,"%d already exists.\n",input);
                }
                else //when there is not same node 
                {
                     root = InsertNode(input,root);
                }
                break;
            case 'p'://print node
                operator = fgetc(fr);
                if(operator == 'i') // print inorder
                {
                    fprintf(fw,"pi - ");
                    PrintInorder(root);
                    fprintf(fw,"\n");
                }
                else if(operator == 'r') // print preorder
                {
                    fprintf(fw,"pr - ");
                    PrintPreorder(root);
                    fprintf(fw,"\n");
                }
                else if(operator == 'o') // print postorder
                {
                    fprintf(fw,"po - ");
                    PrintPostorder(root);
                    fprintf(fw,"\n");
                }
                break;

            case 'd': //delete node
                fgetc(fr); // remove ' '
                fscanf(fr,"%d",&input);
                root = DeleteNode(input,root);
                break;
            case 'f': // finde node
                fgetc(fr); // remove ' '
                fscanf(fr,"%d",&input);
                if(FindNode(input,root)!=NULL) // if node exists
                {
                    fprintf(fw,"%d is in the tree.\n",input);
                }
                else if(FindNode(input,root)==NULL) // there is not found node
                {
                    fprintf(fw,"%d is not in the tree.\n",input);
                }

                break;
        }
    }

    //deallocation

    fclose(fr);
    fclose(fw);
    DeallocateTree(root);
    return 0;
}

