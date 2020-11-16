#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

int createZombie() {
    if (rand() % 67 < 10)
        return 11;

    else
        return rand() % 10 + 1;
}

// game stats
int playerAlive = true;
int playerSkill = 9;
int playerScore = 1;
string playerName = "CasualGamer";
int zombieCount = 2;
int zombiesKilled = 0;


int main() {

    cout << "Get ready to fight for your life, " << playerName << "!" << endl;

    // main game loop
    while (playerAlive && zombiesKilled < zombieCount) {
        // create a random zombie
        int zombieSkill = createZombie();

        // battle sequence
        if (zombieSkill > 10) {
            cout << endl << "Here comes a huge zombie!" << endl;
        }
        else {
            cout << endl << "Here comes zombie " << zombiesKilled + 1 << endl;
        }

        cout << "Fighting..." << endl;
        Sleep(2);

        // zombie killed the player
        if (playerSkill < zombieSkill) {
            playerAlive = false;
            cout << "You have died." << endl;
        }

        // player killed the zombie
        else {
            if (playerSkill - zombieSkill > 7) {
                cout << "You wasted the zombie!" << endl;
                playerScore = playerScore * 2;
            }

            else if (playerSkill - zombieSkill > 5) {
                cout << "You decapitated the zombie!" << endl;
                playerScore = playerScore * 2;
            }

            else if (playerSkill - zombieSkill > 0) {
                cout << "You killed the zombie!" << endl;
                playerScore = playerScore * 2;
            }

            else {
                cout << "You killed the zombie, but suffered injuries." << endl;
            }

            zombiesKilled++;
        }

        cout << endl;
        cout << "Press [ENTER] to start next round.";
        cin.get();
    }

    // end game
    if (zombiesKilled == zombieCount) {
        // victory
        cout << "You have survived the onslaught!" << endl;
    }
    else {
        // lost
        cout << "You did not survive the zombie war." << endl;
    }

    cout << "Zombies killed: " << zombiesKilled << endl;
    cout << "Final score: " << playerScore << endl << endl;
    cin.get();
}