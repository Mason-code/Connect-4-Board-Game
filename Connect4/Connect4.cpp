#include <iostream>
#include <vector>

// tells compiler the functions exists
void resetGrid();
void printGrid();
int dropCoin(int column);
void opponentTurn();
int takeColInput();
void myTurn();

// constants
std::string EMPTY = "( )";
std::string ME = "(M)";
std::string OPPONENT = "(O)";

std::vector<std::vector<std::string>> grid; // grid[level][column]

int main()
{
    resetGrid();

    std::cout << "Do I go first? (yes/no)\n";
    std::string first;
    std::cin >> first;
    system("cls");

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
    int col = takeColInput(); // computer needs to do this
    int lvl = dropCoin(col);
    while (lvl == 0) {
        std::cout << "Column is full, pick another" << "\n";
        col = takeColInput();
        lvl = dropCoin(col);
    }
    grid[lvl-1][col-1] = ME;
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
    int col = takeColInput();
    int lvl = dropCoin(col);
    while (lvl == 0) {
        std::cout << "Column is full, pick another" << "\n";
        col = takeColInput();
        lvl = dropCoin(col);
    }
    grid[lvl-1][col-1] = OPPONENT;
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
        if (grid[lvl-1][column-1] == EMPTY) {
            return lvl;
        } else {
            lvl--;
        }
    }
}

