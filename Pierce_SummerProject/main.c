#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "graphics_HAL.h"
#include "Timer32_HAL.h"
#include "song.h"
#include "ButtonLED_HAL.h"
#include "ADC_HAL.h"

//SWTimer values (measured in us)
#define THREE_SECONDS       3000000
#define ONE_SECOND          1000000
#define HALF_SECOND          500000
#define QUARTER_SECOND       250000
#define EIGHTH_SECOND        125000
#define SIXTEENTH_SECOND      62500

//Joystick threshold values
#define RIGHT_THRESHOLD_OUTSIDE         12000
#define RIGHT_THRESHOLD_INSIDE          10000
#define LEFT_THRESHOLD_OUTSIDE          4000
#define LEFT_THRESHOLD_INSIDE           6000
#define TOP_THRESHOLD_OUTSIDE           12000
#define TOP_THRESHOLD_INSIDE            10000
#define BOTTOM_THRESHOLD_OUTSIDE        3000
#define BOTTOM_THRESHOLD_INSIDE         5000

//Gameplay values
#define NUM_COLORS  2

static int highScores[3] = {0, 0, 0};

typedef enum { MainMenu, HighScores, HowToPlay, HowToPlay_Page2, PlayGame, GameOver } gameState_t;
typedef enum { Play_cursor, HighScores_cursor, HowToPlay_cursor } cursorPos_t;
typedef enum { DisplayCheckSet, SelectTile, Check } modeState_t;

typedef enum { Up, Down, Left, Right, Center } joystick_pos_t;
static joystick_pos_t joystick_pos = Center;

button_t Joystick_Button, Boosterpack_S1, Boosterpack_S2;
extern HWTimer_t timer0, timer1;

button_t Joystick_Button = {GPIO_PORT_P4, GPIO_PIN1, Stable_R, RELEASED_STATE, &timer0};
button_t Boosterpack_S1  = {GPIO_PORT_P5, GPIO_PIN1, Stable_R, RELEASED_STATE, &timer0};
button_t Boosterpack_S2  = {GPIO_PORT_P3, GPIO_PIN5, Stable_R, RELEASED_STATE, &timer0};

static Color checkSet[16] = {Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue};
static Color playSet[16] = {Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue, Blue};

void GameState_FSM(Graphics_Context *g_sContext_p);
void initialize();

void generateRandomTileSet_2_Color();
int numWrong();
void UnHighlightTile(Graphics_Context *g_sContext_p, int position);

bool isJoyStickUp();
bool isJoyStickDown();
bool isJoyStickLeft();
bool isJoyStickRight();

int random_1or0();

int main(void){

    Graphics_Context g_sContext;
    initialize();
    InitGraphics(&g_sContext);

    OneShotSWTimer_t splashScreenTimer;
    InitOneShotSWTimer(&splashScreenTimer, &timer0, THREE_SECONDS);
    StartOneShotSWTimer(&splashScreenTimer);

    DisplayStartupScreen(&g_sContext);
    while(!OneShotSWTimerExpired(&splashScreenTimer));

    while (1)
    {
        GameState_FSM(&g_sContext);
    }
}

void initialize(){
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // initialize the boosterPack LEDs and turn them off except for red LED
    initialize_BoosterpackLED_red();
    initialize_BoosterpackLED_green();
    initialize_BoosterpackLED_blue();
    initialize_LaunchpadLED1();
    initialize_LaunchpadLED2_red();
    initialize_LaunchpadLED2_blue();
    initialize_LaunchpadLED2_green();

    turnOff_BoosterpackLED_red();
    turnOff_BoosterpackLED_green();
    turnOff_BoosterpackLED_blue();
    turnOff_LaunchpadLED1();
    turnOff_LaunchpadLED2_red();
    turnOff_LaunchpadLED2_blue();
    turnOff_LaunchpadLED2_green();

    initialize_BoosterpackTopButton();
    initialize_BoosterpackBottomButton();
    initialize_JoystickButton();

    initHWTimer0();
    initHWTimer1();

    InitSound();

    initADC();
    initJoyStick();
    startADC();
}

void GameState_FSM(Graphics_Context *g_sContext_p){

    OneShotSWTimer_t splashScreenTimer;
    InitOneShotSWTimer(&splashScreenTimer, &timer0, THREE_SECONDS);

    song_t bonk;
    InitSongList_Bonk(&bonk);
    song_t gameOver_song;
    InitSongList_GameOver(&gameOver_song);

    static int highScores[3] = {0, 0, 0};
    static unsigned int gameScore = 0;
    static unsigned int lives = 10;
    static int selected_tile = 0;
    static bool newTopScore = false;
    static bool new2Score = false;
    static bool new3Score = false;

    static gameState_t gameState = MainMenu;
    static cursorPos_t cursorPos = Play_cursor;
    static modeState_t modeState = SelectTile;

    bool allCorrect;
    int i;

    bool joystickButton = ButtonPushed(&Joystick_Button);
    bool boosterpackS1 = ButtonPushed(&Boosterpack_S1);
    bool boosterpackS2 = ButtonPushed(&Boosterpack_S2);

    bool joyStick_Up = isJoyStickUp();
    bool joyStick_Down = isJoyStickDown();
    bool joyStick_Left = isJoyStickLeft();
    bool joyStick_Right = isJoyStickRight();

    int cursorInt;

    static bool changeScreen = true;

    OneShotSWTimer_t joystickErrorCheckTimer;
    InitOneShotSWTimer(&joystickErrorCheckTimer, &timer0, QUARTER_SECOND);
    OneShotSWTimer_t flashTimer;
    InitOneShotSWTimer(&flashTimer, &timer0, HALF_SECOND);

    switch(gameState){
    case MainMenu: {

        if(changeScreen){
            DisplayMenuScreen(g_sContext_p);
            cursorPos = Play_cursor;
            changeScreen = false;
        }

        else if(OneShotSWTimerExpired(&joystickErrorCheckTimer)){
            switch(cursorPos){
            case Play_cursor: {
                cursorInt = 1;
                if(joyStick_Up){
                    //do nothing
                }
                else if(joyStick_Down){
                    MoveMenuCursorDown(g_sContext_p, joyStick_Down, cursorInt);
                    cursorPos = HighScores_cursor;
                    StartOneShotSWTimer(&joystickErrorCheckTimer);
                }
                else if(joystickButton){
                    gameState = PlayGame;
                    changeScreen = true;
                }
                break;
            }
            case HighScores_cursor: {
                cursorInt = 2;
                if(joyStick_Up){
                    MoveMenuCursorUp(g_sContext_p, joyStick_Up, cursorInt);
                    cursorPos = Play_cursor;
                    StartOneShotSWTimer(&joystickErrorCheckTimer);
                }
                else if(joyStick_Down){
                    MoveMenuCursorDown(g_sContext_p, joyStick_Down, cursorInt);
                    cursorPos = HowToPlay_cursor;
                    StartOneShotSWTimer(&joystickErrorCheckTimer);
                }
                else if(joystickButton){
                    gameState = HighScores;
                    changeScreen = true;
                }
                break;
            }
            case HowToPlay_cursor: {
                cursorInt = 3;
                if(joyStick_Up){
                    MoveMenuCursorUp(g_sContext_p, joyStick_Up, cursorInt);
                    cursorPos = HighScores_cursor;
                    StartOneShotSWTimer(&joystickErrorCheckTimer);
                }
                else if(joyStick_Down){
                    //do nothing
                }
                else if(joystickButton){
                    gameState = HowToPlay;
                    //modeState = Throwing;
                    changeScreen = true;
                }
                break;
            }
            };
        }
        break;
    }
    case HighScores: {
        if(changeScreen){
            DisplayHighScores(g_sContext_p, highScores[0], highScores[1], highScores[2]);
            changeScreen = false;
        }
        else if(joystickButton){
            gameState = MainMenu;
            changeScreen = true;
        }
        break;
    }
    case HowToPlay: {
        if(changeScreen){
            DisplayHowToPlayScreen1(g_sContext_p);
            changeScreen = false;
        }
        else if(joystickButton){
            gameState = HowToPlay_Page2;
            changeScreen = true;
        }
        break;
    }
    case HowToPlay_Page2: {
        if(changeScreen){
            DisplayHowToPlayScreen2(g_sContext_p);
            changeScreen = false;
        }
        else if(joystickButton){
            gameState = MainMenu;
            changeScreen = true;
        }
        break;
    }
    case PlayGame: {

        if(changeScreen){
            LCDClearDisplay(g_sContext_p);

            gameScore = 0;
            DrawConstantGameGraphics(g_sContext_p);
            DrawScoreAndLives(g_sContext_p, gameScore, lives);
            modeState = DisplayCheckSet;

            changeScreen = false;
        }

        switch(modeState){
        case DisplayCheckSet:

            generateRandomTileSet_2_Color();

            StartOneShotSWTimer(&splashScreenTimer);

            DrawBoard(g_sContext_p, checkSet);
            while(!OneShotSWTimerExpired(&splashScreenTimer));

            for(i=0; i<16; i++){
                playSet[i] = Blue;
            }

            DrawBoard(g_sContext_p, playSet);

            modeState = SelectTile;

            break;
        case SelectTile:

            HighlightTile(g_sContext_p, selected_tile);
            DrawTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);

            if(joyStick_Up && selected_tile > 3 && OneShotSWTimerExpired(&joystickErrorCheckTimer)){
                UnHighlightTile(g_sContext_p, selected_tile);
                EraseTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);

                selected_tile = selected_tile - 4;

                StartOneShotSWTimer(&joystickErrorCheckTimer);
            }
            else if(joyStick_Down && selected_tile < 12 && OneShotSWTimerExpired(&joystickErrorCheckTimer)){
                UnHighlightTile(g_sContext_p, selected_tile);
                EraseTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);

                selected_tile = selected_tile + 4;

                StartOneShotSWTimer(&joystickErrorCheckTimer);
            }
            else if(joyStick_Left && selected_tile % 4 > 0 && OneShotSWTimerExpired(&joystickErrorCheckTimer)){
                UnHighlightTile(g_sContext_p, selected_tile);
                EraseTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);

                selected_tile = selected_tile - 1;

                StartOneShotSWTimer(&joystickErrorCheckTimer);
            }
            else if(joyStick_Right && selected_tile % 4 < 3 && OneShotSWTimerExpired(&joystickErrorCheckTimer)){
                UnHighlightTile(g_sContext_p, selected_tile);
                EraseTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);

                selected_tile = selected_tile + 1;

                StartOneShotSWTimer(&joystickErrorCheckTimer);
            }
            else if(boosterpackS1){
                //Two-Color version
                if(playSet[selected_tile] == Blue){
                    playSet[selected_tile] = Red;
                    DrawTile(g_sContext_p, selected_tile, Red);
                    HighlightTile(g_sContext_p, selected_tile);
                }
                else if(playSet[selected_tile] == Red){
                    playSet[selected_tile] = Blue;
                    DrawTile(g_sContext_p, selected_tile, Blue);
                    HighlightTile(g_sContext_p, selected_tile);
                }

                EraseTileInfo(g_sContext_p, (unsigned int)selected_tile, playSet[selected_tile]);
            }
            else if(boosterpackS2){
                modeState = Check;
            }

            break;
        case Check:

            EraseScoreAndLives(g_sContext_p, gameScore, lives);

            for(i=0; i<16; i++){
                if(checkSet[i] == playSet[i]){
                    gameScore = gameScore + 1;
                }
                else{
                    if(lives > 0){
                        lives = lives - 1;
                    }

                    allCorrect = false;

                    XTile(g_sContext_p, i);
                }
            }
            if(allCorrect){
                AllCorrect(g_sContext_p);
            }

            DrawScoreAndLives(g_sContext_p, (unsigned int)gameScore, (unsigned int)lives);

            //hold so player can see results
            StartOneShotSWTimer(&splashScreenTimer);
            while(!OneShotSWTimerExpired(&splashScreenTimer));

            if(lives == 0){
                gameState = GameOver;
                changeScreen = true;

            }
            else{
                modeState = DisplayCheckSet;
                selected_tile = 0;
            }

            allCorrect = true;

            break;
        };

        break;

    }
    case GameOver: {
        static bool eraser = false;

        //CODE TO PLAY A JINGLE ON THE GAME OVER SCREEN

        /*

        static bool playSong = true;
        if(playSong){
            if(!PlaySong_nonblocking(gameOver_song)){
                playSong = false;
            }
        }

        */

        int i = 0;
        if(changeScreen){
            DisplayGameOverScreen(g_sContext_p, gameScore); //USES IMAGEREFORMER
            changeScreen = false;
            //playSong = true;

            if(gameScore > highScores[0]){
                highScores[0] = gameScore;
                newTopScore = true;
            }
            else if(gameScore > highScores[1]){
                highScores[1] = gameScore;
                new2Score = true;
            }
            else if(gameScore > highScores[2]){
                highScores[2] = gameScore;
                new3Score = true;
            }
        }
        else if(joystickButton){
            gameState = MainMenu;
            changeScreen = true;
            newTopScore = false;
            new2Score = false;
            new3Score = false;
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        else if(OneShotSWTimerExpired(&flashTimer)){
            //flashes text at one-second intervals
            eraser = !eraser;
            if(eraser){
                Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            }
            else{
                Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            }
            if(newTopScore){
                Graphics_drawString(g_sContext_p, (int8_t *)"NEW HIGH SCORE!", -1, 10, LINE10_Y, true);
                i++;
            }
            else if(new2Score){
                Graphics_drawString(g_sContext_p, (int8_t *)"SECOND BEST SCORE!", -1, 10, LINE10_Y, true);
                i++;
            }
            else if(new3Score){
                Graphics_drawString(g_sContext_p, (int8_t *)"THIRD BEST SCORE!", -1, 10, LINE10_Y, true);
                i++;
            }
            StartOneShotSWTimer(&flashTimer);
        }
        break;
    }
    };
}

void generateRandomTileSet_2_Color(){
    int i;
    for(i=0; i<16; i++){
        int j = random_1or0();
        if(j==1){
            checkSet[i] = Blue;
        }
        else{
            checkSet[i] = Red;
        }
    }
}

int numWrong(){
    int numNotMatchingTiles = 0;
    int i;
    for(i=0; i<16; i++){
        if(checkSet[i] != playSet[i]){
            numNotMatchingTiles = numNotMatchingTiles + 1;
        }
    }
    return numNotMatchingTiles;
}

void UnHighlightTile(Graphics_Context *g_sContext_p, int position){
    DrawTile(g_sContext_p, position, playSet[position]);
}

bool isJoyStickUp(){
    bool move = false;
    unsigned int x, y;
    getSampleJoyStick(&x, &y);
    switch(joystick_pos){
    case Up:
        if(y > TOP_THRESHOLD_OUTSIDE){
            joystick_pos = Up;
            move = false;
        }
        else if(y <= TOP_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    default:
        if(y > TOP_THRESHOLD_OUTSIDE){
            joystick_pos = Up;
            move = true;
        }
        else if(y <= TOP_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    }
    return move;
}
bool isJoyStickDown(){
    bool move = false;
    unsigned int x, y;
    getSampleJoyStick(&x, &y);
    switch(joystick_pos){
    case Down:
        if(y < BOTTOM_THRESHOLD_OUTSIDE){
            joystick_pos = Down;
            move = false;
        }
        else if(y >= BOTTOM_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    default:
        if(y < BOTTOM_THRESHOLD_OUTSIDE){
            joystick_pos = Down;
            move = true;
        }
        else if(y >= BOTTOM_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    }
    return move;
}
bool isJoyStickLeft(){
    bool move = false;
    unsigned int x, y;
    getSampleJoyStick(&x, &y);
    switch(joystick_pos){
    case Left:
        if(x < LEFT_THRESHOLD_OUTSIDE){
            joystick_pos = Left;
            move = true;
        }
        else if(x >= LEFT_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    default:
        if(x < LEFT_THRESHOLD_OUTSIDE){
            joystick_pos = Left;
            move = true;
        }
        else if(x >= LEFT_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    }
    return move;
}
bool isJoyStickRight(){
    bool move = false;
    unsigned int x, y;
    getSampleJoyStick(&x, &y);
    switch(joystick_pos){
    case Right:
        if(x > RIGHT_THRESHOLD_OUTSIDE){
            joystick_pos = Right;
            move = true;
        }
        else if(x <= RIGHT_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    default:
        if(x > RIGHT_THRESHOLD_OUTSIDE){
            joystick_pos = Right;
            move = true;
        }
        else if(x <= RIGHT_THRESHOLD_INSIDE){
            joystick_pos = Center;
            move = false;
        }
        break;
    }
    return move;
}

int random_1or0(){
    unsigned int x, y;
    getSampleJoyStick(&x, &y);
    x = x % 2;
    y = y % 2;
    return x ^ y;
}
