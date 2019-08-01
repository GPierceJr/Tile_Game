// Any function that calls graphic library functions should be declared here and implemented in graphics_HAL.c
#ifndef GRAPHICS_HAL_H_
#define GRAPHICS_HAL_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

//graphics locations on screen
#define MAX_X          127
#define MAX_Y          127
#define CENTER_X       (MAX_X / 2) + 1
#define CENTER_Y       (MAX_Y / 2) + 1

//main menu locations
#define MENU_STRING_X           20
#define MENU_CURSOR_X           5
#define PLAY_STRING_Y           64
#define PLAY_CURSOR_Y           64
#define HIGHSCORES_STRING_Y     PLAY_STRING_Y + 20
#define HIGHSCORES_CURSOR_Y     PLAY_CURSOR_Y + 20
#define HOWTOPLAY_STRING_Y      HIGHSCORES_STRING_Y + 20
#define HOWTOPLAY_CURSOR_Y      HIGHSCORES_CURSOR_Y + 20
#define DEFAULT_CURSOR_LOC      1

//Text line y-locations
#define LINE1_Y     2
#define LINE2_Y     LINE1_Y + 12
#define LINE3_Y     LINE2_Y + 12
#define LINE4_Y     LINE3_Y + 12
#define LINE5_Y     LINE4_Y + 12
#define LINE6_Y     LINE5_Y + 12
#define LINE7_Y     LINE6_Y + 12
#define LINE8_Y     LINE7_Y + 12
#define LINE9_Y     LINE8_Y + 12
#define LINE10_Y    LINE9_Y + 12

//game element constants
#define TILE_SIZE       24
#define HEART_X         94
#define LIVESNUM_X      108
#define SCORENUM_X      2
#define TILESTRING_X    2
#define TILENUM_X       25
#define COLORSTRING_X   105

extern const Graphics_Image gameover1BPP_UNCOMP;

typedef enum {Blue, Red, Green, Yellow} Color;

extern Graphics_Rectangle tile[];

void InitFonts();
void InitGraphics(Graphics_Context *g_sContext_p);
void LCDClearDisplay(Graphics_Context *g_sContext_p);

void make_2digit_NumString(unsigned int num, int8_t *string);
void make_3digit_NumString(unsigned int num, int8_t *string);
void make_5digit_NumString(unsigned int num, int8_t *string);

void DisplayStartupScreen(Graphics_Context *g_sContext_p);
void DisplayMenuScreen(Graphics_Context *g_sContext_p);
void DisplayHowToPlayScreen1(Graphics_Context *g_sContext_p);
void DisplayHowToPlayScreen2(Graphics_Context *g_sContext_p);
void DisplayHighScores(Graphics_Context *g_sContext_p, unsigned int score1, unsigned int score2, unsigned int scored);
void DisplayDifficultySelectScreen(Graphics_Context *g_sContext_p);
void DisplayGameOverScreen(Graphics_Context *g_sContext_p, unsigned int score);

void DrawTile(Graphics_Context *g_sContext_p, int position, Color color);
void DrawBoard(Graphics_Context *g_sContext_p, Color *color);
void HighlightTile(Graphics_Context *g_sContext_p, int position);

void DrawMenuCursor(Graphics_Context *g_sContext_p, int cursor_loc);
void MoveMenuCursorUp(Graphics_Context *g_sContext_p, bool moveUp, int current_cursorLoc);
void MoveMenuCursorDown(Graphics_Context *g_sContext_p, bool moveDown, int current_cursorLoc);

void DrawHeart(Graphics_Context *g_sContext_p, int x, int y);
void DrawLives(Graphics_Context *g_sContext_p, unsigned int lives);
void DrawScore(Graphics_Context *g_sContext_p, unsigned int score);
void DrawTileNumber(Graphics_Context *g_sContext_p, unsigned int tileNum);
void DrawTileColor(Graphics_Context *g_sContext_p, Color color);
void EraseLives(Graphics_Context *g_sContext_p, unsigned int lives);
void EraseScore(Graphics_Context *g_sContext_p, unsigned int score);
void EraseTileNumber(Graphics_Context *g_sContext_p, unsigned int tileNum);
void EraseTileColor(Graphics_Context *g_sContext_p, Color color);
void DrawTileInfo(Graphics_Context *g_sContext_p, unsigned int tileNum, Color color);
void EraseTileInfo(Graphics_Context *g_sContext_p, unsigned int tileNum, Color color);
void DrawScoreAndLives(Graphics_Context *g_sContext_p, unsigned int score, unsigned int lives);
void EraseScoreAndLives(Graphics_Context *g_sContext_p, unsigned int score, unsigned int lives);
void DrawConstantGameGraphics(Graphics_Context *g_sContext_p);

void XTile(Graphics_Context *g_sContext_p, int tileNum);
void AllCorrect(Graphics_Context *g_sContext_p);

#endif
