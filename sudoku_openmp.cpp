#include <iostream>
#include <fstream>
#include <omp.h>
#include <ctime>
#include <cmath>

using namespace std;

int N, M;

// Function to print the Sudoku grid
void printGrid(int **grid)
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to check if it's safe to place a number at a given (row, col) position
bool isSafe(int **grid, int row, int col, int num)
{
    for (int x = 0; x < N; x++)
    {
        if (grid[row][x] == num || grid[x][col] == num)
        {
            return false;
        }
    }

    int startRow = row - row % M;
    int startCol = col - col % M;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (grid[i + startRow][j + startCol] == num)
            {
                return false;
            }
        }
    }

    return true;
}

// Function to solve a Sudoku puzzle
bool solveSudoku(int **grid, int row, int col)
{
    if (row == N)
        return true;
    if (col == N)
    {
        return solveSudoku(grid, row + 1, 0);
    }
    if (grid[row][col] != 0)
    {
        return solveSudoku(grid, row, col + 1);
    }

    bool solved = false;

#pragma omp parallel for
    for (int num = 1; num <= N; num++)
    {
        if (!solved && isSafe(grid, row, col, num))
        {
            grid[row][col] = num;

            if (solveSudoku(grid, row, col + 1))
            {
#pragma omp critical
                {
                    solved = true;
                }
            }

            if (!solved)
            {
                grid[row][col] = 0;
            }
        }
    }

    return solved;
}

int main()
{
    clock_t start_time = clock();
    ifstream inputFile("sudoku_500.txt");
    if (!inputFile)
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    int numGrids;
    inputFile >> numGrids;
    for (int gridNum = 0; gridNum < numGrids; gridNum++)
    {
        printf("Grid %d:\n", gridNum + 1);
        int gridN = 4;   // change the value of gridN acc to size of sudoku

        int **grid = new int *[gridN];
        for (int i = 0; i < gridN; i++)
        {
            grid[i] = new int[gridN];
        }

        N = gridN;
        M = sqrt(N);

        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                inputFile >> grid[row][col];
            }
        }

        if (solveSudoku(grid, 0, 0))
        {
            cout << "Solution:\n";
            printGrid(grid);
        }
        else
        {
            cout << "No solution exists." << endl;
        }
        for (int i = 0; i < N; i++)
        {
            delete[] grid[i];
        }
        delete[] grid;
    }

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", duration);
    cout << "V Usha Sree 2021BCS0022" << endl;
    cout << "J V P S S S R Karthik 2021BCS0048" << endl;
    return 0;
}
