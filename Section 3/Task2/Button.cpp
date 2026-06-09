#include <stdint.h>

/*
 * Enumeration defining all possible button states.
 *
 * STATE_IDLE:
 *     Button is not pressed.
 *
 * STATE_PRESS_DETECTED:
 *     A potential press has been detected and must be validated.
 *
 * STATE_PRESSED:
 *     Button press has been confirmed and accepted as stable.
 */
typedef enum {
    STATE_IDLE,
    STATE_PRESS_DETECTED,
    STATE_PRESSED
} ButtonState;

/*
 * Global state variable storing the current state of the debounce state machine.
 *
 * Initial state is IDLE because the button is assumed to be released
 * when the system starts.
 */
ButtonState state = STATE_IDLE;

/*
 * Debounce routine.
 *
 * This function is intended to be executed periodically
 * (e.g. every 5 ms from an ISR or RTOS task).
 *
 * raw_pin_state:
 *     Current raw GPIO reading.
 *     1 = button released
 *     0 = button pressed
 *
 * Returns:
 *     Stable debounced button state.
 */
int debounce_button(int raw_pin_state)
{
    /*
     * static ensures the value persists between function calls.
     *
     * Without static, stable_state would be recreated and reset
     * every time the function executes, making debouncing impossible.
     */
    static int stable_state = 1;

    /*
     * State machine controlling debounce behavior.
     */
    switch (state)
    {
        /*
         * No active button press detected.
         */
        case STATE_IDLE:

            /*
             * If the raw signal indicates a press,
             * move to validation state.
             */
            if (raw_pin_state == 0)
            {
                state = STATE_PRESS_DETECTED;
            }

            break;

        /*
         * Possible button press detected.
         * A second consecutive confirmation is required
         * before accepting the press as valid.
         */
        case STATE_PRESS_DETECTED:

            if (raw_pin_state == 0)
            {
                /*
                 * Press confirmed.
                 */
                state = STATE_PRESSED;

                /*
                 * Update stable debounced state.
                 */
                stable_state = 0;
            }
            else
            {
                /*
                 * Signal returned high.
                 * This was likely contact bounce,
                 * so return to idle state.
                 */
                state = STATE_IDLE;
            }

            break;

        /*
         * Button is currently considered pressed.
         */
        case STATE_PRESSED:

            /*
             * Detect button release.
             */
            if (raw_pin_state == 1)
            {
                state = STATE_IDLE;

                /*
                 * Update stable state to released.
                 */
                stable_state = 1;
            }

            break;
    }

    /*
     * Return the debounced button state.
     */
    return stable_state;
}