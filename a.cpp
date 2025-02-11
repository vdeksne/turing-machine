#include <iostream>
#include <cstdio>

using namespace std;
#define MAX 5

// 2. uzdevums 
struct Clock {
    char symbol; // Symbol to write on the tape
    char move; // Direction to move the tape head ('L' for left, 'R' for right, 'N' for no move)
    int state; // Next state of the Turing machine

    // Constructor to initialize the Clock structure
    Clock(char s, char m, int st) {
        symbol = s;
        move = m;
        state = st;
    }
};

struct Program {
    char* columns; // Array of symbols
    int symbolNumber; // Number of symbols
    int rows; // Number of states
    Clock*** table; // Transition table

    // Constructor to initialize the Program structure
    Program(char* symbols, int symNum, int statesNum, Clock*** t) {
        columns = symbols;
        symbolNumber = symNum;
        rows = statesNum;
        table = t;
    }

    // Function to get the column index of a symbol
    int getColumnId(char c) {
        for (int i = 0; i < symbolNumber; i++) {
            if (columns[i] == c) {
                return i;
            }
        }
        return -1;
    }

    // Function to get the Clock structure for a given symbol and state
    Clock* getClock(char c, int state) {
        return table[state][getColumnId(c)];
    }
};

struct TuringMachine {

    char tape[MAX + 1]; // Tape of the Turing machine
    char* head; // Head pointer to the current position on the tape
    int state; // Current state of the Turing machine
    Program* program; // Program to be executed by the Turing machine

    // Constructor to initialize the TuringMachine structure
    TuringMachine(Program* p) {
        program = p;
        Refresh();
    }

    // Function to move the head left
    void MoveLeft() {
        head = (head - 1);
    }

    // Function to move the head right
    void MoveRight() {
        head = (head + 1);
    }

    // Function to write a symbol at the current head position
    void WriteSymbol(char s) {
        *head = s;
    }

    // Function to execute a Clock structure
    void ExecuteClock(Clock* clock) {
        WriteSymbol(clock->symbol);
        switch (clock->move) {
        case 'L': MoveLeft(); break;
        case 'R': MoveRight(); break;
        default: break;
        }
        state = clock->state;
    }

    // Function to write the tape content to the console
    void WriteTape() {
        for (int i = 0; i < MAX; i++) {
            cout << tape[i];
        }
    }

    // Function to input a word into the tape
    void InputWord() {
        char symbol;
        for (int i = 0, stop = false; i < MAX && !stop; i++) {
            cout << 's' << i << ": ";
            symbol = getchar();
            switch (symbol) {
            case 'a': tape[i] = 'a'; break;
            case 'b': tape[i] = 'b'; break;
            case 'c': tape[i] = 'c'; break;
            default: stop = true; break;
            }
            symbol = getchar();
        }
    }

    // Function to refresh the tape and reset the state
    void Refresh() {
        head = tape;
        state = 0;
        for (int i = 0; i < MAX + 1; i++) {
            tape[i] = '*';
        }
    }

    // Function to execute the program on the Turing machine
    void ExecuteProgram() {
        char visibleChar;
        Clock* currClock;
        while (state != -1) {
            visibleChar = *head;
            currClock = program->getClock(visibleChar, state);
            WriteClock(currClock);
            ExecuteClock(currClock);
            cout << " ";
            WriteTape();
            cout << endl;
        }
    }

    // Function to write the current Clock structure to the console
    void WriteClock(Clock* clock) {
        cout << clock->symbol << ',' << clock->move << ',' << state;
    }
};

int main() {

    char symbols[] = { 'a', 'b', 'c', '*' }; // Array of symbols

    // Create the transition table
    Clock*** table = new Clock * *[4];
    for (int i = 0; i < 4; i++) {
        table[i] = new Clock * [4];
    }
    table[0][0] = new Clock('*', 'R', 1);
    table[0][1] = new Clock('*', 'R', 2);
    table[0][2] = new Clock('*', 'R', 3);
    table[0][3] = new Clock('*', 'N', -1);

    table[1][0] = new Clock('a', 'R', 1);
    table[1][1] = new Clock('b', 'R', 1);
    table[1][2] = new Clock('c', 'R', 1);
    table[1][3] = new Clock('a', 'R', -1);

    table[2][0] = new Clock('a', 'R', 2);
    table[2][1] = new Clock('b', 'R', 2);
    table[2][2] = new Clock('c', 'R', 2);
    table[2][3] = new Clock('b', 'R', -1);

    table[3][0] = new Clock('a', 'R', 3);
    table[3][1] = new Clock('b', 'R', 3);
    table[3][2] = new Clock('c', 'R', 3);
    table[3][3] = new Clock('c', 'R', -1);

    // Create the program and Turing machine
    Program prog = Program(symbols, 4, 4, table);
    TuringMachine tm = TuringMachine(&prog);

    // Input a word into the tape
    tm.InputWord();
    cout << endl;
    tm.WriteTape();
    cout << endl << endl;

    // Execute the program
    tm.ExecuteProgram();

    // Clean up the transition table
    for (int h = 0; h < 4; h++) {
        for (int w = 0; w < 4; w++) {
            delete table[h][w];
        }
        delete[] table[h];
    }
    delete[] table;

    cout << "Press Enter to continue..."; 
    cin.ignore();
    cin.get();

    return 0;
}
