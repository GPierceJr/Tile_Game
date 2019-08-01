#include "sound.h"
#include "Timer32_HAL.h"


#define SYSTEMCLOCK 48000000
extern HWTimer_t timer0;

void InitSound() {
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P2,
            GPIO_PIN7,
            GPIO_PRIMARY_MODULE_FUNCTION);
}

/*
 * Configures the PWM for a specific note in a song
 */
void ConfigurePWM(Timer_A_PWMConfig *pwmConfig_p, basic_note_t note) {

    double toneFreq[49] =
    {
        0.00,   // silent
        16.35,  // c0
        18.35,  // d0
        20.60,  // e0
        21.83,  // f0
        24.50,  // g0
        27.50,  // a0
        30.87,  // b0
        21.22,  // f0 flat
        29.14,  // b0 flat
        32.70,  // c1
        36.71,  // d1
        41.20,  // e1
        43.65,  // f1
        49.00,  // g1
        55.00,  // a1
        61.74,  // b1
        65.41,  // c2
        73.42,  // d2
        82.41,  // e2
        87.31,  // f2
        98.00,  // g2
        110.00, // a2
        123.47, // b2
        130.81, // c3
        146.83, // d3
        164.81, // e3
        174.61, // f3
        196.00, // g3
        220.00, // a3
        246.94, // b3
        261.63, // C4
        293.66, // D4
        329.63, // E4
        349.63, // F4
        392.00, // G4
        440.00, // A4
        493.88, // B4
        523.25, // C5
        587.33, // D5
        659.25, // E5
        698.46, // F5
        783.99, // G5
        880.00, // A5
        987.77, // B5
        739.99, // F5 sharp
        369.99, // F4 sharp
        369.99, // g4 flat
        1046.50 // C6
    };


    /*
     * BUZZER OUTPUT IS CONNECTED TO TA0.4
     */
    pwmConfig_p->compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_4;

    pwmConfig_p->compareOutputMode =  TIMER_A_OUTPUTMODE_RESET_SET;

    // The clock source used for the Timer_A. This is not the frequency of the clock. It is a flag that tells what clock source is used.
    pwmConfig_p->clockSource = TIMER_A_CLOCKSOURCE_SMCLK;

    /*
     * clockSourceDivider = toneFreq / (48M*timerPeriod) // try 48
     */
    pwmConfig_p->clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48;

    /*
     * timerPeriod = toneFreq / (48M*clockSourceDivider)
     */
    pwmConfig_p->timerPeriod = SYSTEMCLOCK / (toneFreq[(int)note] * pwmConfig_p->clockSourceDivider);

    /*
     * Musical notes are similar to sine waves, therefore timeOn = timeOff
     */
    pwmConfig_p->dutyCycle = pwmConfig_p->timerPeriod * 0.5;

}

//DONE FOR NONBLOCKING
bool PlayNote_nonblocking(song_note_t songNote)
{
    Timer_A_PWMConfig pwmConfig;
    static OneShotSWTimer_t noteLength;
    static bool startedPlay = false;
    if(!startedPlay){
        ConfigurePWM(&pwmConfig, songNote.note_name);
        InitOneShotSWTimer(&noteLength, &timer0, songNote.note_length*1000);
        StartOneShotSWTimer(&noteLength);
        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
        startedPlay = true;
    }
    if(OneShotSWTimerExpired(&noteLength)){
        Timer_A_stopTimer(TIMER_A0_BASE);
        startedPlay = false;
    }
    return startedPlay;
}

void PlayNote_blocking(song_note_t songNote)
{
    // The struct that holds all the info for PWM driving the buzzer
    Timer_A_PWMConfig pwmConfig;
    ConfigurePWM(&pwmConfig, songNote.note_name);

    //  the one shot timer for playing the note
    OneShotSWTimer_t noteLength;
    InitOneShotSWTimer(&noteLength, &timer0, songNote.note_length*1000);
    StartOneShotSWTimer(&noteLength);

    // Start driving the pwm to generate the sound
    if (songNote.note_name != note_silent)
        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    // wait until the duration of the note is over, i.e. the timer is expired
    while (!(OneShotSWTimerExpired(&noteLength)));

    // We stop the PWM
    Timer_A_stopTimer(TIMER_A0_BASE);
}
