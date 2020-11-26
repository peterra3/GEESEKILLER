#include <iostream>
#include "geesespotter_lib.h"

char* createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char* board, std::size_t xdim, std::size_t ydim);
void hideBoard(char* board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char* board);
void printBoard(char* board, std::size_t xdim, std::size_t ydim);
int reveal(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char* board, std::size_t xdim, std::size_t ydim);

char* createBoard(std::size_t xdim, std::size_t ydim) {
	char* p_char(new char[xdim * ydim]{ 0 });

	return p_char;

}

void computeNeighbors(char* board, std::size_t xdim, std::size_t ydim) {
	int counter{ 0 };
	
	int myxdim{ 0 };
	int myydim{ 0 };

	myxdim = xdim;
	myydim = ydim;

	for (int a{ 0 }; a < myydim; ++a) {
		for (int b{ 0 }; b < myxdim; ++b) {

			if (board[a * myxdim + b] != 9) {
				for (int k{ a - 1 }; k <= a + 1; ++k) {
					for (int i{ b - 1 }; i <= b + 1; ++i) {
						//check if still on board
						if ((i < myxdim) && (k < myydim) && (i >= 0) && (k >= 0)) {
							if (((board[k * myxdim + i]) == 9)) {
								

								++counter;
							}
						}



					}
				}


				board[a * xdim + b] = counter;
				counter = 0;


			}


		}

	}
}




void hideBoard(char* board, std::size_t xdim, std::size_t ydim) {
	for (std::size_t k{ 0 }; k < xdim * ydim; ++k) {
		board[k] = board[k] | hiddenBit();

	}
}

void cleanBoard(char* board) {
	delete[] board;
	board = nullptr;

}

void printBoard(char* board, std::size_t xdim, std::size_t ydim) {
	std::size_t counter{ 0 };
	for (std::size_t k{ 0 }; k < ydim; ++k) {
		for (std::size_t k{ 0 }; k < xdim; ++k) {
			if ((board[counter] & markedBit()) == markedBit()) {
				std::cout << "M";
			}
			else if ((board[counter] & hiddenBit()) == hiddenBit()) {
				std::cout << '*';
			}
			//print adjacency value
			else {
				std::cout << (int)board[counter];
			}






			++counter;
		}
		std::cout << std::endl;
		

				
	}
}




int reveal(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

	
	
	if ((board[(yloc * xdim + xloc)] & markedBit()) == markedBit()) {

		return 1;
	}



	//hiddenbit should be false if it is not hidden (therefore revealed)
	else if ((board[yloc * xdim + xloc] & hiddenBit()) != hiddenBit()) {
		return 2;



	}

	//is a geese
	else if (((board[(yloc * xdim + xloc)] & valueMask()) == 9)) {
		board[(yloc * xdim + xloc)] = (board[(yloc * xdim + xloc)] & valueMask());

		return 9;
	}




	else {
		
		//^tells us that none above were actually a goose
		int myxloc{ 0 };
		int myyloc{ 0 };
		myxloc = xloc;
		myyloc = yloc;


		if (((board[(yloc * xdim + xloc)] & valueMask()) == 0)) {

			for (int reveal_y{ myyloc - 1 }; reveal_y <= myyloc + 1; ++reveal_y) {
				for (int reveal_x{ myxloc - 1 }; reveal_x <= myxloc + 1; ++reveal_x) {
					//check if still on board
					if ((reveal_x < xdim) && (reveal_y < ydim) && (reveal_x >= 0) && (reveal_y >= 0)) {




						//statement below tells us the field is not marked, and can go ahead and reveal
						if (((board[reveal_y * xdim + reveal_x] & markedBit()) == 0) && ((board[reveal_y * xdim + reveal_x] & valueMask()) != 9))

						{//form statement to reveal space
							board[reveal_y * xdim + reveal_x] = (board[reveal_y * xdim + reveal_x] & valueMask());

						}





					}
				}
			}
			//only return 0 will reveal


		}
		else {
			board[(yloc * xdim + xloc)] &= valueMask();

		}
		return 0;
	}


}
	




int mark(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	
	if ((board[yloc * xdim + xloc] & markedBit()) == markedBit()) {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] | hiddenBit();
		return 0;
	}

	else if ((board[yloc * xdim + xloc] & hiddenBit()) != hiddenBit()) {

		return 2;
	}
	else {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] | markedBit();
		return 0;

	}


}


bool isGameWon(char* board, std::size_t xdim, std::size_t ydim) {
	//counts how many are true
	std::size_t counter_geese{ 0 };
	std::size_t counter_revealed{ 0 };
	for (std::size_t k{ 0 }; k < xdim * ydim; ++k) {
		// count total amount of geese in game
		if ((board[k] & valueMask()) == 9) {
			++counter_geese;
		}
		//check and count how many are revealed (not hidden) and is not a goose (does not equal 9 underneath the valuemask)
		else if (((board[k] & hiddenBit()) == 0x00) && ((board[k] & valueMask()) != 9)) {
			++counter_revealed;
		}
	}

	if (xdim * ydim - counter_revealed == counter_geese) {
		return 1;
	}
	else {
		return 0;
	}
}
