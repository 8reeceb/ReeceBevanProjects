#include <iostream>
#include <conio.h> // for keyboard input
#include <random> //for random generation
#include <ctime> //for random generation
#include <windows.h> //for sleep method



using namespace std;


class Node {

public:
    int x;
    int y;
    Node* next; 

    Node(int xval, int yval) {
        x = xval;
        y = yval;
        next = nullptr; 
    }
};


class SnakeGame {

    private:
        Node* head;
        Node* tail;
        int length;
        bool bgrow = false;
        int foodx;
        int foody;
        int score = 0;


    public:
        SnakeGame(int startx, int starty) {
            head = new Node(startx, starty); //creates a new node with starting coordinates
            tail = head; //length of 1 means head and tail are the same
            length = 1;
        }

        void move(int mx, int my){ //m stands for move, so move x and move y
            Node* newhead = new Node(head->x + mx, head->y + my); //adds a new head that is one position forward of the previous head
            newhead->next = head; //now the new head's next is the previous head
            head = newhead; //resets the head
            if (bgrow == false) {
                Node* temp = head; //sets temp to the first node
                while (temp->next != tail) temp = temp->next; //while the next node is not the tail, starting at the front. Finds the second to last node
                delete tail; //deletes the tail
                tail = temp; //sets the tail as the second to last node
                tail->next = nullptr; //further sets the tail to the second last node 
            } else {
                bgrow = false;
            }
        }

        void grow() {
            bgrow = true; //if true, the snake doesn't delete the tail like in the move function, allowing it to grow
            length++; //increases the length counter
            score++; //increases the score
            
        }

        void foodSpawn(){
            static mt19937 rng(static_cast<unsigned int>(time(0))); 
            uniform_int_distribution<int> randX(2, 18); //creates a random from 2-18
            uniform_int_distribution<int> randY(2, 18);
            foodx = randX(rng); //sets the random numbers with the rng to the food
            foody = randY(rng);
        }

        void drawBoard(){
            int const height = 20;
            int const width = 20;
            for (int y = 0; y < height; y++){ //for each x and y value
                for (int x = 0; x < width; x++){
                    if (y == 0 || y == height - 1 || x == 0 || x == width - 1){
                    cout << "#"; //the border of the game
                    }
                    else if (x == foodx && y == foody) {
                        cout << "*"; //checks for the spot of the food
                    }
                    else {
                        bool snakeCheck = false; //checks if the coordinate is part of the snake
                        Node* cur = head;
                        if (cur != nullptr) { //checks if the head coordinates are the same as the position in the loop
                            if (x == cur->x && y == cur->y) {
                                cout << "@"; // head
                                snakeCheck = true; //turns the check to true
                            } else {
                                cur = cur->next; //the rest of the body and checks the coordinates for the body
                                while (cur != nullptr) {
                                    if (x == cur->x && y == cur->y) {
                                        cout << "O"; //the rest of the body
                                        snakeCheck = true;
                                        break; 
                                    }
                                cur = cur->next;
                                } 
                            }  
                            
                        }
                        if (!snakeCheck) //if the coordinate is not part of the snake, then it prints a blank space 
                        cout << " ";
                    }
                }
                cout << endl; //end of the x values for the y value or end of the row
            }
        }

        bool borderCollision(int height, int width){ //boolean to detect true or false
            return (head->x <= 0 || head->x >= width - 1 || //returns true if the head x is less than or equal to 0 (left wall) or greater or equal to width - 1 (right wall)
                    head->y <= 0 || head->y >= height - 1); //vise versa
        }

        bool selfCollision() {
            Node* cur = head->next;  // skips the head
            while (cur != nullptr) { //loops through each body part
                if (head->x == cur->x && head->y == cur->y) //if the head coordinates are equal to any of the body parts, it will return true
                    return true;
                cur = cur->next; //checks for the next node
            }
            return false; //otherwise return false
        }

        bool foodCollision() { 
            return (head->x == foodx && head->y == foody); //returns true if coordinates of the head are the same as the food
        }

        bool checkCollisions(int width, int height) { //uses the previous functions in one check function
            if (borderCollision(width, height)) {
                cout << "Game Over! You hit a wall." << endl;
                cout << "Score: " << score;
                return true;
            }
            if (length > 1 && selfCollision()){ //only detects if there is more than just the head
                cout << "Game Over! You hit yourself." << endl;
                    cout << "Score: " << score;
                    return true;
            }

            if (foodCollision()) {
                grow(); //will grow at the current coordinates
                foodSpawn(); //respawns a new food
            }
                
            return false;
            
        }
};



int main() {

    int snakex = 10; //starts at (10,10)
    int snakey = 10; 

    SnakeGame snake(snakex, snakey);
    int dx = 1, dy = 0; // start moving right

    snake.foodSpawn();
    bool gameOver = false;

    while(!gameOver) {
        snake.drawBoard();
        if (_kbhit()) { //if keyboard input
            char key = _getch(); //key is set to get character
            switch (key) {
                case 'w': dy = -1; dx = 0; break;  // up
                case 's': dy = 1; dx = 0; break;   // down
                case 'a': dx = -1; dy = 0; break;  // left
                case 'd': dx = 1; dy = 0; break;   // right
            }
        }

        snake.move(dx, dy);
        if (snake.checkCollisions(20, 20)) {
            gameOver = true;
            break;
        }

        Sleep(150);  // delay for smooth movement, sets the speed of the game
        system("cls");  // clear screen
    }
    

    return 0;
}