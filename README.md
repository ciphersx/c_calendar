# Calendar and Date Conversion Tool

This program is a command-line tool written in C that serves as a calendar and date conversion utility. It provides features to display the Shamsi (Iranian) calendar, calculate age based on the Shamsi calendar, and convert dates between Shamsi, Gregorian, and Lunar calendars.

## Features

1. **Shamsi Calendar Display:** View the Shamsi calendar for a specific year and month.
2. **Age Calculation:** Calculate age based on the Shamsi calendar, providing the birthdate.
3. **Date Conversion:** Convert dates between Shamsi, Gregorian, and Lunar calendars.
4. **Current Date Display:** View the current date in Shamsi, Gregorian, and Lunar calendars.

## How to Use

1. **Compile the Code:**
    - Ensure you have a C compiler installed (e.g., GCC).
    - Compile the code using the command: `gcc yourfilename.c -o calendar_tool`.

2. **Run the Program:**
    - Execute the compiled program: `./calendar_tool`.

3. **Navigate the Menu:**
    - Use the numeric keys to navigate through the menu options.
    - Follow on-screen prompts to input required information.

## Menu Options

- **Calendar (Option 1):** Display the Shamsi calendar for a specific year and month.
- **Age Calculation (Option 2):** Calculate age based on the Shamsi calendar by providing the birthdate.
- **Date Conversion (Option 3):** Convert dates between Shamsi, Gregorian, and Lunar calendars.
- **Current Date (Option 4):** View the current date in Shamsi, Gregorian, and Lunar calendars.
- **Quit (Option 0):** Exit the program.

## Additional Notes

- **Input Validation:** The program includes input validation to ensure the user enters valid input for years, months, and days.
- **Calendar Navigation Methods:** The program now supports navigation using arrow keys for a more intuitive user experience. Use the following keys:
   - **Right Arrow (→):** Navigate to the next Month.
   - **Left Arrow (←):** Navigate to the previous Month.
   - **Up Arrow (↑):** Navigate to the next Year.
   - **Down Arrow (↓):** Navigate to the previous Year.
   - **ESC:** EXIT the calendar.
- **Clear Screen Function:** The program uses a function to clear the screen (`system("cls")`), specifically designed for Windows systems. If you are using a different operating system, such as Linux or macOS, the clear screen function may not work as intended. In such cases, you might see the screen cluttered with previous outputs. Consider using platform-specific commands (e.g., `system("clear")` for Unix-based systems) or rely on alternative methods to improve the user interface.

## Dependencies

- The program uses standard C libraries and functions.

## Acknowledgments

The code includes comments for better understanding. Feel free to explore and modify the code as needed. If you encounter any issues or have suggestions, please contribute to the project.

**Happy coding!**