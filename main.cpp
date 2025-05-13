#include <algorithm>
#include <cstdio>
#include <unistd.h> // för sleep
#include <termio.h>
#include <fcntl.h>
#include <bits/stdc++.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const char BALL = 'o';
const char WALL = '|';
const char FLOOR = '-';
const char EMPTY = ' ';
const int POINTS_TO_WIN = 3;

// Gör terminalen icke-blockerande
void setNonBlockingInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Stäng av "line mode" och echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Gör läsning icke-blockerande
}

// Återställ terminalinställningar
void resetInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


class Ball{
    public:
    int x;
    int y;
    int vel_x = -1;
    int vel_y = 0;
    Ball(int start_pos_x, int start_pos_y){
        x = start_pos_x;
	y = start_pos_y;	

    };
};

class Player{
public:
    int id;
    int x;
    int y;
    int score = 0;
    int height = 1;
    char sprite = 'I';
    Player(int start_pos_x, int start_pos_y, int _id){
        x = start_pos_x;
        y = start_pos_y;
        id = _id;
    }; 
};

void drawScore(int score1, int score2){
    cout << "Player 1: " << score1 << "                  ";
    cout << "Player 2: " << score2 << endl;
}
void drawBoard(Ball& ball, Player& player1, Player& player2) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) {
                cout << WALL;
                continue;
            }
            // Roof and Floor
            if (y == 0 || y == HEIGHT - 1) {
                cout << FLOOR; 
                continue;
            }
 
            if (x == player1.x && (player1.height + player1.y == y ||player1.y - player1.height  == y || player1.y == y)){
                cout << player1.sprite;
                continue;
            }
            if (x == player2.x && (player2.height + player2.y == y ||player2.y - player2.height  == y || player2.y == y)){
                cout << player2.sprite;
                continue;
            }
            if (x == ball.x && y == ball.y){
                cout << BALL;
                continue;
            }
           // Empty
            cout << EMPTY;
        }
        cout << endl;
    }
}


set<char> readLatestKey() {
    char c = 0;
    int ch;
    // Töm alla tangenttryck, behåll bara den sista
    char prev = -1;
    set<char> keypresses;
    while ((ch = getchar()) != -1) {
        keypresses.insert(ch);
    }
    return keypresses;
}

void checkPlayerCollision(Player& player, Ball& ball){
        int side = 0; 
        if (player.id == 1){
            side = -1; 
        }else{
            side = 1;
        }
        if (ball.x + side == player.x){
            if (player.y + player.height == ball.y){
                ball.vel_x = ball.vel_x * (-1);
                ball.vel_y = 1;
            }else if (ball.y == player.y - player.height) {
                ball.vel_x = ball.vel_x * (-1);
                ball.vel_y = -1; 
            }else if (player.y == ball.y) {
                ball.vel_x *= -1;
                ball.vel_y = 0;
            }
        }

}

void checkBallCollision(Ball& ball, Player& player1, Player& player2){
        if (ball.x == 0){
            ball.x = player2.x - 2;
            ball.y = player2.y;
            ball.vel_y = 0;
            ball.vel_x *= -1;
            player2.score += 1;
        }
        // check if player 1 scored
        else if ( ball.x == WIDTH - 1) {
            
            ball.x = player1.x + 2;
            ball.y = player1.y;
            ball.vel_y = 0;
            ball.vel_x *= -1;
            player1.score += 1;
        }
        // check if we hit the ceiling
        if (ball.y == 0 || ball.y == HEIGHT - 1){
            ball.y -= ball.vel_y;
            ball.vel_y = ball.vel_y * (-1);
            ball.y += ball.vel_y;

        }
 
}

bool checkWin(Player& player){
    if (player.score == POINTS_TO_WIN){
        return true;
    }
    return false;
}
int main() {
    setNonBlockingInput();  
    Ball ball(20, 5);
    Player player1(1, 2, 1);
    Player player2(WIDTH - 2 ,1, 2);
    set<char> keys;
    while(true){
        if (checkWin(player1)){
            drawScore(player1.score, player2.score);
            cout << "PLAYER 1 WINS!" << endl;
            break;
        } 
        if (checkWin(player2)){
            drawScore(player1.score, player2.score);
            cout << "PLAYER 2 WINS!" << endl;
            break;
        }
        system("clear");
        drawScore(player1.score, player2.score);
        drawBoard(ball, player1, player2);

        usleep(100000); // Pausa i 2 sek så du ser den
        for (char key : keys){
            cout << key;
            if (key == 'q' || key == 'Q'){
                break;
            }
            if (key == 'w' && player1.y != 1){
                player1.y -= 1;
            }
            else if (key == 's' && player1.y != HEIGHT - 2) {
                player1.y += 1;
            }
            if (key == 'i' && player2.y != 1){
                player2.y -= 1;
            }
            else if (key == 'k' && player2.y != HEIGHT - 2) {
                player2.y += 1;
            }
        }
        cout << endl;

        // Move ball
        ball.x += ball.vel_x;
        ball.y += ball.vel_y;
        // check if player 1 hit ball
        checkPlayerCollision(player1, ball);
        // check if player 2 hit ball
        checkPlayerCollision(player2, ball);
        checkBallCollision(ball, player1, player2);
        keys = readLatestKey();
    }
    resetInput();
    return 0;
}


