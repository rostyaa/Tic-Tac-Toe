#include <iostream> 
#include <string> 
#include <vector> 
#include <windows.h> 
#include <unordered_map> 
#include <thread>  
#include <chrono>  
#include <conio.h> 

using namespace std;


class Game {
public:

    vector<vector<char>> grid;
    vector<vector<int>> grid_for_calculations;
    bool playersTurn;
    char playerSign = 'X';
    bool isLost = false;
    int wins = 0;
    int loses = 0;
    int drawes = 0;
    int counter = 0;

    Game(bool players = true) {

        //    |    |    
        //    |    |    
        //    |    |    
        // ———————————— 
        //    |    |    
        //    |    |    
        //    |    |    
        // ————————————   
        //    |    |    
        //    |    |    
        //    |    |  

        playersTurn = players;
        vector<vector<char>> temp(11, vector<char>(11, ' '));
        grid = temp;
        for (int y = 0; y < 11; y++) {
            for (int x = 0; x < 11; x++) {
                if (x == 3 or x == 7) {
                    grid[y][x] = '|';
                }
                if (y == 3 or y == 7) {
                    grid[y][x] = '-';
                }
            }
        }
        vector<vector<int>> g(3, vector<int>(3));
        grid_for_calculations = g;
    }

    char get_bots_char() {
        return playerSign == 'X' ? '0' : 'X';
    }

    void delay(int time) {
        this_thread::sleep_for(chrono::milliseconds(time));
    }

    void draw() {
        cout << "   wins: " << wins << "   loses: " << loses << "   draws: " << drawes << endl << endl;
        string t = "          ";
        for (const auto& row : grid) {
            cout << t;
            for (const auto& e : row) {
                cout << e;
            }
            cout << endl;
        }
        cout << endl << "input collomn and row like that '1 3' " << endl;
    }

    bool isValid(string cords) {

        if (cords[0] - '0' > 3 or cords[0] - '0' < 1 or cords[2] - '0' > 3 or cords[2] - '0' < 1) {
            return false;
        }

        if (grid_for_calculations[cords[2] - '0' - 1][cords[0] - '0' - 1] != 0) {
            return false;
        }
        grid_for_calculations[cords[2] - '0' - 1][cords[0] - '0' - 1] = 1;
        return true;
    }

    void update_gird() {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (grid_for_calculations[y][x] == 1) {
                    grid[1 + 4 * y][1 + 4 * x] = playerSign;
                }
                else if (grid_for_calculations[y][x] == -1) {
                    grid[1 + 4 * y][1 + 4 * x] = get_bots_char();
                }
                else {
                    grid[1 + 4 * y][1 + 4 * x] = ' ';
                }
            }
        }
    }

    void delete_old() {
        system("cls");
    }

    int find_move(int search) {
        pair<int, int> compare = { -1, -1 };
        pair<int, int> cord;
        if (grid_for_calculations[1][1] == search) {
            if (grid_for_calculations[0][0] == search and grid_for_calculations[2][2] == 0) {
                return 9;
            }
            if (grid_for_calculations[2][2] == search and grid_for_calculations[0][0] == 0) {
                return 1;
            }
            if (grid_for_calculations[0][2] == search and grid_for_calculations[2][0] == 0) {
                return 7;
            }
            if (grid_for_calculations[0][2] == 0 and grid_for_calculations[2][0] == search) {
                return 3;
            }
        }
        for (int y = 0; y < 3; y++) {
            cord = compare;

            int counter = 0;
            for (int x = 0; x < 3; x++) {
                if (grid_for_calculations[y][x] == search) {
                    counter++;
                }
                if (grid_for_calculations[y][x] == 0) {
                    cord = { y, x };
                }
            }
            if (counter == 2 and cord != compare) {
                return cord.first * 3 + cord.second + 1;
            }
        }
        for (int x = 0; x < 3; x++) {
            cord = compare;

            int counter = 0;
            for (int y = 0; y < 3; y++) {
                if (grid_for_calculations[y][x] == search) {
                    counter++;
                }
                if (grid_for_calculations[y][x] == 0) {
                    cord = { y, x };
                }
            }
            if (counter == 2 and cord != compare) {
                return cord.first * 3 + cord.second + 1;
            }
        }
        return 0;
    }


    int otherMoves() {
        if (grid_for_calculations[0][0] == 0) {
            return 1;
        }
        if (grid_for_calculations[2][2] == 0) {
            return 9;
        }
        if (grid_for_calculations[2][0] == 0) {
            return 7;
        }
        if (grid_for_calculations[0][2] == 0) {
            return 3;
        }
        if (grid_for_calculations[1][0] == 0) {
            return 4;
        }
        if (grid_for_calculations[0][1] == 0) {
            return 2;
        }
        if (grid_for_calculations[2][1] == 0) {
            return 8;
        }
        if (grid_for_calculations[1][2] == 0) {
            return 6;
        }
    }

    void makeprediction() {
        int t;

        if (find_move(-1)) {
            t = find_move(-1) - 1;
            grid_for_calculations[t / 3][t % 3] = -1;
            isLost = true;
            return;
        }

        if (find_move(1)) {
            t = find_move(1) - 1;
            grid_for_calculations[t / 3][t % 3] = -1;
            return;
        }

        if (grid_for_calculations[1][1] == 0) {
            grid_for_calculations[1][1] = -1;
            return;
        }

        if ((grid_for_calculations[0][0] == 1 and grid_for_calculations[2][2] == 1) or (grid_for_calculations[2][0] == 1 and grid_for_calculations[0][2] == 1)) {
            if (grid_for_calculations[1][0] == 0 and grid_for_calculations[1][2] == 0) {
                grid_for_calculations[1][0] = -1;
            }
            else if (grid_for_calculations[0][1] and grid_for_calculations[2][1] == 0) {
                grid_for_calculations[0][1] = -1;
            }

            return;
        }

        t = otherMoves() - 1;
        grid_for_calculations[t / 3][t % 3] = -1;

    }

    void play() {
        for (; true;) {
            update_gird();
            delay(200);
            delete_old();
            draw();
            if (playersTurn) {
                string cords;
                getline(cin, cords);
                cords.resize(3);

                if (!isValid(cords)) {
                    delete_old();
                    continue;
                }

                playersTurn = false;

            }
            else {
                makeprediction();
                playersTurn = true;
                delay(200);
            }
            counter++;

            if (counter == 9 or isLost) {
                delete_old();
                update_gird();
                draw();
                if (isLost) {
                    cout << "You have lost. Press any button to continue";
                    while (true) {
                        if (_kbhit()) {
                            break;
                        }
                    }
                    loses++;
                }
                else {
                    cout << "Good game, but still only draw";
                    while (true) {
                        if (_kbhit()) {
                            break;
                        }
                    }
                    drawes++;
                }
                counter = 0;
                isLost = false;
                for (int y = 0; y < 3; y++) {
                    for (int x = 0; x < 3; x++) {
                        grid_for_calculations[y][x] = 0;
                    }
                }
                if (playerSign == 'X') {
                    playerSign = '0';
                    playersTurn = false;
                }
                else {
                    playerSign = 'X';
                    playersTurn = true;
                }
            }
        }

    }


};

int main() {
    Game g(true);
    g.play();

}
