#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

/**
 * This section defines ANSI escape codes for different text and background colors, as well as text formatting styles.
 * These escape codes are used to change the appearance of text in a terminal/console.
 * Each escape code is a string that can be inserted into a printed string to modify its appearance.
 */
#define BLACK_TEXT "\x1b[30m" //Sets the text color to black.
#define RED_TEXT "\x1b[31m" //Sets the text color to red.
#define GRAY_TEXT "\x1b[90m" //Sets the text color to gray.
#define WHITE_BACKGROUND "\x1b[47m" //Sets the background color to white.
#define UNDERLINE "\x1b[4m" //Applies underline formatting to the text.
#define ITALIC "\x1b[3m" //Applies italic formatting to the text.
#define RESET "\x1b[0m" //Resets all text formatting to default (removes any applied styles or colors).

/**
 * This array represents the number of days in each month of the Shamsi (Solar Hijri) calendar.
 * The values represent the number of days in each month, from the first month (Farvardin) to the last month (Esfand).
 * The last two elements of the array represent the number of days in the leap year
 * -which occurs every 33 and 4 years in the Shamsi calendar.
 */
int days_in_shamsi_month[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29, 29};

/**
 * This array of strings represents the names of the months in the Shamsi (Solar Hijri) calendar.
 * Each element of the array corresponds to a specific month, indexed from 1 to 12.
 * The strings are formatted with escape sequences for displaying colored text in the terminal.
 * The colors are used for highlighting the month names when printed.
 */
char *shamsiMonths[] = {
        " ",
        BLACK_TEXT WHITE_BACKGROUND "               Farvardin              " RESET,
        BLACK_TEXT WHITE_BACKGROUND "              Ordibehesht             " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                Khordad               " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                  Tir                 " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                 Mordad               " RESET,
        BLACK_TEXT WHITE_BACKGROUND "               Shahrivar              " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                  Mehr                " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                  Aban                " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                  Azar                " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                  Dey                 " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                 Bahman               " RESET,
        BLACK_TEXT WHITE_BACKGROUND "                 Esfand               " RESET
};

/**
 * This function clears the screen by executing the "cls" command in the system.
 * This function is specific to Windows systems and uses the "system" function to execute the command.
 * After calling this function, the screen will be cleared and any previous output will be removed.
 */
void clearScreen()
{
    system("cls");
}

/**
 * A function that clears the input buffer by reading and discarding any remaining characters in the buffer
 until a newline character or the end of the file is encountered.
 * This function is useful for clearing any unwanted characters
 that may be left in the input buffer after a previous input operation.
 * It ensures that the input buffer is empty before proceeding with the next input operation.
 */
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * This function retrieves the current date and time from the system.
 * It uses the time() function from the C standard library to get the current time in seconds since the Epoch.
 * The time value is then passed to the localtime() function,
 which converts it to a struct tm representing the local date and time.
 * The struct tm is returned as the result.
 *
 * @return A struct tm representing the current date and time.
 */
struct tm getCurrentDateAndTime(){
    // Get the current time in seconds since the Epoch
    time_t t = time(NULL);
    // Convert the time to a struct tm representing the local date and time
    struct tm current_date = *localtime(&t);
    // Return the struct tm
    return current_date;
}

/**
 * This function reads keyboard input from the user.
 * It uses the Windows API function to handle input from the standard input (keyboard).
 * The function sets the console mode to enable processed input and mouse input.
 * It then enters a loop to continuously read input until a valid key is pressed.
 * Once a valid key is pressed, the function checks the key code and returns a corresponding value.
 *
 * @return An integer representing the keyboard input:
 *         - 0 if the Escape key is pressed (breaks out of the function)
 *         - 1 if the Left arrow key is pressed
 *         - 2 if the Right arrow key is pressed
 *         - 3 if the Up arrow key is pressed
 *         - 4 if the Down arrow key is pressed
 */
int readKeyboardInput()
{
    // Get the handle for the standard input (keyboard)
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs;
    DWORD InputsRead;
    INPUT_RECORD irInput;

    // Set the console mode to enable processed input and mouse input
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    while (1) {
        // Read input from the console
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

        if (InputsRead > 0)
        {
            if (irInput.EventType == KEY_EVENT && irInput.Event.KeyEvent.bKeyDown)
            {
                // Check the key code of the pressed key
                if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_LEFT)
                {
                    return 1; // Left arrow key pressed
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)
                {
                    return 2; // Right arrow key pressed
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_UP)
                {
                    return 3; // Up arrow key pressed
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
                {
                    return 4; // Down arrow key pressed
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
                {
                    return 0; // Escape key pressed
                    break;
                }
            }
        }
    }
}

/**
 * This function displays the calendar menu to the user.
 * It uses the printf() function to print out the menu options and information.
 * The menu includes options for returning to the main menu,
 as well as information about the valid range of calendar years and months.
 */
void calendarMenu()
{
    printf("\n%s", BLACK_TEXT WHITE_BACKGROUND "               Calendar Menu               \n" RESET);
    printf("\n%s Back to menu!\n",
           GRAY_TEXT "[0]" RESET);
    printf("\n%s Calendar Year is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1206" RESET,
           UNDERLINE GRAY_TEXT "1498" RESET);
    printf("\n%s Calendar Month is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1" RESET,
           UNDERLINE GRAY_TEXT "12" RESET);
    printf("\n%s\n", BLACK_TEXT WHITE_BACKGROUND "-------------------------------------------" RESET);
    // Prompt the user to enter a year
    printf("\nPlease enter a year: ");
}

/**
 * This function prompts the user to input a calendar year and validates the input.
 * It uses the calendarMenu() function to display the calendar menu to the user.
 * The function then uses scanf() to read the inputted year from the user.
 * If the user enters 0, indicating they want to go back to the main menu, the function returns 0.
 * If the inputted year is not within the valid range of 1206 to 1498, the function displays an error message,
 prompts the user to try again, and clears the input buffer and screen.
 * The function continues to loop until a valid year is inputted.
 * Once a valid year is inputted, the function returns the year.
 *
 * @return An integer representing the inputted calendar year, or 0 if the user wants to go back to the main menu.
 */
int inputCalendarYear(void)
{
    int year;

    do
    {
        // Display the calendar menu
        calendarMenu();
        // Read the inputted year
        scanf("%d", &year);

        if (year == 0)
        {
            // Return 0 to go back to the main menu
            return 0;
        }

        if (year < 1206 || year > 1498)
        {
            printf("\n%s.\n", RED_TEXT "Invalid year! Please enter a year between 1206 and 1498" RESET);
            printf("\nPress Enter to try again...");
            // Clear the input buffer
            clearInputBuffer();
            getchar();

            // Clear the screen
            clearScreen();
        }
    } while (year < 1206 || year > 1498);

    // Return the inputted year
    return year;
}

/**
 * This function prompts the user to input a calendar month and validates the input.
 * It takes the calendar year as a parameter to display the year information to the user in case of an invalid month input.
 * The function uses printf() to prompt the user to enter a month and scanf() to read the inputted month from the user.
 * If the user enters 0, indicating they want to go back to the main menu, the function returns 0.
 * If the inputted month is not within the valid range of 1 to 12, the function displays an error message,
 prompts the user to try again, and clears the input buffer and screen.
 * The function continues to loop until a valid month is inputted.
 * If an invalid month is entered,
 the function also calls the inputCalendarYear() function to allow the user to re-enter the year.
 * Once a valid month is inputted, the function returns the month.
 *
 * @param year The calendar year entered by the user.
 * @return An integer representing the inputted calendar month, or 0 if the user wants to go back to the main menu.
 */
int inputCalendarMonth(int year)
{
    int month;

    do
    {
        // Prompt the user to enter a month
        printf("Please enter a month: ");
        // Read the inputted month
        scanf("%d", &month);

        if (month == 0)
        {
            // Return 0 to go back to the main menu
            return 0;
        }

        if (month < 1 || month > 12)
        {
            printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
            printf("\nPress Enter to try again...");
            // Clear the input buffer
            clearInputBuffer();
            getchar();

            // Clear the screen
            clearScreen();

            // Re-enter the calendar year
            year = inputCalendarYear();

            if (year == 0)
            {
                // Return 0 to go back to the main menu
                return 0;
            }
        }
    } while (month < 1 || month > 12);

    // Return the inputted month
    return month;
}

/**
 * This function determines the daycode (day of the week) for a given calendar year and month.
 * It takes the calendar year and month as parameters.
 * The function calculates the daycode using a formula based on the inputted year and the number of days in each month.
 * If the inputted year falls within a specific range (1206 to 1218), the daycode is adjusted by adding 2.
 * If the inputted year is 1219, the daycode is adjusted by adding 2 and then taking the modulo 7.
 * If the inputted year falls within a different range (1220 to 1299), the daycode is adjusted by adding 1 and then taking the modulo 7.
 * Finally, the function iterates through each month before the inputted month
 and adjusts the daycode based on the number of days in each month.
 * The function returns the calculated daycode.
 *
 * @param year The calendar year.
 * @param month The calendar month.
 * @return An integer representing the daycode (day of the week) for the inputted year and month.
 */
int determineDaycode(int year, int month)
{
    // Calculate the daycode
    int daycode = (year * 365 + (year / 4) + 1) % 7;

    if (year >= 1206 && year <= 1218)
    {
        // Adjust daycode by adding 2
        daycode += 2;
    }

    if (year == 1219 || year == 1214 || year == 1213 || year == 1208)
    {
        // Adjust daycode by adding 2 and taking modulo 7
        daycode = (daycode + 2) % 7;
    }

    if (year >= 1220 && year < 1300)
    {
        // Adjust daycode by adding 1 and taking modulo 7
        daycode = (daycode + 1) % 7;
    }

    for (int i = 1; i < month; i++)
    {
        // Adjust daycode based on the number of days in each month
        daycode = (daycode + days_in_shamsi_month[i]) % 7;
    }

    // Return the calculated daycode
    return daycode;
}

/**
 * This function determines if a given year is a leap year or not.
 * It takes the calendar year as a parameter.
 * The function uses an if statement to check if the year is divisible by 400
 or if it is divisible by 4 but not divisible by 100.
 * If the condition is true, indicating that the year is a leap year, the function returns 1.
 * Otherwise, it returns 0, indicating that the year is not a leap year.
 *
 * @param year The calendar year.
 * @return 1 if the year is a leap year, 0 otherwise.
 */
int determineLeapYear(int year)
{
    if ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0)))
        return 1; // Year is a leap year
    else
        return 0; // Year is not a leap year
}

/**
 * This function displays the calendar for a given year and month.
 * It takes the calendar year, month, and daycode (day of the week) as parameters.
 * The function first determines if the inputted year is a leap year by calling the determineLeapYear() function.
 * If the year is a leap year, it sets the number of days in the 12th month (Esfand) to 29 in the days_in_shamsi_month array.
 * If the year is not a leap year, it sets the number of days in the 12th month to 30.
 * The function then prints the month name and the days of the week as column headers using printf().
 * It uses escape codes to set the text color to gray for the column headers.
 * The function then prints the calendar days, starting from the appropriate daycode.
 * It uses a loop to print empty spaces for the days before the start of the month.
 * It then prints each day number, using printf() with the %-6d format specifier for left-aligned printing.
 * If the day is the last day of the week or the last day of the month, a new line is printed.
 * Finally, the function prints a horizontal line with the calendar year, and prompts the user to press ESC to go back.
 *
 * @param year The calendar year.
 * @param month The calendar month.
 * @param daycode The daycode (day of the week) for the first day of the month.
 */
void calendar(int year, int month, int daycode)
{
    int day;

    if (determineLeapYear(year) == 0)
    {
        // Set the number of days in the 12th month to 30 for non-leap year
        days_in_shamsi_month[12] = 30;
    }
    else if (determineLeapYear(year) == 1)
    {
        // Set the number of days in the 12th month to 29 for leap year
        days_in_shamsi_month[12] = 29;
    }

    printf("\n%s\n\n%s    %s    %s    %s    %s    %s    %s\n\n",
            // Print the month name
           shamsiMonths[month],
            // Print the column headers with gray text color
           GRAY_TEXT "SH" RESET,
           GRAY_TEXT "YE" RESET,
           GRAY_TEXT "DO" RESET,
           GRAY_TEXT "SE" RESET,
           GRAY_TEXT "CH" RESET,
           GRAY_TEXT "PA" RESET,
           GRAY_TEXT "JO" RESET
    );

    for (day = 1; day <= daycode; day++)
    {
        // Print empty spaces for days before the start of the month
        printf("      ");
    }

    for (day = 1; day <= days_in_shamsi_month[month]; day++)
    {
        // Print each day number with left-aligned formatting
        printf("%-6d", day);

        if ((day + daycode) % 7 == 0 || day == days_in_shamsi_month[month])
            // Print a new line for the last day of the week or the last day of the month
            printf("\n");
    }

    printf("\n%s%d%s\n", BLACK_TEXT WHITE_BACKGROUND "---------------- ",
           year,
            // Print a horizontal line with the calendar year
           " ----------------" RESET);
    // Prompt the user to press ESC to go back
    printf("\nPress ESC to go back..."); 
}

/**
 * This function converts a given date in the Shamsi (Solar Hijri) calendar to the Gregorian calendar.
 * It takes the year, month, and day in the Shamsi calendar as parameters,
 as well as three pointers to integers to store the converted year, month, and day in the Gregorian calendar.
 * The function uses two arrays, sumShamsi[] and gregorianDays[],
 to represent the cumulative number of days in each month of the Shamsi calendar
 and the number of days in each month of the Gregorian calendar, respectively.
 * The function calculates the total number of days in the Shamsi calendar from the start of the year to the given date.
 * It then adds 79 days to account for the difference between the Shamsi and Gregorian calendars.
 * If the year in the Shamsi calendar is a leap year,
 the function adjusts the number of days in February in the Gregorian calendar to 29.
 * The function determines the year, month, and day in the Gregorian calendar by subtracting
 the appropriate number of days from the total, iterating through the gregorianDays[] array
 until the remaining day count is less than the number of days in the current month.
 * Finally, the function stores the converted year, month, and day in the provided pointers.
 *
 * @param y The year in the Shamsi calendar.
 * @param m The month in the Shamsi calendar.
 * @param d The day in the Shamsi calendar.
 * @param gYear Pointer to store the converted year in the Gregorian calendar.
 * @param gMonth Pointer to store the converted month in the Gregorian calendar.
 * @param gDay Pointer to store the converted day in the Gregorian calendar.
 */
void shamsiToGregorian(int y, int m, int d, int *gYear, int *gMonth, int *gDay)
{
    int sumShamsi[] = {31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365};
    int gregorianDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int yy, mm, dayCount;
    dayCount = d;
    if (m > 1)
        // Calculate the total number of days in the Shamsi calendar from the start of the year to the given date
        dayCount = dayCount + sumShamsi[m - 2];

    // Convert the year to the Gregorian calendar by adding 621
    yy = y + 621;
    // Add 79 days to account for the difference between the Shamsi and Gregorian calendars
    dayCount = dayCount + 79;

    if (determineLeapYear(yy) == 1)
    {
        if (dayCount > 366)
        {
            // Adjust the day count if it exceeds the number of days in a leap year
            dayCount = dayCount - 366;
            // Increment the year by 1
            yy = yy + 1;
        }
    }
    else if (dayCount > 365)
    {
        // Adjust the day count if it exceeds the number of days in a non-leap year
        dayCount = dayCount - 365;
        // Increment the year by 1
        yy = yy + 1;
    }

    if (determineLeapYear(yy) == 1)
        // Adjust the number of days in February if the year is a leap year
        gregorianDays[1] = 29;

    mm = 0;
    while (dayCount > gregorianDays[mm])
    {
        // Subtract the number of days in the current month
        dayCount = dayCount - gregorianDays[mm];
        // Increment the month by 1
        mm = mm + 1;
    }

    *gYear = yy; // Store the converted year in the provided pointer
    *gMonth = mm + 1; // Store the converted month in the provided pointer
    *gDay = dayCount; // Store the converted day in the provided pointer
}


/**
 * This function converts a given date in the Gregorian calendar to the Shamsi (Solar Hijri) calendar.
 * It takes the year, month, and day in the Gregorian calendar as parameters,
 as well as three pointers to integers to store the converted year, month, and day in the Shamsi calendar.
 * The function uses an array, countDays[], to represent the number of days in each month of the Gregorian calendar.
 * The function calculates the total number of days in the Gregorian calendar from the start of the year to the given date.
 * It adds 1 to the day count if the year is a leap year and the month is greater than February.
 * If the day count is less than or equal to 79, the function calculates the year,
 month, and day in the Shamsi calendar based on a special rule.
 * If the day count is greater than 79, the function calculates the year,
 month, and day in the Shamsi calendar based on another rule.
 * Finally, the function stores the converted year, month, and day in the provided pointers.
 *
 * @param year The year in the Gregorian calendar.
 * @param month The month in the Gregorian calendar.
 * @param day The day in the Gregorian calendar.
 * @param sYear Pointer to store the converted year in the Shamsi calendar.
 * @param sMonth Pointer to store the converted month in the Shamsi calendar.
 * @param sDay Pointer to store the converted day in the Shamsi calendar.
 */
void gregorianToShamsi(int year, int month, int day, int *sYear, int *sMonth, int *sDay)
{
    int countDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, dayYear;
    int newMonth, newYear, newDay;

    dayYear = 0;
    for (i = 1; i < month; i++)
    {
        // Calculate the total number of days in the Gregorian calendar from the start of the year to the given date
        dayYear = dayYear + countDays[i - 1];
    }
    dayYear = dayYear + day;

    if (determineLeapYear(year) && month > 2)
        // Add 1 to the day count if the year is a leap year and the month is greater than February
        dayYear = dayYear + 1;

    if (dayYear <= 79)
    {
        if ((year - 1) % 4 == 0)
        {
            /*
             * Calculate the day count based on the special rule for
             the Shamsi calendar when the day count is less than or equal to 79
             */
            dayYear = dayYear + 11;
        }
        else
        {
            dayYear = dayYear + 10;
        }

        // Calculate the year in the Shamsi calendar by subtracting 622
        newYear = year - 622;
        if (dayYear % 30 == 0)
        {
            // Calculate the month in the Shamsi calendar when the day count is divisible by 30
            newMonth = (dayYear / 30) + 9;
            newDay = 30;
        }
        else
        {
            // Calculate the month in the Shamsi calendar when the day count is not divisible by 30
            newMonth = (dayYear / 30) + 10;
            newDay = dayYear % 30;
        }
    }
    else
    {
        // Calculate the year in the Shamsi calendar by subtracting 621
        newYear = year - 621;
        dayYear = dayYear - 79;

        if (dayYear <= 186)
        {
            if (dayYear % 31 == 0)
            {
                // Calculate the month in the Shamsi calendar when the day count is divisible by 31
                newMonth = dayYear / 31;
                newDay = 31;
            }
            else
            {
                // Calculate the month in the Shamsi calendar when the day count is not divisible by 31
                newMonth = (dayYear / 31) + 1;
                newDay = dayYear % 31;
            }
        }
        else
        {
            dayYear = dayYear - 186;

            if (dayYear % 30 == 0)
            {
                // Calculate the month in the Shamsi calendar when the day count is divisible by 30
                newMonth = (dayYear / 30) + 7;
                newDay = 30;
            } else
            {
                // Calculate the month in the Shamsi calendar when the day count is notdivisible by 30
                newMonth = (dayYear / 30) + 8;
                newDay = dayYear % 30;
            }
        }
    }

    *sYear = newYear; // Store the converted year in the provided pointer
    *sMonth = newMonth; // Store the converted month in the provided pointer
    *sDay = newDay; // Store the converted day in the provided pointer
}

/**
 * This function returns the integer part of a given double value.
 * It takes a double value as a parameter and checks if it is less than 0.
 * If the value is less than 0, it subtracts 0.5 and converts it to an integer using type casting.
 * If the value is greater than or equal to 0, it adds 0.5 and converts it to an integer using type casting.
 * Finally, it returns the resulting integer value.
 *
 * @param value The double value to find the integer part of.
 * @return The integer part of the given double value.
 */
int intPart(double value)
{
    if (value < 0) {
        // Subtract 0.5 and convert to an integer if the value is less than 0
        return (int)(value - 0.5);
    } else {
        // Add 0.5 and convert to an integer if the value is greater than or equal to 0
        return (int)(value + 0.5);
    }
}

/**
 * This function converts a given date in the Gregorian calendar to the Lunar calendar.
 * It takes the year, month, and day in the Gregorian calendar as parameters, as well as three pointers to integers to store the converted year, month, and day in the Lunar calendar.
 * The function calculates the Julian date based on the given Gregorian date.
 * If the Gregorian date is on or after October 15, 1582 (the start of the Gregorian calendar), it uses a formula to calculate the Julian date.
 * If the Gregorian date is before October 15, 1582, it uses a different formula to calculate the Julian date.
 * The function then performs a series of calculations to determine the corresponding date in the Lunar calendar.
 * Finally, the function stores the converted year, month, and day in the provided pointers.
 *
 * @param year The year in the Gregorian calendar.
 * @param month The month in the Gregorian calendar.
 * @param day The day in the Gregorian calendar.
 * @param lYear Pointer to store the converted year in the Lunar calendar.
 * @param lMonth Pointer to store the converted month in the Lunar calendar.
 * @param lDay Pointer to store the converted day in the Lunar calendar.
 */
void gregorianToLunar(int year, int month, int day, int *lYear, int *lMonth, int *lDay)
{
    int juliandate;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
    {
        juliandate = intPart((1461 * (year + 4800 + intPart((month - 14) / 12))) / 4)
                     + intPart((367 * (month - 2 - 12 * intPart((month - 14) / 12))) / 12)
                     - intPart((3 * intPart((year + 4900 + intPart((month - 14) / 12)) / 100)) / 4)
                       // Calculate the Julian date using the Gregorian date formula
                     + day - 32075;
    }
    else
    {
        juliandate = 367 * year
                     - intPart((7 * (year + 5001 + intPart((month - 9) / 7))) / 4)
                     + intPart((275 * month) / 9)
                       // Calculate the Julian date using the pre-Gregorian date formula
                     + day + 1729777;
    }

    int l = juliandate - 1948440 + 10632;
    int n = intPart((l - 1) / 10631);
    l = l - 10631 * n + 354;

    int j = (intPart((10985 - l) / 5316)) * (intPart((50 * l) / 17719))
            + (intPart(l / 5670)) * (intPart((43 * l) / 15238));

    l = l - (intPart((30 - j) / 15)) * (intPart((17719 * j) / 50))
            // Perform calculations to determine the Lunar date
        - (intPart(j / 16)) * (intPart((15238 * j) / 43)) + 29;

    int lunarM = intPart((24 * l) / 709);
    int lunarD = l - intPart((709 * lunarM) / 24);
    int lunarY = 30 * n + j - 30;

    *lYear = lunarY; // Store the converted year in the provided pointer
    *lMonth = lunarM; // Store the converted month in the provided pointer
    *lDay = lunarD; // Store the converted day in the provided pointer
}

/**
 * This function displays the date conversion menu.
 * It prints the menu options for the user to select from.
 * The menu options include: going back to the main menu, converting from Shamsi (Solar Hijri) to Gregorian/Lunar, and converting from Gregorian to Shamsi/Lunar.
 * The function prompts the user to select an option.
 */
void dateConversionMenu()
{
    printf("\n%s\n%s Back to menu!\n\n%s Shamsi to Gregorian/Lunar\n\n%s Gregorian to Shamsi/Lunar"
           "\n\n%s\n\nselect option: ",
           BLACK_TEXT WHITE_BACKGROUND "          Date Conversion Menu          \n" RESET,
           GRAY_TEXT "[0]" RESET,
           GRAY_TEXT "[1]" RESET,
           GRAY_TEXT "[2]" RESET,
           BLACK_TEXT WHITE_BACKGROUND "----------------------------------------" RESET);
}

/**
 * This function converts a given date from one calendar system to another.
 * It takes the input date and the source and target calendar systems as parameters.
 * The function uses the appropriate conversion formulas and algorithms to calculate the equivalent date in the target calendar system.
 * It handles both forward and backward conversions, depending on the source and target calendars.
 * The function performs input validation to ensure that the input date is valid and within the range supported by the calendar systems.
 * It also checks for leap years and adjusts the number of days in February accordingly.
 * The function returns the converted date in the target calendar system.
 *
 * @param choice The user's choice for date conversion.
 * @param sYear The year in the Shamsi calendar.
 * @param sMonth The month in the Shamsi calendar.
 * @param sDay The day in the Shamsi calendar.
 * @param gYear The year in the Gregorian calendar.
 * @param gMonth The month in the Gregorian calendar.
 * @param gDay The day in the Gregorian calendar.
 * @param lYear The year for a leap date in the Gregorian calendar.
 * @param lMonth The month for a leap date in the Gregorian calendar.
 * @param lDay The day for a leap date in the Gregorian calendar.
 *
 * @return The converted date in the target calendar system.
 */
int dateConversion(void)
{
    int choice;
    int sYear, sMonth, sDay;
    int gYear, gMonth, gDay;
    int lYear, lMonth, lDay;

    do
    {
        // Clears the screen (assumed function)
        clearScreen();

        dateConversionMenu();

        // Check if the input for choice is not a valid integer
        if (scanf("%d", &choice) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            continue;
        }

        // Execute the corresponding case based on the choice
        switch (choice)
        {
            case 1:
                // Clear the screen and display the Shamsi to Gregorian conversion header
                clearScreen();
                printf("\n%s", BLACK_TEXT WHITE_BACKGROUND "           Shamsi >> Gregorian           \n" RESET);
                printf("\n%s Back to menu!\n",
                       GRAY_TEXT "[0]" RESET);
                printf("\n%s Shamsi Month is between %s and %s.\n",
                       GRAY_TEXT "{!}" RESET,
                       UNDERLINE GRAY_TEXT "1" RESET,
                       UNDERLINE GRAY_TEXT "12" RESET);
                printf("\n%s Shamsi Day is between %s and %s.\n",
                       GRAY_TEXT "{!}" RESET,
                       UNDERLINE GRAY_TEXT "1" RESET,
                       UNDERLINE GRAY_TEXT "31" RESET);
                printf("\n%s\n", BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
                printf("\nYear: ");

                // Check if the input for sYear is not a valid positive integer
                if (scanf("%d", &sYear) != 1 || sYear < 0)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid positive integer for the year." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                // Check if the input for sYear is 0, indicating the user wants to go back to the menu
                if (sYear == 0)
                {
                    return -1;
                }

                printf("Month: ");

                // Check if the input for sMonth is not a valid month between 1 and 12
                if (scanf("%d", &sMonth) != 1 || sMonth < 1 || sMonth > 12)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid month between 1 and 12." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                printf("Day: ");

                // Check if the input for sDay is not a valid day within the valid range for the given month
                if (scanf("%d", &sDay) != 1 || sDay < 1 || sDay > days_in_shamsi_month[sMonth])
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid day within the valid range." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                // Convert the Shamsi date to Gregorian and Lunar dates using the provided functions
                shamsiToGregorian(sYear, sMonth, sDay, &gYear, &gMonth, &gDay);
                gregorianToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);

                // Clear the screen and display the converted Gregorian and Lunar dates
                clearScreen();
                printf("\nConverted Gregorian date: %s%d/%02d/%02d%s\n",
                       ITALIC GRAY_TEXT,
                       gYear, gMonth, gDay,
                       RESET);
                printf("\nConverted Lunar date: %s%d/%02d/%02d%s\n",
                       ITALIC GRAY_TEXT,
                       lYear, lMonth, lDay,
                       RESET);
                printf("\nPress Enter to continue...");
                clearInputBuffer();
                getchar();
                break;

            case 2:
                // Clear the screen and display the Gregorian to Shamsi conversion header
                clearScreen();
                printf("\n%s", BLACK_TEXT WHITE_BACKGROUND "           Gregorian >> Shamsi           \n" RESET);
                printf("\n%s Back to menu!\n",
                       GRAY_TEXT "[0]" RESET);
                printf("\n%s Gregorian Month is between %s and %s.\n",
                       GRAY_TEXT "{!}" RESET,
                       UNDERLINE GRAY_TEXT "1" RESET,
                       UNDERLINE GRAY_TEXT "12" RESET);
                printf("\n%s Gregorian Day is between %s and %s.\n",
                       GRAY_TEXT "{!}" RESET,
                       UNDERLINE GRAY_TEXT "1" RESET,
                       UNDERLINE GRAY_TEXT "31" RESET);
                printf("\n%s\n", BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
                printf("\nYear: ");

                // Check if the input for sYear is not a valid positive integer
                if (scanf("%d", &gYear) != 1 || gYear < 0)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid positive integer for the year." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                // Check if the input for sYear is 0, indicating the user wants to go back to the menu
                if (gYear == 0)
                {
                    return -1;
                }

                printf("Month: ");

                // Check if the input for sMonth is not a valid month between 1 and 12
                if (scanf("%d", &gMonth) != 1 || gMonth < 1 || gMonth > 12)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid month between 1 and 12." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                printf("Day: ");

                // Check if the input for sDay is not a valid day within the valid range for the given month
                if (scanf("%d", &gDay) != 1 || gDay < 1 || gDay > days_in_shamsi_month[gMonth])
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid day within the valid range." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                // Convert the Gregorian date to Shamsi and Lunar dates using the provided functions
                gregorianToShamsi(gYear, gMonth, gDay, &sYear, &sMonth, &sDay);
                gregorianToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);
                clearScreen();
                printf("\nConverted Shamsi date: %s%d/%02d/%02d%s\n",
                       ITALIC GRAY_TEXT,
                       sYear, sMonth, sDay,
                       RESET);
                printf("\nConverted Lunar date: %s%d/%02d/%02d%s\n",
                       ITALIC GRAY_TEXT,
                       lYear, lMonth, lDay,
                       RESET);
                printf("\nPress Enter to continue...");
                clearInputBuffer();
                getchar();
                break;

                // Check if the input for choice is 0, indicating the user wants to go back to the menu.
            case 0:
                return -1;

            //For other cases, if "choice" is not 1, 2, 3, or 4, it displays an error message.
            default:
                printf("\n%s\n", RED_TEXT "Invalid choice!" RESET);
                printf("\nPress Enter to try again...");
                clearInputBuffer();
                getchar();
                break;
        }

    } while (choice != 0);

    return 0;
}

/**
 * This function displays the age menu, which provides information about the valid ranges for birth month and birth day.
 * It uses printf() to print the menu and formatting options for better visual presentation.
 * The menu includes options to go back to the main menu, as well as the valid ranges for birth month and birth day.
 * The function also includes a horizontal line separator for better readability.
 */
void ageMenu()
{
    // Display the title of the age menu in black text on a white background
    printf("\n%s", BLACK_TEXT WHITE_BACKGROUND "                Age Menu              \n" RESET);

    printf("\n%s Back to menu!\n",
            // Display the option to go back to the main menu, with gray text
           GRAY_TEXT "[0]" RESET);

    printf("\n%s Birth Month is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1" RESET,
            // Display the valid range for the birth month, with gray text and underlined values
           UNDERLINE GRAY_TEXT "12" RESET);

    printf("\n%s Birth Day is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1" RESET,
            // Display the valid range for the birthday, with gray text and underlined values
           UNDERLINE GRAY_TEXT "31" RESET);
    // Display a horizontal line separator in black text on a white background
    printf("\n%s\n", BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
}

/**
 * This function calculates the age of a person based on their birth date in the Shamsi calendar.
 * It takes the birth year, birth month, and birth day as parameters.
 * The function first gets the current date in the Gregorian calendar using the getCurrentDateAndTime() function.
 * It then converts the current date from the Gregorian calendar
 to the Shamsi calendar using the gregorianToShamsi() function.
 * The function checks if the input birth month is valid (between 1 and 12)
 and if the input birth day is within the valid range for the given birth month.
 * If any of the inputs are invalid, the function displays an error message and prompts the user to try again.
 * The function calculates the age in years, months, and days by subtracting the birth date from the current date.
 * If the number of days in the age is negative, it adjusts the months and days accordingly.
 * The function determines the day of the week of the birth date using the determineDaycode() function.
 * If the calculated age in years is negative, it displays an error message for an invalid birth year.
 * The function creates arrays containing the names of the days of the week in both the Shamsi and Gregorian calendars.
 * It then converts the birth date from the Shamsi calendar
 to the Gregorian calendar using the shamsiToGregorian() function.
 * The function calculates the number of seconds and days lived since the birth date using the difftime() function.
 * Finally, the function displays the age result, the day of the week of the birth date,
 the number of days lived since the birth date, and the Gregorian birth date.
 *
 * @param birth_year The year of birth in the Shamsi calendar.
 * @param birth_month The month of birth in the Shamsi calendar.
 * @param birth_day The day of birth in the Shamsi calendar.
 */
void calculateAge(int birth_year, int birth_month, int birth_day)
{
    // Clears the screen (assumed function)
    clearScreen();

    // Gets the current date and time
    struct tm current_date = getCurrentDateAndTime();

    int current_gregorian_year = current_date.tm_year + 1900;
    int current_gregorian_month = current_date.tm_mon;
    int current_gregorian_day = current_date.tm_mday;

    int current_year, current_month, current_day;

    // Converts the current date from Gregorian to Shamsi calendar
    gregorianToShamsi(current_gregorian_year, current_gregorian_month, current_gregorian_day,
                      &current_year, &current_month, &current_day);

    // Error handling for invalid birth month
    if (birth_month < 1 || birth_month > 12)
    {
        printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
        printf("\nPress Enter to try again...");
        clearInputBuffer();
        getchar();
        return;
    }

    // Error handling for invalid birth day
    if (birth_day < 1 || birth_day > days_in_shamsi_month[birth_month])
    {
        printf("\n%s.\n", RED_TEXT "Invalid day! Please enter a day within the valid range" RESET);
        printf("\nPress Enter to try again...");
        clearInputBuffer();
        getchar();
        return;
    }

    // Calculate the age in years, months, and days
    int age_years = current_year - birth_year;
    int age_months = current_month - birth_month;
    int age_days = current_day - birth_day;

    // Adjust the age if the current day is earlier than the birthday
    if (age_days < 0)
    {
        age_months--;
        age_days += days_in_shamsi_month[current_month];
    }

    // Adjust the age if the current month is earlier than the birth month
    if (age_months < 0)
    {
        age_years--;
        age_months += 12;
    }

    // Determine the day of the week for the birthdate
    int birthdate_daycode = determineDaycode(birth_year, birth_month);
    int day_of_week_index = (birthdate_daycode + birth_day - 1) % 7;

    // Error handling for invalid birth year
    if (age_years < 0)
    {
        clearScreen();
        printf("\n%s.\n\n", RED_TEXT "Invalid year! Please enter a year between 1 and 1402" RESET);
        return;
    }

    // Arrays for storing the names of days of the week in both Shamsi and Gregorian calendars
    char *days_of_week_shamsi[] = {"SHANBE", "YEKSHANBE", "DOSHANBE", "SESHANBE", "CHAHARSHANBE", "PANJESHANBE", "JOOMEH"};
    char *days_of_week_gregorian[] = {"SATURDAY", "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY"};

    // Convert the birthdate from Shamsi to Gregorian calendar
    int gYear, gMonth, gDay;
    shamsiToGregorian(birth_year, birth_month, birth_day, &gYear, &gMonth, &gDay);

    birth_year = gYear;
    birth_month = gMonth;
    birth_day = gDay;

    // Calculate the time lived in seconds and days since birth
    time_t birth_time = mktime(&(struct tm){0, 0, 0, birth_day, birth_month - 1, birth_year - 1900});
    time_t current_time = mktime(&current_date);

    int seconds_lived = difftime(current_time, birth_time);
    int days_lived = seconds_lived / (60 * 60 * 24);

    // Print the age result and other information
    printf("\n%s\n Your Age: %s%d years, %d months, %d days%s\n",
           BLACK_TEXT WHITE_BACKGROUND "                  Age Result                  \n" RESET,
           ITALIC GRAY_TEXT, age_years, age_months, age_days, RESET);
    printf("\n You were born on %s%s%s\n", ITALIC, GRAY_TEXT, days_of_week_shamsi[day_of_week_index]);
    printf("%s", RESET);
    printf("\n Number of days passed since your birth: %s%s%d%s\n", ITALIC, GRAY_TEXT, days_lived, RESET);
    printf("\n Gregorian birth date: %s%d/%02d/%02d",
           ITALIC GRAY_TEXT,
           gYear, gMonth, gDay);
    printf(" [%s%s%s]\n", ITALIC, GRAY_TEXT, days_of_week_gregorian[day_of_week_index]);
    printf("\n%s\n\n", BLACK_TEXT WHITE_BACKGROUND "----------------------------------------------" RESET);
}

/**
 * This function allows the user to enter their birth year, month, and day to calculate their age.
 * It uses a do-while loop to repeatedly prompt the user
 for input until a valid date is entered or the user chooses to exit.
 * The function clears the screen and displays the age menu before prompting for input.
 * It validates the user's input to ensure they enter valid integers for the birth year, month, and day.
 * If the user enters an invalid input, an error message is displayed and the user is prompted to try again.
 * If the user enters a birth year, month, or day of 0, the function returns 0, indicating the user has chosen to exit.
 * After the user enters a valid date, the function calls the calculateAge()
 function to calculate the age based on the birth date.
 * Once the age is calculated, the function prompts the user to press Enter
 to continue and waits for the user to press Enter.
 * The do-while loop continues until the user chooses to exit.
 *
 * @return The age of the user, or 0 if the user chooses to exit.
 */
int age(void)
{
    int birth_year, birth_month, birth_day;

    // Start a loop that continues until a valid input is received
    do
    {
        // Clear the console screen
        clearScreen();
        // Display the age menu
        ageMenu();

        // Prompt the user to enter the birth year
        printf("\nPlease enter the birth year: ");

        // Check if the input is a valid integer
        if (scanf("%d", &birth_year) != 1)
        {
            // Display an error message for invalid input
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            // Restart the loop to get a valid input
            continue;
        }

        // If the birth year is 0, exit the function and return 0
        if (birth_year == 0)
        {
            return 0;
        }

        // If the birth year is less than 1, display an error message
        if (birth_year < 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid year! Please enter a positive year." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            // Restart the loop to get a valid input
            continue;
        }

        // Prompt the user to enter the birth month
        printf("Please enter a month: ");

        // Check if the input is a valid integer
        if (scanf("%d", &birth_month) != 1)
        {
            // Display an error message for invalid input
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            // Restart the loop to get a valid input
            continue;
        }

        // If the birth month is 0, exit the function and return 0
        if (birth_month == 0)
        {
            return 0;
        }

        // If the birth month is not between 1 and 12, display an error message
        if (birth_month < 1 || birth_month > 12)
        {
            printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
            printf("\nPress Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            // Restart the loop to get a valid input
            continue;
        }

        // Prompt the user to enter the birthday
        printf("Please enter a day: ");

        // Check if the input is a valid integer
        if (scanf("%d", &birth_day) != 1)
        {
            // Display an error message for invalid input
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            // Restart the loop to get a valid input
            continue;
        }

        // If the birthday is 0, exit the function and return 0
        if (birth_day == 0)
        {
            return 0;
        }

        // Calculate the age based on the provided birth year, month, and day
        calculateAge(birth_year, birth_month, birth_day);

        // Prompt the user to press Enter to continue
        printf("Press Enter to continue...");
        clearInputBuffer();
        getchar();

    } while (1); // Loop indefinitely until a valid input is obtained
}

/**
 * This function displays the current date in three different calendars: Shamsi, Gregorian, and Lunar.
 * It retrieves the current date and time using the getCurrentDateAndTime() function, which returns a struct tm object.
 * It then extracts the year, month, and day from the struct tm object representing the current date in the Gregorian calendar.
 * The function calls the gregorianToShamsi() function to convert the Gregorian date to the Shamsi date, storing the result in the variables current_year, current_month, and current_day.
 * The function also calls the gregorianToLunar() function to convert the Gregorian date to the Lunar date, storing the result in the variables lYear, lMonth, and lDay.
 * Finally, the function prints the current dates in the Shamsi, Gregorian, and Lunar calendars using printf statements.
 * It also prompts the user to press Enter to go back to the menu and waits for user input.
 */
void currentDate()
{
    int lYear, lMonth, lDay;

    // Get the current date and time
    struct tm current_date = getCurrentDateAndTime();

    // Extract the current year in the Gregorian calendar
    int current_gregorian_year = current_date.tm_year + 1900;
    // Extract the current month in the Gregorian calendar
    int current_gregorian_month = current_date.tm_mon;
    // Extract the current day in the Gregorian calendar
    int current_gregorian_day = current_date.tm_mday;

    int current_year, current_month, current_day;
    // Convert the Gregorian date to the Shamsi date
    gregorianToShamsi(current_gregorian_year, current_gregorian_month, current_gregorian_day,
                      &current_year, &current_month, &current_day);

    // Convert the Gregorian date to the Lunar date
    gregorianToLunar(current_gregorian_year, current_gregorian_month, current_gregorian_day, &lYear, &lMonth, &lDay);

    printf("\n%s\n\n",
           BLACK_TEXT WHITE_BACKGROUND "                 Dates                " RESET);
    // Print the current Shamsi date
    printf("   Current Shamsi Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, current_year, current_month, current_day ,RESET);
    // Print the current Gregorian date
    printf("\n   Current Gregorian Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, current_gregorian_year, current_gregorian_month, current_gregorian_day , RESET);
    // Print the current Lunar date
    printf("\n   Current Lunar Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, lYear, lMonth, lDay ,RESET);
    printf("\n%s\n\n",
           BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
    printf("Press Enter to go back to menu...");
    clearInputBuffer();
    // Wait for user input
    getchar();
}

/**
 * The main function of the program. It serves as the entry point for the program execution.
 * It displays a menu to the user and takes their input to perform various operations.
 * The function uses a do-while loop to continuously display the menu until the user chooses to quit.
 * It also handles input validation to ensure that the user enters valid choices and integers.
 * The function calls other functions based on the user's choice to perform different operations like calendar, age calculation, date conversion, and displaying the current date.
 * After each operation, the function waits for the user to press Enter before displaying the menu again.
 * The function uses a switch statement to execute different code blocks based on the user's choice.
 * It also clears the screen before displaying the menu to provide a clean user interface.
 * If the user enters an invalid choice or invalid input, appropriate error messages are displayed and the function prompts the user to try again.
 * The function continues to display the menu until the user chooses to quit.
 *
 * @return 0 indicating successful program execution.
 */
int main(void)
{
    int choice;
    int shamsi_year, shamsi_month, shamsi_daycode;

    do
    {
        // Clear the console screen
        clearScreen();

        // Display the main menu.
        printf("\n%s\n%s Quit\n\n%s Calendar\n\n%s Age Calculation\n\n%s Date Conversion\n\n%s Current Date\n\n%s\n\n%s",
               BLACK_TEXT WHITE_BACKGROUND "                 Menu                 \n" RESET,
               GRAY_TEXT "[0]" RESET,
               GRAY_TEXT "[1]" RESET,
               GRAY_TEXT "[2]" RESET,
               GRAY_TEXT "[3]" RESET,
               GRAY_TEXT "[4]" RESET,
               BLACK_TEXT WHITE_BACKGROUND "---------------------------------------" RESET,
               RESET);
        printf(" select option: ");

        // Check if the input is a valid integer.
        if (scanf("%d", &choice) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        switch (choice)
        {
            case 0:
                /**
                 * The user has selected option 0, which is to exit the program.
                 * Clear the screen, display a goodbye message, and sleep for 3 seconds before exiting.
                 */
                clearScreen();
                printf("Bye!\n");
                sleep(3);
                break;

            case 1:
                /**
                 * The user has selected option 1, which is to view the calendar.
                 * Enter a loop to continuously display the calendar until the user chooses to go back.
                 */
                do {
                    clearScreen();
                    shamsi_year = inputCalendarYear();

                    if (shamsi_year == 0)
                    {
                        break;
                    }

                    shamsi_month = inputCalendarMonth(shamsi_year);

                    if (shamsi_month == 0)
                    {
                        break;
                    }

                    shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                    determineLeapYear(shamsi_year);

                    int arrowResult;
                    do {
                        clearScreen();
                        calendar(shamsi_year, shamsi_month, shamsi_daycode);
                        printf("\n");

                        arrowResult = readKeyboardInput();

                        if (arrowResult == 1)
                        {
                            if (shamsi_year >= 1206)
                            {
                                shamsi_month--;
                            }
                            if (shamsi_month < 1)
                            {
                                shamsi_month = 12;
                                shamsi_year--;
                            }
                            if (shamsi_year == 1205)
                            {
                                shamsi_year++;
                            }
                            shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                        }
                        else if (arrowResult == 2)
                        {
                            if (shamsi_year <= 1498)
                            {
                                shamsi_month++;
                            }
                            if (shamsi_month > 12)
                            {
                                shamsi_month = 1;
                                shamsi_year++;
                            }
                            if (shamsi_year == 1499)
                            {
                                shamsi_year--;
                            }
                            shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                        }
                        else if (arrowResult == 3)
                        {
                            if (shamsi_year == 1498)
                            {
                                shamsi_year--;
                            }
                            shamsi_year++;
                            shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                        }
                        else if (arrowResult == 4)
                        {
                            if (shamsi_year >= 1206)
                            {
                                shamsi_year--;
                            }
                            shamsi_daycode = determineDaycode(shamsi_year, shamsi_month);
                        }
                    } while (arrowResult != 0);

                } while (1);
                break;

            case 2:
                clearScreen();
                age();
                break;

            case 3:
                clearScreen();
                dateConversion();
                break;

            case 4:
                clearScreen();
                currentDate();
                break;

            default:
                if (choice != 0 && choice != 1 && choice != 2 && choice != 3)
                {
                    printf("\n%s\n", RED_TEXT "Invalid choice!" RESET);
                    printf("\ndPress Enter to try again...");
                    clearInputBuffer();
                    getchar();
                }
                break;
        }

    } while (choice != 0);

    return 0;
}