
#include <iostream>
#include <vector>
using namespace std;


class Grid
{
public:
  Grid() {
    row=3;
    col=3;
    player=1;
    line_length=3;
    // 0 means empty cell, X=1, O=2
    board=vector<vector<int> > (row, vector<int> (col, 0));
  }

  virtual ~Grid(){}

  int get_player(){
    return player;
  }

private:
  vector<vector<int> > board;
  int row, col;
  //1 is X and goes first, 2 is O
  int player;
  int line_length;

  void set_cell(int r, int c, int value)
  {
    //If (r, c) is a valid cell, sets it to value
    //Otherwise, logs an error
    if (0<=r and r<row and 0<=c and c<col){
      board[r][c]=value;
    }
    else {
      cerr<<"Attempt to set invalid cell ("<<r<<", "<<c<<") to "<<value<<endl;
    }
  }

  int find_cell(int cell_num) {
    // The player inputs a number from 1 to 9, corresponding to a cell:
    /*
    0|1|2
    -+-+-
    3|4|5
    -+-+-
    6|7|8
    */
    //This returns the value of the corresponding cell
    return get(cell_num/row, cell_num%col);
  }

  char get_char(int cell_val) {
    // Converts 0, 1 and 2 to the relevant character
    switch (cell_val)
    {
    case 0:
      return ' ';
    case 1:
      return 'X';
    case 2:
      return 'O';

    default:
      break;
    }
  }

  void print() {
    for (int i = 0; i < row; ++i)
    {
      for (int j = 0; j < col; ++j)
      {
        cout<<get_char(get(i, j));
        if (j<col-1){
          cout<<"|";
        }
      }
      cout<<endl;
      if (i<row-1)
      {
        cout<<"-----\n";
      }
    }
  }

  int get(int r, int c){
    //Returns the cell (r, c)
    //If these are invalid coordinates, logs an error and returns -1
    if (0<=r and r<3 and 0<=c and c<3){
      return board[r][c];
    }
    else
    {
      cerr<<"Invalid coordinates used to access a cell ("<<r<<", "<<c<<")"<<endl;
      return 0;
    }
  }

  int check_line(int startr, int startc, int incr, int incc){
    //Verifies if the 3 cells (startr, startc), (startr+incr, startc+incc),
    //(startr+2*incr, startc+2*incc) have the same non-zero value, ie. are a
    //winning line for that player. Returns 0 if it is not a victory, and
    //return's the player's number if it is
    int first_cell=get(startr, startc);
    if (first_cell==0){
      return 0;
    }
    for (int i = 0; i < line_length-1; ++i)
    {
      startr+=incr;
      startc+=incc;
      if (first_cell!=get(startr, startc)){
        return 0;
      }
    }
    //This point is only reached if every element was the same and non-zero
    return first_cell;
  }

  int victory() {
    //Check vertical and horizontal lines
    int line_val;
    for (int r=0; r<row-line_length+1; r++){
      for (int c = 0; c < col; ++c)
      {
        line_val=check_line(r, c, 1, 0);
        if (line_val>0)
        {
          return line_val;
        }
      }
    }

    for (int r=0; r<row; r++){
      for (int c = 0; c < col-line_length+1; ++c)
      {
        line_val=check_line(r, c, 0, 1);
        if (line_val>0)
        {
          return line_val;
        }
      }
    }

    //Check diagonal lines
    for (int r=0; r<row-line_length+1; r++){
      for (int c = 0; c < col-line_length+1; ++c)
      {
        line_val=check_line(r, c, 1, 1);
        if (line_val>0)
        {
          return line_val;
        }
      }
    }

    for (int r=line_length-1; r<row; r++){
      for (int c = 0; c < col-line_length+1; ++c)
      {
        line_val=check_line(r, c, -1, 1);
        if (line_val>0)
        {
          return line_val;
        }
      }
    }

    //No one has won yet, so now check to see if the grid is full, ie. is it a
    //stalemate?
    for (int r = 0; r < row; ++r)
    {
      for (int c = 0; c < col; ++c)
      {
        if (get(r, c)==0)
        {
          return 0;
        }
      }
    }

    //This point can only be reached if no one has won and the grid is full - a
    //stalemate
    return 3;
  }

public:
  int move(int cell_num){
    //Takes in an integer from 0 to 8 corresponding to a cell
    //If a valid move, makes the move and prints the board
    //
    //Returns 0 if the move was normal, the player number if it was a victory,
    //3 if it's a stalemate, -1 if the cell is occupied and -2 if the cell
    //number is out of bounds
    if (0>cell_num or row*col<=cell_num){
      return -2;
    }
    else if (find_cell(cell_num)==0){
      set_cell(cell_num/row, cell_num%col, player);
      //cout<<cell_num/row<<" "<<cell_num%col<<endl;
      //Cycles between 1 and 2
      player=1+(player)%2;
      print();
      //Victory outputs an integer of the desired format
      return victory();
    }
    else
    {
      return -1;
    }
  }

};


int main(int argc, char *argv[])
{
  Grid board;
  //-1 if previous move failed, 0 if previous move was a success, 1 or 2 if the
  //corresponding player won, 3 if it's a stalemate
  int move_success=0;
  int cell_num;
  cout<<"Welcome to Noughts and Crosses!"<<endl;
  cout<<"Cells are labelled as follows:"<<endl;
  cout<<"0|1|2\n-+-+-\n3|4|5\n-+-+-\n6|7|8"<<endl;
  while (move_success<=0){
    if (move_success==0){
      cout<<"Player "<<board.get_player()<<"'s move"<<endl;
    }
    else if (move_success==-1) {
      cout<<"Sorry, that cell is occupied. Please try again"<<endl;
    }
    else if (move_success==-2){
      cout<<"Sorry, that is not a valid cell number. Please try again"<<endl;
    }
    else {
      cerr<<"Unspecified value returned by move"<<endl;
    }

    cout<<"Please enter a cell number from 0 to 8"<<endl;
    cin>>cell_num;
    move_success=board.move(cell_num);
  }
  cout<<"Game over"<<endl;
  switch (move_success)
  {
  case 1:
    cout<<"Player 1 wins!"<<endl;
    break;
  case 2:
    cout<<"Player 2 wins!"<<endl;
    break;
  case 3:
    cout<<"It's a draw!"<<endl;
    break;
  default:
    break;
  };

  return 0;
}
