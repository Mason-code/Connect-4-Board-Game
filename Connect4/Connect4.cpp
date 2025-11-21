// manipulates human values but uses computer values when parsing
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>


// tells compiler the functions exists
void resetGrid();
void printGrid();
int dropCoin(int column);
void opponentTurn();
int takeColInput();
void myTurn();
std::vector<std::pair<int, int>> myPathfinder();
std::vector<std::vector<std::pair<int, int>>> findAllPossibleFours();
int preventOppFour();
int chooseMyColumn();

// constants
std::string EMPTY = "( )";
std::string ME = "(M)";
std::string OPPONENT = "(O)";
std::string POSSIBLE = "(P)";

std::vector<std::vector<std::string>> grid; // grid[level][column]
std::vector<std::vector<std::pair<int, int>>> possibilities;

int main()
{
    resetGrid();

    std::cout << "Do I go first? (yes/no)\n";
    std::string first;
    std::cin >> first;
    system("cls");
    possibilities = findAllPossibleFours();


    while (true) {
        printGrid();
        if (first != "yes") {
            opponentTurn();
            system("cls");
            printGrid();
            myTurn();
        }
        else {
            myTurn();
            system("cls");
            printGrid();
            opponentTurn();
        }
        system("cls");
    }
}

void myTurn() {
    std::cout << "-----------------" << "\n";
    std::cout << "Your Turn" << "\n";
    int col = chooseMyColumn(); // computer needs to do this
    int lvl = dropCoin(col);
    while (lvl == 0) {
        std::cout << "Column is full, pick another" << "\n";
        col = takeColInput();
        lvl = dropCoin(col);
    }
    grid[lvl-1][col] = ME;
    std::cout << "I chose column: " << col+1 << "\n" << std::endl;


    std::cout << "(Type something to continue!)" << std::endl;
    std::string temp = "";
    std::cin >> temp;
}


std::vector<std::pair<int, int>> myPathfinder() {
    std::vector<std::vector<std::string>> tempGrid = grid;
    std::vector<std::pair<int, int>> myIndexCoords; // [level],[column]
    for (int lvl = 0; lvl < 6; lvl++) {
        for (int col = 0; col < 7; col++) {
            if (tempGrid[lvl][col] == ME) {
                myIndexCoords.push_back({ lvl, col });
            }
        }
    }

    std::vector<std::pair<int,int>> chooseFromThese = {};
    bool foundMe = false;
    int howManyMe = 0;
    bool foundOpp = false;
    for (auto& fours : possibilities) {
        foundMe = false;
        foundOpp = false;
        howManyMe = 0;
        for (auto& coord : fours) {
            if (grid[coord.first][coord.second] == OPPONENT) {
                foundOpp = true;
            }
            if (grid[coord.first][coord.second] == ME) {
                foundMe = true;
                howManyMe++;
            }

        }

        if (foundMe && !foundOpp) {
            for (auto& coord : fours) {
                chooseFromThese.push_back({ coord.first, coord.second });
            }
        }
    }    


    //print
    for (int l = 0; l < chooseFromThese.size(); l++) {
        if (tempGrid[chooseFromThese[l].first][chooseFromThese[l].second] == EMPTY)
            tempGrid[chooseFromThese[l].first][chooseFromThese[l].second] = POSSIBLE;
    }
    std::cout << "---------My Possible moves--------" << std::endl;
    for (int i = 0; i < 6; i++) {
        for (int k = 0; k < 7; k++) std::cout << tempGrid[i][k];
        std::cout << "\n";
    }

    return chooseFromThese;
}

int preventOppFour() {
    std::vector<std::vector<std::string>> tempGrid = grid;
    std::vector<std::pair<int, int>> myIndexCoords; // [level],[column]
    for (int lvl = 0; lvl < 6; lvl++) {
        for (int col = 0; col < 7; col++) {
            if (tempGrid[lvl][col] == OPPONENT) {
                myIndexCoords.push_back({ lvl, col });
            }
        }
    }

    std::vector<std::pair<int, int>> chooseFromThese = {};
    int foundOpp = 0;
    std::pair<int, int> which = {};
    for (auto& fours : possibilities) {
        foundOpp = 0;
        which = {};
        for (auto& coord : fours) {
            if (grid[coord.first][coord.second] == OPPONENT) {
                foundOpp++;
            }
            else {
                which = { coord.first, coord.second };
            }
           

        }
        
        if (foundOpp == 3) {
            if (grid[which.first][which.second] != EMPTY) continue;

            int r = which.first;
            int c = which.second;
            if (r == 5 || grid[r + 1][c] != EMPTY) {
                return c; 
            }
        }

    }

    //print
    /*for (int l = 0; l < chooseFromThese.size(); l++) {
        if (tempGrid[chooseFromThese[l].first][chooseFromThese[l].second] == EMPTY)
            tempGrid[chooseFromThese[l].first][chooseFromThese[l].second] = POSSIBLE;
    }
    std::cout << "---------My Possible moves--------" << std::endl;
    for (int i = 0; i < 6; i++) {
        for (int k = 0; k < 7; k++) std::cout << tempGrid[i][k];
        std::cout << "\n";
    }*/

    return -1;
}


int chooseMyColumn() {
    int tempNum = preventOppFour();
    if (tempNum != -1) return tempNum;

    std::vector<std::pair<int, int>> options = myPathfinder();
    if (options.size() > 0) {
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, options.size()-1);
        return options[distrib(gen)].second;

    } else {
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, 6);
        while (true) {
            int randCol = distrib(gen);
            if (grid[0][randCol] == EMPTY) return randCol;
        } 
    }


}

// such a good idea
std::vector<std::vector<std::pair<int, int>>> findAllPossibleFours() {
    std::vector<std::vector<std::pair<int, int>>> possibilities; // [level-index5],[column-0]
    for (int lvl = 5; lvl >= 0; lvl--) {
        for (int col = 0; col < 7; col++) {
            // vert - up
            if (lvl - 3 >= 0) {
                possibilities.push_back({ { lvl,col }, { lvl - 1, col }, { lvl - 2, col }, { lvl - 3, col } });
            }

            // horiz - right
            if (col <= 3) {
                possibilities.push_back({ { lvl,col }, { lvl, col+1 }, { lvl, col +2}, { lvl, col +3} });

            }


            // diagnol - up right
            if (lvl - 3 >= 0 && 6 - col >= 3) {
                possibilities.push_back({ { lvl,col }, { lvl -1, col + 1 }, { lvl -2, col + 2}, { lvl-3, col + 3} });

            }

            // diagnol - up left
            if(lvl - 3 >= 0 && col - 3 >= 0) {
                possibilities.push_back({ { lvl,col }, { lvl - 1, col - 1 }, { lvl - 2, col - 2}, { lvl - 3, col - 3} });
            }
        }
    }
    return possibilities;
}

void resetGrid() 
{
    grid = {};
    for (int i = 0; i < 6; i++) {
        std::vector<std::string> temp = {};
        for (int k = 0; k < 7; k++) {
            temp.push_back(EMPTY);
        }
        grid.push_back(temp);
    }
}

void printGrid() 
{
    for (int i = 0; i < 6; i++) {
        for (int k = 0; k < 7; k++) std::cout << grid[i][k]; 
        std::cout << "\n";
    }
}

void opponentTurn() 
{
    std::cout << "-----------------" << "\n";
    std::cout << "Oppenent's Turn" << "\n";
    int col = takeColInput() -1;
    int lvl = dropCoin(col);
    while (lvl == 0) {
        std::cout << "Column is full, pick another" << "\n";
        col = takeColInput() -1;
        lvl = dropCoin(col);
    }
    grid[lvl-1][col] = OPPONENT;
}

int takeColInput() {
    int col;
    while (true) {
        std::cout << "What column? (1-7)" << std::endl;
        std::cin >> col;
        // thanks chat
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input.";
            continue;
        }
        if (col < 1 || col > 7) {
            std::cout << "Invalid input.";
            continue;
        }
        break;
    }
    return col;
}

int dropCoin(int column) // manipulates human values but uses computer values when parsing
{
    int lvl = 6;
    while (lvl > 0) {
        if (grid[lvl-1][column] == EMPTY) {
            return lvl;
        } else {
            lvl--;
        }
    }
    return 0;
}

