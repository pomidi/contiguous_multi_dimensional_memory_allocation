#ifndef _MEMORY_MANAGEMENT_H_
#define _MEMORY_MANAGEMENT_H_

#define MAX_NUMBER_OF_ALLOCATED_ARRAYS  150

typedef enum{
    INT = 0,
    UNSIGNED_INT,
    CHAR,
    LONG,
    FLOAT,
    DOUBLE
}VariableType;

void *malloc1D(unsigned int size,VariableType type);

void **malloc2D(unsigned int size_x,unsigned int size_y,VariableType type);

void *** malloc3D(unsigned int size_x,unsigned int size_y,unsigned size_z,
                      VariableType type);

void ****malloc4D(unsigned int size_x, unsigned int size_y,
                            unsigned int size_z, unsigned int size_p, VariableType type);

void free1D(void *array,unsigned int size);

void free2D(void *array,unsigned int size_x,unsigned int size_y);

void free3D(void *array, unsigned int size_x, unsigned int size_y,
             unsigned int size_z);

void free4D(void *array,unsigned int size_x,unsigned int size_y,
             unsigned int size_z,unsigned int size_p);

#endif //_MEMORY_MANAGEMENT_H_
