/*-------------------------------------------
Program 7: Outrun or Outlast the Baddies
Course: CS 211, Fall 2023, UIC
System: Advanced zyLab
Author: Jeremy Caceres
------------------------------------------- */
#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
    struct ROW {
        T*  Cols; 
        size_t NumCols; 
    };

    ROW* Rows; 
    size_t  NumRows; 

public:
    /**
     * Grid() constructor
     *
     * Initializes a 4x4 grid.
     *
     * Allocates space for 4 rows (NumRows = 4).
     *
     * Loops through each row:
     * - Allocates space for 4 columns in that row (NumCols = 4).
     * - Loops through each column in that row:
     *   - Initializes element to default value (T()).
    */
    Grid() {
        Rows = new ROW[4];
        NumRows = 4;

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[4];
            Rows[r].NumCols = 4;

            // initialize the elements to their default value:
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = T(); 
            }
        }
    }


    /**
     * Grid(size_t R, size_t C) constructor
     *
     * Initializes a grid with R rows and C columns.
     *
     * Parameters:
     * - R: Number of rows
     * - C: Number of columns
    */
    Grid(size_t R, size_t C) {
        Rows = new ROW[R];
        NumRows = R;

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[C];
            Rows[r].NumCols = C;

            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                // Initializes element to default value for arbitrary value
                Rows[r].Cols[c] = T();
            }
        }
    }


    /**
     * ~Grid() destructor
     *
     * Called when Grid object is deleted.
     *
    */
    virtual ~Grid() {
        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }

        delete[] Rows;
    }


    /**
     * Grid(const Grid<T>& other) copy constructor
     *
     * Initializes this grid as a copy of the other grid.
     *
     * Parameters:
     * - other: The grid to copy from
    */
    Grid(const Grid<T>& other) {
        // Allocates space for other.NumRows rows for more rows later
        Rows = new ROW[other.NumRows];
        NumRows = other.NumRows;

        for (size_t r = 0; r < NumRows; ++r) {
            // Allocates space for other.Rows[r].NumCols columns in that row to include more later
            Rows[r].Cols = new T[other.Rows[r].NumCols];
            Rows[r].NumCols = other.Rows[r].NumCols;

            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }
    }


    /**
     * Grid& operator=(const Grid& other)
     *
     * Assignment operator.
     * Assigns the contents of other grid to this grid.
     *
     * Parameters:
     * - other: The grid to copy to
    */
    Grid& operator=(const Grid& other) {
        // Checks if this grid is the same as other grid.
        if (this == &other) {
            return *this;
        }

        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }
        delete[] Rows;

        // Allocates space for other.NumRows rows for more rows later
        Rows = new ROW[other.NumRows];
        NumRows = other.NumRows;

        for (size_t r = 0; r < NumRows; ++r) {
            // Allocates space for other.Rows[r].NumCols columns in that row to include more later
            Rows[r].Cols = new T[other.Rows[r].NumCols];
            Rows[r].NumCols = other.Rows[r].NumCols;

            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }

        return *this;
    }


    /**
     * size_t numrows() const
     *
     * Returns the number of rows in the grid.
    */
    size_t numrows() const {
        return NumRows;
    }


    /**
     * size_t numcols(size_t r) const
     *
     * Returns the number of columns in row r.
     *
     * Throws out_of_range exception if r is not a valid row index.
    */
    size_t numcols(size_t r) const {
        // Checks if r is a valid row index.
        if (r >= NumRows) {
            throw out_of_range("Invalid row index");
        }

        return Rows[r].NumCols;
    }


    /**
     * size_t size() const
     *
     * Returns the total number of elements in the grid.
     *
     * Loops through each row:
     * - Sums the number of columns in each row.
     *
     * Returns the total sum.
    */
    size_t size() const {
        size_t total = 0;

        // Loops through each row:
        for (size_t r = 0; r < NumRows; ++r) {
            total += Rows[r].NumCols;
        }

        return total;
    }


    /**
     * T& operator()(size_t r, size_t c)
     *
     * Returns a reference to the element at row r and column c.
     *
     * Throws out_of_range exception if r or c are invalid indices.
    */
    T& operator()(size_t r, size_t c) {
        // Checks if r and c are valid indices.
        if (r >= NumRows) {
            throw out_of_range("Invalid row index");
        }
        if (c >= Rows[r].NumCols) {
            throw out_of_range("Invalid column index");
        }
        return Rows[r].Cols[c];
    }


    /**
     * void _output()
     *
     * Outputs the grid to stdout.
     *
     * Loops through each row:
     * - Prints the row number
     * - Loops through each column, printing the element
    */
    void _output() {
        for (size_t r = 0; r < NumRows; ++r) {
            cout << "Row " << r << ": ";
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                cout << Rows[r].Cols[c] << " ";
            }
            cout << endl;
        }
    }

};
