#include <algorithm>
#include <cstdio>
#include <unistd.h> // för sleep
#include <termio.h>
#include <fcntl.h>
#include <bits/stdc++.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 10;


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
    int x;
    int y;
    int score = 0;
    int height = 1;
    Player(int start_pos_x, int start_pos_y){
        x = start_pos_x;
        y = start_pos_y;
    }; 
};

void drawScore(int score1, int score2){
    cout << "Player 1: " << score1 << "                  ";
    cout << "Player 2: " << score2 << endl;
}
void drawBoard(Ball& ball, Player& player1, Player& player2) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == ball.x && y == ball.y){
                cout << "o";
            }               	    	
            else if ((x == player1.x && y == player1.y) || (x == player2.x && y == player2.y)) {
                cout << "I";
            }
            else if (x == 0 || x == WIDTH - 1) {
                cout << "|";
            }
            // Toppen och botten
            else if (y == 0 || y == HEIGHT - 1) {
                cout << "-";
            }
            // Tom yta
            else {
                cout << " ";
            }
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


int main() {
    setNonBlockingInput();  
    Ball ball(20, 5);
    Player player1(1, 2);
    Player player2(WIDTH - 2 ,1);
    set<char> keys;
    while(true){
        
        system("clear");
        drawScore(player1.score, player2.score);
        drawBoard(ball, player1, player2);

        usleep(200000); // Pausa i 2 sek så du ser den
        // Move ball
        ball.x += ball.vel_x;
        ball.y += ball.vel_y;
        // check if player 1 hit ball
        if (ball.x == player1.x && (player1.y + player1.height >= ball.y && ball.y >= player1.y - player1.height)){
            ball.x -= ball.vel_x;
            ball.vel_x = ball.vel_x * (-1);
            ball.x += ball.vel_x; 
        }
        // check if player 2 hit ball
        if (ball.x == player2.x && ball.y == player2.y){
            ball.x -= ball.vel_x;
            ball.vel_x = ball.vel_x * (-1);
            ball.x += ball.vel_x; 
        }
        // Hit from side technology
        if (ball.x + 1 == player1.x && ball.y == player1.y){
            ball.x -= ball.vel_x;
            if (ball.vel_y == 0){
                ball.vel_y = 1;
            }
            ball.vel_x = ball.vel_x * (-1);
            ball.x += ball.vel_x; 
            ball.y += ball.vel_y;
        }
        
        // Hit from side technology
        if (ball.x - 1 == player2.x && ball.y == player2.y){
            ball.x -= ball.vel_x;
            if (ball.vel_y == 0){
                ball.vel_y = 1;
            }
            ball.vel_x = ball.vel_x * (-1);
            ball.x += ball.vel_x; 
            ball.y += ball.vel_y;
        }
        // Check if player 2 scored
        if (ball.x == 0){
            ball.x = 20;
            ball.y = 5;
            player2.score += 1;
        }
        // check if player 1 scored
        else if ( ball.x == WIDTH - 1) {
            ball.x = 20;
            ball.y = 5;
            player1.score += 1;
        }
        // check if we hit the ceiling
        if (ball.y == 0 || ball.y == HEIGHT - 1){
            ball.y -= ball.vel_y;
            ball.vel_y = ball.vel_y * (-1);
            ball.y += ball.vel_y;

        }
        
        keys = readLatestKey();
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
        
    }
    resetInput();
    return 0;
}


