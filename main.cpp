#include "TXLib.h"
#include <vector>
#include <iostream>

#define WIND_X_SIZE 800
#define WIND_Y_SIZE 800
#define MOUSE_LEFT_CLICK 1
#define MOUSE_RIGHT_CLICK 2

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
    QUIT
};

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

inline void fill_buttons_coords(std::vector<RECT> &buttons, game_mode current_mode)
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

inline void hide_HDC(HDC button)
{
    txBitBlt(txDC(), 0, 0, 0, 0, button);
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

    const HDC numerals[] = {

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
        if (!numerals[i])
        {
            std::string error_cause = "Failed to load ";
            error_cause += std::to_string(i);
            error_cause += ".bmp";
            MessageBox(GetActiveWindow(), error_cause.c_str(), "Fatal error", MB_ICONERROR);
            exit(1);
        }

    draw_main_menu(title, start_button, settings_button, quit_button);

    game_mode current_mode = MAIN_MENU;
    std::vector<RECT> screen_buttons;
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        int mouse_button = txMouseButtons();
        fill_buttons_coords(screen_buttons, current_mode);
        if (current_mode == MAIN_MENU)
        {
            //need to nide all other buttons
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
                    break;
                }
            }

        }

        if (current_mode == BEFORE_START)
        {
            //need to hide all other buttons
            hide_HDC(title);
            hide_HDC(start_button);
            hide_HDC(settings_button);
            hide_HDC(quit_button);


        }
        txSleep(20);
    }

    free_HDC_mem(title, start_button, settings_button, quit_button, left_arrow, right_arrow);
    for (int i = 0; i < 10; ++i)
        free_HDC_mem(numerals[i]);

    return 0;
}
