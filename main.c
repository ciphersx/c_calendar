#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

#define BLACK_TEXT "\x1b[30m"
#define RED_TEXT "\x1b[31m"
#define GRAY_TEXT "\x1b[90m"
#define WHITE_BACKGROUND "\x1b[47m"
#define UNDERLINE "\x1b[4m"
#define ITALIC "\x1b[3m"
#define RESET "\x1b[0m"

int days_in_shamsi_month[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29, 29};
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

void clearScreen()
{
    system("cls");
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

struct tm getCurrentDateAndTime(){
    time_t t = time(NULL);
    struct tm current_date = *localtime(&t);
    return current_date;
}

int readArrowKeys()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs;
    DWORD InputsRead;
    INPUT_RECORD irInput;


    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    while (1) {
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

        if (InputsRead > 0)
        {
            if (irInput.EventType == KEY_EVENT &&
                irInput.Event.KeyEvent.bKeyDown)
            {
                if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_LEFT)
                {
                    return 1;
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)
                {
                    return 2;
                }
            }
        }
    }
}

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
    printf("\nPlease enter a year: ");
}

int inputCalendarYear(void)
{
    int year;

    do
    {
        calendarMenu();
        scanf("%d", &year);

        if (year == 0)
        {
            return 0;
        }

        if (year < 1206 || year > 1498)
        {
            printf("\n%s.\n", RED_TEXT "Invalid year! Please enter a year between 1206 and 1498" RESET);
            printf("\nPress Enter to try again...");
            clearInputBuffer();
            getchar();

            clearScreen();
        }
    } while (year < 1206 || year > 1498);

    return year;
}

int inputCalendarMonth(int year)
{
    int month;

    do
    {
        printf("Please enter a month: ");
        scanf("%d", &month);

        if (month == 0)
        {
            return 0;
        }

        if (month < 1 || month > 12)
        {
            printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
            printf("\nPress Enter to try again...");
            clearInputBuffer();
            getchar();

            clearScreen();

            year = inputCalendarYear();

            if (year == 0)
            {
                return 0;
            }
        }
    } while (month < 1 || month > 12);

    return month;
}

int determineDaycode(int year, int month)
{
    int daycode = (year * 365 + (year / 4) + 1) % 7;

    if (year >= 1206 && year <= 1218)
    {
        daycode += 2;
    }

    if (year == 1219)
    {
        daycode = (daycode + 2) % 7;
    }

    if (year >= 1220 && year < 1300)
    {
        daycode = (daycode + 1) % 7;
    }

    for (int i = 1; i < month; i++)
    {
        daycode = (daycode + days_in_shamsi_month[i]) % 7;
    }

    return daycode;
}

int determineLeapYear(int year)
{
    if ((year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0)))
        return 1;
    else
        return 0;
}

void calendar(int year, int month, int daycode)
{
    int day;

    if (determineLeapYear(year) == 0)
    {
        days_in_shamsi_month[12] = 30;
    }
    else if (determineLeapYear(year) == 1)
    {
        days_in_shamsi_month[12] = 29;
    }

    printf("\n%s\n\n%s    %s    %s    %s    %s    %s    %s\n\n",
           shamsiMonths[month],
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
        printf("      ");
    }

    for (day = 1; day <= days_in_shamsi_month[month]; day++)
    {
        printf("%-6d", day);

        if ((day + daycode) % 7 == 0 || day == days_in_shamsi_month[month])
            printf("\n");
    }

    printf("\n%s%d%s\n", BLACK_TEXT WHITE_BACKGROUND "---------------- ",
           year,
           " ----------------" RESET);
}

void shamsiToGregorian(int y, int m, int d, int *gYear, int *gMonth, int *gDay)
{
    int sumShamsi[] = {31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365};
    int gregorianDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int yy, mm, dayCount;
    dayCount = d;
    if (m > 1)
        dayCount = dayCount + sumShamsi[m - 2];
    yy = y + 621;
    dayCount = dayCount + 79;

    if (determineLeapYear(yy) == 1)
    {
        if (dayCount > 366)
        {
            dayCount = dayCount - 366;
            yy = yy + 1;
        }
    } else if (dayCount > 365)
    {
        dayCount = dayCount - 365;
        yy = yy + 1;
    }

    if (determineLeapYear(yy) == 1)
        gregorianDays[1] = 29;

    mm = 0;
    while (dayCount > gregorianDays[mm])
    {
        dayCount = dayCount - gregorianDays[mm];
        mm = mm + 1;
    }

    *gYear = yy;
    *gMonth = mm + 1;
    *gDay = dayCount;
}

void gregorianToShamsi(int year, int month, int day, int *sYear, int *sMonth, int *sDay)
{
    int countDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, dayYear;
    int newMonth, newYear, newDay;

    dayYear = 0;
    for (i = 1; i < month; i++)
    {
        dayYear = dayYear + countDays[i - 1];
    }
    dayYear = dayYear + day;

    if (determineLeapYear(year) && month > 2)
        dayYear = dayYear + 1;

    if (dayYear <= 79)
    {
        if ((year - 1) % 4 == 0)
        {
            dayYear = dayYear + 11;
        }
        else
        {
            dayYear = dayYear + 10;
        }

        newYear = year - 622;
        if (dayYear % 30 == 0)
        {
            newMonth = (dayYear / 30) + 9;
            newDay = 30;
        }
        else
        {
            newMonth = (dayYear / 30) + 10;
            newDay = dayYear % 30;
        }
    }
    else
    {
        newYear = year - 621;
        dayYear = dayYear - 79;

        if (dayYear <= 186)
        {
            if (dayYear % 31 == 0)
            {
                newMonth = dayYear / 31;
                newDay = 31;
            }
            else
            {
                newMonth = (dayYear / 31) + 1;
                newDay = dayYear % 31;
            }
        }
        else
        {
            dayYear = dayYear - 186;

            if (dayYear % 30 == 0)
            {
                newMonth = (dayYear / 30) + 7;
                newDay = 30;
            } else
            {
                newMonth = (dayYear / 30) + 8;
                newDay = dayYear % 30;
            }
        }
    }

    *sYear = newYear;
    *sMonth = newMonth;
    *sDay = newDay;
}

int intPart(double value)
{
    if (value < 0) {
        return (int)(value - 0.5);
    } else {
        return (int)(value + 0.5);
    }
}

void gregorianToLunar(int year, int month, int day, int *lYear, int *lMonth, int *lDay)
{
    int juliandate;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
    {
        juliandate = intPart((1461 * (year + 4800 + intPart((month - 14) / 12))) / 4)
                     + intPart((367 * (month - 2 - 12 * intPart((month - 14) / 12))) / 12)
                     - intPart((3 * intPart((year + 4900 + intPart((month - 14) / 12)) / 100)) / 4)
                     + day - 32075;
    }
    else
    {
        juliandate = 367 * year
                     - intPart((7 * (year + 5001 + intPart((month - 9) / 7))) / 4)
                     + intPart((275 * month) / 9)
                     + day + 1729777;
    }

    int l = juliandate - 1948440 + 10632;
    int n = intPart((l - 1) / 10631);
    l = l - 10631 * n + 354;

    int j = (intPart((10985 - l) / 5316)) * (intPart((50 * l) / 17719))
            + (intPart(l / 5670)) * (intPart((43 * l) / 15238));

    l = l - (intPart((30 - j) / 15)) * (intPart((17719 * j) / 50))
        - (intPart(j / 16)) * (intPart((15238 * j) / 43)) + 29;

    int lunarM = intPart((24 * l) / 709);
    int lunarD = l - intPart((709 * lunarM) / 24);
    int lunarY = 30 * n + j - 30;

    *lYear = lunarY;
    *lMonth = lunarM;
    *lDay = lunarD;
}

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

int dateConversion(void)
{
    int choice;
    int sYear, sMonth, sDay;
    int gYear, gMonth, gDay;
    int lYear, lMonth, lDay;

    do
    {
        clearScreen();

        dateConversionMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
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

                if (scanf("%d", &sYear) != 1 || sYear < 0)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid positive integer for the year." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                if (sYear == 0)
                {
                    return -1;
                }

                printf("Month: ");
                if (scanf("%d", &sMonth) != 1 || sMonth < 1 || sMonth > 12)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid month between 1 and 12." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                printf("Day: ");

                if (scanf("%d", &sDay) != 1 || sDay < 1 || sDay > days_in_shamsi_month[sMonth])
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid day within the valid range." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                shamsiToGregorian(sYear, sMonth, sDay, &gYear, &gMonth, &gDay);
                gregorianToLunar(gYear, gMonth, gDay, &lYear, &lMonth, &lDay);
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

                if (scanf("%d", &gYear) != 1 || gYear < 0)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid positive integer for the year." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                if (gYear == 0)
                {
                    return -1;
                }

                printf("Month: ");

                if (scanf("%d", &gMonth) != 1 || gMonth < 1 || gMonth > 12)
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid month between 1 and 12." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

                printf("Day: ");

                if (scanf("%d", &gDay) != 1 || gDay < 1 || gDay > days_in_shamsi_month[gMonth])
                {
                    printf("\n%s\n", RED_TEXT "Invalid input! Please enter a valid day within the valid range." RESET);
                    printf("Press Enter to continue...");
                    clearInputBuffer();
                    getchar();
                    break;
                }

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

            case 0:
                return -1;

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

void ageMenu()
{
    printf("\n%s", BLACK_TEXT WHITE_BACKGROUND "                Age Menu              \n" RESET);
    printf("\n%s Back to menu!\n",
           GRAY_TEXT "[0]" RESET);
    printf("\n%s Birth Month is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1" RESET,
           UNDERLINE GRAY_TEXT "12" RESET);
    printf("\n%s Birth Day is between %s and %s.\n",
           GRAY_TEXT "{!}" RESET,
           UNDERLINE GRAY_TEXT "1" RESET,
           UNDERLINE GRAY_TEXT "31" RESET);
    printf("\n%s\n", BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
}

void calculateAge(int birth_year, int birth_month, int birth_day)
{
    clearScreen();

    struct tm current_date = getCurrentDateAndTime();

    int current_gregorian_year = current_date.tm_year + 1900;
    int current_gregorian_month = current_date.tm_mon;
    int current_gregorian_day = current_date.tm_mday;

    int current_year, current_month, current_day;
    gregorianToShamsi(current_gregorian_year, current_gregorian_month, current_gregorian_day,
                      &current_year, &current_month, &current_day);

    if (birth_month < 1 || birth_month > 12)
    {
        printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
        printf("\nPress Enter to try again...");
        clearInputBuffer();
        getchar();
        return;
    }

    if (birth_day < 1 || birth_day > days_in_shamsi_month[birth_month])
    {
        printf("\n%s.\n", RED_TEXT "Invalid day! Please enter a day within the valid range" RESET);
        printf("\nPress Enter to try again...");
        clearInputBuffer();
        getchar();
        return;
    }

    int age_years = current_year - birth_year;
    int age_months = current_month - birth_month;
    int age_days = current_day - birth_day;

    if (age_days < 0)
    {
        age_months--;
        age_days += days_in_shamsi_month[current_month];
    }

    if (age_months < 0)
    {
        age_years--;
        age_months += 12;
    }

    int birthdate_daycode = determineDaycode(birth_year, birth_month);
    int day_of_week_index = (birthdate_daycode + birth_day - 1) % 7;

    if (age_years < 0)
    {
        clearScreen();
        printf("\n%s.\n\n", RED_TEXT "Invalid year! Please enter a year between 1 and 1402" RESET);
        return;
    }

    char *days_of_week_shamsi[] = {"SHANBE", "YEKSHANBE", "DOSHANBE", "SESHANBE", "CHAHARSHANBE", "PANJESHANBE", "JOOMEH"};
    char *days_of_week_gregorian[] = {"SATURDAY", "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY"};

    int gYear, gMonth, gDay;
    shamsiToGregorian(birth_year, birth_month, birth_day, &gYear, &gMonth, &gDay);

    birth_year = gYear;
    birth_month = gMonth;
    birth_day = gDay;

    time_t birth_time = mktime(&(struct tm){0, 0, 0, birth_day, birth_month - 1, birth_year - 1900});
    time_t current_time = mktime(&current_date);

    int seconds_lived = difftime(current_time, birth_time);
    int days_lived = seconds_lived / (60 * 60 * 24);

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

int age(void)
{
    int birth_year, birth_month, birth_day;

    do
    {
        clearScreen();
        ageMenu();
        printf("\nPlease enter the birth year: ");
        if (scanf("%d", &birth_year) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        if (birth_year == 0)
        {
            return 0;
        }

        if (birth_year < 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid year! Please enter a positive year." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        printf("Please enter a month: ");
        if (scanf("%d", &birth_month) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        if (birth_month == 0)
        {
            return 0;
        }

        if (birth_month < 1 || birth_month > 12)
        {
            printf("\n%s.\n", RED_TEXT "Invalid month! Please enter a month between 1 and 12" RESET);
            printf("\nPress Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        printf("Please enter a day: ");
        if (scanf("%d", &birth_day) != 1)
        {
            printf("\n%s.\n", RED_TEXT "Invalid input! Please enter a valid integer." RESET);
            printf("Press Enter to try again...");
            clearInputBuffer();
            getchar();
            clearScreen();
            continue;
        }

        if (birth_day == 0)
        {
            return 0;
        }

        calculateAge(birth_year, birth_month, birth_day);

        printf("Press Enter to continue...");
        clearInputBuffer();
        getchar();

    } while (1);

}

void currentDate()
{
    int lYear, lMonth, lDay;

    struct tm current_date = getCurrentDateAndTime();

    int current_gregorian_year = current_date.tm_year + 1900;
    int current_gregorian_month = current_date.tm_mon;
    int current_gregorian_day = current_date.tm_mday;

    int current_year, current_month, current_day;
    gregorianToShamsi(current_gregorian_year, current_gregorian_month, current_gregorian_day,
                      &current_year, &current_month, &current_day);
    gregorianToLunar(current_gregorian_year, current_gregorian_month, current_gregorian_day, &lYear, &lMonth, &lDay);

    printf("\n%s\n\n",
           BLACK_TEXT WHITE_BACKGROUND "                 Dates                " RESET);
    printf("   Current Shamsi Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, current_year, current_month, current_day ,RESET);
    printf("\n   Current Gregorian Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, current_gregorian_year, current_gregorian_month, current_gregorian_day , RESET);
    printf("\n   Current Lunar Date: %s %d/%d/%d %s\n",
           ITALIC GRAY_TEXT, lYear, lMonth, lDay ,RESET);
    printf("\n%s\n\n",
           BLACK_TEXT WHITE_BACKGROUND "--------------------------------------" RESET);
    printf("Press Enter to go back to menu...");
    clearInputBuffer();
    getchar();
}

int main(void)
{
    int choice;
    int shamsi_year, shamsi_month, shamsi_daycode;

    do
    {
        clearScreen();

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
                clearScreen();
                printf("Bye!\n");
                sleep(3);
                break;

            case 1:
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

                    clearScreen();
                    calendar(shamsi_year, shamsi_month, shamsi_daycode);
                    printf("\n");

                    printf("Press Enter to continue...");

                    clearInputBuffer();
                    getchar();
                }while (1);
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