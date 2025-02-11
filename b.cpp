#include <iostream>

using namespace std;

#define MAX 5	//maximal length of word

// 3. uzdevums
struct Clock {
	char symbol;
	char move;
	int state;

	Clock(char s, char m, int st) {
		symbol = s;
		move = m;
		state = st;
	}
};

struct Program {
	char* columns;
	int symbolNumber;
	int rows;
	Clock*** table;

	Program(char* symbols, int symNum, int statesNum, Clock*** t) {
		columns = symbols;
		symbolNumber = symNum;
		rows = statesNum;
		table = t;
	}

	int getColumnId(char c) {
		for (int i = 0; i < symbolNumber; i++) {
			if (columns[i] == c) {
				return i;
			}
		}
		return -1;
	}

	Clock* getClock(char c, int state) {
		return table[state][getColumnId(c)];
	}
};

struct TuringMachine {

	char tape[MAX + 1];
	char* head;
	int state;
	Program* program;

	TuringMachine(Program* p) {
		program = p;
		Refresh();
	}

	void MoveLeft() {
		head = (head - 1);
	}

	void MoveRight() {
		head = (head + 1);
	}

	void WriteSymbol(char s) {
		*head = s;
	}

	void ExecuteClock(Clock* clock) {
		WriteSymbol(clock->symbol);
		switch (clock->move) {
		case 'L': {
			MoveLeft();
			break;
		}
		case 'R': {
			MoveRight();
			break;
		}
		default: {
			break;
		}
		}
		state = clock->state;
	}

	/**
	 * @brief Writes the current state of the tape to the standard output.
	 */
	void WriteTape() {
		for (int i = 0; i < MAX; i++) {
			cout << tape[i];
		}
	}

	/**
	 * @brief Inputs a word into the tape from the standard input.
	 */
	void InputWord() {
		char symbol;
		for (int i = 0, stop = false; i < MAX && !stop; i++) {
			cout << 's' << i << ": ";
			symbol = getchar();
			cin.get(symbol);
			switch (symbol) {
			case 'a': {
				tape[i] = 'a';
				break;
			}
			case 'b': {
				tape[i] = 'b';
				break;
			}
			case 'c': {
				tape[i] = 'c';
				break;
			}
			default: {
				stop = true;
				break;
			}
			}
			}
			symbol = getchar();	//ENTER, ASCII code = 13
			cin.get(symbol);	//ENTER, ASCII code = 13
	}

	void Refresh() {
		head = tape;
		state = 0;
		for (int i = 0; i < MAX + 1; i++) {
			tape[i] = '*';
		}
	}

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

	void WriteClock(Clock* clock) {
		cout << clock->symbol << ',' << clock->move << ',' << state;
	}
};

int main() {

	char symbols[] = { 'a', 'b', 'c', '*' };		// '*' = empty

	Clock*** table = new Clock * *[2];
	for (int i = 0; i < 2; i++) {
		table[i] = new Clock * [4];
	}
	table[0][0] = new Clock('a', 'R', 0);
	table[0][1] = new Clock('b', 'R', 0);
	table[0][2] = new Clock('c', 'R', 1);
	table[0][3] = new Clock('*', 'N', -1);

	table[1][0] = new Clock('a', 'N', -1);
	table[1][1] = new Clock('a', 'N', -1);
	table[1][2] = new Clock('a', 'N', -1);
	table[1][3] = new Clock('a', 'N', -1);

	Program prog = Program(symbols, 4, 2, table);

	TuringMachine tm = TuringMachine(&prog);

	tm.InputWord();
	cout << endl;
	tm.WriteTape();
	cout << endl << endl;

	tm.ExecuteProgram();

	for (int h = 0; h < 2; h++) {
		for (int w = 0; w < 4; w++) {
			delete table[h][w];
		}
		delete[] table[h];
	}
	delete[] table;

	system("pause");
}