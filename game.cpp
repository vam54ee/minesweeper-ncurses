#include<iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>
#include <curses.h>
#include "matrixGraph.h"
#define MINETRUE 99
#define MINEFALSE 98
using namespace std;

void board(WINDOW *win, int starty, int startx, int lines, int cols,
  int tile_width, int tile_height
) {
  int endy, endx, i, j;

  endy = starty + lines * tile_height;
  endx = startx + cols  * tile_width;

  for(j = starty; j <= endy; j += tile_height)
    for(i = startx; i <= endx; ++i)
      mvwaddch(win, j, i, ACS_HLINE);
  for(i = startx; i <= endx; i += tile_width)
    for(j = starty; j <= endy; ++j)
      mvwaddch(win, j, i, ACS_VLINE);
  mvwaddch(win, starty, startx, ACS_ULCORNER);
  mvwaddch(win, endy, startx, ACS_LLCORNER);
  mvwaddch(win, starty, endx, ACS_URCORNER);
  mvwaddch(win, 	endy, endx, ACS_LRCORNER);
  for(j = starty + tile_height; j <= endy - tile_height; j += tile_height)
  {	mvwaddch(win, j, startx, ACS_LTEE);
    mvwaddch(win, j, endx, ACS_RTEE);
    for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
      mvwaddch(win, j, i, ACS_PLUS);
  }
  for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
  {	mvwaddch(win, starty, i, ACS_TTEE);
    mvwaddch(win, endy, i, ACS_BTEE);
  }
  wrefresh(win);
}


void drawInitialArena() {

}

void printHelp() {
  ifstream in("help.txt");
  stringstream buffer;
  buffer << in.rdbuf();
  string contents(buffer.str());
  cout<<contents<<endl;
}

bool getGameSize(string sizeInput, char& gameSize) {
  if (sizeInput.length() <= 0) return false;
  if (sizeInput[0] == 'S' || sizeInput[0] == 'M' || sizeInput[0] == 'L' || sizeInput[0] == 'C') {
    gameSize = sizeInput[0];
    return true;
  }
  return false;
}

// void insertMine(bool& a) {
//   if (countOfToBeInsertedMines > 0 && !a && rand() % 2 == 0) {
//     a = true;
//     countOfToBeInsertedMines--;
//   }
// }


class game {
  public:
    int m__ = 1;
    int nii = 1;
    int noOfMines = 0;
    static int countOfToBeInsertedMines;
    int gameWon = false;
    int gameLost = false;
    void play();
    game(int noOfX = 1, int noOfY = 1, int noOfO = 0) : m__(noOfX), nii(noOfY), noOfMines(noOfO){
      minesArray = matrixGraph<uint8_t>(m__, nii, MINEFALSE);
      statusArray = matrixGraph<char>(m__, nii, 'U');
    }

    void renderGrid() {
      board(stdscr, 0, 0, m__, nii, 4, 2);
      if (!initialClick) return;
      for (int i = 0; i < m__; i++) {
        for (int j = 0; j < nii; j++) {
          renderBox(i, j);
        }
      }
    }
    void init() {
      changeState(1, 1);
    }
    void printStatus() {
      statusArray.print();
    }

    void printMines() {
      minesArray.print();
      for (size_t i = 0; i < m__; i++) {
        for (size_t j = 0; j < nii; j++) {
          cout<<getNumberOfMinesAround(i,j)<<"\t";
        }
        cout<<endl;
      }
    }

    void resolveInput(int x, int y, bool leftClick = true) {
      char newState = changeState(x, y, leftClick);
      if (newState == 'O') {
        // reveal all mines and show gameover.
        // renderGrid();
        // refresh();
        revealAllMines();
        gameLost = true;
      }
      if (newState == '0') {
        revealAllZeroesUntilNonZero(x, y);
      }
      if (gameCompleted()) gameWon = true;
    }
    bool gameCompleted() {
      for (short i = 0; i < m__; i++) {
        for (short j = 0; j < nii; j++) {
          char currentStatus = statusArray(i, j);
          if (minesArray(i, j) == MINEFALSE &&  currentStatus == 'U' || currentStatus == 'F' || currentStatus == 'Q') return false;
        }
      }
      return true;
    }

    void revealAllMines() {
      // all ncurses windows should be called before calling this function.
      for (short i = 0; i < m__; i++) {
        for (short j = 0; j < nii; j++) {
          if (minesArray(i, j) == MINETRUE) {
            char newState = changeState(i, j);
            if (newState == 'O') {
              renderGrid();
              refresh();
            }
          }
        }
      }
    }

    void revealAllZeroesUntilNonZero(int x, int y) {
      for (short delX = -1; delX < 2; delX++) {
        for (short delY = -1; delY < 2; delY++) {
          if (!delX && !delY) continue;
          if (!(x + delX >= 0 && x + delX < m__ && y + delY >= 0 && y + delY < nii)) continue;
          if (statusArray(x + delX, y + delY) != 'U') continue;
          if (changeState(x + delX, y + delY) == '0') revealAllZeroesUntilNonZero(x + delX, y + delY);
        }
      }
    }

    char changeState(int x, int y, bool leftClick = true) {
      // returns true if it is a mine and state is other than F or Q else false
      if (leftClick && !initialClick) {
        game::countOfToBeInsertedMines = noOfMines;
        // while (game::countOfToBeInsertedMines > 0) minesArray.bfs(x, y, game::insertMine);
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> distribution1(0, m__ - 1);
        uniform_int_distribution<int> distribution2(0, nii - 1);
        while (game::countOfToBeInsertedMines > 0) {
          int xx = distribution1(mt);
          int yy = distribution2(mt);
          if (abs(xx - x) <= 1 &&  abs(yy - y) <= 1) continue;
          if (minesArray(xx, yy) != MINETRUE) {
            minesArray(xx, yy) = MINETRUE;
            countOfToBeInsertedMines--;
          }
        }
        initialClick = true;
      }
      if (leftClick && statusArray(x, y) != 'U') return '\0';
      if (!leftClick && (statusArray(x, y) != 'U' && statusArray(x, y) != 'F' && statusArray(x, y) != 'Q')) return '\0';
      if (!leftClick) {
        return statusArray(x, y) = statusArray(x, y) == 'U' ? 'F' : statusArray(x, y) == 'F' ? 'Q' : 'U' ;
      }
      if (minesArray(x, y) == MINETRUE) return statusArray(x, y) = 'O';
      int noOfMinesAround = getNumberOfMinesAround(x, y);
      return statusArray(x, y) = '0' + noOfMinesAround;
    }

  private:
    bool initialClick = false;
    matrixGraph<uint8_t> minesArray;
    matrixGraph<char> statusArray; // O: mineFound U: Untouched, 0-9: noOfMines, F:flag, Q: question mark
    int getNumberOfMinesAround(int x, int y) {
      int count = 0;
      for (short i = -1; i < 2; i++) {
        for (short j = -1; j < 2; j++) {
          if (!i && !j) continue;
          if (x + i >= 0 && x + i < m__ && y + j >= 0 && y + j < nii && minesArray(x + i, y + j) == MINETRUE) count++;
        }
      }
      return count;
    }
    static void insertMine(uint8_t& a) {
      random_device rd;
      mt19937 mt(rd());
      uniform_int_distribution<int> distribution(0, 1);
      int coinToss = distribution(mt);  // generates number in the range 1..6
      cout<<coinToss<<endl;
      if (game::countOfToBeInsertedMines > 0 && a != MINETRUE && coinToss == 1) {
        a = MINETRUE;
        game::countOfToBeInsertedMines--;
      }
    }


    void renderBox(int x, int y) {
      if (statusArray(x, y) == 'U') {
        mvwaddch(stdscr, 2 * y + 1, 4 * x + 2, ' ');
        return;
      }
      mvwaddch(stdscr, 2 * y + 1, 4 * x + 2, statusArray(x, y));
    }


    void resolveControllerInput(){
      string input;
      // while(!getGameSize(sizeInput, gameSize)) getline (cin, sizeInput);
    };

};

int game::countOfToBeInsertedMines;


int main() {
  char gameSize;
  string sizeInput;
  // printHelp();
  // cout<<"Enter game size:  ";
  int ct = 10;
  // while(!getGameSize(sizeInput, gameSize)) {while(_kbhit() && ct--)cout<<_getch()<<endl;/*getline (cin, sizeInput);cout<<sizeInput.length()<<endl;*/}
  game gameInstance = game(10, 10, 20);


  if (gameSize == 'M') gameInstance = game(20, 20, 20);
  else if (gameSize == 'L') gameInstance = game(30, 30, 30);
  else {
    // TODO: write inputs for C Custom.
  }

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  int ch;
  // gameInstance.init();
  // gameInstance.printStatus();
  // gameInstance.printMines();
  // gameInstance.resolveInput(2, 2);
  // gameInstance.printStatus();
  gameInstance.renderGrid();
    //  nodelay(stdscr, TRUE);
  mousemask(BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED, NULL);
  for (;;) {
    MEVENT event;

    ch = getch();
    if(ch == KEY_MOUSE) {
      if(getmouse(&event) == OK) {
        if (event.x / 4 >= gameInstance.m__ || event.x % 4 == 0) continue;
        if (event.y / 2 >= gameInstance.nii || event.y % 2 == 0) continue;
        stringstream ss;
        ss << event.x;
        ss << "\t";
        ss << event.y;
        ss << "\t";
        ss << event.z;
        if(event.bstate & BUTTON1_CLICKED) {
          mvaddstr(50, 0, ss.str().c_str());
          gameInstance.resolveInput(event.x / 4, event.y / 2, true);
        } else if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
          mvaddstr(50, 0, ss.str().c_str());
          gameInstance.resolveInput(event.x / 4, event.y / 2, false);
        }
      }
    }
    else continue;
    gameInstance.renderGrid();
    refresh();
    if (gameInstance.gameWon || gameInstance.gameLost) mvaddstr(50, 0, gameInstance.gameWon ? "won" : "lost");
  }

  endwin();
}
