#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <mpi.h>

using namespace std;

int N, M;

// Function to print the Sudoku grid
void printGrid(const vector<vector<int>>& grid) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to check if it's safe to place a number at a given (row, col) position
int isSafe(const vector<vector<int>>& grid, int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % M;
    int startCol = col - col % M;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }

    return 1;
}

// Function to solve a Sudoku puzzle
int solveSudoku(vector<vector<int>>& grid) {
    int row, col;

    // Find an empty cell in the grid
    int found = 0;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                found = 1;
                break;
            }
        }
        if (found) break;
    }

    // If no empty cell is found, the puzzle is solved
    if (!found) return 1;

    // Try placing numbers from 1 to N in the empty cell
    for (int num = 1; num <= N; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            // Recursively solve the puzzle
            if (solveSudoku(grid)) return 1;

            // If the current configuration does not lead to a solution, backtrack
            grid[row][col] = 0;
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    clock_t start_time = clock();
    if (rank == 0) {
        ifstream inputFile("sudoku_500.txt");
        if (!inputFile) {
            cerr << "Error opening the file!" << endl;
            MPI_Finalize();
            return 1;
        }

        int numGrids;
        inputFile >> numGrids;

        for (int gridNum = 0; gridNum < numGrids; gridNum++) {
            N = 4;  // change the value of n acc to size of sudoku
            M = static_cast<int>(sqrt(N));

            vector<vector<int>> grid(N, vector<int>(N, 0));

            for (int row = 0; row < N; row++) {
                for (int col = 0; col < N; col++) {
                    inputFile >> grid[row][col];
                }
            }

            int result = solveSudoku(grid);
            MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
            for (int i = 0; i < N; i++) {
                MPI_Bcast(grid[i].data(), N, MPI_INT, 0, MPI_COMM_WORLD);
            }

            if (result) {
                printf("Solution for Grid %d:\n", gridNum + 1);
                printGrid(grid);
            } else {
                printf("No solution exists for Grid %d.\n", gridNum + 1);
            }
        }

        inputFile.close();
    } else {
        int result;
        MPI_Bcast(&result, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (result) {
            vector<vector<int>> grid(N, vector<int>(N, 0));
            for (int i = 0; i < N; i++) {
                MPI_Bcast(grid[i].data(), N, MPI_INT, 0, MPI_COMM_WORLD);
            }
        }
    }

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    MPI_Finalize();
    if (rank == 0) {
        printf("Total Execution Time: %f seconds\n", duration);
        cout << "V Usha Sree 2021BCS0022" << endl;
        cout << "J V P S S S R Karthik 2021BCS0048" << endl;
    }

    return 0;
}
