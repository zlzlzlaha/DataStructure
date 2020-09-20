#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * fw = NULL; // to use fprintf in function
typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef struct
{
    int studentID;
    char* studentName;
}ElementType;


struct Node
{
    ElementType element;
    PtrToNode next;
};

List FindNode(ElementType X, List L) // Find same Node With X in List L and return Node pointer
{
    List tmp = L;
    
    while (tmp != NULL )
    {
        if(tmp->element.studentID == X.studentID)
        {
            return tmp;
        }
        tmp = tmp -> next;
    }
    return tmp; // Node not in list, return NULL
}

void PrintCurrentList(List L) // print Current List L
{
    List tmp = L->next; // Not print Head; 
    int single = 1;
    fprintf(fw,"Current List > ");
    
    while(tmp != NULL)
    {
        if(single == 1)  // there is a one node in List 
        {
            fprintf(fw,"%d %s",tmp->element.studentID,tmp->element.studentName);
            single = 0;
        }
        else // if Nodes are more than two, print '-' front of student informaiton
        {
            fprintf(fw,"-%d %s",tmp->element.studentID,tmp->element.studentName);    

        }
        tmp = tmp -> next; 
       
    }
    fprintf(fw,"\n");  
}
void Insert(ElementType X, List L) // Insert element  X to List L by sorting student ID
{
    List NewNode = NULL;
    List tmp = L;

    // distinguish already exist
    if(FindNode(X,L)==NULL) // not exist in List
    {  //make  NewNode with Data X
        NewNode = (List)malloc(sizeof(struct Node));
        NewNode->element.studentName = (char*)malloc(sizeof(char)*strlen(X.studentName)+1);
        NewNode->element.studentID = X.studentID;
        NewNode->next = NULL;
        strcpy(NewNode->element.studentName,X.studentName);
        while(tmp -> next != NULL &&( tmp->next->element.studentID < X.studentID)) // distingush location of Node 
        {
            tmp = tmp -> next ;
        }
        NewNode->next= tmp->next;
        tmp->next = NewNode;
        fprintf(fw,"Insertion Success : %d\n",NewNode->element.studentID);
        PrintCurrentList(L);
        
    }
    else if(FindNode(X,L) != NULL)// exist in List
    {
       fprintf(fw,"Insertion Failed. ID %d already exists.\n",X.studentID);
    }
   
 
}
void Delete(ElementType X, List L)// Delete X Node in List L
{
    List tmp = L;
    List remove = NULL;

    if(FindNode(X,L)!= NULL) // when exists in List
    {

        while((tmp->next != NULL) && (tmp->next->element.studentID != X.studentID))
        {
          tmp = tmp->next;
        }

         remove = tmp->next;
         tmp->next = remove->next;

         //Deallocation Node
         free(remove->element.studentName);
         free(remove);
         fprintf(fw,"Deletion Success : %d\n",X.studentID);
         PrintCurrentList(L);
    }
    else  // There is not Node in List
    {
        fprintf(fw,"Deletion Failed : Student ID %d is not in the list\n",X.studentID);
    }
}
void PrintList(List L) // Print all Nodes in List L
{
  List tmp = L->next;

  fprintf(fw,"-----LIST-----\n");
  while(tmp != NULL)
  {
    fprintf(fw,"%d %s\n",tmp->element.studentID,tmp->element.studentName);
    tmp = tmp -> next;
  }
  fprintf(fw,"--------------\n");
}

List MakeEmpty(void) // Make head, and return Head Node pointer
{
    List Head = (List)malloc(sizeof(struct Node));
    Head->element.studentName = (char*)malloc(sizeof(char));
    //Head information
    Head->element.studentID = -1;  
    Head->element.studentName = NULL;
    Head->next = NULL;
    return Head;
}

void DeleteList(List L) //deallocation all memory of List L
{
    List delete = L , tmp;

    while (delete != NULL)
    {
        tmp = delete->next ;
        free(delete->element.studentName);
        free(delete);
        delete = tmp;
    }

 
}

int main(void)
{
    int option;
    FILE * fread = fopen("input.txt","rt");
    fw = fopen("output.txt","wt");
    ElementType tmp_element;
    List Head = NULL;
    List tmp_list = NULL;
    char* tmp_name;


    // make head 
     Head = MakeEmpty();

    //when fail to open files
    if(fread == NULL)
    {
        printf("failed to open input.txt !\n");
        return -1;
    }
  
    if(fw == NULL)
    {
        printf("failed to open output.txt !\n");
        return -1;
    }
  
    // allocation for tmp student name

    tmp_element.studentName = (char*)malloc(sizeof(char)*31);// including \0
    tmp_name = (char*)malloc(sizeof(char)*31);
    //Get information from file
    while ((option = fgetc(fread)) != EOF)
    {
        switch(option)
        {
        case 'i':
             fscanf(fread,"%d",&tmp_element.studentID);
             fgetc(fread); // eliminate ' '
             
             // get student name information from file 
             fscanf(fread,"%s %s",tmp_element.studentName,tmp_name);
             strcat(tmp_element.studentName," " ); // get name with ' ' 
             strcat(tmp_element.studentName, tmp_name);
             Insert(tmp_element ,Head);
             break;
        case 'p':
             PrintList(Head);
             break;
            
        case 'f':
             fscanf(fread,"%d",&tmp_element.studentID);
             tmp_list = FindNode(tmp_element,Head);

             if(tmp_list != NULL && (tmp_list->element.studentID == tmp_element.studentID))
             {
                fprintf(fw,"Find Success : %d %s\n",tmp_list->element.studentID,tmp_list->element.studentName);
             }
             else 
             {
                fprintf(fw,"Find %d Failed. There is no student ID\n",tmp_element.studentID);
             }

             break;

       case 'd':    
      
            fscanf(fread,"%d",&tmp_element.studentID);
            Delete(tmp_element,Head); 
            break;
        }
       
   }
   //Deallocation memory 
    free(tmp_name);
    free (tmp_element.studentName);
    DeleteList(Head);
    fclose(fw);
    fclose(fread);
    return 0;
}
