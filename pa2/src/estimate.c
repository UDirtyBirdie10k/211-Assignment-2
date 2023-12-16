#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** createMatrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return;
}

double** transposeMatrix(double** X, int rows, int cols) {
    double ** matrixTransposed = (double **)malloc(sizeof(double *) * cols);
	if (!matrixTransposed) return NULL;
	for (int i=0; i<cols; i++) {
		matrixTransposed[i] = (double *)malloc(sizeof(double) * rows);
		if (!matrixTransposed[i])return NULL;
	}
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixTransposed[j][i] = X[i][j];
        }
    }
    return matrixTransposed;
}



double** multiplyMatrix(double** X, double** Y, int rows, int cols, int rows2, int cols2) {

    if(cols != rows2){
        return NULL;
    }
   
    double ** product = (double **)malloc(sizeof(double *) * rows);
	if (!product) return NULL;
	int i;
	for (i =0; i<rows; i++) {
		product[i] = (double *)malloc(sizeof(double)* cols2);
		if (!product[i]) return NULL;
	}
        
    double total = 0.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int z = 0; z < cols; z++) {
                total += X[i][z] * Y[z][j];

            }
             product[i][j] = total;  // Store the result in the product matrix
            total = 0.0;  // Reset the total for the next iteration
        }
    }
    return product;
}



double** gaussJordanEliminationInverse(double** X, int rows, int cols) {
    double ** identityMatrix = (double **)malloc(sizeof(double *)* rows);
	if (!identityMatrix) return NULL;

	for(int i = 0; i < rows; i++) {
		identityMatrix[i] = (double *)malloc(sizeof(double) * cols);
		if (!identityMatrix[i]) return NULL;
	}


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            identityMatrix[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int i = 0; i < rows; i++) {
        double f = X[i][i];

        if (f == 0) {
            printf("Pivot element is zero. The matrix is not invertible.\n");
            return NULL;
        }

        for (int j = 0; j < cols; j++) {
            X[i][j] /= f;
            identityMatrix[i][j] /= f;
        }

        for (int j = 0; j < rows; j++) {
            if (j != i) {
                f = X[j][i];
                for (int z = 0; z < cols; z++) {
                    X[j][z] -= X[i][z] * f;
                    identityMatrix[j][z] -= identityMatrix[i][z] * f;
                }
            }
        }
    }

    for (int i = rows - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            double f = X[j][i];
            for (int z = 0; z < cols; z++) {
                X[j][z] -= X[i][z] * f;
                identityMatrix[j][z] -= identityMatrix[i][z] * f;
            }
        }
    }

    return identityMatrix;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    FILE* file = fopen(argv[1], "r");

    if (file == NULL) {
        return 1;
    }

    char * stringData = (char *)malloc(sizeof(char) * 6);

    if (!stringData) {
        return 1;
    }

    fscanf(file, "%s", stringData);

    free(stringData);


    int rows, cols;

    fscanf(file, "%d", &cols);
    fscanf(file, "%d", &rows);
    cols++;

    double** X = createMatrix(rows, cols);
    double** Y = createMatrix(rows, 1);




    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols + 1; j++) {
            if (j == 0) {
                X[i][0] = 1.0;
            }
            else if (j == cols) {
                fscanf(file, "%lf", &Y[i][0]);
            }
            else{
                fscanf(file, "%lf", &X[i][j]);
                
            }
        }
    }

    double** matrixTransposed = transposeMatrix(X, rows, cols);
    double** product = multiplyMatrix(matrixTransposed, X, cols, rows, rows, cols);
    double** identityMatrix = gaussJordanEliminationInverse(product, cols, cols);
    double** product2 = multiplyMatrix(identityMatrix, matrixTransposed, cols, cols, cols, rows);
    double** W = multiplyMatrix(product2, Y, cols, rows, rows, 1);

    FILE* file2 = fopen(argv[2], "r");

    if (file2 == NULL) {
        return 1;
    }

    char* stringData2 = (char*)malloc(sizeof(char) * 6);

    if (stringData2 == NULL) {
        return 1;
    }

    fscanf(file2, "%s", stringData2);

    free(stringData2);


    int rows2, cols2;

    fscanf(file2, "%d", &cols2);
    fscanf(file2, "%d", &rows2);
    cols2++;

double** data = createMatrix(rows2, cols2);

for (int i = 0; i < rows2; i++) {                            /*Inputs results into data*/
        for (int j = 0; j < cols2; j++) {
            if (j == 0) {
                data[i][j] = 1.0;
            }
            else{
                fscanf(file2, "%lf", &data[i][j]);
               	

            }
        }
    }


double** result = multiplyMatrix(data, W, rows2, cols2, cols, 1);
if (result==NULL) {
    return 1;
}
for(int i = 0; i < rows2; i++){
    for(int j = 0; j < 1; j++){
        printf("%.0f", result[i][j]);
    }
    printf("\n");
}

freeMatrix(X, rows);
freeMatrix(product, cols);
freeMatrix(matrixTransposed, cols);
freeMatrix(identityMatrix, cols);
freeMatrix(Y, rows);
freeMatrix(product2, cols);
freeMatrix(data, rows2);
freeMatrix(W, cols);
freeMatrix(result, rows2);

fclose(file);

fclose(file2);


    return 0;

}





