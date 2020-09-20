#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Init(int * set, int size) // get set array and its size from input and initialize array
{
    int i = 0;
    for(i=0; i<size ; i++)
    {
        set[i] = 0;
    }
}

int Find(int *set, int element) // get set array and element data from input and find root of set and return its index
{
    while(set[element]>0) //until find root of set or initialized to 0 set
    {
        element = set[element];
    }
    return element; //return root index
}

void Union(int *set, int element1, int element2) // get set array and two element from input and do Set Union operation
{
    if(set[element1]>set[element2]) // if elemnet2 have higher height
    {
        set[element1] = element2; // make element1 be part of set of element1

    }
    else
    {
        if(set[element1]==set[element2]) // if they have same height 
        {    
            set[element1] = set[element1] -1; // height +1
        }
        set[element2] = element1; // join two set 
    }
}



int main(void)
{
    int * disset = NULL;
    int * garo_edge = NULL;
    int * sero_edge = NULL;
    int input;
    int i,j;
    int random;
    int tmp[2];// to store result of Find();
    int index;
    int set_number;
    FILE * fr = fopen("input.txt","r");
    FILE * fw = fopen("output.txt","w");

    srand((unsigned int)time(NULL));

    //when failed to open file

    if(fr == NULL)
    {
        printf("failed to open input.txt");
        return -1;
    }

    if(fw == NULL)
    {
        printf("failed to open output.txt");
        return -1;
    }

    //get input size from file
    fscanf(fr,"%d",&input);
    
    // dynamic allocation
    disset =(int*)malloc(sizeof(int)*input*input+1); // set start from 1 , so we not use disset[0]
    if(disset == NULL)
    {
        printf("failed to create disjoint set\n");
    }
    garo_edge = (int*)malloc(sizeof(int)*input*(input-1)); // make n*n-1 size of horizon line edge list
    if(garo_edge == NULL)
    {
        printf("failed to create garo_edge\n");
    }
    sero_edge = (int*)malloc(sizeof(int)*input*(input-1)); // make n*n-1 size of vertical line edge list
    if(sero_edge == NULL)
    {
        printf("failed to create sero edge\n");
    }

    Init(disset,input*input+1); //initialize  set number to 0
    set_number = input * input;// first , input * input number of sets exist
    // initialize  edge lists to 0
    Init(garo_edge,input*(input-1));
    Init(sero_edge,input*(input-1));


    while(set_number != 1) // when there is only one set, exit loop
    {
        random = rand()%2;
        if(random ==0) //when garo edge
        {
            random = rand()%(input*(input-1));// make random numbers 0~n*(n-1)-1
            //edge between set number and its just below set number
            tmp[0] = Find(disset,random+1);//horizon list index start from 0, need +1
            tmp[1] = Find(disset,random+input+1);
            if(tmp[0]!= tmp[1]) // if they dont have same root 
            {                
               Union(disset,tmp[0],tmp[1]); 
               set_number = set_number -1; // update set number
               garo_edge[random] = 1;//erase edege
            }
               
        }
        else // when sero edge
        {
            random = rand()%(input*(input-1));
            index =( (int)(random /(input-1)))*input+(int)random%(input-1)+1;// index is set number located in left side of vertical edge (vertical edge list index start from 0) 
            //edge between number and its right side number
            tmp[0] = Find(disset,index);
            tmp[1] = Find(disset,index+1); 
            if(tmp[0]!=tmp[1])
            {
                Union(disset,tmp[0],tmp[1]);
                set_number = set_number -1;
                sero_edge[random] =1;
            }
            
        }
   }
    //print maze
    for(i=0; i<input;i++) // when edge list number is 0 print edge or not , print " "
    {
        
        if(i==0) // print highest wall
        {
            for(j=0;j<input;j++)
            {
                fprintf(fw,"+-");
            }
            fprintf(fw,"+\n");
        }

        if(i==0)// to make entrance of maze     
        {
            fprintf(fw,"  ");
        }
        else//if not entrance, print left side of horizon wall
        {
            fprintf(fw,"| ");
        }
        
        for(j=0; j<input-1; j++) // print vertical lines of wall
        {

             if(sero_edge[j+i*(input-1)] == 0) 
             {
                 fprintf(fw,"|");
             }
             else // when edge is erased
             {
                 fprintf(fw," ");
             }
             fprintf(fw," ");
      
        }
        if(i == input-1) // to make exit of make
        {  
            fprintf(fw," \n");
        }
        else // if it is not exit ,print right size of verical wall
        {
            fprintf(fw,"|\n");
        }
        
       if(i!=input-1) // when it is not lowest horizon line wall
       {
              for(j=0; j<input; j++) // print horizon lines of wall
              {
                 fprintf(fw,"+");
                 if(garo_edge[j+i*input] == 0)
                 {
                    fprintf(fw,"-");
                 }

                 else//when edge is erased
                 {
                     fprintf(fw," ");
                 }
             }
             fprintf(fw,"+");
       }
       else //when it is lowest horizon line wall
       {
               for(j=0; j<input ;j++)
               {
                    fprintf(fw,"+-");
               }
               fprintf(fw,"+");
       }
      fprintf(fw,"\n");
    }
    //deallocation
    fclose(fr);
    fclose(fw);
    free(disset);
    free(sero_edge);
    free(garo_edge);
    return 0;
}
