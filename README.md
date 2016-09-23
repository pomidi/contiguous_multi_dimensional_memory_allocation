# contiguous multi dimensional memory allocation
This code guarantees the allocation of a multi dimensional matrix in a contiguous way.

available :
	1,2,3 dimensional
to be added :
	4 dimensional


# Example
```
	double **matrix2D = (duoble**)malloc2D(100,50,DOUBLE);

	//then you can write
	matrix2D[2][5] = 42;
	printf("[2][5] = %f, [40][41] = %f\n", matrix2D[2][5],matrix[40][41]);
	
	free2D(matrix2D);
```
