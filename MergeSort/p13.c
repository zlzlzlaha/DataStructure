#include <stdio.h>
#include <stdlib.h>

FILE * fw = NULL;


//function declaration
void MergeSort(int * array , int * tmp_array, int left, int right);
void Merge(int * array, int * tmp_array, int lpos, int rpos, int rightend);


int main(void)
{

    FILE * fr = fopen("input.txt","r");
    fw = fopen("output.txt","w");
    int input_size;
    int i;
    int* input_array;
    int* tmp_array;
    //when failed to open files
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


    fscanf(fr,"%d",&input_size); // get input size from file
    //dynamic allocation for arrays in input_ size
    input_array = (int*)malloc(sizeof(int)*input_size);
    tmp_array = (int*)malloc(sizeof(int)*input_size);
    // get numbers from file
    fprintf(fw,"input :\n");
    for(i = 0 ; i < input_size ; i++)
    {
       fscanf(fr,"%d",&input_array[i]);
       fprintf(fw,"%d ",input_array[i]);
    }
    fprintf(fw,"\n\n");
    // merge sort
    fprintf(fw,"recursive : \n");
    MergeSort(input_array,tmp_array,0,input_size-1);
   
    //deallocation
    fclose(fw);
    fclose(fr);
    free(tmp_array);
    free(input_array);
    return 0;
}


void MergeSort(int * array , int * tmp_array, int left, int right) // get elements array pointer , tmp array pointer, left, rignt end size of index from input and do merge sort recursive way
{
    int center;
    if(left < right) // when splited element is not only one
    {
        center = (left + right) /2;
        // split array
        MergeSort(array, tmp_array, left, center);
        MergeSort(array, tmp_array, center +1 , right);
        Merge(array,tmp_array, left ,center +1 , right); // combine two splited arrays
    }
}

/*
get elemetns array pointer, tmp array pointer , left array's start index, righg array's start and end idnex from input
combine two splited arrays by sorting
*/

void Merge(int * array, int * tmp_array, int lpos, int rpos, int rightend) 
{
    int index = lpos;
    int leftend = rpos -1;
    int size = rightend - lpos +1; // number of elements in array
    int start = lpos; 
    int i;

    while(lpos <= leftend && rpos <= rightend) // until right or left array is not moved to tmp array fully
    {
        if(array[lpos] <= array[rpos])
        {
            tmp_array[index++] = array[lpos++]; 
        }
        else
        {
            tmp_array[index++] = array[rpos++];
        }
    }

    // when right or left array's elements are exist, add to tmp array
    while(lpos <= leftend)
    {
        tmp_array[index++] = array[lpos++];
    }
    while(rpos <= rightend)
    {
        tmp_array[index++] = array[rpos++];
    }
    
    // copy tmp array in origin array
    for(i = 0 ; i< size ; i++)
    {
        array[start + i] = tmp_array[start + i];
        fprintf(fw,"%d ",array[start+i]); // print sorted array in outputfile
    }
    fprintf(fw,"\n");

  
}

