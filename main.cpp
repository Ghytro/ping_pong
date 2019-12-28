
#include "TXLib.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>

#define WIND_X_SIZE 800
#define WIND_Y_SIZE 800
#define MOUSE_LEFT_CLICK 1
#define MOUSE_RIGHT_CLICK 2
#define DEFAULT_START_SCORE 10
#define EASY_DIFF 0
#define NORMAL_DIFF 1
#define HARD_DIFF 2
#define EXPERT_DIFF 3
#define PI acos(-1)

enum game_mode
{
    MAIN_MENU,
    SETTINGS_MENU,
    BEFORE_START,
    PLAYING,
    PAUSE,
};

enum main_menu_button
{
    PLAY,
    SETTINGS,
    QUIT,
};

enum before_start_button
{
    DECREASE_SCORE,
    INCREASE_SCORE,
    DECREASE_DIFF,
    INCREASE_DIFF,
    START_GAME,
    BACK,
};

inline void number_to_numerals(int number, std::vector<int> &numerals)
{
    std::vector<int> ans;
    while (number >= 10)
    {
        ans.push_back(number % 10);
        number /= 10;
    }
    ans.push_back(number);
    std::reverse(ans.begin(), ans.end());
    numerals = ans;
}

inline void draw_frame(RECT frame, COLORREF color, double thickness)
{
    txSetColor(color, thickness);

    POINT
        lb = {frame.left,     frame.bottom},
        lt = {frame.left,        frame.top},
        rt = {frame.right,       frame.top},
        rb = {frame.right,    frame.bottom};

    txLine(lb.x,    lb.y,   lt.x,   lt.y);
    txLine(lt.x,    lt.y,   rt.x,   rt.y);
    txLine(rt.x,    rt.y,   rb.x,   rb.y);
    txLine(rb.x,    rb.y,   lb.x,   lb.y);
}

inline void draw_main_menu(HDC title, HDC start_button, HDC settings_button, HDC quit_button)
{
    int
        title_margin_left = (WIND_X_SIZE - 446) / 2,
        title_margin_top  = 100;
    txBitBlt(txDC(), title_margin_left, title_margin_top, 446, 102, title);

    int
        start_button_margin_left = (WIND_X_SIZE - 256) / 2,
        start_button_margin_top  = title_margin_top + 200;
    txBitBlt(txDC(), start_button_margin_left, start_button_margin_top, 256, 99, start_button);

    int
        settings_button_margin_left = (WIND_X_SIZE - 256) / 2,
        settings_button_margin_top  = start_button_margin_top + 120;
    txBitBlt(txDC(), settings_button_margin_left, settings_button_margin_top, 256, 99, settings_button);

    int
        quit_button_margin_left = (WIND_X_SIZE - 256) / 2,
        quit_button_margin_top  = settings_button_margin_top + 120;
    txBitBlt(txDC(), quit_button_margin_left, quit_button_margin_top, 256, 99, quit_button);

}

inline void draw_before_game(HDC score_to_victory, HDC dec_stv, HDC inc_stv, HDC *numerals, HDC difficulty, HDC *difficulties, HDC start_game, HDC back_button, int stv, int current_difficulty, std::vector<RECT> &buttons)
{
    int
        stv_margin_left = (WIND_X_SIZE - 523) / 2,
        stv_margin_top  = 50;
    txBitBlt(txDC(), stv_margin_left, stv_margin_top, 523, 59, score_to_victory);

    std::vector<int> score_numerals;
    number_to_numerals(stv, score_numerals);
    int number_width = 65 * (score_numerals.size() + 1);
    int score_left_margin = (WIND_X_SIZE - number_width - 65) / 2;
    int number_margin_stv = 80;

    //refreshing button sizes here because of the temporary size of the number
    std::vector<RECT> temp;
    txBitBlt(txDC(), score_left_margin, stv_margin_top + number_margin_stv + 15, 50, 50, dec_stv);
    temp.push_back({score_left_margin, stv_margin_top + number_margin_stv + 15, score_left_margin + 50, stv_margin_top + number_margin_stv + 50});

    for (size_t i = 0; i < score_numerals.size(); ++i)
        txBitBlt(txDC(), score_left_margin + (i + 1)*65, stv_margin_top + number_margin_stv, 65, 80, numerals[score_numerals[i]]);

    txBitBlt(txDC(), score_left_margin + number_width + 10, stv_margin_top + number_margin_stv + 15, 50, 50, inc_stv);
    temp.push_back({score_left_margin + number_width + 10, stv_margin_top + number_margin_stv + 15, score_left_margin + number_width + 50, stv_margin_top + number_margin_stv + 50});

    int
        diff_margin_left = (WIND_X_SIZE - 347) / 2,
        diff_margin_top  = stv_margin_top + number_margin_stv + 125;
    txBitBlt(txDC(), diff_margin_left, diff_margin_top, 347, 65, difficulty);
    int diff_select_width;

    switch(current_difficulty)
    {

    case 0:
        diff_select_width = 136 + 2*50;
        break;

    case 1:
        diff_select_width = 202 + 2*50;
        break;

    case 2:
        diff_select_width = 138 + 2*50;
        break;

    case 3:
        diff_select_width = 202 + 2*50;
        break;

    default:
        break;

    }


    int diff_select_margin_left = (WIND_X_SIZE - diff_select_width - 50) / 2;
    txBitBlt(txDC(), diff_select_margin_left, diff_margin_top + 100, 50, 50, dec_stv);
    temp.push_back({diff_select_margin_left, diff_margin_top + 100, diff_select_margin_left + 50, diff_margin_top + 150});

    txBitBlt(txDC(), diff_select_margin_left + 80, diff_margin_top + 100, diff_select_width - 2*50, 65, difficulties[current_difficulty]);

    txBitBlt(txDC(), diff_select_margin_left + diff_select_width, diff_margin_top + 100, 50, 50, inc_stv);
    temp.push_back({diff_select_margin_left + diff_select_width, diff_margin_top + 100, diff_select_margin_left + diff_select_width + 50, diff_margin_top + 150});

    int
        start_but_margin_left = (WIND_X_SIZE - 261) / 2,
        start_but_margin_top = diff_margin_top + 250;

    txBitBlt(txDC(), start_but_margin_left, start_but_margin_top, 261, 81, start_game);
    temp.push_back({start_but_margin_left, start_but_margin_top, start_but_margin_left + 261, start_but_margin_top + 81});

    int
        back_button_margin_left = (WIND_X_SIZE - 218) / 2,
        back_button_margin_top  = start_but_margin_top + 150;

    txBitBlt(txDC(), back_button_margin_left, back_button_margin_top, 218, 80, back_button);
    temp.push_back({back_button_margin_left, back_button_margin_top, back_button_margin_left + 218, back_button_margin_top + 80});

    buttons = temp;
}

inline void draw_countdown(HDC *numerals)
{
    txBitBlt(txDC(), (WIND_X_SIZE - 65) / 2, (WIND_Y_SIZE - 80) / 2, 65, 80, numerals[3]);
    txSleep(1000);
    txClear();

    txBitBlt(txDC(), (WIND_X_SIZE - 65) / 2, (WIND_Y_SIZE - 80) / 2, 65, 80, numerals[2]);
    txSleep(1000);
    txClear();

    txBitBlt(txDC(), (WIND_X_SIZE - 65) / 2, (WIND_Y_SIZE - 80) / 2, 65, 80, numerals[1]);
    txSleep(1000);
    txClear();
}

inline void play_game(int difficulty, int stv, game_mode &current_mode)
{
    std::pair<int, int> score(0, 0);
    const int racket_w = 10;
    int
        game_wind_x_sz,
        game_wind_y_sz,
        racket_l,
        ball_speed_x,
        ball_speed_y,
        ball_size,
        ball_y_pos;

    switch (difficulty)
    {
    case 0:
        game_wind_x_sz    = 400;
        game_wind_y_sz    = 300;
        racket_l          = 65;
        ball_speed_x      = 10;
        ball_speed_y      = std::rand() % 12 - 6;
        ball_size         = 20;
        ball_y_pos        = std::rand() % 300 + 150;
        break;

    case 1:
        game_wind_x_sz    = 600;
        game_wind_y_sz    = 400;
        racket_l          = 65;
        ball_speed_x      = 15;
        ball_speed_y      = std::rand() % 18 - 9;
        ball_size         = 15;
        ball_y_pos        = std::rand() % 400;
        break;


    case 2:
        game_wind_x_sz    = 800;
        game_wind_y_sz    = 600;
        racket_l          = 50;
        ball_speed_x      = 20;
        ball_speed_y      = std::rand() % 24 - 12;
        ball_size         = 10;
        ball_y_pos        = std::rand() % 600;
        break;

    case 3:
        game_wind_x_sz    = 800;
        game_wind_y_sz    = 600;
        racket_l          = 25;
        ball_speed_x      = 50;
        ball_speed_y      = std::rand() % 60 - 30;
        ball_size         = 5;
        ball_y_pos        = std::rand() % 600;
        break;

    default:
        break;

    }

    int ball_x_pos              = (game_wind_x_sz - ball_size) / 2;
    int game_window_margin_x    = (WIND_X_SIZE - game_wind_x_sz) / 2;
    int game_window_margin_top  = 150;
    int frame_thickness         = 2;
    int racket_margin_x         = 20;
    ball_x_pos += game_window_margin_x;

    RECT screen_frame = {game_window_margin_x, game_window_margin_top, game_window_margin_x + game_wind_x_sz, game_window_margin_top + game_wind_y_sz};
    txSetColor(TX_WHITE);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        draw_frame(screen_frame, TX_WHITE, frame_thickness);
        POINT cursor_pos = txMousePos();

        if (cursor_pos.y - racket_l < screen_frame.top)
            cursor_pos.y = screen_frame.top + racket_l;

        if (cursor_pos.y + racket_l > screen_frame.bottom)
            cursor_pos.y = screen_frame.bottom - racket_l;

        txRectangle(ball_x_pos - ball_size, ball_y_pos - ball_size, ball_x_pos + ball_size, ball_y_pos + ball_size); ///ball
        txRectangle(game_window_margin_x + game_wind_x_sz - racket_margin_x - racket_w, cursor_pos.y - racket_l, game_window_margin_x + game_wind_x_sz - racket_margin_x + racket_w, cursor_pos.y + racket_l); ///player's rocket

        if (ball_x_pos + ball_size >= screen_frame.right || ball_x_pos - ball_size <= screen_frame.left)
            ball_speed_x *= -1;

        if (ball_y_pos + ball_size >= screen_frame.bottom || ball_y_pos - ball_size <= screen_frame.top)
            ball_speed_y *= -1;

        ball_x_pos += ball_speed_x;
        ball_y_pos += ball_speed_y;
        txSleep(20);
        txClear();
    }
    current_mode = MAIN_MENU;
}

inline void fill_buttons_coords(std::vector<RECT> &buttons, game_mode current_mode, std::vector<int> left_button_coord = {}, std::vector<int> right_button_coord = {})
{
    std::vector<RECT> temp;
    if (current_mode == MAIN_MENU)
    {
        int
            title_margin_left           = (WIND_X_SIZE - 446) / 2,
            title_margin_top            = 100,
            start_button_margin_left    = (WIND_X_SIZE - 256) / 2,
            start_button_margin_top     = title_margin_top + 200,
            settings_button_margin_left = (WIND_X_SIZE - 256) / 2,
            settings_button_margin_top  = start_button_margin_top + 120,
            quit_button_margin_left     = (WIND_X_SIZE - 256) / 2,
            quit_button_margin_top      = settings_button_margin_top + 120;

        temp.push_back({start_button_margin_left, start_button_margin_top, start_button_margin_left + 256, start_button_margin_top + 99});
        temp.push_back({settings_button_margin_left, settings_button_margin_top, settings_button_margin_left + 256, settings_button_margin_top + 99});
        temp.push_back({quit_button_margin_left, quit_button_margin_top, quit_button_margin_left + 256, quit_button_margin_top + 99});
    }
    buttons = temp;
}

inline void free_HDC_mem(HDC pic, ...)
{
    HDC *it = &pic;
    while (*it)
    {
        txDeleteDC(*it);
        ++it;
    }
}

int main()
{

    std::srand(std::time(NULL));
    txCreateWindow(WIND_X_SIZE, WIND_Y_SIZE);
    txSetFillColor(TX_BLACK);

    HDC title = txLoadImage("Resources\\logo.bmp");
    if (!title)
    {
        txMessageBox("Failed to load logo.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC start_button = txLoadImage("Resources\\play_button.bmp");
    if (!start_button)
    {
        txMessageBox("Failed to load play_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC settings_button = txLoadImage("Resources\\settings_button.bmp");
    if (!settings_button)
    {
        txMessageBox("Failed to load settings_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC quit_button = txLoadImage("Resources\\quit_button.bmp");
    if (!quit_button)
    {
        txMessageBox("Failed to load quit_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC left_arrow = txLoadImage("Resources\\left_arrow.bmp");
    if (!left_arrow)
    {
        txMessageBox("Failed to load left_arrow.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC right_arrow = txLoadImage("Resources\\right_arrow.bmp");
    if (!left_arrow)
    {
        txMessageBox("Failed to load right_arrow.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC score_to_victory = txLoadImage("Resources\\max_score.bmp");
    if (!score_to_victory)
    {
        txMessageBox("Failed to load max_score.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC goodbye = txLoadImage("Resources\\gbye.bmp");
    if (!goodbye)
    {
        txMessageBox("Failed to load goodbye.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC inc_stv = txLoadImage("Resources\\inc_stv.bmp");
    if (!inc_stv)
    {
        txMessageBox("Failed to load inc_stv.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC dec_stv = txLoadImage("Resources\\dec_stv.bmp");
    if (!dec_stv)
    {
        txMessageBox("Failed to load dec_stv.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC difficulty = txLoadImage("Resources\\difficulty.bmp");
    if (!difficulty)
    {
        txMessageBox("Failed to load difficulty.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC start_game = txLoadImage("Resources\\start_game.bmp");
    if (!start_game)
    {
        txMessageBox("Failed to load start_game.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC back_button = txLoadImage("Resources\\back_button.bmp");
    if (!back_button)
    {
        txMessageBox("Failed to load back_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }


    HDC numerals[] =
    {
        txLoadImage("Resources\\0.bmp"),
        txLoadImage("Resources\\1.bmp"),
        txLoadImage("Resources\\2.bmp"),
        txLoadImage("Resources\\3.bmp"),
        txLoadImage("Resources\\4.bmp"),
        txLoadImage("Resources\\5.bmp"),
        txLoadImage("Resources\\6.bmp"),
        txLoadImage("Resources\\7.bmp"),
        txLoadImage("Resources\\8.bmp"),
        txLoadImage("Resources\\9.bmp"),
    };

    HDC difficulties[] =
    {
        txLoadImage("Resources\\easy_dif.bmp"),
        txLoadImage("Resources\\normal_dif.bmp"),
        txLoadImage("Resources\\hard_dif.bmp"),
        txLoadImage("Resources\\expert_dif.bmp"),
    };

    for (int i = 0; i < 10; ++i)
    {
        if (!numerals[i])
        {
            std::string error_cause = "Failed to load " + std::to_string(i) + ".bmp";
            txMessageBox(error_cause.c_str(), "Fatal error", MB_ICONERROR);
            return 1;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (!difficulties[i])
        {
            std::string error_cause = "Failed to load ";
            switch(i)
            {
            case 0:
                error_cause += "easy_dif.bmp";
                break;

            case 1:
                error_cause += "normal_dif.bmp";
                break;

            case 2:
                error_cause += "hard_dif.bmp";
                break;

            case 3:
                error_cause += "expert_dif.bmp";
                break;

            default:
                break;

            }
            txMessageBox(error_cause.c_str(), "Fatal error", MB_ICONERROR);
            return 1;
        }
    }


    game_mode current_mode = MAIN_MENU;
    std::vector<RECT> screen_buttons;
    int victory_score = DEFAULT_START_SCORE;
    int current_difficulty = NORMAL_DIFF;
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        int mouse_button = txMouseButtons();

        if (current_mode == PLAYING)
        {
            draw_countdown(numerals);
            play_game(current_difficulty, victory_score, current_mode);
        }

        if (current_mode == MAIN_MENU)
        {
            //need to nide all other buttons
            fill_buttons_coords(screen_buttons, current_mode);
            draw_main_menu(title, start_button, settings_button, quit_button);
            if (txMouseButtons() == MOUSE_LEFT_CLICK)
            {
                POINT cursor_pos = txMousePos();
                if (In(cursor_pos, screen_buttons[PLAY]))
                {
                    current_mode = BEFORE_START;
                }

                if (In(cursor_pos, screen_buttons[SETTINGS]))
                {
                    current_mode = SETTINGS_MENU;
                }

                if (In(cursor_pos, screen_buttons[QUIT]))
                {
                    txSleep(20);
                    break;
                }
            }
        }

        if (current_mode == BEFORE_START)
        {
            draw_before_game(score_to_victory, dec_stv, inc_stv, numerals, difficulty, difficulties, start_game, back_button, victory_score, current_difficulty, screen_buttons);
            if (txMouseButtons() == MOUSE_LEFT_CLICK)
            {
                POINT cursor_pos = txMousePos();
                if (In(cursor_pos, screen_buttons[DECREASE_SCORE]) && victory_score > 1)
                {
                    --victory_score;
                    txSleep(100);
                }

                if (In(cursor_pos, screen_buttons[INCREASE_SCORE]) && victory_score < 1000)
                {
                    ++victory_score;
                    txSleep(100);
                }

                if (In(cursor_pos, screen_buttons[DECREASE_DIFF]) && current_difficulty > 0)
                {
                    --current_difficulty;
                    txSleep(100);
                }

                if (In(cursor_pos, screen_buttons[INCREASE_DIFF]) && current_difficulty < 3)
                {
                    ++current_difficulty;
                    txSleep(100);
                }

                if (In(cursor_pos, screen_buttons[START_GAME]))
                {
                    current_mode = PLAYING;
                }

                if (In(cursor_pos, screen_buttons[BACK]))
                {
                    current_mode = MAIN_MENU;
                }
            }
        }

        txSleep(20);
        txClear();
    }

    ///clearing HDC memory
    free_HDC_mem(title, start_button, settings_button, quit_button, left_arrow, right_arrow, score_to_victory, inc_stv, dec_stv, difficulty, start_game, back_button);

    for (int i = 0; i < 10; ++i)
        free_HDC_mem(numerals[i]);

    for (int i = 0; i < 4; ++i)
        free_HDC_mem(difficulties[i]);

    txClear();

    txBitBlt(txDC(), 0, 0, 800, 800, goodbye);
    free_HDC_mem(goodbye);
    return 0;
}
