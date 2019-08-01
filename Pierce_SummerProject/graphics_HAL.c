#include "graphics_HAL.h"

Graphics_Rectangle tile[16] = { {16 , 16 , 39 , 39 },
                                {40 , 16 , 63 , 39 },
                                {64 , 16 , 87 , 39 },
                                {88 , 16 , 111, 39 },
                                {16 , 40 , 39 , 63 },
                                {40 , 40 , 63 , 63 },
                                {64 , 40 , 87 , 63 },
                                {88 , 40 , 111, 63 },
                                {16 , 64 , 39 , 87 },
                                {40 , 64 , 63 , 87 },
                                {64 , 64 , 87 , 87 },
                                {88 , 64 , 111, 87 },
                                {16 , 88 , 39 , 111},
                                {40 , 88 , 63 , 111},
                                {64 , 88 , 87 , 111},
                                {88 , 88 , 111, 111}
                              };

Graphics_Rectangle intHighlight[16] = { {16 , 16 , 39 , 39 },
                                        {40 , 16 , 63 , 39 },
                                        {64 , 16 , 87 , 39 },
                                        {88 , 16 , 111, 39 },
                                        {16 , 40 , 39 , 63 },
                                        {40 , 40 , 63 , 63 },
                                        {64 , 40 , 87 , 63 },
                                        {88 , 40 , 111, 63 },
                                        {16 , 64 , 39 , 87 },
                                        {40 , 64 , 63 , 87 },
                                        {64 , 64 , 87 , 87 },
                                        {88 , 64 , 111, 87 },
                                        {16 , 88 , 39 , 111},
                                        {40 , 88 , 63 , 111},
                                        {64 , 88 , 87 , 111},
                                        {88 , 88 , 111, 111}
                                      };

void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void InitGraphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmss12);
    InitFonts();
    Graphics_clearDisplay(g_sContext_p);
}

void LCDClearDisplay(Graphics_Context *g_sContext_p) {
    Graphics_clearDisplay(g_sContext_p);
}

void make_2digit_NumString(unsigned int num, int8_t *string)
{
    string[0] = num/10 + '0';
    string[1] = num%10 + '0';
}

void make_3digit_NumString(unsigned int num, int8_t *string){
    string[0] = num/100 + '0';
    string[1] = (num%100)/10 + '0';
    string[2] = num%10 + '0';
}

void make_5digit_NumString(unsigned int num, int8_t *string)
{
    string[0]= (        num  / 10000) +'0';
    string[1]= ((num%10000) / 1000) +'0';
    string[2]= ((num%1000) / 100) +'0';
    string[3]= ((num%100) / 10) +'0';
    string[4]= ((num%10) / 1) +'0';
    string[5]= 0;
}

void DisplayStartupScreen(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    Graphics_drawString(g_sContext_p, (int8_t *)"TILES", -1, 0, LINE2_Y, true);
    Graphics_drawString(g_sContext_p, (int8_t *)"A Memory Game", -1, 0, LINE4_Y, true);
}

void DisplayMenuScreen(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    int8_t playString[5]        = "Play";
    int8_t highScoresString[12] = "High Scores";
    int8_t howToPlayString[12]  = "How To Play";

    Graphics_drawString(g_sContext_p, playString      , -1, MENU_STRING_X, PLAY_STRING_Y      , true);
    Graphics_drawString(g_sContext_p, highScoresString, -1, MENU_STRING_X, HIGHSCORES_STRING_Y, true);
    Graphics_drawString(g_sContext_p, howToPlayString , -1, MENU_STRING_X, HOWTOPLAY_STRING_Y , true);

    DrawMenuCursor(g_sContext_p, DEFAULT_CURSOR_LOC);
}

void DisplayDifficultySelectScreen(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    int8_t easyString[5]    = "Easy";
    int8_t mediumString[7]  = "Medium";
    int8_t hardString[5]    = "Hard";

    Graphics_drawString(g_sContext_p, easyString  , -1, MENU_STRING_X, PLAY_STRING_Y      , true);
    Graphics_drawString(g_sContext_p, mediumString, -1, MENU_STRING_X, HIGHSCORES_STRING_Y, true);
    Graphics_drawString(g_sContext_p, hardString  , -1, MENU_STRING_X, HOWTOPLAY_STRING_Y , true);

    DrawMenuCursor(g_sContext_p, DEFAULT_CURSOR_LOC);
}

void DisplayHowToPlayScreen1(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    int8_t line1String[]    = "How To Play";
    int8_t line2String[]    = "";
    int8_t line3String[]    = "You will be shown a 4x4";
    int8_t line4String[]    = "board of tiles. Remember";
    int8_t line5String[]    = "the tile colors!";
    int8_t line6String[]    = "You need to match the";
    int8_t line7String[]    = "colors of the tile set";
    int8_t line8String[]    = "you saw.";
    int8_t line9String[]    = "";
    int8_t line10String[]   = "Next - Joystick Button";

    Graphics_drawString(g_sContext_p, line1String , -1, 2, LINE1_Y , true);
    Graphics_drawString(g_sContext_p, line2String , -1, 2, LINE2_Y , true);
    Graphics_drawString(g_sContext_p, line3String , -1, 2, LINE3_Y , true);
    Graphics_drawString(g_sContext_p, line4String , -1, 2, LINE4_Y , true);
    Graphics_drawString(g_sContext_p, line5String , -1, 2, LINE5_Y , true);
    Graphics_drawString(g_sContext_p, line6String , -1, 2, LINE6_Y , true);
    Graphics_drawString(g_sContext_p, line7String , -1, 2, LINE7_Y , true);
    Graphics_drawString(g_sContext_p, line8String , -1, 2, LINE8_Y , true);
    Graphics_drawString(g_sContext_p, line9String , -1, 2, LINE9_Y , true);
    Graphics_drawString(g_sContext_p, line10String, -1, 2, LINE10_Y, true);
}

void DisplayHowToPlayScreen2(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    int8_t line1String[]    = "How To Play";
    int8_t line2String[]    = "";
    int8_t line3String[]    = "Joystick: navigate board";
    int8_t line4String[]    = "S1: cycle tile color";
    int8_t line5String[]    = "S2: submit board";
    int8_t line6String[]    = "Each correct tile is +1";
    int8_t line7String[]    = "point, while each incorrect";
    int8_t line8String[]    = "tile is -1 life.";
    int8_t line9String[]    = "For Main Menu, press";
    int8_t line10String[]   = "the Joystick Button.";

    Graphics_drawString(g_sContext_p, line1String , -1, 2, LINE1_Y , true);
    Graphics_drawString(g_sContext_p, line2String , -1, 2, LINE2_Y , true);
    Graphics_drawString(g_sContext_p, line3String , -1, 2, LINE3_Y , true);
    Graphics_drawString(g_sContext_p, line4String , -1, 2, LINE4_Y , true);
    Graphics_drawString(g_sContext_p, line5String , -1, 2, LINE5_Y , true);
    Graphics_drawString(g_sContext_p, line6String , -1, 2, LINE6_Y , true);
    Graphics_drawString(g_sContext_p, line7String , -1, 2, LINE7_Y , true);
    Graphics_drawString(g_sContext_p, line8String , -1, 2, LINE8_Y , true);
    Graphics_drawString(g_sContext_p, line9String , -1, 2, LINE9_Y , true);
    Graphics_drawString(g_sContext_p, line10String, -1, 2, LINE10_Y, true);
}

void DisplayHighScores(Graphics_Context *g_sContext_p, unsigned int score1, unsigned int score2, unsigned int score3){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    Graphics_drawString(g_sContext_p, (int8_t *)"HIGH SCORES", -1, 30, LINE2_Y, true);

    int8_t first[2], second[2], third[2];
    make_2digit_NumString(score1, first );
    make_2digit_NumString(score2, second);
    make_2digit_NumString(score3, third );

    Graphics_drawString(g_sContext_p, first , -1, 60, LINE4_Y, true);
    Graphics_drawString(g_sContext_p, second, -1, 60, LINE6_Y, true);
    Graphics_drawString(g_sContext_p, third , -1, 60, LINE8_Y, true);
}

void DisplayGameOverScreen(Graphics_Context *g_sContext_p, unsigned int score){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    LCDClearDisplay(g_sContext_p);

    Graphics_drawImage(g_sContext_p, &gameover1BPP_UNCOMP, 0, 0);

    int8_t scoreString[23];
    make_3digit_NumString(score, scoreString);

    int8_t text[10] = "Score:   ";
    text[7] = scoreString[0];
    text[8] = scoreString[1];
    text[9] = scoreString[2];

    Graphics_drawString(g_sContext_p, text, -1, 15, LINE9_Y, true);
}

void DrawTile(Graphics_Context *g_sContext_p, int position, Color color){
    switch(color){
    case Red:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
        break;
    case Blue:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
        break;
    case Green:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_GREEN);
        break;
    case Yellow:
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_YELLOW);
        break;
    };

    Graphics_fillRectangle(g_sContext_p, &tile[position]);
}

void DrawBoard(Graphics_Context *g_sContext_p, Color *color){
    int i;
    for(i=0; i<16; i++){
        DrawTile(g_sContext_p, i, color[i]);
    }
}

void HighlightTile(Graphics_Context *g_sContext_p, int position){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_drawRectangle(g_sContext_p, &tile[position]);
    Graphics_drawRectangle(g_sContext_p, &intHighlight[position]);
}

void DrawMenuCursor(Graphics_Context *g_sContext_p, int cursor_loc){
    switch(cursor_loc){
    case 1:
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, PLAY_CURSOR_Y, MENU_CURSOR_X + 10, PLAY_CURSOR_Y + 5);
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, PLAY_CURSOR_Y + 10, MENU_CURSOR_X + 10, PLAY_CURSOR_Y + 5);
        break;
    case 2:
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, HIGHSCORES_CURSOR_Y, MENU_CURSOR_X + 10, HIGHSCORES_CURSOR_Y + 5);
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, HIGHSCORES_CURSOR_Y + 10, MENU_CURSOR_X + 10, HIGHSCORES_CURSOR_Y + 5);
        break;
    case 3:
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, HOWTOPLAY_CURSOR_Y, MENU_CURSOR_X + 10, HOWTOPLAY_CURSOR_Y + 5);
        Graphics_drawLine(g_sContext_p, MENU_CURSOR_X, HOWTOPLAY_CURSOR_Y + 10, MENU_CURSOR_X + 10, HOWTOPLAY_CURSOR_Y + 5);
        break;
    default:
        Graphics_drawString(g_sContext_p, (int8_t *)"Error: DrawMenuCursor", -1, 0, 0, true);
        break;
    };
}


void MoveMenuCursorUp(Graphics_Context *g_sContext_p, bool moveUp, int current_cursorLoc){
    if(current_cursorLoc > 1 && moveUp){
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        DrawMenuCursor(g_sContext_p, current_cursorLoc);
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        DrawMenuCursor(g_sContext_p, current_cursorLoc-1);
    }
}


void MoveMenuCursorDown(Graphics_Context *g_sContext_p, bool moveDown, int current_cursorLoc){
    if(current_cursorLoc < 3 && moveDown){
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        DrawMenuCursor(g_sContext_p, current_cursorLoc);
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        DrawMenuCursor(g_sContext_p, current_cursorLoc+1);
    }
}

void DrawHeart(Graphics_Context *g_sContext_p, int x, int y){
    Graphics_drawLineV(g_sContext_p, x  , y+1, y+3);
    Graphics_drawLineV(g_sContext_p, x+1, y  , y+4);
    Graphics_drawLineV(g_sContext_p, x+2, y  , y+5);
    Graphics_drawLineV(g_sContext_p, x+3, y+1, y+6);
    Graphics_drawLineV(g_sContext_p, x+4, y+2, y+7);
    Graphics_drawLineV(g_sContext_p, x+5, y+1, y+6);
    Graphics_drawLineV(g_sContext_p, x+6, y  , y+5);
    Graphics_drawLineV(g_sContext_p, x+7, y  , y+4);
    Graphics_drawLineV(g_sContext_p, x+8, y+1, y+3);
}

void DrawLives(Graphics_Context *g_sContext_p, unsigned int lives){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    int8_t lives_string[2];
    make_2digit_NumString(lives, lives_string);
    Graphics_drawString(g_sContext_p, lives_string, -1, LIVESNUM_X, LINE1_Y, true);
}

void DrawScore(Graphics_Context *g_sContext_p, unsigned int score){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    int8_t score_string[3];
    make_3digit_NumString(score, score_string);
    Graphics_drawString(g_sContext_p, score_string, -1, SCORENUM_X, LINE1_Y, true);
}

void DrawTileNumber(Graphics_Context *g_sContext_p, unsigned int tileNum){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    int8_t tileNum_string[2];
    make_2digit_NumString(tileNum, tileNum_string);
    Graphics_drawString(g_sContext_p, tileNum_string, -1, TILENUM_X, LINE10_Y + 5, true);
}

void DrawTileColor(Graphics_Context *g_sContext_p, Color color){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    switch(color){
    case Red:
        Graphics_drawString(g_sContext_p, (int8_t *)"Red", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Blue:
        Graphics_drawString(g_sContext_p, (int8_t *)"Blue", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Green:
        Graphics_drawString(g_sContext_p, (int8_t *)"Green", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Yellow:
        Graphics_drawString(g_sContext_p, (int8_t *)"Yellow", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    };
}

void EraseLives(Graphics_Context *g_sContext_p, unsigned int lives){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    int8_t lives_string[2];
    make_2digit_NumString(lives, lives_string);
    Graphics_drawString(g_sContext_p, lives_string, -1, LIVESNUM_X, LINE1_Y, true);
}
void EraseScore(Graphics_Context *g_sContext_p, unsigned int score){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    int8_t score_string[3];
    make_3digit_NumString(score, score_string);
    Graphics_drawString(g_sContext_p, score_string, -1, SCORENUM_X, LINE1_Y, true);
}
void EraseTileNumber(Graphics_Context *g_sContext_p, unsigned int tileNum){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    int8_t tileNum_string[2];
    make_2digit_NumString(tileNum, tileNum_string);
    Graphics_drawString(g_sContext_p, tileNum_string, -1, TILENUM_X, LINE10_Y + 5, true);
}
void EraseTileColor(Graphics_Context *g_sContext_p, Color color){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    switch(color){
    case Red:
        Graphics_drawString(g_sContext_p, (int8_t *)"Red", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Blue:
        Graphics_drawString(g_sContext_p, (int8_t *)"Blue", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Green:
        Graphics_drawString(g_sContext_p, (int8_t *)"Green", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    case Yellow:
        Graphics_drawString(g_sContext_p, (int8_t *)"Yellow", -1, COLORSTRING_X, LINE10_Y + 5, true);
        break;
    };
}

void DrawTileInfo(Graphics_Context *g_sContext_p, unsigned int tileNum, Color color){
    DrawTileNumber(g_sContext_p, tileNum);
    DrawTileColor(g_sContext_p, color);
}

void EraseTileInfo(Graphics_Context *g_sContext_p, unsigned int tileNum, Color color){
    EraseTileNumber(g_sContext_p, tileNum);
    EraseTileColor(g_sContext_p, color);
}

void DrawScoreAndLives(Graphics_Context *g_sContext_p, unsigned int score, unsigned int lives){
    DrawLives(g_sContext_p, lives);
    DrawScore(g_sContext_p, score);
}

void EraseScoreAndLives(Graphics_Context *g_sContext_p, unsigned int score, unsigned int lives){
    EraseScore(g_sContext_p, score);
    EraseLives(g_sContext_p, lives);
}

void DrawConstantGameGraphics(Graphics_Context *g_sContext_p){
    Graphics_drawString(g_sContext_p, (int8_t *)"Tile", -1, TILESTRING_X, LINE10_Y + 5, true);
    DrawHeart(g_sContext_p, HEART_X, LINE1_Y + 2);
}

void XTile(Graphics_Context *g_sContext_p, int tileNum){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

    int x1 = 16 + ((tileNum % 4) * 24) + 3;
    int y1 = 16 + ((tileNum / 4) * 24) + 3;
    int y2 = y1 + 17;

    Graphics_drawLine(g_sContext_p, x1    , y1    , x1 + 17    , y1 + 17    );
    Graphics_drawLine(g_sContext_p, x1 - 1, y1 + 1, x1 - 1 + 17, y1 + 1 + 17);
    Graphics_drawLine(g_sContext_p, x1 + 1, y1 - 1, x1 + 1 + 17, y1 - 1 + 17);

    Graphics_drawLine(g_sContext_p, x1    , y2    , x1 + 17    , y2 - 17    );
    Graphics_drawLine(g_sContext_p, x1 - 1, y2 + 1, x1 - 1 + 17, y2 + 1 - 17);
    Graphics_drawLine(g_sContext_p, x1 + 1, y2 - 1, x1 + 1 + 17, y2 - 1 - 17);
}

void AllCorrect(Graphics_Context *g_sContext_p){
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

    Graphics_drawLine(g_sContext_p, 35, 68, 55, 100);
    Graphics_drawLine(g_sContext_p, 34, 69, 54, 101);
    Graphics_drawLine(g_sContext_p, 36, 67, 56,  99);

    Graphics_drawLine(g_sContext_p, 55, 100, 95, 30);
    Graphics_drawLine(g_sContext_p, 54, 101, 94, 31);
    Graphics_drawLine(g_sContext_p, 56,  99, 96, 29);
}
