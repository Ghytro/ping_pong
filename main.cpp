#include "TXLib.h"
#include <vector>

#define WIND_X_SIZE 800
#define WIND_Y_SIZE 800

enum game_mode
{
    MAIN_MENU,
    SETTINGS_MENU,
    BEFORE_START,
    PLAYING,
    PAUSE,
};

enum button
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

    draw_main_menu(title, start_button, settings_button, quit_button);

    game_mode current_mode = MAIN_MENU;
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        POINT cursor_pos = txMousePos();

        if (current_mode == MAIN_MENU)
        {



        }
    }

    txDeleteDC(title);
    txDeleteDC(start_button);
    txDeleteDC(settings_button);
    txDeleteDC(quit_button);
}
