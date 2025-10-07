#include <iostream>
#include <cstdlib>  // for function rand() and srand();
#include <ctime>    // for time(nullptr) used to seed random numbers
#include <cmath>
#include <unistd.h>	// for usleep() to control the game speed
#include <ncurses.h>	//Essential for terminal graphics and input

const float JUMP_VELOCITY =	-2.2f;	// Negative velocity is up
const float GRAVITY = 0.15f;
const int PIPE_GAP_HEIGHT = 8;
const int FRAME_DELAY_US = 50000;	// 50 milliseconds (20FPS)
const int BIRD_X_POS = 8;

using namespace std;

class FlappyBirdGame {
    private:
        // -- Bird State --
        float y_pos;
        float y_vel;
        int score;

        // -- Terminal State --
        int max_y;      // max of y-axis of terminal
        int max_x;      // max of x-axis of terminal -this would be the right most x
        bool is_running;

        // -- Pipe State --
        int pipe_x;
        int pipe_hole_y;

        void setup_ncurses() {
            initscr();
            cbreak();
            noecho();
            curs_set(0);
            timeout(0);  // Non-blocking input
            keypad(stdscr, TRUE);
        }

        void tear_down_ncurses() {
            endwin();
        }

        void initialize_game() {
            getmaxyx(stdscr, max_y, max_x);     // get terminal dimensions
            y_pos = max_y / 2.0f;
            y_vel = 0.0f;
            score = 0;

            // Initial pipe position (off-screen to the right)
            pipe_x = max_x + 5;

            // Initial random pipe hole position
            pipe_hole_y = max_y / 2;

            is_running = true;
        }

        void handle_input() {
            int ch = getch();

            if (ch == ' ') {
                y_vel = JUMP_VELOCITY;
            }
            else if (ch == 'q' || ch == 'Q') {
                is_running = false;
            }
        }

        void update_game() {
            // -- 1. BIRD PHYSICS --
            y_vel += GRAVITY;
            y_pos += y_vel;

            // Ensure y_pos is not negative before casting for bounds check
            int bird_y = std::round(y_pos); 

            // -- 2. BIRD BOUNDS AND COLLISION (Floor/Ceiling) --
            if (bird_y >= max_y - 1) {  // Hit the floor
                y_pos = max_y - 1;
                is_running = false;
            }
            else if (bird_y <= 0) {
                y_pos = 0;
                y_vel = 0;
                is_running = false;
            }

            // -- 3. PIPE MOVEMENT AND REGENERATION --
            pipe_x -= 1;

            if (pipe_x < -2) {  // pipe went off the screen
                pipe_x = max_x - 1;

                // Generate a new random hole height
                int min_hole = PIPE_GAP_HEIGHT;
                int max_hole = max_y - PIPE_GAP_HEIGHT;
                pipe_hole_y = (rand() % (max_hole - min_hole + 1)) + min_hole;

                score++;
            }

            // -- 4. COLLISION CHECK (pipe) --
            if (pipe_x == BIRD_X_POS) {
                int top_pipe_end = pipe_hole_y - (PIPE_GAP_HEIGHT / 2);
                int bottom_pipe_start = pipe_hole_y + (PIPE_GAP_HEIGHT / 2);

                if (bird_y <= top_pipe_end || bird_y >= bottom_pipe_start) {
                    is_running = false;
                }
            }
        }

        void draw_game() {
            clear();

            // 1. Draw the Bird (use round() for stable drawing position)
            mvprintw(round(y_pos), BIRD_X_POS, ">");

            // 2. Draw the pipe
            for (int y = 0; y < max_y; y++) {
                int top_pipe_end = pipe_hole_y - (PIPE_GAP_HEIGHT / 2);
                int bottom_pipe_start = pipe_hole_y + (PIPE_GAP_HEIGHT / 2);

                if (y <= top_pipe_end || y >= bottom_pipe_start) {
                    if (pipe_x >= 0 && pipe_x < max_x) {
                        mvprintw(y, pipe_x, "#");
                    }
                }
            }

            // 3. Draw the score
            mvprintw(0, 0, "Score: %d | Press 'SPACE' to jump, 'q' to quit.", score);

            refresh();
        }

    public:
        FlappyBirdGame() {
            // C++ style random seed setup
            srand(static_cast<unsigned int>(time(nullptr)));
        }

        void run() {
            setup_ncurses();
            initialize_game();

            // MAIN GAME LOOP
            while (is_running) {
                handle_input();
                update_game();
                draw_game();

                usleep(FRAME_DELAY_US);
            }

            tear_down_ncurses();

            cout << "Game Over! Final Score: " << score << "\n";
        }
};

int main() {
    FlappyBirdGame game;
    game.run();
    return 0;
}