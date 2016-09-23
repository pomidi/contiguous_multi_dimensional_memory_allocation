/*
 * Author : Pourya Omidi
 * Date   : 20-Jun-2016
 *
 *
 * Description: This library allows you to allocate multidimensional
 * contiguous arrays. This is limited up to 4 dimension. You can expand it based
 * on the same priciple.
 * to avoid memory leakage remember to call memory_free_xd() when you are done.
 *
 * NOTE : of you are allocating more than MAX_NUMBER_OF_ALLOCATED_ARRAYS arrays,
 * increase the amount to the desirable value.
 *
 * Supported variable types :
 * int, float,double and long
 *
 * Note: addressed are calculated based on row major
 * see : https://en.wikipedia.org/wiki/Row-major_order
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "memory_management.h"

#define log_error(x) printf(stderr,x)
#define UNUSED(expr) do { (void)(expr); } while (0)

typedef struct
{
    void *elements[MAX_NUMBER_OF_ALLOCATED_ARRAYS];
    void *address[MAX_NUMBER_OF_ALLOCATED_ARRAYS];
    unsigned long total_size;
    unsigned int counter;
}Arrays;

Arrays arrays = {.counter = 0, .total_size = 0};

#define memory_shortage_msg()  log_error("could not allocate enough memory.\n" \
                                         "total allocated memory %lu kbytes\n",\
                                            arrays.total_size/1000)

static void *_calloc(size_t __nmemb, size_t __size)
{
    arrays.total_size+= (__nmemb * __size);
    return calloc(__nmemb,__size);
}

static size_t get_variable_size(VariableType type);

static inline unsigned short int isLimitReached()
{
    if(arrays.counter >= MAX_NUMBER_OF_ALLOCATED_ARRAYS)
    {
        log_error("increase the MAX_NUMBER_OF_ALLOCATED_ARRAYS"
               "to allocate more variables\n");
        return 1;
    }
    return 0;
}

void *malloc1D(unsigned int size, VariableType type)
{
    size_t variable_size = get_variable_size(type);

    void *array1D = _calloc(variable_size , size);

    if(array1D == NULL)
    {
        memory_shortage_msg();
        return NULL;
    }
    return array1D;
}

void **malloc2D(unsigned int size_x,unsigned int size_y,VariableType type)
{
    unsigned int i;
    if(isLimitReached())
    {
        return NULL;
    }

    void **array2D = (void**)_calloc(size_x , sizeof(void*));

    if(array2D == NULL)
    {
        memory_shortage_msg();
        return NULL;
    }

    unsigned index = arrays.counter++;
    size_t variable_size = get_variable_size(type);

    if((arrays.elements[index] = _calloc(size_x * size_y , variable_size)) == NULL)
    {
        memory_shortage_msg();
        free(array2D);
        return NULL;
    }

    arrays.address[index] = array2D;

    char *elements = (char*)arrays.elements[index];

    for(i = 0; i < size_x; i++)
    {
        array2D[i] = elements +  ((i * size_y) * variable_size);
    }

    return array2D;
}

void ***malloc3D(unsigned int size_x,unsigned int size_y,unsigned size_z,
                      VariableType type)
{
    if(isLimitReached())
    {
        return NULL;
    }

    unsigned int i,j;

    void ***array3D = (void***)_calloc(size_x , sizeof(void**));

    if(array3D == NULL)
    {
        memory_shortage_msg();
        return NULL;
    }

    unsigned index = arrays.counter++;
    size_t variable_size = get_variable_size(type);

    if((arrays.elements[index] = _calloc(size_x * size_y * size_z , variable_size)) == NULL)
    {
        memory_shortage_msg();
        free(array3D);
        return NULL;
    }

    arrays.address[index] = array3D;

    char *elements = (char*)arrays.elements[index];

    for(i = 0; i < size_x; i++)
    {
        if((array3D[i] = (void**)_calloc(size_y , sizeof(void*))) == NULL)
        {
            memory_shortage_msg();
            while(i != 0 )
            {
                free(array3D[--i]);
            }
            free(arrays.elements[index]);
            free(array3D);
            return NULL;
        }

        for(j = 0; j < size_y;j++)
        {
            array3D[i][j] = elements + (((i * size_y * size_z) + (j * size_z)) * variable_size);
        }
    }

    return array3D;
}

void free1D(void *array,unsigned int size)
{
    UNUSED(size);
    if(array != NULL)
    {
        free(array);
        array = NULL;
    }

}

void free2D(void *array,unsigned int size_x,unsigned int size_y)
{
    void **_array = (void**)array;
    UNUSED(size_x);
    UNUSED(size_y);

    unsigned int i;

    for(i = 0; i < arrays.counter;i++)
    {
        if(_array == arrays.address[i])
        {
            if(arrays.elements[i] != NULL)
            {
                free(arrays.elements[i]);
                arrays.elements[i] = NULL;
            }
            break;
        }
    }

    if(_array != NULL)
    {
        free (_array);
        _array = NULL;
    }
}

void free3D(void *array,unsigned int size_x,unsigned int size_y,
             unsigned int size_z)
{
    void ***_array = (void***)array;
    UNUSED(size_y);
    UNUSED(size_z);
    unsigned int i;
    for(i = 0; i < arrays.counter;i++)
    {
        if(_array == arrays.address[i])
        {
            if(arrays.elements[i] != NULL)
            {
                free(arrays.elements[i]);
                arrays.elements[i] = NULL;
            }
            break;
        }
    }

    for(i = 0; i < size_x; i++)
    {
        if(_array[i] != NULL)
        {
            free(_array[i]);
            _array[i] = NULL;
        }
    }
    if(_array != NULL)
    {
        free (_array);
        _array = NULL;
    }

}

static size_t get_variable_size(VariableType type)
{
    switch(type)
    {
        case INT:
            return sizeof(int);
        break;
        case LONG:
            return sizeof(long);
        break;
        case FLOAT:
            return sizeof(float);
        break;
        case DOUBLE:
            return sizeof(double);
        break;
        default:
            log_error("variable type not supported");
            return 0;
       break;
    }
}
