#include <iostream>
#include <ctime> 
#include <cmath>
#include <vector>
#include <fstream>
#include <sys/time.h>

void fillMatrix(short **matrix, int size);
void fillZeroMatrix(short **matrix, int size);

void deleteMatrix(short **matrix, int size);
void displayMatrix(short **matrix, int size);

void multiplyClassic(short **matrix1, short **matrix2, short **matrix3, int size);
void multiplyBlock(short **matrix1, short **matrix2, short **matrix3, int size, int block_size);

void timingMultiplyClassic(int size, std::vector<double> & TmultiplyClassic, std::vector<double> & TmultiplyBlock);
double average(std::vector<double> T, int size);

int main()
{	
    std::ofstream fichero;
    fichero.open("fichero.csv", std::ios::app);
	for(int i = 2; i < 11; i++)
	{
		int size = pow(2,i);
		
		std::vector<double> TmultiplyClassic;
		std::vector<double> TmultiplyBlock;
		
		timingMultiplyClassic(size, TmultiplyClassic, TmultiplyBlock);
		
		double TmulClassic = average(TmultiplyClassic, 10);
		double TmulBlock = average(TmultiplyBlock, 10);
		
		std::cout << size << "\t" << TmulClassic << "\t" << TmulBlock << "\n";	
        fichero << size << "\t" << TmulClassic << "\t" << TmulBlock << "\n";	
	}

	fichero.close();
	return 0;
}
	
	
void fillMatrix(short **matrix, int size)
{
	srand(time(NULL));
	for(int i = 0; i < size; i++)
	{
		matrix[i] = new short[size];
		for(int j = 0; j< size; j++)
		{
			matrix[i][j] = std::rand() % 10;
		}
	}	
}

void fillZeroMatrix(short **matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i] = new short[size];
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = 0.0f;
		}
	}
}

void deleteMatrix(short **matrix, int size)
{
	for (int i = 0; i < size; i++) 
	{
        delete [] matrix[i];
    }
    delete [] matrix;
}

void displayMatrix(short **matrix, int size)
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j< size; j++)
			std::cout << matrix[i][j] << "\t";
		std::cout<<"\n";
	}	
}

void multiplyClassic(short **matrix1, short **matrix2, short **matrix3, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}	
}

void multiplyBlock(short **matrix1, short **matrix2, short **matrix3, int size, int block_size)
{
	int en = block_size * (size/block_size); 
	double sum;
	
	for (int kk = 0; kk < en; kk += block_size) 
	{
		for (int jj = 0; jj < en; jj += block_size) 
		{
			for (int i = 0; i < size; i++) 
			{
				for (int j = jj; j < jj + block_size; j++) 
				{
					sum = matrix3[i][j];
					for (int k = kk; k < kk + block_size; k++) 
					{
						sum += matrix1[i][k]*matrix2[k][j];
					}
					matrix3[i][j] = sum;
				}
			}
		}
	}
}	

void timingMultiplyClassic(int size, std::vector<double> & TmultiplyClassic, std::vector<double> & TmultiplyBlock)
{
	std::clock_t start, end;
	//double timeLoop1, timeLoop2;
	double timeLoop1, timeLoop2;

	struct timeval time_start;
	struct timeval time_end;

	for(int i = 0 ; i < 10; i++)
	{
		short **matrix1 = new short *[size];
		short **matrix2 = new short *[size];
		short **matrix3 = new short *[size];
		
		/* Fill matrix */
		fillMatrix(matrix1, size);
		fillMatrix(matrix2, size);
		fillZeroMatrix(matrix3, size);
		
		/* Multiply - 2 loops */
		start = clock();
		for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
		end = clock();
		
		timeLoop1 = double(end - start) / CLOCKS_PER_SEC;
		
		/* Fill matrix3 with 0*/
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix3[i][j] = 0.0;
		
		/* Multiply - 6 loops */
		start = clock();
        int block_size = size/4;
		int en = block_size * (size/block_size); 
        double sum;
        
        for (int kk = 0; kk < en; kk += block_size) 
        {
            for (int jj = 0; jj < en; jj += block_size) 
            {
                for (int i = 0; i < size; i++) 
                {
                    for (int j = jj; j < jj + block_size; j++) 
                    {
                        sum = matrix3[i][j];
                        for (int k = kk; k < kk + block_size; k++) 
                        {
                            sum += matrix1[i][k]*matrix2[k][j];
                        }
                        matrix3[i][j] = sum;
                    }
                }
            }
        }

		end = clock();
		
		timeLoop2  = double(end - start) / CLOCKS_PER_SEC;
		
		deleteMatrix(matrix1, size);
		deleteMatrix(matrix2, size);
		deleteMatrix(matrix3, size);
		
 		TmultiplyClassic.push_back(timeLoop1);
		TmultiplyBlock.push_back(timeLoop2);
	}
}
double average(std::vector<double> T, int size)
{
	int sum;
	for(int i = 0; i < size; i++)
		sum += T[i];
	return sum /size;
}