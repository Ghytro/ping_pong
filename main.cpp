#include "TXLib.h"
#include <vector>
#include <iostream>
#include <algorithm>

#define WIND_X_SIZE 800
#define WIND_Y_SIZE 800
#define MOUSE_LEFT_CLICK 1
#define MOUSE_RIGHT_CLICK 2
#define DEFAULT_START_SCORE 10

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

inline void draw_before_game(HDC score_to_victory, HDC dec_stv, HDC inc_stv, HDC *numerals, int stv, std::vector<RECT> &buttons)
{
    int
        stv_margin_left = (WIND_X_SIZE - 523) / 2,
        stv_margin_top  = 100;
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
    {
        txBitBlt(txDC(), score_left_margin + (i + 1)*65, stv_margin_top + number_margin_stv, 65, 80, numerals[score_numerals[i]]);
    }
    txBitBlt(txDC(), score_left_margin + number_width + 10, stv_margin_top + number_margin_stv + 15, 50, 50, inc_stv);
    temp.push_back({score_left_margin + number_width + 10, stv_margin_top + number_margin_stv + 15, score_left_margin + number_width + 50, stv_margin_top + number_margin_stv + 50});
    buttons = temp;
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

    txCreateWindow(WIND_X_SIZE, WIND_Y_SIZE);
    txSetFillColor(TX_BLACK);

    HDC title = txLoadImage("Resources\\logo.bmp");
    if (!title)
    {
        MessageBox(GetActiveWindow(), "Failed to load logo.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC start_button = txLoadImage("Resources\\play_button.bmp");
    if (!start_button)
    {
        MessageBox(GetActiveWindow(), "Failed to load play_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC settings_button = txLoadImage("Resources\\settings_button.bmp");
    if (!settings_button)
    {
        MessageBox(GetActiveWindow(), "Failed to load settings_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC quit_button = txLoadImage("Resources\\quit_button.bmp");
    if (!quit_button)
    {
        MessageBox(GetActiveWindow(), "Failed to load quit_button.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC left_arrow = txLoadImage("Resources\\left_arrow.bmp");
    if (!left_arrow)
    {
        MessageBox(GetActiveWindow(), "Failed to load left_arrow.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC right_arrow = txLoadImage("Resources\\right_arrow.bmp");
    if (!left_arrow)
    {
        MessageBox(GetActiveWindow(), "Failed to load right_arrow.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC score_to_victory = txLoadImage("Resources\\max_score.bmp");
    if (!score_to_victory)
    {
        MessageBox(GetActiveWindow(), "Failed to load max_score.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC goodbye = txLoadImage("Resources\\gbye.bmp");
    if (!goodbye)
    {
        MessageBox(GetActiveWindow(), "Failed to load goodbye.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC inc_stv = txLoadImage("Resources\\inc_stv.bmp");
    if (!inc_stv)
    {
        MessageBox(GetActiveWindow(), "Failed to load inc_stv.bmp", "Fatal error", MB_ICONERROR);
        return 1;
    }

    HDC dec_stv = txLoadImage("Resources\\dec_stv.bmp");
    if (!dec_stv)
    {
        MessageBox(GetActiveWindow(), "Failed to load dec_stv.bmp", "Fatal error", MB_ICONERROR);
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

    for (int i = 0; i < 10; ++i)
    {
        if (!numerals[i])
        {
            std::string error_cause = "Failed to load " + std::to_string(i) + ".bmp";
            MessageBox(GetActiveWindow(), error_cause.c_str(), "Fatal error", MB_ICONERROR);
            return 1;
        }
    }


    game_mode current_mode = MAIN_MENU;
    std::vector<RECT> screen_buttons;
    int victory_score = DEFAULT_START_SCORE;
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        int mouse_button = txMouseButtons();
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
                    txSleep(20);
                }

                if (In(cursor_pos, screen_buttons[SETTINGS]))
                {
                    current_mode = SETTINGS_MENU;
                    txSleep(20);
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
            draw_before_game(score_to_victory, dec_stv, inc_stv, numerals, victory_score, screen_buttons);
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


            }
        }
        txSleep(20);
        txClear();
    }

    free_HDC_mem(title, start_button, settings_button, quit_button, left_arrow, right_arrow, score_to_victory, inc_stv, dec_stv);
    for (int i = 0; i < 10; ++i)
        free_HDC_mem(numerals[i]);
    txClear();

    txBitBlt(txDC(), 0, 0, 800, 800, goodbye);
    free_HDC_mem(goodbye);
    return 0;
}
