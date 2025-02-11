#include <iostream>
#include <cstdio>

using namespace std;
#define MAX 5

// 4. uzdevums

// Structure representing a single state transition in the Turing Machine
struct Clock {
    char symbol; // Symbol to write
    char move;   // Direction to move ('L' for left, 'R' for right, 'N' for no move)
    int state;   // Next state

    Clock(char s, char m, int st) {
        symbol = s;
        move = m;
        state = st;
    }
};

// Structure representing the program of the Turing Machine
struct Program {
    char* columns; // Array of symbols
    int symbolNumber; // Number of symbols
    int rows; // Number of states
    Clock*** table; // Transition table

    Program(char* symbols, int symNum, int statesNum, Clock*** t) {
        columns = symbols;
        symbolNumber = symNum;
        rows = statesNum;
        table = t;
    }

    // Get the column index for a given symbol
    int getColumnId(char c) {
        for (int i = 0; i < symbolNumber; i++) {
            if (columns[i] == c) {
                return i;
            }
        }
        return -1;
    }

    // Get the Clock (transition) for a given symbol and state
    Clock* getClock(char c, int state) {
        return table[state][getColumnId(c)];
    }
};

// Structure representing the Turing Machine
struct TuringMachine {

    char tape[MAX + 1]; // Tape of the Turing Machine
    char* head; // Head pointer
    int state; // Current state
    Program* program; // Program of the Turing Machine

    TuringMachine(Program* p) {
        program = p;
        Refresh();
    }

    // Move the head left
    void MoveLeft() {
        head = (head - 1);
    }

    // Move the head right
    void MoveRight() {
        head = (head + 1);
    }

    // Write a symbol at the current head position
    void WriteSymbol(char s) {
        *head = s;
    }

    // Execute a single transition (Clock)
    void ExecuteClock(Clock* clock) {
        WriteSymbol(clock->symbol);
        switch (clock->move) {
        case 'L': MoveLeft(); break;
        case 'R': MoveRight(); break;
        default: break;
        }
        state = clock->state;
    }

    // Print the current tape
    void WriteTape() {
        for (int i = 0; i < MAX; i++) {
            cout << tape[i];
        }
    }

    // Input a word into the tape
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

    // Refresh the Turing Machine (reset tape and state)
    void Refresh() {
        head = tape;
        state = 0;
        for (int i = 0; i < MAX + 1; i++) {
            tape[i] = '*';
        }
    }

    // Execute the entire program
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

    // Print the current transition (Clock)
    void WriteClock(Clock* clock) {
        cout << clock->symbol << ',' << clock->move << ',' << state;
    }
};

int main() {

    // Define the symbols used in the Turing Machine
    char symbols[] = { 'a', 'b', 'c', '*' };

    // Create the transition table
    Clock*** table = new Clock * *[10];
    for (int i = 0; i < 10; i++) {
        table[i] = new Clock * [4];
    }

    // Define the transitions for each state and symbol
    table[0][0] = new Clock('*', 'R', 1); // a >> * >> q1
    table[0][1] = new Clock('*', 'R', 2); // b >> * >> q2
    table[0][2] = new Clock('*', 'R', 3); // c >> * >> q3
    table[0][3] = new Clock('*', 'N', -1); // * >> stop
    //  ---------------------------------------------------------------------------
    //a
    table[1][0] = new Clock('a', 'R', 1); // R līdz beigām
    table[1][1] = new Clock('b', 'R', 1); // R līdz beigām
    table[1][2] = new Clock('c', 'R', 1); // R līdz beigām
    table[1][3] = new Clock('*', 'L', 4); // ja * >> L >> q4
    //b
    table[2][0] = new Clock('a', 'R', 2); // R līdz beigām
    table[2][1] = new Clock('b', 'R', 2); // R līdz beigām
    table[2][2] = new Clock('c', 'R', 2); // R līdz beigām
    table[2][3] = new Clock('*', 'L', 5); // ja * >> L >> q5
    //c
    table[3][0] = new Clock('a', 'R', 3); // R līdz beigām
    table[3][1] = new Clock('b', 'R', 3); // R līdz beigām
    table[3][2] = new Clock('c', 'R', 3); // R līdz beigām
    table[3][3] = new Clock('*', 'L', 6); // ja * >> L >> q6
    //  ---------------------------------------------------------------------------
    //a
    table[4][0] = new Clock('a', 'L', 7); // ieraksta a >> L >> q7 ja pirmais a
    table[4][1] = new Clock('a', 'L', 8); // ieraksta a >> L >> q8 ja pirmais b
    table[4][2] = new Clock('a', 'L', 9); // ieraksta a >> L >> q8 ja pirmais c
    table[4][3] = new Clock('*', 'N', -1);
    //b
    table[5][0] = new Clock('b', 'L', 7); // ieraksta b >> L >> q7 ja pirmais a
    table[5][1] = new Clock('b', 'L', 8); // ieraksta b >> L >> q8 ja pirmais b
    table[5][2] = new Clock('b', 'L', 9); // ieraksta b >> L >> q9 ja pirmais c
    table[5][3] = new Clock('*', 'N', -1); 
    //c
    table[6][0] = new Clock('c', 'L', 7); // ieraksta c >> L >> q7 ja pirmais a
    table[6][1] = new Clock('c', 'L', 8); // ieraksta c >> L >> q8 ja pirmais b
    table[6][2] = new Clock('c', 'L', 9); // ieraksta c >> L >> q9 ja pirmais c
    table[6][3] = new Clock('*', 'N', -1);
    //  ---------------------------------------------------------------------------
    //a
    table[7][0] = new Clock('a', 'L', 7); // L līdz sākumam
    table[7][1] = new Clock('b', 'L', 7); // L līdz sākumam
    table[7][2] = new Clock('c', 'L', 7); // L līdz sākumam
    table[7][3] = new Clock('a', 'N', -1); // ieraksta a >> beidz darbu
    //b
    table[8][0] = new Clock('a', 'L', 8); // L līdz sākumam
    table[8][1] = new Clock('b', 'L', 8); // L līdz sākumam
    table[8][2] = new Clock('c', 'L', 8); // L līdz sākumam
    table[8][3] = new Clock('b', 'N', -1); // ieraksta b >> beidz darbu
    //c
    table[9][0] = new Clock('a', 'L', 9); // L līdz sākumam
    table[9][1] = new Clock('b', 'L', 9); // L līdz sākumam
    table[9][2] = new Clock('c', 'L', 9); // L līdz sākumam
    table[9][3] = new Clock('c', 'N', -1); // ieraksta c >> beidz darbu

    // Create the program and Turing Machine
    Program prog = Program(symbols, 4, 10, table);
    TuringMachine tm = TuringMachine(&prog);

    // Input a word into the Turing Machine
    tm.InputWord();
    cout << endl;
    tm.WriteTape();
    cout << endl << endl;

    // Execute the program
    tm.ExecuteProgram();

    // Clean up dynamically allocated memory
    for (int h = 0; h < 10; h++) {
        for (int w = 0; w < 4; w++) {
            delete table[h][w];
        }
        delete[] table[h];
    }
    delete[] table;

    system("pause");

    return 0;
}
