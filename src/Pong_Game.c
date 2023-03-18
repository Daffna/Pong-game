#include <stdio.h>

#define COLS 80
#define ROWS 25

#define LEFT -1
#define RIGHT +1
#define UP -1
#define DOWN +1

#define BORDER '='
#define MIDLINE '|'
#define BALL 'O'
#define RACKET '#'
#define SPACE ' '

#define WHITE "\033[0;37m"
#define LIGHT_BLUE "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define LIGHT_GRAY "\033[2;30m"
#define PURPLE "\033[1;35m"

int GetInput(int v_x);
int P1_Move(int action, int p1);
int P2_Move(int action, int p2);
int Ball_move_x(int b_x, int b_y, int v_x, int p1, int p2);
int Ball_move_y(int b_y, int v_y);
int CheckGoal(int b_x, int b_y, int p1, int p2);
void Render(int p1, int p2, int b_x, int b_y, int v_x);
void Begin();
void Finish(int p1_score, int p2_score);

int main() {
    // задание исходного положение кареток и шарика
    int p1 = ROWS / 2;
    int p2 = ROWS / 2;
    int b_x = COLS / 2;
    int b_y = ROWS / 2;
    // исходное направление шарика
    int v_x = LEFT;
    int v_y = DOWN;
    // начальные параметры
    int p1_score = 0;
    int p2_score = 0;

    // вывод правил (начальный экран)
    Begin();

    // отрисовка поля с исходными позициями
    Render(p1, p2, b_x, b_y, v_x);

    // основной цикл игры
    while (p1_score < 21 && p2_score < 21) {
        // ввод действия
        int action = GetInput(v_x);

        // ДВИЖЕНИЕ КАРЕТОК
        p1 += P1_Move(action, p1);
        p2 += P2_Move(action, p2);

        // расчет положения мяча
        v_y = Ball_move_y(b_y, v_y);
        v_x = Ball_move_x(b_x, b_y, v_x, p1, p2);

        // движения мяча
        b_x += v_x;
        b_y += v_y;

        // проверка гола
        int goal = CheckGoal(b_x, b_y, p1, p2);
        // ГОООООЛ !!!
        if (goal) {
            b_x = COLS / 2;
            b_y = ROWS / 2;
            p1 = p2 = ROWS / 2;
            // левому забили
            if (goal == 2) {
                p2_score += 1;
                v_x = LEFT;
                v_y = UP;
            }
            // правому забили
            else {
                p1_score += 1;
                v_x = RIGHT;
                v_y = UP;
            }
        }

        // отрисовка
        Render(p1, p2, b_x, b_y, v_x);
        // вывод текущего счета
        printf("%sp1_score: %i\t\t\t\t p2_score: %i%s\n", LIGHT_BLUE, p1_score, p2_score, WHITE);
    }

    Finish(p1_score, p2_score);

    return 0;
}

void Begin() {
    printf("\n\n\n\n");

    printf("%s\t\t\tWelcome to Pong Game!\n\n\n\n", PURPLE);

    printf("\t\t%s    Read the rules before playing%s\n\n\n", LIGHT_BLUE, WHITE);

    printf("%sTo move the rackets use A/Z for Player 1 and K/M for Player 2. ", PURPLE);

    printf("To skip an action\n\nuse Space Bar. ");

    printf("The game ends when one of the players scores 21 points. Good luck!\n");

    printf("%s\n\n\t\t\t       ٩(｡•́‿•̀｡)۶%s\n", LIGHT_BLUE, WHITE);

    printf("%s\n\n\n\t\t\tpress Enter for start\n\n\n\n%s", PURPLE, WHITE);
    getchar();
}

void Finish(int p1_score, int p2_score) {
    printf("\n\n\n\n");

    printf("\n\n\n\t\t\t%sOhoooo, that's the end of the game!\n\n\n", PURPLE);

    printf("\t\tСongratulations on winning player number ");

    if (p1_score == 21) {
        printf("1 (left gamer)!\n\n\n\t\t\t\t\t%s(ﾉ◕ヮ◕)ﾉ*:･ﾟ✧%s", LIGHT_BLUE, WHITE);
    } else if (p2_score == 21) {
        printf("2 (right gamer)!\n\n\n\t\t\t\t\t%s(ﾉ◕ヮ◕)ﾉ*:･ﾟ✧%s", LIGHT_BLUE, WHITE);
    }

    printf("\n\n\n\n\n\n");
}

int CheckGoal(int b_x, int b_y, int p1, int p2) {
    int goal = 0;

    if (b_x == 1) {
        if (b_y > p1 + 1 || b_y < p1 - 1) {
            goal = 2;
        }
    } else if (b_x == 80) {
        if (b_y > p2 + 1 || b_y < p2 - 1) {
            goal = 1;
        }
    }

    return goal;
}

int P1_Move(int action, int p1) {
#define P1_UP 11
#define P1_DOWN 12
    int move = 0;

    if (action == P1_UP && (p1 - 1) - 1 > 0) {
        move = UP;
    } else if (action == P1_DOWN && (p1 + 1) + 1 < ROWS + 1) {
        move = DOWN;
    }

    return move;
}

int P2_Move(int action, int p2) {
#define P2_UP 21
#define P2_DOWN 22
    int move = 0;

    if (action == P2_UP && (p2 - 1) - 1 > 0) {
        move = UP;
    } else if (action == P2_DOWN && (p2 + 1) + 1 < ROWS + 1) {
        move = DOWN;
    }

    return move;
}

int GetInput(int v_x) {
#define P1_UP 11
#define P1_DOWN 12
#define P2_UP 21
#define P2_DOWN 22
#define SKIP 0
#define NONE -1
    int action = NONE;

    while (action == NONE) {
        char key = getchar();

        // ПРОПУСК
        if (key == ' ') {
            action = SKIP;
            // while(getchar() != '\n');
        }

        // кнопки ПЕРВОГО пользователя
        if (v_x == LEFT) {
            if (key == 'a' || key == 'A') action = P1_UP;
            if (key == 'z' || key == 'Z') action = P1_DOWN;
        }

        // кнопки ВТОРОГО пользователя
        if (v_x == RIGHT) {
            if (key == 'k' || key == 'K') action = P2_UP;
            if (key == 'm' || key == 'M') action = P2_DOWN;
        }
    }

    while (getchar() != '\n')
        ;

    return action;
}

int Ball_move_x(int b_x, int b_y, int v_x, int p1, int p2) {
    int next_position_x = b_x + v_x;

    // столкновение с левой кареткой
    if (next_position_x == 1) {
        if (b_y >= p1 - 1 && b_y <= p1 + 1) return RIGHT;
    }
    // столкновение с правой кареткой
    else if (next_position_x == 80) {
        if (b_y >= p2 - 1 && b_y <= p2 + 1) return LEFT;
    }

    return v_x;
}

int Ball_move_y(int b_y, int v_y) {
#define UPPER_BORDER 0
#define BOTTOM_BORDER 26

    int next_position_y = b_y + v_y;

    // столкновение с потолком
    if (next_position_y == UPPER_BORDER) return DOWN;

    // столкновение с полом
    else if (next_position_y == BOTTOM_BORDER)
        return UP;

    return v_y;
}

void Render(int p1, int p2, int b_x, int b_y, int v_x) {
    for (int y = 0; y < ROWS + 2; y++) {
        for (int x = 0; x < COLS + 2; x++)  // cтрока
        {
            if (y == 0 || y == ROWS + 1)  // границы - верх и низ
            {
                putchar(BORDER);
            } else if (x == 1)  // линии движения кареток (игроков)
            {
                if (y >= p1 - 1 && y <= p1 + 1)  // левая каретка
                {
                    if (v_x < 0)
                        printf("%s%c%s", GREEN, RACKET, WHITE);
                    else
                        putchar(RACKET);
                } else if (x == b_x && y == b_y)
                    putchar(BALL);
                else
                    putchar(SPACE);
            } else if (x == COLS) {
                if (y >= p2 - 1 && y <= p2 + 1)  // правая каретка
                {
                    if (v_x > 0)
                        printf("%s%c%s", YELLOW, RACKET, WHITE);
                    else
                        putchar(RACKET);
                } else if (x == b_x && y == b_y)
                    putchar(BALL);
                else
                    putchar(SPACE);
            }  // остальная часть игрового поля (где в основном летал шар)
            else {
                if (x == b_x && y == b_y) {
                    if (v_x < 0)
                        printf("%s%c%s", GREEN, BALL, WHITE);
                    else
                        printf("%s%c%s", YELLOW, BALL, WHITE);
                } else if (x == COLS / 2) {
                    printf("%s%c%s", LIGHT_GRAY, MIDLINE, WHITE);
                } else
                    putchar(SPACE);
            }
        }
        putchar('\n');
    }
}
