#include <iostream>
#include <vector>
#include <bitset>
#include <vector>
#include <utility>

//Converter:
std::vector<short> convertBitmaskToShorts(short const bitmask);

//Excluders:
void excludeSingleNumInRow(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions);

void excludeSingleNumInColumn(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions);

void excludeSingleNumInBox(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions);

void excludeSingleNumber(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions);

void performExclusionByBitmask(short bitmask, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard,
	void (*excluder)(char, int, int, std::vector<std::vector<short>>&, std::vector<std::pair<short, short>> const&),
	std::vector<std::pair<short, short>>& exceptions);

//Solver rules:
bool findHiddenSingle(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard);

bool findHiddenPairs(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard);

bool findAndSeparateTriples(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard);

bool findAndSeparateUniforms(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard);

//Analyser tools:
int countActiveBits(short num);

//Diagnostic:
void printBoard(std::vector<std::vector<short>>& bitsetBoard);

void printBoard(std::vector<std::vector<char>>& board);

bool solveSoduku(std::vector<std::vector<char>>& board, std::vector<std::vector<short>>& bitsetBoard, int emptyFields);

//"Unit tests":

void testHiddenSingles(std::vector<std::vector<char>> board);

void testPairs(std::vector<std::vector<char>> board);

void testTriples(std::vector<std::vector<char>> board);

void testHiddenPairs(std::vector<std::vector<char>> board);


int main() {
	std::vector<std::vector<char>> board2= { {'5', '3', '.', '.', '7', '.', '.', '.', '.'}, 
											 {'6', '.', '.', '1', '9', '5', '.', '.', '.'}, 
											 {'.', '9', '8', '.', '.', '.', '.', '6', '.'},	
											 {'8', '.', '.', '.', '6', '.', '.', '.', '3'}, 
											 {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
											 {'7', '.', '.', '.', '2', '.', '.', '.', '6'}, 
											 {'.', '6', '.', '.', '.', '.', '2', '8', '.'}, 
											 {'.', '.', '.', '.', '1', '9', '.', '.', '5'}, 
											 {'.', '.', '.', '.', '8', '.', '.', '7', '9'} };

	std::vector<std::vector<char>> board = { {'.','.','9','7','4','8','.','.','.'},
											 {'7','.','.','.','.','.','.','.','.'},
											 {'.','2','.','1','.','9','.','.','.'},
											 {'.','.','7','.','.','.','2','4','.'},
											 {'.','6','4','.','1','.','5','9','.'},
											 {'.','9','8','.','.','.','3','.','.'},
											 {'.','.','.','8','.','3','.','2','.'},
											 {'.','.','.','.','.','.','.','.','6'},
											 {'.','.','.','2','7','5','9','.','.'} };
	//TESTING
	//std::vector<std::vector<char>> testBoardSP = { {'.','5','.','4','2','3','.','9','.'}, //hidden single (0,0 should be 7)
	//											   {'.','.','.','.','.','.','.','.','.'},
	//											   {'.','.','.','.','7','1','.','.','.'},
	//											   {'.','.','7','.','6','2','.','.','.'},
	//											   {'.','.','.','.','5','4','.','.','7'},
	//											   {'.','.','.','.','4','5','.','.','.'},
	//											   {'.','.','.','.','3','6','.','.','.'},
	//											   {'.','.','.','.','1','7','.','.','.'},
	//											   {'5','.','6','3','.','.','7','4','.'} }; //Pair is here (8,4 and 8,5)

	//std::vector<std::vector<char>> testBoardT = { {'2','.','.','.','.','.','.','.','.'}, //Triple is in the first column
	//											  {'.','.','.','3','.','.','.','.','.'},
	//											  {'.','.','.','6','7','.','.','.','.'},
	//											  {'4','.','.','.','.','.','.','.','.'},
	//											  {'.','.','1','.','.','.','.','.','.'},
	//											  {'.','.','3','.','.','.','7','.','.'},
	//											  {'.','1','.','.','.','.','.','.','.'},
	//											  {'5','3','.','.','.','.','.','.','.'},
	//											  {'8','.','.','.','.','.','.','.','.'} };

	//std::vector<std::vector<char>> testBoardP = { {'.','.','.','.','.','.','.','.','9'}, //Hidden pairs
	//											  {'.','.','.','.','.','.','.','.','.'},
	//											  {'.','.','.','.','.','.','.','.','.'},
	//											  {'.','.','.','.','.','.','.','.','.'},
	//											  {'.','.','.','.','.','.','.','.','.'},
	//											  {'.','.','.','.','.','.','3','.','1'},
	//											  {'.','.','.','.','.','.','.','.','.'},
	//											  {'.','1','9','3','.','.','2','.','.'},
	//											  {'7','.','.','.','4','8','.','6','.'} };

	///*std::cout << "Testing hidden sindgles" << "\n";
	//testHiddenSingles(testBoardSP);
	//std::cout << "Testing pairs" << "\n";
	//testPairs(testBoardSP);
	//std::cout << "Testing triples" << "\n";
	//testTriples(testBoardT);*/
	//std::cout << "Testing hidden pairs" << "\n";
	//testHiddenPairs(testBoardP);

	//FINAL SOLUTION:
	std::vector<std::vector<short>> bitsetBoard( 9, std::vector<short>(9, 511) );
	int emptyFields = 0;

	//Setting the initial bitmask based on the input table:
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] != '.') {
				bitsetBoard[i][j] = 0;
				excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
			}
			else {
				emptyFields++;
			}
		}
	}

	std::cout << "Afer setting the table: \n";
	printBoard(bitsetBoard);

	if (solveSoduku(board, bitsetBoard, emptyFields)) {
		std::cout << "We have found a solution:" << std::endl;
		printBoard(board);
	}
	else {
		std::cout << "There was no solution..." << std::endl;
	}

	return 0;
}

//Converter:
std::vector<short> convertBitmaskToShorts(short const bitmask) {
	std::vector<short> numbers;

	for (short i = 0; i < 9; i++) {
		if ((bitmask & (1 << i))) {
			numbers.push_back(i + 1);
		}
	}

	return numbers;
}

//Excluders:
void excludeSingleNumInRow(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions) {
	bool toBeSkipped = false;
	for (int i = 0; i < 9; ++i) {
		for (int k = 0; k < exceptions.size(); ++k) {
			if (exceptions[k].first == currentRow && exceptions[k].second == i) toBeSkipped = true;
		}
		if (!toBeSkipped) {
			bitsetBoard[currentRow][i] = bitsetBoard[currentRow][i] & ~(1 << (static_cast<int>(num) - 49));
		}
		toBeSkipped = false;
	}
}

void excludeSingleNumInColumn(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions) {
	bool toBeSkipped = false;
	for (int i = 0; i < 9; ++i) {
		for (int k = 0; k < exceptions.size(); ++k) {
			if (exceptions[k].first == i && exceptions[k].second == currentColumn) toBeSkipped = true;
		}
		if (!toBeSkipped) {
			bitsetBoard[i][currentColumn] = bitsetBoard[i][currentColumn] & ~(1 << (static_cast<int>(num) - 49));
		}
		toBeSkipped = false;
	}
}

void excludeSingleNumInBox(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions) {
	int startRow = (currentRow / 3) * 3;
	int startColumn = (currentColumn / 3) * 3;

	bool toBeSkipped = false;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < exceptions.size(); ++k) {
				if (exceptions[k].first == startRow + i && exceptions[k].second == startColumn + j) toBeSkipped = true;
			}
			if (!toBeSkipped) {
				bitsetBoard[startRow + i][startColumn + j] = bitsetBoard[startRow + i][startColumn + j] & ~(1 << (static_cast<int>(num) - 49));
			}
			toBeSkipped = false;
		}
	}
}

void excludeSingleNumber(char num, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard, std::vector<std::pair<short, short>> const& exceptions) {
	excludeSingleNumInRow(num, currentRow, currentColumn, bitsetBoard, exceptions);
	excludeSingleNumInColumn(num, currentRow, currentColumn, bitsetBoard, exceptions);
	excludeSingleNumInBox(num, currentRow, currentColumn, bitsetBoard, exceptions);
}

void performExclusionByBitmask(short bitmask, int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard,
							   void (*excluder)(char, int, int, std::vector<std::vector<short>>&, std::vector<std::pair<short, short>> const&),
	                           std::vector<std::pair<short, short>>& exceptions) {
	for (int i = 0; i < 9; i++) {
		if (bitmask & (1 << i)) {
			excluder(static_cast<char>(i + 48 + 1), currentRow, currentColumn, bitsetBoard, exceptions);
		}
	}
}

//Solver rules:
bool findHiddenSingle(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard) {
	short currentBitmask = bitsetBoard[currentRow][currentColumn];

	//checking a row
	for (int i = 0; i < 9; ++i) {
		if (i == currentColumn) continue;
		//std::cout << i << ".:" << std::endl;
		//std::cout << "Current Bitmask: " << std::bitset<9>(currentBitmask) << '\n';
		//std::cout << "Current Element: " << std::bitset<9>(bitsetBoard[currentRow][i]) << '\n';
		currentBitmask = currentBitmask & ~(bitsetBoard[currentRow][i]);
		//std::cout << "Result Bitmask:  " << std::bitset<9>(currentBitmask) << '\n';
	}

	if (countActiveBits(currentBitmask) == 1) {
		bitsetBoard[currentRow][currentColumn] = currentBitmask;
		return true;
	}
	else {
		currentBitmask = bitsetBoard[currentRow][currentColumn];
	}

	//checking a column
	for (int i = 0; i < 9; ++i) {
		if (i == currentRow) continue;
		currentBitmask = currentBitmask & ~(bitsetBoard[i][currentColumn]);
	}

	if (countActiveBits(currentBitmask) == 1) {
		bitsetBoard[currentRow][currentColumn] = currentBitmask;
		return true;
	}
	else {
		currentBitmask = bitsetBoard[currentRow][currentColumn];
	}

	//checking a box
	int startRow = (currentRow / 3) * 3;
	int startColumn = (currentColumn / 3) * 3;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (startRow + i == currentRow && startColumn + j == currentColumn) continue;
			currentBitmask = currentBitmask & ~(bitsetBoard[startRow + i][startColumn + j]);
		}
	}

	if (countActiveBits(currentBitmask) == 1) {
		bitsetBoard[currentRow][currentColumn] = currentBitmask;
		return true;
	}
	else {
		return false;
	}
}

bool findHiddenPairs(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard) {
	short tmpBitmask = 0;
	bool success = false;

	//Checking a row
	for (int i = 0; i < 9; ++i) {
		if (i == currentColumn) continue;
		tmpBitmask = (bitsetBoard[currentRow][i] & bitsetBoard[currentRow][currentColumn]);
		if (countActiveBits(tmpBitmask) >= 2) {
			//std::cout << "Found a possible hidden pair at " << currentRow << "," << i << " " << currentRow << "," << currentColumn << std::endl;
			for (int j = 0; j < 9; ++j) {
				if (j == currentColumn || j == i) {
					continue;
				}
				tmpBitmask = (tmpBitmask & ~bitsetBoard[currentRow][j]);
			}

			if (countActiveBits(tmpBitmask) == 2) {
				//std::cout << "Found a valid hidden pair at " << currentRow << "," << i << " " << currentRow << "," << currentColumn << std::endl;
				bitsetBoard[currentRow][i] = bitsetBoard[currentRow][currentColumn] = tmpBitmask;
				success = true;
			}
		}
	}

	//Checking a column
	for (int i = 0; i < 9; ++i) {
		if (i == currentRow) continue;
		tmpBitmask = (bitsetBoard[i][currentColumn] & bitsetBoard[currentRow][currentColumn]);
		if (countActiveBits(tmpBitmask) >= 2) {
			for (int j = 0; j < 9; ++j) {
				if (j == currentRow || j == i) {
					continue;
				}
				tmpBitmask = (tmpBitmask & ~bitsetBoard[j][currentColumn]);
			}

			if (countActiveBits(tmpBitmask) == 2) {
				bitsetBoard[i][currentColumn] = bitsetBoard[currentRow][currentColumn] = tmpBitmask;
				success = true;
			}
		}
	}

	//checking a box
	int startRow = (currentRow / 3) * 3;
	int startColumn = (currentColumn / 3) * 3;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (startRow + i == currentRow && startColumn + j == currentColumn) continue;
			tmpBitmask = (bitsetBoard[startRow + i][startColumn + j] & bitsetBoard[currentRow][currentColumn]);
			if (countActiveBits(tmpBitmask) >= 2) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						if ((startRow + k == currentRow && startColumn + l == currentColumn) || (startRow + k == startRow + i && startColumn + j == startColumn + l)) {
							continue;
						}
						tmpBitmask = (tmpBitmask & ~bitsetBoard[startRow + k][startColumn + l]);
					}
				}

				if (countActiveBits(tmpBitmask) == 2) {
					bitsetBoard[startRow + i][startColumn + j] = bitsetBoard[currentRow][currentColumn] = tmpBitmask;
					success = true;
				}
			}

		}
	}
	return success;
}

bool findAndSeparateTriples(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard) {
	bool success = false;

	if (countActiveBits(bitsetBoard[currentRow][currentColumn]) != 3) {
		return success;
	}

	std::vector<std::pair<short, short>> uniformCoordinates;

	//checking a row
	for (int i = 0; i < 9; ++i) {
		if (i == currentColumn) continue;
		if (countActiveBits(bitsetBoard[currentRow][i]) && (bitsetBoard[currentRow][i] & ~bitsetBoard[currentRow][currentColumn]) == 0) {
			uniformCoordinates.push_back(std::pair<short, short>(currentRow, i));
		}

	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInRow, uniformCoordinates);
		success = true;
	}
	uniformCoordinates.clear();

	//checking a column
	for (int i = 0; i < 9; ++i) {
		if (i == currentRow) continue;
		if (countActiveBits(bitsetBoard[i][currentColumn]) && (bitsetBoard[i][currentColumn] & ~bitsetBoard[currentRow][currentColumn]) == 0) {
			//std::cout << "WE HAVE FOUND A TRIPLE IN COLUMN:" << currentRow << "," << currentColumn << " and " << i << "," << currentColumn << std::endl;
			uniformCoordinates.push_back(std::pair<short, short>(i, currentColumn));
		}
	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInColumn, uniformCoordinates);
		success = true;
	}
	uniformCoordinates.clear();

	//checking a box
	int startRow = (currentRow / 3) * 3;
	int startColumn = (currentColumn / 3) * 3;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (startRow + i == currentRow && startColumn + j == currentColumn) continue;
			if (countActiveBits(bitsetBoard[startRow + i][startColumn + j]) && (bitsetBoard[startRow + i][startColumn + j] & ~bitsetBoard[currentRow][currentColumn]) == 0) {
				uniformCoordinates.push_back(std::pair<short, short>(startRow + i, startColumn + j));
			}
		}
	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInBox, uniformCoordinates);
		success = true;
	}

	return success;
}

bool findAndSeparateUniforms(int currentRow, int currentColumn, std::vector<std::vector<short>>& bitsetBoard) {
	std::vector<std::pair<short, short>> uniformCoordinates;
	bool success = false;

	//checking a row
	for (int i = 0; i < 9; ++i) {
		if (i == currentColumn) continue;
		if (bitsetBoard[currentRow][currentColumn] == bitsetBoard[currentRow][i]) {
			uniformCoordinates.push_back(std::pair<short, short>(currentRow, i));
		}
	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInRow, uniformCoordinates);
		success = true;
	}
	uniformCoordinates.clear();

	//checking a column
	for (int i = 0; i < 9; ++i) {
		if (i == currentRow) continue;
		if (bitsetBoard[currentRow][currentColumn] == bitsetBoard[i][currentColumn]) {
			uniformCoordinates.push_back(std::pair<short, short>(i, currentColumn));
		}
	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInColumn, uniformCoordinates);
		success = true;
	}
	uniformCoordinates.clear();

	//checking a box
	int startRow = (currentRow / 3) * 3;
	int startColumn = (currentColumn / 3) * 3;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (startRow + i == currentRow && startColumn + j == currentColumn) continue;
			if (bitsetBoard[currentRow][currentColumn] == bitsetBoard[startRow + i][startColumn + j]) {
				uniformCoordinates.push_back(std::pair<short, short>(startRow + i, startColumn + j));
			}
		}
	}

	if (uniformCoordinates.size() == (countActiveBits(bitsetBoard[currentRow][currentColumn]) - 1)) {
		uniformCoordinates.push_back(std::pair<short, short>(currentRow, currentColumn));
		performExclusionByBitmask(bitsetBoard[currentRow][currentColumn], currentRow, currentColumn, bitsetBoard, excludeSingleNumInBox, uniformCoordinates);
		success = true;
	}

	return success;
}

//Analyser tools:
int countActiveBits(short num) {
	int res = 0;
	for (int i = 0; i < 9; i++) {
		if (num & (1 << i)) {
			res++;
		}
	}

	return res;
}

//Diagnostic:
void printBoard(std::vector<std::vector<short>>& bitsetBoard) {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			std::cout << std::bitset<9>(bitsetBoard[i][j]) << ' ';
		}
		std::cout << "\n";
	}
}

void printBoard(std::vector<std::vector<char>>& board) {
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			std::cout << board[i][j] << ' ';
		}
		std::cout << "\n";
	}
}

bool solveSoduku(std::vector<std::vector<char>>& board, std::vector<std::vector<short>>& bitsetBoard, int emptyFields) {
	bool isStucked = true;
	while (1) {
		//Separate uniforms:
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				findAndSeparateUniforms(i, j, bitsetBoard);
			}
		}

		std::cout << "The boards after separate uniforms." << "\n";
		printBoard(bitsetBoard);
		printBoard(board);

		//Separate triples:
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				findAndSeparateTriples(i, j, bitsetBoard);
			}
		}

		std::cout << "The boards after separate triples." << "\n";
		printBoard(bitsetBoard);
		printBoard(board);

		//Find hidden pairs:
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				findHiddenPairs(i, j, bitsetBoard);
			}
		}

		std::cout << "The boards after hidden pairs." << "\n";
		printBoard(bitsetBoard);
		printBoard(board);

		//Resolving singles:
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (countActiveBits(bitsetBoard[i][j]) == 1) {
					board[i][j] = static_cast<char>(convertBitmaskToShorts(bitsetBoard[i][j])[0] + 48);
					bitsetBoard[i][j] = 0;
					excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
					emptyFields--;
					isStucked = false;
				}
			}
		}


		std::cout << "The boards after resolving singles." << "\n";
		printBoard(bitsetBoard);
		printBoard(board);

		//Find hidden singles:
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				if (findHiddenSingle(i, j, bitsetBoard)) {
					board[i][j] = static_cast<char>(convertBitmaskToShorts(bitsetBoard[i][j])[0] + 48);
					bitsetBoard[i][j] = 0;
					excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
					emptyFields--;
					isStucked = false;
				}
			}
		}

		std::cout << "The boards after finding hidden singles." << "\n";
		printBoard(bitsetBoard);
		printBoard(board);
		std::cout << "EMPTY FIELDS AFTER CURRENT ITERATION: " << emptyFields << std::endl;

		if (isStucked) {
			std::vector<short> probeCellValues;
			std::vector<std::vector<char>> boardSnapShot = board;
			std::vector<std::vector<short>> bitsetBoardSnapShot = bitsetBoard;


			for (int i = 0; i < 9; ++i) {
				for (int j = 0; j < 9; ++j) {
					if (countActiveBits(bitsetBoard[i][j] != 0)) {
						probeCellValues = convertBitmaskToShorts(bitsetBoard[i][j]);
						for (int index = 0; index < probeCellValues.size(); ++index) {
							board = boardSnapShot;
							bitsetBoard = bitsetBoardSnapShot;

							board[i][j] = static_cast<char>(probeCellValues[index] + 48);
							bitsetBoard[i][j] = 0;
							emptyFields--;
							std::cout << "We are trying: " << static_cast<char>(probeCellValues[index] + 48) << " at " << i << "," << j << std::endl;
							excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));

							if (solveSoduku(board, bitsetBoard, emptyFields)) {
								return true;
							}
							else {
								emptyFields++;
							}
						}
					}
				}
			}
			return false;
		}

		if (emptyFields == 0) {
			std::cout << "EMPTY FIELDS ARE ZERO" << std::endl;
			return true;
		}
		isStucked = true;
	}
}

//Unit tests:

void testHiddenSingles(std::vector<std::vector<char>> board) {
	std::vector<std::vector<short>> bitsetBoard(9, std::vector<short>(9, 511));
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] != '.') {
				bitsetBoard[i][j] = 0;
				excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
			}
		}
	}

	std::cout << "Afer setting the table: \n";
	printBoard(bitsetBoard);
	if (findHiddenSingle(0, 0, bitsetBoard)) {
		board[0][0] = static_cast<char>(convertBitmaskToShorts(bitsetBoard[0][0])[0] + 48);
		bitsetBoard[0][0] = 0;
		excludeSingleNumber(board[0][0], 0, 0, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(0, 0)));
	}

	std::cout << "The boards after resolving singles" << "\n";
	printBoard(bitsetBoard);
	printBoard(board);
}

void testPairs(std::vector<std::vector<char>> board) {
	std::vector<std::vector<short>> bitsetBoard(9, std::vector<short>(9, 511));
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] != '.') {
				bitsetBoard[i][j] = 0;
				excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
			}
		}
	}

	std::cout << "Afer setting the table: \n";
	printBoard(bitsetBoard);

	findAndSeparateUniforms(8, 4, bitsetBoard);

	std::cout << "The boards after separating pairs" << "\n";
	printBoard(bitsetBoard);
	printBoard(board);
}

void testTriples(std::vector<std::vector<char>> board) {
	std::vector<std::vector<short>> bitsetBoard(9, std::vector<short>(9, 511));
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] != '.') {
				bitsetBoard[i][j] = 0;
				excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
			}
		}
	}

	std::cout << "Afer setting the table: \n";
	printBoard(bitsetBoard);

	findAndSeparateTriples(4, 0, bitsetBoard);

	std::cout << "The boards after separating triples" << "\n";
	printBoard(bitsetBoard);
	printBoard(board);
}

void testHiddenPairs(std::vector<std::vector<char>> board) {
	std::vector<std::vector<short>> bitsetBoard(9, std::vector<short>(9, 511));
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (board[i][j] != '.') {
				bitsetBoard[i][j] = 0;
				excludeSingleNumber(board[i][j], i, j, bitsetBoard, std::vector<std::pair<short, short>>(1, std::pair<short, short>(i, j)));
			}
		}
	}

	std::cout << "Afer setting the table: \n";
	printBoard(bitsetBoard);

	findHiddenPairs(8, 3, bitsetBoard);

	std::cout << "The boards after finding hidden pairs" << "\n";
	printBoard(bitsetBoard);
	printBoard(board);
}