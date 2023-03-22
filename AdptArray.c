#include "AdptArray.h"
#include <stdlib.h>
#include <stdio.h>

// define the AdptArray object:
typedef struct AdptArray_{
    DEL_FUNC delFunc;
    COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;
    PElement* pElement_array;
    int size; // array size
}AdptArray;


PAdptArray CreateAdptArray(COPY_FUNC c_f, DEL_FUNC d_f,PRINT_FUNC p_f)
{
    PAdptArray paa = (PAdptArray)malloc(sizeof(AdptArray));
    if(paa == NULL){ return NULL;}
    paa->printFunc = p_f;
    paa->delFunc = d_f;
    paa->copyFunc = c_f;
    paa->pElement_array = NULL;
    paa->size = 0;
    return paa;
}



void DeleteAdptArray(PAdptArray paa)
{
    if(paa == NULL) return;

    for(int i = 0; i < paa->size; i++)
    {
        if((paa->pElement_array)[i] != NULL){ // check that the element is not null
            paa->delFunc((paa->pElement_array)[i]);
        }
    }
    if(paa->pElement_array != NULL)
        free(paa->pElement_array);
    free(paa);
}

Result SetAdptArrayAt(PAdptArray paa, int index, PElement p_element)
{
    if(index < 0) return FAIL; // invalid index

    // creat new array
    if(index >= paa->size)
    {
        PElement* new_array = (PElement*)calloc(index+1, sizeof(PElement)); // use calloc to get null array
        if(new_array == NULL) return FAIL;

        for (int i = 0; i < paa->size; i++)
        {
            if( paa->pElement_array[i] != NULL)
            {
                new_array[i] = paa->copyFunc(paa->pElement_array[i]); // copy the element to the new array
                paa->delFunc(paa->pElement_array[i]); // delete the old element
            }
        }
        if(new_array[index] != NULL)
        {
            paa->delFunc(new_array[index]);
        }
        if(p_element != NULL)
            new_array[index] = paa->copyFunc(p_element); // make copy of the element to add
        free(paa->pElement_array); // free the old array
        paa->pElement_array = new_array; // update the new array
        paa->size = index+1; // update the array size
    }
    else
    {
        // delete the old element
        if(paa->pElement_array[index] != NULL)
        {
            paa->delFunc(paa->pElement_array[index]);
        }
        if(p_element != NULL)
            paa->pElement_array[index] = paa->copyFunc(p_element); // make copy of the element to add
    }
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray paa, int index)
{
    if(paa == NULL || index < 0 || index >= paa->size ||
        paa->pElement_array == NULL || paa->pElement_array[index] == NULL) //Check that the index is valid.
    {
        return NULL;
    }
    return paa->copyFunc( (paa->pElement_array)[index] );
}

int GetAdptArraySize(PAdptArray paa)
{
    if(paa == NULL || paa->pElement_array == NULL) return -1;
    else return paa->size;
}

void PrintDB(PAdptArray paa)
{
    if(paa == NULL || paa->pElement_array == NULL)
    {
        return;
    }
    for (int i = 0; i < paa->size; i++)
    {
        if(paa->pElement_array[i] != NULL)
        {
            paa->printFunc((paa->pElement_array)[i]);
        }
    }
}