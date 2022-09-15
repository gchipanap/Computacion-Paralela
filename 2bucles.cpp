#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>

const int MAX_ = 20000;
short A[MAX_][MAX_], x[MAX_], y[MAX_];

int main() 
{
  std::clock_t start, end;
  double timeLoop1, timeLoop2;

  std::ofstream fichero;
  fichero.open("fichero.csv", std::ios::app);
  
  for (int k = 0; k < 20; ++k) {
    int MAX = 1 << k;

    /* Initialize A and x, assign y = 0 */

    for (int i = 0; i < MAX; ++i) {
      for (int j = 0; j < MAX; ++j)
        A[i][j] = 1;
      x[i] = 1;
      y[i] = 0;
    }

    start = clock();
    for (int i = 0; i < MAX; ++i)
      for (int j = 0; j < MAX; ++j)
        y[i] += A[i][j] * x[j];
    end = clock();

    timeLoop1 = double(end - start) / CLOCKS_PER_SEC;

    /* Assign y = 0 */
    for (int i = 0; i < MAX; i++)
      y[i] = 0;

    start = clock();
    for (int j = 0; j < MAX; ++j)
      for (int i = 0; i < MAX; ++i)
        y[i] += A[i][j] * x[j];
    end = clock();

    timeLoop2 = double(end - start) / CLOCKS_PER_SEC;

    std::cout << MAX << "\t" << timeLoop1 << "\t" << timeLoop2 << "\n";
    fichero << MAX << "\t" << timeLoop1 << "\t" << timeLoop2 << "\n";
  }
  
  fichero.close();
  return 0;
}