#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <locale.h>
#include <memory.h>
#include <stdlib.h>

const int width = 100;
const int height = 20;
const int paddleWidth = 14;

WINDOW *wnd;

bool running = true;

int posX;
int posY;
int dX = 1;
int dY = 1;
int lastPosX;
int lastPosY;
int tickNo = 0;
double speed = 10.0;
int points = 0;

int paddlePos;
int lastPaddlePos;

void print(char buffer[], int x, int y) {
    mvwprintw(wnd, y, x, buffer);
}

void drawFrame() {
    color_set(1, 0);
    print(" ", 0, 0);
    for (int x = 1; x <= width; x++) {
        print(" ", x, 0);
    }
    print(" ", width + 1, 0);

    for (int y = 1; y <= height; y++) {
        print(" ", 0, y);
        print(" ", width + 1, y);
    }

    print(" ", 0, height + 1);
    for (int x = 1; x <= width; x++) {
        print(" ", x, height + 1);
    }
    print(" ", width + 1, height + 1);

    color_set(0, 0);
    print("Points ", 1, height + 2);
    print("Highscore ", 16, height + 2);

    print("Speed ", 61, height + 2);
    print("X ", 72, height + 2);
    print("Y ", 80, height + 2);
    print("dX ", 88, height + 2);
    print("dY ", 96, height + 2);

    refresh();
}

void gameOver() {
    char text[] = " ________  ________  _____ ______   _______           ________  ___      ___ _______   ________     |\\   ____\\|\\   __  \\|\\   _ \\  _   \\|\\  ___ \\         |\\   __  \\|\\  \\    /  /|\\  ___ \\ |\\   __  \\    \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\\\\\__\\ \\  \\ \\   __/|        \\ \\  \\|\\  \\ \\  \\  /  / | \\   __/|\\ \\  \\|\\  \\    \\ \\  \\  __\\ \\   __  \\ \\  \\\\|__| \\  \\ \\  \\_|/__       \\ \\  \\\\\\  \\ \\  \\/  / / \\ \\  \\_|/_\\ \\   _  _\\    \\ \\  \\|\\  \\ \\  \\ \\  \\ \\  \\    \\ \\  \\ \\  \\_|\\ \\       \\ \\  \\\\\\  \\ \\    / /   \\ \\  \\_|\\ \\ \\  \\\\  \\|    \\ \\_______\\ \\__\\ \\__\\ \\__\\    \\ \\__\\ \\_______\\       \\ \\_______\\ \\__/ /     \\ \\_______\\ \\__\\\\ _\\     \\|_______|\\|__|\\|__|\\|__|     \\|__|\\|_______|        \\|_______|\\|__|/       \\|_______|\\|__|\\|__|";

    color_set(0, 0);
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 100; x++) {
            print((char[]) {text[y * 100 + x], '\0'}, x + 1, y + 6);
        }
    }

    char *number = {
            "  ___    / _ \\  | | | | | |_| |  \\___/          " // 0 (7)

                    "   _      / |     | |     | |     |_|           " // 1 (3)

                    "  ____   |___ \\    __) |  / __/  |_____|        " // 2 (7)

                    "  _____  |___ /    |_ \\   ___) | |____/         " // 3 (7)

                    " _  _   | || |  | || |_ |__   _|   |_|          " // 4 (8)

                    "  ____   | ___|  |___ \\   ___) | |____/         " // 5 (7)

                    "   __     / /_   | '_ \\  | (_) |  \\___/         " // 6 (7)

                    "  _____  |___  |    / /    / /    /_/           " // 7 (7)

                    "   ___    ( _ )   / _ \\  | (_) |  \\___/         " // 8 (7)

                    "   ___    / _ \\  | (_) |  \\__, |    /_/         " // 9 (7)
    };

    int digits[4] = {};
    int remainingPoints = points;
    for (int i = 3; i >= 0; i--) {
        digits[i] = remainingPoints % 10;
        remainingPoints /= 10;
    }

    for (int i = 0; i < 4; i++) {
        for (int y = 0; y < 6; y++) {
            for (int x = 0; x < 8; x++) {
                print((char[]) {number[digits[i] * 8 * 6 + y * 8 + x], '\0'}, x + 20 + i * 8, y + 14);
            }
        }
    }

    char pointsText[] = "   ___      _       _         / _ \\___ (_)_ __ | |_ ___  / /_)/ _ \\| | '_ \\| __/ __|/ ___/ (_) | | | | | |_\\__ \\\\/    \\___/|_|_| |_|\\__|___/                            ";
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 28; x++) {
            print((char[]) {pointsText[y * 28 + x], '\0'}, x + 54, y + 14);
        }
    }

}

void draw() {
    color_set(0, 0);
    for (int x = lastPaddlePos; x < lastPaddlePos + paddleWidth; x++) {
        print(" ", x + 1, height);
    }

    color_set(2, 0);
    for (int x = paddlePos; x < paddlePos + paddleWidth; x++) {
        print(" ", x + 1, height);
    }

    color_set(3, 0);
    print(" ", lastPosX + 1, lastPosY + 1);
    print("o", posX + 1, posY + 1);


    // Text:

    char str[12];
    sprintf(str, "%05d", points);
    print(str, 8, height + 2);

    sprintf(str, "%02d", 10 - (int) speed);
    print(str, 67, height + 2);

    sprintf(str, "%03d", posX);
    print(str, 74, height + 2);

    sprintf(str, "%03d", posY);
    print(str, 82, height + 2);

    sprintf(str, "%+d", dX);
    print(str, 91, height + 2);

    sprintf(str, "%+d", dY);
    print(str, 99, height + 2);

    move(height + 3, 0);

    refresh();

    lastPaddlePos = paddlePos;
    lastPosX = posX;
    lastPosY = posY;
}

void tick() {
    /*points = 103;
    running = false;
    gameOver();*/
    if (running) {
        if (paddlePos < 0)
            paddlePos = 0;
        if (paddlePos + paddleWidth > width)
            paddlePos = width - paddleWidth;

        if (tickNo % (int) speed == 0) {
            posX += dX;
            posY += dY;

            if (posY < 0) {
                posY = -posY;
                dY = -dY;
            }
            if (posY > height - 2) {
                if (posX >= paddlePos && posX < paddlePos + paddleWidth) {
                    posY = height - (posY - (height - 4));
                    dY = -dY;
                    posX -= dX;
                    dX = rand() / (RAND_MAX / 4) - 2;
                    if (dX >= 0)
                        dX += 1;
                    posX += dX;
                    points++;
                } else {
                    running = false;
                    gameOver();
                }
            }

            if (posX < 0) {
                posX = -posX;
                dX = -dX;
            }
            if (posX > width - 1) {
                posX = width - (posX - (width - 2));
                dX = -dX;
            }
        }
    }

    draw();
    tickNo++;
    if (tickNo > 300) {
        tickNo = 0;
    }
    speed *= 0.9997;
}

void showSplashScreen() {
    char text[] =
            " ________  ________  ________   ________     "
                    "|\\   __  \\|\\   __  \\|\\   ___  \\|\\   ____\\    "
                    "\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\\\ \\  \\ \\  \\___|    "
                    " \\ \\   ____\\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\  ___  "
                    "  \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\\\ \\  \\ \\  \\|\\  \\ "
                    "   \\ \\__\\    \\ \\_______\\ \\__\\\\ \\__\\ \\_______\\"
                    "    \\|__|     \\|_______|\\|__| \\|__|\\|_______|";

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 45; x++) {
            print((char[]) {text[y * 45 + x], '\0'}, x + 25, y + 6);
        }
    }

    move(height + 3, 0);
    refresh();

    sleep(1);

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 45; x++) {
            print(" ", x + 25, y + 6);
        }
    }
    refresh();
}

int main() {
    posX = width / 2;
    posY = height / 5;
    paddlePos = width / 2 - paddleWidth / 2;

    srand(time(NULL));
    setlocale(LC_ALL, "");
    wnd = initscr();
    timeout(0);
    start_color();
    cbreak();
    noecho();
    keypad(wnd, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_GREEN, 0);

    drawFrame();

    showSplashScreen();

    int d;
    while (1) {
        d = wgetch(wnd);
        switch (d) {
            case KEY_LEFT:
                if (running)
                    paddlePos -= 2;
                break;
            case KEY_RIGHT:
                if (running)
                    paddlePos += 2;
                break;
            case 'q':
                endwin();
                return 0;
        }
        tick();
        nanosleep((const struct timespec[]) {{0, (long) 16e6}}, NULL);
    }
}
