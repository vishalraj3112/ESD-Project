/**
 * @file    :   rtc.c
 * @brief   :   An abstraction for RTC functions
 *
 *              This source file provides abstraction of RTC functions which are
 *              used to initialize and keep track of time
 *
 * @author  :   Sanish Kharade
 * @date    :   November 28, 2021
 * @version :   1.0
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   MSP432 Reference Manual
*/
#include <stdbool.h>
#include <inc/rtc.h>

static volatile RTC_C_Calendar current_time;
static volatile bool one_second_tracker;


RTC_C_Calendar RTC_get_current_time(void)
{
    return current_time;
}

//check logic
bool one_second_elapsed(void)
{
    return one_second_tracker;
}
void clear_one_second_elapsed_flag(void)
{
    one_second_tracker = false;
}
void RTC_init(RTC_C_Calendar start_time)
{
    /* Initialize the RTC, some functions are done for debugging purposes */

    /* Initializing RTC with current time */
    MAP_RTC_C_initCalendar(&start_time, RTC_C_FORMAT_BINARY);

    /* Setup Calendar Alarm for 10:04pm (for the flux capacitor) */
    MAP_RTC_C_setCalendarAlarm(0x04, 0x22, RTC_C_ALARMCONDITION_OFF,
            RTC_C_ALARMCONDITION_OFF);

    /* Specify an interrupt to assert every minute */
    MAP_RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);

    /* Enable interrupt for RTC Ready Status, which asserts when the RTC
     * Calendar registers are ready to read.
     * Also, enable interrupts for the Calendar alarm and Calendar event. */
    MAP_RTC_C_clearInterruptFlag(
            RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
                    | RTC_C_CLOCK_ALARM_INTERRUPT);
    MAP_RTC_C_enableInterrupt(
            RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
                    | RTC_C_CLOCK_ALARM_INTERRUPT);

    /* Start RTC Clock */
    MAP_RTC_C_startClock();

    /* Enable interrupts and go to sleep. */
    MAP_Interrupt_enableInterrupt(INT_RTC_C);

    MAP_Interrupt_enableMaster();
}


/*********************************************************************************
 * @function:   RTC_C_IRQHandler
 *
 * @brief   :   ISR for RTC
 *
 * @param   :   none
 *
 * @return  :   void
 *
**********************************************************************************/
void RTC_C_IRQHandler(void)
{
    uint32_t status;
    one_second_tracker = true;

    /*Update current time */
    current_time = MAP_RTC_C_getCalendarTime();

    status = MAP_RTC_C_getEnabledInterruptStatus();
    MAP_RTC_C_clearInterruptFlag(status);

    if (status & RTC_C_CLOCK_READ_READY_INTERRUPT)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

    if (status & RTC_C_TIME_EVENT_INTERRUPT)
    {
        /* Interrupts every minute - Set breakpoint here */
        __no_operation();
        current_time = MAP_RTC_C_getCalendarTime();
    }

    if (status & RTC_C_CLOCK_ALARM_INTERRUPT)
    {
        /* Interrupts at 10:04pm */
        __no_operation();
    }

}





