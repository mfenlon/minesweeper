#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;

// 0 indicates nothing in the square
// -1 indicates bomb
// >0 indicates number of bombs in the surrounding 8 squares

vector<vector<char> > generateBoard(int dimension)
{
	vector<vector<char> > board;
	int x,y;
	int mines_placed = 0;
	int mines_adjacent;

	cout << "The board will be " << dimension << "x" << dimension << endl;
	cout << "The second 0-9 sequence represents 10-19" << endl;
	cout << "Use 10-19 instead of 0-9 when referencing these squares" << endl << endl;

	// fill board with 0s
	vector<char> row;
	for (int i=0;i<dimension;i++)
	{
		row.push_back('0');
	}
	
	for (int j=0;j<dimension;j++)
	{
		board.push_back(row);
	}

	// randomly fill board with 50 mines, represented by M
	srand(time(0));

	while (mines_placed<50)  //50 should be dynamic as the board changes
	{
		x = rand() % dimension;
		y = rand() % dimension;

		if (board[x][y] == '0')
		{
			board[x][y] = 'M';
			mines_placed++;
		}
	}

	// fill remaining squares with number of adjacent mines
	for (int i=0;i<dimension;i++)
	{
		for (int j=0;j<dimension;j++)
		{
			if (board[i][j] == 'M') continue; // move on if a mine is here

			mines_adjacent = 0;
			if (i>0)
			{
				if (j>0 && board[i-1][j-1]=='M') mines_adjacent++;
				if (j<dimension-1 && board[i-1][j+1]=='M') mines_adjacent++;
				if (board[i-1][j]=='M') mines_adjacent++;
			}
			if (i<dimension-1)
			{
				if (j>0 && board[i+1][j-1]=='M') mines_adjacent++;
				if (j<dimension-1 && board[i+1][j+1]=='M') mines_adjacent++;
				if (board[i+1][j]=='M') mines_adjacent++;
			}
			if (j>0 && board[i][j-1]=='M') mines_adjacent++;
			if (j<dimension-1 && board[i][j+1]=='M') mines_adjacent++;

			board[i][j] = (char)mines_adjacent+48;
		}
	}

	return board;
}

vector<vector<char> > generateDisplayBoard(int dimension_int)
{
	vector<vector<char> > displayBoard;
	for (int i=0;i<dimension_int;i++)
	{
		vector<char> row;
		for (int j=0;j<dimension_int;j++)
		{
			row.push_back('-');
		}
		displayBoard.push_back(row);
	}
	return displayBoard;
}

int recurseZeros(vector<vector<char> >& board, vector<vector<char> >& displayBoard, int x, int y, int dimension)
{
	int total_changes = 0;
	if (displayBoard[x][y] != '-') total_changes = 0; // leave if we've already been to this square
	else 
	{
		displayBoard[x][y] = board[x][y];
		total_changes = 1;
		if (board[x][y] == '0')
		{
			total_changes = 1;
			displayBoard[x][y] = board[x][y];
			if (x>0)
			{
				total_changes += recurseZeros(board,displayBoard,x-1,y,dimension);
				if (y>0) total_changes += recurseZeros(board,displayBoard,x-1,y-1,dimension);
				if (y<dimension-1) total_changes += recurseZeros(board,displayBoard,x-1,y+1,dimension);
			}
			if (x<dimension-1)
			{
				total_changes += recurseZeros(board,displayBoard,x+1,y,dimension);
				if (y>0) total_changes += recurseZeros(board,displayBoard,x+1,y-1,dimension);
				if (y<dimension-1) total_changes += recurseZeros(board,displayBoard,x+1,y+1,dimension);
			}
			if (y>0) total_changes += recurseZeros(board,displayBoard,x,y-1,dimension);
			if (y<dimension-1) total_changes += recurseZeros(board,displayBoard,x,y+1,dimension);
		}
	}
	return total_changes;
}

void printBoard(vector<vector<char> >& board,int dimension)
{
	for (int i=0;i<dimension;i++)
	{
		if (i==0)
		{
			string s(41,'_');
			cout << "    ";
			for (int n=0;n<dimension;n++)
			{
				cout << n%10 << ' ';
			}
			cout << endl << "  " << s << endl;
		}
		cout << i%10 << " | ";
		for (int j=0;j<dimension;j++)
		{
			cout << board[i][j] << ' ';
		}
		cout << endl;
	}
}

int main()
{
	/*
	// get board size from user
	cout << endl << "Enter a board dimension (length and width will be the same)" << endl;
	cout << "For example, entering 10 will create a 10x10 board" << endl;
	cout << "Board sizes over 20x20 will default to 20x20" << endl;
	cout << "Negative entries will terminate the program" << endl << endl;
	cout << "Dimension: ";

	string dimension;
	cin >> dimension;

	int dimension_int = atoi(dimension.c_str());
	*/

	// initaialize board state
	int dimension_int = 20;

	vector<vector<char> > board = generateBoard(dimension_int);
	vector<vector<char> > displayBoard = generateDisplayBoard(dimension_int);

	// start game
	int squares_remaining = 350; //350 is the number of non-mine squares
	int squares_changed = 0;


	while (squares_remaining>0)
	{
		string str_input,str_x,str_y;
		string input;
		int x,y;

		printBoard(displayBoard,dimension_int);
		cout << "Squares remaining: " << squares_remaining << endl;

		cout << endl << "Prompts: (1) type of selection, (2) x coordinate, (3) y coordinate" << endl << endl;

		cout << endl << "(1) Enter 'r' to resolve a square or 'f' to flag a mine" << endl;
		cin >> input;

		cout << "(2) Enter an x coordinate" << endl;
		cin >> str_y;
		y = atoi(str_y.c_str());

		cout << "(3) Enter a y coordinate" << endl;
		cin >> str_x;
		x = atoi(str_x.c_str());

		// validate inputs
		if (x<0 || x>19 || y<0 || y>19)
		{
			cout << "Invalid coordinates. Press any key to try again" << endl << endl;
			continue;
		}

		if (input != "f" && input != "r")
		{
			cout << "Invalid command (input either f or r). Press any key to try again" << endl << endl;
			continue;
		}

		// use inputs to update display board
		if (input == "f")
		{
			if (displayBoard[x][y] == '-') displayBoard[x][y] = 'F';
			else cout << "Can't flag an already numbered square" << endl << endl;
		}

		else
		{
			if (board[x][y] == 'M')
			{
				cout << "You hit a mine. Bummer." << endl;
				break;
			}

			else if (board[x][y] != '0')
			{
				displayBoard[x][y] = board[x][y];
				squares_remaining--;
			}

			else
			{
				// recursively resolve 0s until all edges are either edges of the grid or numbers >0
				squares_changed = recurseZeros(board,displayBoard,x,y,dimension_int);
				squares_remaining -= squares_changed;
				squares_changed = 0;
			}
		}


	}


}