#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE * fw = NULL;

typedef struct ListNode * List;
typedef struct HashTbl* HashTable;
typedef struct ListNode{
    int Element;
}ListNode;

typedef struct HashTbl{
    int TableSize;
    List TheLists;
}HashTbl;

//function declaration
HashTable CreateTable(int size);
void DeallocateTable(HashTable H);
void Insert(HashTable H, int value , int solution);
void Delete(HashTable H, int value , int solution);
int Find(HashTable H , int value, int solution);
void Print(HashTable H);
int Hash(int value, int size, int i, int solution);
int main(void)
{   
    FILE * fr = fopen("input.txt","r");
    fw = fopen("output.txt","w");
    int case_number;
    int ht_size;
    int i,input;
    char input_solution[10];
    int collision_solution;
    HashTable table = NULL;
    //whe failed to open file

    if(fr == NULL)
    {
        printf("failed to open input.txt");
        return -1;
    }

    if(fw == NULL)
    {
        printf("faield to open ouput.txt");
        return-1;
    }
    
    //get case number from input file
    fscanf(fr,"%d ",&case_number);

    for(i =0; i < case_number;i++)
    {
    
       
        fscanf(fr,"%s",input_solution); //get collision solution from input file
        fscanf(fr,"%d",&ht_size); // get hash table size from input 
        table = CreateTable(ht_size); // every case make ht_size table

        //determine hash function from input
        if(strcmp(input_solution,"Linear")==0)
        {
            collision_solution = 1;
            fprintf(fw,"Linear\n");
        }
        else if(strcmp(input_solution,"Quadratic") ==0)
        {
            collision_solution = 2;
            fprintf(fw,"Quadratic\n");
        }
        else if(strcmp(input_solution,"Double") == 0)
        {
            collision_solution = 3;
            fprintf(fw,"Double\n");
        }

        while((input = getc(fr)) != 'q' )
        {
            switch(input)
            {
                case 'i' :
                    fgetc(fr);//remove ' ' 
                    fscanf(fr," %d",&input); // get insert key
                    Insert(table,input,collision_solution);
                    break;
                case 'f': 
                    fgetc(fr); //remove ' '
                    fscanf(fr," %d",&input);
                    
                    if(Find(table,input,collision_solution) != -1) //when find same key in table
                    {
                        fprintf(fw,"%d index : %d\n",input,Find(table,input,collision_solution));
         
                    }
                    else // when can't fine key in table
                    {
                        fprintf(fw,"Not found\n");
                    }
                    
                    break;
                case 'd':  
                    fgetc(fr);// remove ' '
                    fscanf(fr," %d",&input);
                    Delete(table,input,collision_solution);
                    break;
                case 'p':
                    Print(table);
                    break;
            }
        }
        DeallocateTable(table); //when case is end, deallocate table
        fprintf(fw,"\n\n");
            
    }
    
    //deallocation
    
    fclose(fr);
    fclose(fw);
    return 0;
}

void Insert(HashTable H, int value , int solution) // get HashTable pointer , key value, collision solution from input ,and do insert operation
{
    int index;
    int addition = 0;

    index = Find(H,value,solution);
    if(index != -1) // when same key is in table
    {
        fprintf(fw,"Already exists\n");
        return; //not insert key
    }
   
    index = Hash(value,H->TableSize,0,0);
    while(( H->TheLists[index].Element != 0 ) &&  (H->TheLists[index].Element != -1)) // find location whicih is deleted or empty space
    {

        index = Hash(value,H->TableSize,++addition,solution);
    }
    H->TheLists[index].Element = value; //insert key in table
    fprintf(fw,"Inserted %d\n",value);
  }
void Delete(HashTable H, int value , int solution) //get HashTable pointer, key value, collision solution from input and delete key in table
{
    int index;
    index =Find(H,value,solution); //get deleted key index 
    if( index == -1) //can't find same key in table
    {
        fprintf(fw,"%d not exists\n",value);
        return;
    }
  
    H->TheLists[index].Element = -1; //delete key in table, -1 means deleted
    fprintf(fw,"Deleted %d\n",value);

}
int Find(HashTable H , int value, int solution) // get HashTable pointer , key value, collision solution from input, and return key index in table ,when cant find key, return -1
{
    int index;
    int addition =0;
    index = Hash(value,H->TableSize,addition,0);
     
    
    while((H->TheLists[index].Element != 0) || (H->TheLists[index].Element == -1)) // find key until meet empty space, when find key, deleted space regard as not empty sapce 
    {
        if(H -> TheLists[index].Element == value)// when find same ke in table
        {
            return index; //return key index
        }
        index = Hash(value,H->TableSize,++addition,solution);
        
        if(index == -1) // when can't solve collision problem
        {
            return -1;
        }
       
    }
    return -1; //when can't find key in table
    
}


void Print(HashTable H) // get HashTable pointer from input and print all key in talbe (deleted key also print 0)
{
    int i;
    for(i = 0 ; i < H->TableSize ; i ++) // print all keys in table
    {
        if(H->TheLists[i].Element == -1) // if key is deleted key, print in 0
        {
            fprintf(fw,"%d ",0);
        }
        else
        {
            fprintf(fw,"%d ",H->TheLists[i].Element);
        }

    }
    fprintf(fw,"\n");
}
int Hash(int value, int size, int i, int solution) // get key value , table size, function addition i, collision solution form input and return hash function value
{
    int hash_value;
    switch(solution)
    {
        case 0:
                hash_value = value%size;
                break;
        case 1:// linear
                if(i == size) // when can't solve collision problem
                {
                    hash_value =  -1;
                }
                else
                {
                    hash_value = (value%size+i)%size;
                }
                break;
        case 2://quadratic
                hash_value = (value%size+ i*i)%size;
                break;
        case 3://double
                hash_value = ((value%size)+i*(7-(value%7)))%size;
                break;
    }
    return hash_value;
}

HashTable CreateTable(int size) // get table size from input , make HashTable in size, and return HashTable pointer
{
    int i; 
    HashTable table = (HashTable)malloc(sizeof(HashTbl));
    table->TableSize = size;
    table->TheLists = (List)malloc(sizeof(ListNode)*size);

    for( i = 0 ; i < table->TableSize ; i ++) // initialzie all key empty 
    {
        table->TheLists[i].Element = 0;
    }

    return table;
}

void DeallocateTable(HashTable H) // get HashTable pointer from input and deallocate HashTable
{
    free(H->TheLists);
    free(H);
}

