#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>

using namespace std;

// Function to print the Sudoku grid
void printGrid(const vector<vector<int>>& grid, int N) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function to check if it's safe to place a number at a given (row, col) position
bool isSafe(const vector<vector<int>>& grid, int row, int col, int num) {
    int N = grid.size();
    int M = sqrt(N);

    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return false;
        }
    }

    int startRow = row - row % M;
    int startCol = col - col % M;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

// Function to solve a Sudoku puzzle
bool solveSudoku(vector<vector<int>>& grid, int N) {
    int M = sqrt(N);

    int row = -1, col = -1;
    bool isEmpty = true;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) {
            break;
        }
    }

    if (isEmpty) {
        return true;
    }

    for (int num = 1; num <= N; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid, N)) {
                return true;
            }
            grid[row][col] = 0;
        }
    }

    return false;
}

int main() {
    clock_t start_time = clock();
    ifstream inputFile("sudoku_500.txt");
    if (!inputFile) {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    int numGrids;
    inputFile >> numGrids;

    for (int gridNum = 0; gridNum < numGrids; gridNum++) {
        printf("Grid %d:\n", gridNum + 1);

        int N = 4;  // change the value of n acc to size of sudoku

        vector<vector<int>> grid(N, vector<int>(N, 0));

        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                inputFile >> grid[row][col];
            }
        }

        if (solveSudoku(grid, N)) {
            cout << "Solution:\n";
            printGrid(grid, N);
        } else {
            cout << "No solution exists." << endl;
        }
    }

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", duration);
    cout << "V Usha Sree 2021BCS0022" << endl;
    cout << "J V P S S S R Karthik 2021BCS0048" << endl;
    inputFile.close();
    return 0;
}
