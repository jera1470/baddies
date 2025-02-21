/*-------------------------------------------
Author: Jeremy Caceres
Fall 2023, UIC
------------------------------------------- */
#ifndef BOARDCELL_H
#define BOARDCELL_H

class BoardCell {

public:
    BoardCell() {}
    virtual ~BoardCell() {}

    virtual char display( ) = 0; // pure virtual function; this is an abstract base class

    // Virtual method that attempts to move the current object to a new row and column
    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
        newR = myRow;
        newC = myCol;
    }


    virtual bool isHero( ) {return false;}
    virtual bool isBaddie( ) {return false;}
    virtual bool isSpace( ) {return false;}
    virtual bool isBarrier( ) {return false;}
    virtual bool isHole( ) {return false;}
    virtual bool isExit( ) {return false;}
    virtual bool isStatic( ) {return true;}

    virtual void setPower(int num) {}
    virtual int getPower() {return 1;}
    virtual void setNextMove(char inChar) {}

    void setMoved(bool m) {moved = m;}
    bool getMoved() {return moved;}
    void setRow(size_t r) {myRow = r;}
    size_t getRow() {return myRow;}
    void setCol(size_t c) {myCol = c;}
    size_t getCol() {return myCol;}
    void setPos(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }

private:
    size_t myRow;
    size_t myCol;
    bool moved;   // boolean to signify if player has moved

}; // BoardCell (abstract base class)

class Hero: public BoardCell {

public:
    Hero(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }

    virtual bool isHero( ) {return true;}
    virtual bool isStatic( ) {return false;}
    virtual char display( ) {return 'H';}

    virtual void setNextMove(char inChar ) {
        nextMove = inChar;
    }

    // Override the base attemptMoveTo method
    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
        // Switch on the nextMove character
        switch(nextMove) {
                // Up-left
            case 'q':
                newR = hRow - 1;
                newC = hCol - 1;
                break;

                // Up
            case 'w':
                newR = hRow - 1;
                newC = hCol;
                break;

                // Up-right
            case 'e':
                newR = hRow - 1;
                newC = hCol + 1;
                break;

                // Left
            case 'a':
                newR = hRow;
                newC = hCol - 1;
                break;

                // Wait
            case 's':
                newR = hRow;
                newC = hCol;
                break;

                // Right
            case 'd':
                newR = hRow;
                newC = hCol + 1;
                break;

                // Down-left
            case 'z':
                newR = hRow + 1;
                newC = hCol - 1;
                break;

                // Down
            case 'x':
                newR = hRow + 1;
                newC = hCol;
                break;

                // Down-right
            case 'c':
                newR = hRow + 1;
                newC = hCol + 1;
                break;

                // Default case - no move
            default:
                newR = hRow;
                newC = hCol;
                break;
        }
    }


private:
    char nextMove;

}; // Hero


class Monster: public BoardCell {

public:
    Monster(size_t r, size_t c) {
        setRow(r);
        setCol(c);
        power = 1;
    }

    virtual bool isBaddie( ) {return true;}
    virtual bool isStatic( ) {return false;}

    virtual char display( ) {
        if (power == 2) {
            return 'M'; // Super Monster
        } else {
            return 'm'; // Monster
        }
    }

    virtual void setPower(int num) {
        power = num;
    }

    virtual int getPower() {
        return power;
    }

    // Override the base attemptMoveTo method for monsters and super monsters
    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) override {
        // Check if this monster's power level is 2 (super monster)
        if (getPower() == 2) {
            // Super monsters move 2 steps vertically and horizontally toward hero
            if (getRow() > hRow) {
                newR = getRow() - 2;
            }
            else if (getRow() < hRow) {
                newR = getRow() + 2;
            }
            else {
                newR = getRow();
            }

            if (getCol() > hCol) {
                newC = getCol() - 2;
            }
            else if (getCol() < hCol) {
                newC = getCol() + 2;
            }
            else {
                newC = getCol();
            }

        }
        else {
            // Regular monsters move 1 step vertically and horizontally toward hero
            if (getRow() > hRow) {
                newR = getRow() - 1;
            }
            else if (getRow() < hRow) {
                newR = getRow() + 1;
            }
            else {
                newR = getRow();
            }

            if (getCol() > hCol) {
                newC = getCol() - 1;
            }
            else if (getCol() < hCol) {
                newC = getCol() + 1;
            }
            else {
                newC = getCol();
            }
        }
    }


private:
    int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
public:
    Bat(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual bool isBaddie( ) {return true;}
    virtual bool isStatic( ) {return false;}
    virtual char display( ) {return '~';}

    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) override {
        newR = getRow();
        newC = hCol;
    }
}; // Bat


class Abyss: public BoardCell {
public:
    Abyss(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display( ) {return '#';}
    virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
public:
    Wall(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display( ) {return '+';}
    virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
public:
    Nothing(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display( ) {return ' ';}
    virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
public:
    EscapeLadder(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display( ) {return '*';}
    virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //BOARDCELL_H
