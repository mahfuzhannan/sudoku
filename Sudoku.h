#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"

// TODO: Your Sudoku class goes here:
#include <stdio.h>
#include <math.h> 

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <memory>

using std::string;
using std::ostream;
using std::vector;
using std::set;
using std::map;
using std::unique_ptr;

class Sudoku {
private:
	vector< vector< set<int> > > board;
	int boardSize;
	bool removeFromRow(int row, int col, int value);
	bool removeFromColumn(int row, int col, int value);
	bool removeFromBox(int row, int col, int value);

public:
	Sudoku(int size); // Contructor

	int getSquare(int row, int col); // Get Value from cell
	bool setSquare(int row, int col, int value); // Set value in cell
};

/**
* Constructor
*/
Sudoku::Sudoku(int size) {
	boardSize = size;
	// create list of numbers
	set< int > cell;
	for(int i=1; i<=size; i++ ) {
		cell.insert(i);
	}
	vector< std::set< int > > row (size, cell);
	board.assign(size, row);
}

/**
* Get value in cell, -1 if nor value
*/
int Sudoku::getSquare(int row, int col) {
	std::set<int> cell = board[row][col];
	// std::cout << "cell(" << row << "," << col << "), size() = " << cell.size() << "\n"; 
	if(cell.size() == 1) {
		return *cell.begin();
	} else {
		return -1;
	}
}

/**
* Set value of given cell, remove given value from all cells of same
* row, col and box
*/
bool Sudoku::setSquare(int row, int col, int value) {
	// printf("inserting cell[%d][%d] -> %d\n", row, col, value);
	// create new empty set
	set< int > newCell;
	// insert single value into set
	newCell.insert(value);
	// replace set with new set
	board[row][col] = newCell;

	// search each row
	for(int i=0; i<boardSize; i++) {
		// search columns in current row
		for(int j=0; j<boardSize; j++) {
			// get the current cell
			std::set< int > currentCell = board[i][j];
			// if cell is empty there is no solution
			if(currentCell.empty()) {
				return false;
			} else if(currentCell.size() == 1) {
				// remove from row, column and box
				bool hasAnyCellBeenErased = removeFromRow(i, j, *currentCell.begin()) || removeFromColumn(i, j, *currentCell.begin()) || removeFromBox(i, j, *currentCell.begin());
				// if a cell has been erased reset the loops and iterate through grid again
				if(hasAnyCellBeenErased) {
					i=0;
					j=0;
				}
			}
		}
	}
	return true;
}

bool Sudoku::removeFromRow(int row, int col, int value) {
	bool erased = false;
	for(int i=0; i<boardSize; i++) {
		// since removing from row, row is fixed and iterate through columns
		set< int > currentCell = board[row][i];
		// ignore cell with same column as this must be of size()=1
		// and contains only the value we are checking
		// therefore removing will cause empty set on solved cell
		if(i != col ) {
			if(currentCell.find(value) != currentCell.end()) {
				currentCell.erase(value);
				board[row][i] = currentCell;
				erased = true;
			}
		}
	}
	return erased;
}

bool Sudoku::removeFromColumn(int row, int col, int value) {
	bool erased = false;
	for(int i=0; i<boardSize; i++) {
		// since removing from column, column is fixed and iterate through rows
		set< int > currentCell = board[i][col];
		// ignore cell with same row as this must be of size()=1
		// and contains only the value we are checking
		// therefore removing will cause empty set on solved cell
		if(i != row ) {
			if(currentCell.find(value) != currentCell.end()) {
				currentCell.erase(value);
				board[i][col] = currentCell;
				erased = true;
			}
		}
	}
	return erased;
}

bool Sudoku::removeFromBox(int row, int col, int value) {
	bool erased = false;
	// calculate size of each box, which is the sqrt of board size
	int boxSize = sqrt(boardSize);
	// get start of box row/col, 
	// floor() as we want to start the the beginning of each box
	int rowStart = floor(row / boxSize) * boxSize;
	int colStart = floor(col / boxSize) * boxSize;
	// get end of box row/col
	int rowEnd = rowStart + boxSize;
	int colEnd = colStart + boxSize;
	// iterate through row
	for(int i=rowStart; i<rowEnd; i++) {
		// iterate through column
		for(int j=colStart; j<colEnd; j++) {
			std::set< int > currentCell = board[i][j];
			// ignore cell with same row and column as this must be of size()=1
			// and contains only the value we are checking
			// therefore removing will cause empty set on solved cell
			if(i != row && j != col) {
				if(currentCell.find(value) != currentCell.end()) {
					// printf("erasing %d from cell[%d][%d]\n", value, i, j);
					currentCell.erase(value);
					board[i][j] = currentCell;
					erased = true;
				}
			}
			
		}
	}
	return erased;;
}

#endif
