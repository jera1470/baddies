/*-------------------------------------------
Author: Jeremy Caceres
Fall 2023, UIC
------------------------------------------- */
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using std::runtime_error;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        int HeroRow;
	    int HeroCol;
        size_t numMonsters;
        size_t numSuperMonsters;
        size_t numAbysses;
        size_t numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }


        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }


        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }


        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }


        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }


        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }


        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);

            size_t r = rand() % numRows;
            size_t c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            size_t sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (size_t i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (size_t i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (size_t i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (size_t i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }


        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }


        void setNumAbysses(int num) {
            numAbysses = num;
        }


        void setNumBats(int num) {
            numBats = num;
        }


        size_t getNumRows() {
            return numRows;
        }


        size_t getNumCols() {
            return numCols;
        }

        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {
            // Set the output row parameter to the stored hero row
            row = HeroRow;
            // Set the output col parameter to the stored hero column
            col = HeroCol;

        }



        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            // Set the stored hero row to the passed in row
            HeroRow = row;
            // Set the stored hero column to the passed in column
            HeroCol = col;

        }



        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            // Search board for Hero
            for (size_t r = 0; r < getNumRows(); r++) {
                for (size_t c = 0; c < getNumCols(); c++) {
                    // Check if current cell contains the Hero
                    if (board(r,c)->display() == 'H') {
                        // Found Hero, update position
                        setHeroPosition(r,c);
                        // Return since Hero was found
                        return;
                    }
                }
            }
            // Hero not found, set to default invalid position
            HeroRow = -1;
            HeroCol = -1;
        }



        //---------------------------------------------------------------------------------
        // bool makeMoves(char HeroNextMove)
        //
        // This function handles moving the Hero and all other baddies on the board for
        // the current turn.
        //
        // Parameters:
        //  - HeroNextMove: The next move direction input for the Hero ('w'/'a'/'x'/'d'/'s' etc.)
        //
        // Returns:
        //  - bool: True if game should continue (hero is fine), false if game is over (win/lose condition)
        //
        // Functionality:
        // - Reset 'moved' flag to false for all baddies
        // - Set Hero's next move based on input param
        // - Call Hero's attemptMoveTo() to get new proposed position
        // - Check for out of bounds, barriers, monsters, exit along move path
        // - If valid, swap Hero and Nothing objects to 'move' Hero
        // - Call attemptMoveTo() for each baddie and move if possible
        // - Handle any collisions/lose conditions for each baddie
        // - Return true to continue game, false if gameover condition happens
        //
        bool makeMoves(char HeroNextMove) {
            // Loop through all cells on the board to check if baddie has moved, and makes it false if so
            for (size_t row = 0; row < getNumRows(); row++) {
                for (size_t col = 0; col < getNumCols(); col++) {
                    if (board(row, col)->isBaddie()) {
                        board(row, col)->setMoved(false);
                    }
                }
            }
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);

            // Check if new row is out of bounds
            try {
                if (newR < 0 || newR >= numRows) {
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // Reset row to current position to avoid going out of bounds
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;

            }

            // Check if new column is out of bounds
            try {
                if (newC < 0 || newC >= numCols) {
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // Reset column to current position to avoid going out of bounds
                newC = HeroCol;
                cout << "Changing column for Hero position to stay in-bounds" << endl;
            }

            // Check if hero is attempting to move into a barrier
            try {
                if (board(newR, newC)->isBarrier()) {
                    throw runtime_error("Hero trying to move into a barrier");
                }
            }
            catch (runtime_error& excpt) {
                if (HeroCol != newC) {
                    cout << excpt.what() << endl;
                    newC = HeroCol;
                    cout << "Changing Hero position to stay off barrier" << endl;
                }
                else if (HeroRow != newR) {
                    cout << excpt.what() << endl;
                    newR = HeroRow;
                    cout << "Changing Hero position to stay off barrier" << endl;
                }
            }

            // Check if hero is attempting to move onto the escapeladder
            try {
                if (board(newR, newC)->isExit()) {
                    throw runtime_error("Hero has reached the exit!");
                }
            }
            catch(runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // Set hero position to invalid
                setHeroPosition(-1, -1);
                wonGame = true;
                cout << "Hero has escaped!" << endl;
                return false;

            }

            // Check if hero is attempting to move into a monster
            try {
                if (board(newR, newC)->isBaddie()) {
                    throw runtime_error("Uh Oh! Hero has moved into a Baddie...");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // Set hero's position to invalid
                setHeroPosition(-1, -1);
                cout << "Be more careful next time..." << endl;
                return false;
            }

            // Check if hero is attempting to move into the abyss
            try {
                if (board(newR, newC)->isHole()) {
                    throw runtime_error("Uh Oh! Hero has moved into the Abyss...");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // Set hero's position to invalid
                setHeroPosition(-1, -1);
                cout << "Be more careful next time..." << endl;
                return false;

            }

            // Move into empty cell
            if (board(newR, newC)->isSpace()) {
                freeCell(newR, newC);
                board(newR, newC) = new Hero(newR, newC);
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                board(newR, newC)->setMoved(true);
                setHeroPosition(newR, newC);
            }

            // Check for monster's movement (out of bounds, incorrect placement)
            for (size_t BaddieRow = 0; BaddieRow < numRows; BaddieRow++){
                for (size_t BaddieCol = 0; BaddieCol < numCols; BaddieCol++){
                    if (board(BaddieRow, BaddieCol)->isBaddie() && !board(BaddieRow, BaddieCol)->getMoved()) {
                        size_t newBadR, newBadC;
                        board(BaddieRow, BaddieCol)->attemptMoveTo(newBadR, newBadC, HeroRow, HeroCol);

                        // Check if baddie is attempting to move out of bounds on the rows
                        try {
                            if (newBadR < 0 || newBadR >= numRows) {
                                // Throw exception indicating out of bounds
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            }
                        }
                        // Catch out of bounds exception
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBadR = BaddieRow;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }

                        // Check if baddie is attempting to move out of bounds on the columns
                        try {
                            if (newBadC < 0 || newBadC >= numCols) {
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBadC = BaddieCol;
                            cout << "Changing column for Baddie position to stay in-bounds" << endl;

                        }

                        // Check if baddie is attempting to move into a barrier
                        try {
                            if (board(newBadR, newBadC)->isBarrier()) {
                                throw runtime_error("Baddie trying to move into a barrier");
                            }
                        }
                        catch (runtime_error& excpt) {
                            if (BaddieCol != newBadC) {
                                cout << excpt.what() << endl;
                                newBadC = BaddieCol;
                                cout << "Changing Baddie position to stay off barrier" << endl;
                            }
                            else if (BaddieRow != newBadR) {
                                cout << excpt.what() << endl;
                                newBadR = BaddieRow;
                                cout << "Changing Baddie position to stay off barrier" << endl;
                            }
                        }

                        // Check if baddie is attempting to move into the abyss
                        try {
                            if (board(newBadR, newBadC)->isHole()) {
                                throw runtime_error("Yipee! A Baddie has moved into the Abyss...");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            freeCell(BaddieRow, BaddieCol);
                            // Set cell baddie is moving from to be empty, removing them
                            board(BaddieRow, BaddieCol) = new Nothing(BaddieRow, BaddieCol);
                            cout << "One less Baddie on the board!" << endl;
                        }

                        // Check if baddie is attempting to move into hero's cell
                        try {
                            if (board(newBadR, newBadC)->isHero()) {
                                throw runtime_error("Uh Oh! A Baddie has caught the hero...");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            freeCell(newBadR, newBadC);
                            board(newBadR, newBadC) = board(BaddieRow, BaddieCol);
                            board(newBadR, newBadC)->setPos(newBadR, newBadC);
                            // Set baddie's old cell to be empty, removing them
                            board(BaddieRow, BaddieCol) = new Nothing(BaddieRow, BaddieCol);
                            cout << "Better luck next time..." << endl;
                        }

                        // Check if baddie is attempting to move into a space cell
                        if (board(newBadR, newBadC)->isSpace()) {
                            BoardCell* temp = board(newBadR, newBadC);
                            board(newBadR, newBadC) = board(BaddieRow, BaddieCol);
                            board(BaddieRow, BaddieCol) = temp;
                            board(newBadR, newBadC)->setPos(newBadR, newBadC);
                            board(newBadR, newBadC)->setMoved(true);
                        }
                    }
                }
            }

            // Call findHero() to get and set current hero position
            findHero();

            // Check if hero position is invalid (-1, -1)
            if (HeroRow == -1 || HeroCol == -1){
                return false;
            }

            // If valid position, return true, continuing the game
            // Clear the terminal
            #ifdef _WIN32
                system("cls"); // For Windows
            #else
                system("clear"); // For Linux and macOS
            #endif
            return true;
        }
};

#endif //GAMEBOARD_H