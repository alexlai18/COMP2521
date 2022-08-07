// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"

void maze_traverse(Maze m, Cell curr, Queue q, bool **visited, int row, int col, Cell **prev_cell);
void exit_found(Maze m, Cell res, Cell start, Cell **prev_cell);

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions

    // Creating a matrix which shows which index has been visited or not
    int row = MazeWidth(m);
    int col = MazeHeight(m);

    bool **visited = createBoolMatrix(row, col);
    Cell **prev_cell = createCellMatrix(row, col);

    // INITIATED QUEUE TO LOOK AT GRAPH IN BFS METHOD
    Queue q = QueueNew();

    // Getting the start of the maze
    Cell start = MazeGetStart(m);
    

    QueueEnqueue(q, start);

    // Looking at the head of the queue
    while (QueueIsEmpty(q) != true) {
        Cell curr = QueueDequeue(q);
        // If the current cell has been visited, continue
        if (visited[curr.row][curr.col] == true) {
            continue;
        }

        // If the current cell has not been visited before, visit the cell
        bool res = MazeVisit(m, curr);
        visited[curr.row][curr.col] = true;


        // Checking if the cell visited was the end. If not, traverse through it
        if (res == true) {
            exit_found(m, curr, start, prev_cell);
            QueueFree(q);
            freeBoolMatrix(visited);
            freeCellMatrix(prev_cell);
            return true;
        }

        // Adds non-wall neighbours to stack
        maze_traverse(m, curr, q, visited, row, col, prev_cell);
    }
    QueueFree(q);
    freeBoolMatrix(visited);
    freeCellMatrix(prev_cell);
    return false;
}


void maze_traverse(Maze m, Cell curr, Queue q, bool **visited, int row, int col, Cell **prev_cell) {
    // Checking up
    Cell upper = curr;
    upper.row--;
    if (upper.row >= 0 && visited[upper.row][upper.col] != true && MazeIsWall(m, upper) == false) {
        QueueEnqueue(q, upper);
        prev_cell[upper.row][upper.col] = curr;
    }

    Cell lower = curr;
    lower.row++;
    if (lower.row < row && visited[lower.row][lower.col] != true && MazeIsWall(m, lower) == false) {
        QueueEnqueue(q, lower);
        prev_cell[lower.row][lower.col] = curr;
    }

    Cell left = curr;
    left.col--;
    if (left.col > 0 && visited[left.row][left.col] != true && MazeIsWall(m, left) == false) {
        QueueEnqueue(q, left);
        prev_cell[left.row][left.col] = curr;
    }
    
    Cell right = curr;
    right.col++;
    if (curr.row < col && visited[right.row][right.col] != true && MazeIsWall(m, right) == false) {
        QueueEnqueue(q, right);
        prev_cell[right.row][right.col] = curr;
    }
}

void exit_found(Maze m, Cell res, Cell start, Cell **prev_cell) {
    Cell curr = res;
    while ((curr.row != start.row) || (curr.col != start.col)) {
        MazeMarkPath(m, curr);
        curr = prev_cell[curr.row][curr.col];
    }
    MazeMarkPath(m, start);
}