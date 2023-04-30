#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>

using namespace std;

// ******Console boilerplate ******* //
// required for font colors
#define CONSO GetConsoleScreenBufferInfo
#define GREEN 10
#define CYAN 11
#define RED 12
#define MAGENTA 13
#define YELLOW 14
#define WHITE 15
void *h = ::GetStdHandle((DWORD)-11); // gets info about of output (console) screen
CONSOLE_SCREEN_BUFFER_INFO csbInfo; // structure to store screen information
int attr = CONSO(h, &csbInfo); // to get screen dimensions
void setTextColor(int color)
{
    SetConsoleTextAttribute(h, color);
}
void setCursor(int row, int col)
{
    COORD coord = {col, row}; // structure that store column and row number of the cursor
    SetConsoleCursorPosition(h, coord); // sets the cursor to coordinates passed
}

/*******************data structure Class and methods*****************************/
struct date
{
    int date, month, year;
};

// A ordered doubly linked list node
class notes
{
public:
    date dt;
    string info;
    notes *prev;
    notes *next;

    void getDefault(int dd, int mm, int yy, string txt)
    {
        dt.date = dd;
        dt.month = mm;
        dt.year = yy;
        info = txt;
    }
};

// check date, dt1 <= dt2
bool compareAsc(notes d1, notes d2) // d1 <= d2
{

    if (d1.dt.year < d2.dt.year || d1.dt.year == d2.dt.year && d1.dt.month < d2.dt.month || d1.dt.year == d2.dt.year && d1.dt.month == d2.dt.month && d1.dt.date < d2.dt.date)
        return true;

    // If none of the above cases satisfy, return false
    return false;
}

// check date, dt1 >= dt2
bool compareDesc(notes d1, notes d2) // d1 >= d2
{
    if (d1.dt.year > d2.dt.year || d1.dt.year == d2.dt.year && d1.dt.month > d2.dt.month || d1.dt.year == d2.dt.year && d1.dt.month == d2.dt.month && d1.dt.date > d2.dt.date)
        return true;

    // If none of the above cases satisfy, return false
    return false;
}

// function to check two dates are same
bool dateSame(date dt1, date dt2) // d1 == d2
{
    if (dt1.date == dt2.date && dt1.month == dt2.month && dt1.year == dt2.year)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Function to sort and add new notes
void addAppoinment(notes **head, notes **tail, notes newDate)
{

    notes *p = new notes();
    p->dt.date = newDate.dt.date;
    p->dt.month = newDate.dt.month;
    p->dt.year = newDate.dt.year;
    p->info = newDate.info;
    p->next = NULL;

    // If first node to be inserted in doubly linked list
    if ((*head) == NULL)
    {
        (*head) = p;
        (*tail) = p;
        (*head)->prev = NULL;
        return;
    }

    // If node to be inserted has value less than first node
    // compareASC (new.d1 < head.dt)
    if (compareAsc(*p, **head))
    {
        p->prev = NULL;
        (*head)->prev = p;
        p->next = (*head);
        (*head) = p;
        return;
    }

    // If node to be inserted has value more than first node
    // compareDesc (new.d1 > tail.dt)
    if (compareDesc(*p, **tail))
    {
        p->prev = (*tail);
        (*tail)->next = p;
        (*tail) = p;
        return;
    }

    // Find the node before which we need to insert p.
    notes *temp = (*head)->next;
    while (compareAsc(*temp, *p))
        temp = temp->next;

    // Insert new node before temp
    (temp->prev)->next = p;
    p->prev = temp->prev;
    temp->prev = p;
    p->next = temp;
}

// function to delete a note
void deleteAppointment(notes **head, date delNode)
{
    notes *temp, *dump;
    temp = *head;
    if (temp != NULL && dateSame(temp->dt, delNode)) // if the delNode is head itself
    {
        setCursor(15, 15);
        cout << "Deleting Record";
        setCursor(16, 15);
        setTextColor(YELLOW);
        cout << "Date: " << temp->dt.date << "-" << temp->dt.month << "-" << temp->dt.year;
        setCursor(17, 15);
        cout << "Note: " << temp->info;
        setCursor(19, 15);
        setTextColor(GREEN);
        cout << "Record deleted successfully!";
        setTextColor(WHITE);
        *head = temp->next; // head pointer is getting updated
        delete temp;
        return;
    }
    // Finding the key to be deleted
    while (temp != NULL && !dateSame(temp->dt, delNode))
    {
        dump = temp;
        temp = temp->next;
    }
    // If the key is not present
    if (temp == NULL)
    {
        setCursor(15, 15);
        cout << "No records found on Date: ";
        setTextColor(YELLOW);
        cout << delNode.date << "-" << delNode.month << "-" << delNode.year;
        setCursor(17, 15);
        setTextColor(RED);
        cout << "Unable to delete record!";
        setTextColor(WHITE);
        return;
    }
    // node found, delete the node
    setCursor(15, 15);
    cout << "Deleting Record";
    setCursor(16, 15);
    setTextColor(YELLOW);
    cout << "Date: " << temp->dt.date << "-" << temp->dt.month << "-" << temp->dt.year;
    setCursor(17, 15);
    cout << "Note: " << temp->info;
    setCursor(19, 15);
    setTextColor(GREEN);
    cout << "Record deleted successfully!";
    setTextColor(WHITE);
    
    // Remove the node
    dump->next = temp->next;

    delete temp;
}

// funtion to update a note
void updateAppointment(notes **head, notes upNote)
{
    notes *temp, *dump;
    temp = *head;
    while (temp != NULL && !dateSame(temp->dt, upNote.dt))
    {
        dump = temp;
        temp = temp->next;
    }
    // If the key is not present
    if (temp == NULL)
    {
        setCursor(15, 15);
        cout << "No records found on Date: ";
        setTextColor(YELLOW);
        cout << upNote.dt.date << "-" << upNote.dt.month << "-" << upNote.dt.year;
        setCursor(17, 15);
        setTextColor(RED);
        cout << "Unable to update record!";
        setTextColor(WHITE);
        return;
    }
    // if the key is present
    setCursor(15, 15);
    cout << "Updating Record";
    setCursor(16, 15);
    setTextColor(YELLOW);
    cout << "Date: " << temp->dt.date << "-" << temp->dt.month << "-" << temp->dt.year;
    setCursor(17, 15);
    cout << "Note: " << temp->info;
    setCursor(19, 15);
    setTextColor(WHITE);
    cout << "Enter new note: ";
    cin.ignore();
    getline(cin, temp->info);
    setCursor(21, 15);
    setTextColor(GREEN);
    cout << "Record Updated successfully!";
    setTextColor(WHITE);
}

// Function to print notes in from left to right, passed head as argument
void printAllAppointments(notes *temp)
{
    system("cls");
    int row = 4;
    setTextColor(GREEN);
    setCursor(row, 36);
    cout << "ALL APPOINTMENTS PRESENT";
    setCursor(row += 1, 32);
    cout << "================================";
    setCursor(row += 1, 32);
    while (temp != NULL)
    {
        setCursor(row += 1, 32);
        setTextColor(MAGENTA);
        cout << temp->dt.date << "-";
        cout << temp->dt.month << "-";
        cout << temp->dt.year << " :: ";
        setTextColor(YELLOW);
        cout << temp->info << " ";
        temp = temp->next;
        setCursor(row += 1, 32);
    }
    setCursor(row + 2, 32);
    setTextColor(GREEN);
    cout << "Done. Press any key...";
}

/*********************data structure Class and methods*****************************/

// ******function declarations  **** //
void getInput(); // month n year 
BOOL checkDate(); // validation of date
BOOL printCalendar(int);
void printAppointments(); // display appointments of month 
int getNumber(); // take number as input

// global variables used for the project
int day, month, year;
int curDay, curMonth, curYear;
// flags used in the program
BOOL tookInput;
// globally declared appointment
notes newAppoint;
// globally declared head and tail pointer to the ordered doubly linkedlist
notes *head = NULL, *tail = NULL;
// ********************************************

// main function
int main(void)
{
    // predefined appointments, adding them to the doubly linked list when program starts
    notes app1, app2, app3;
    app1.getDefault(25, 6, 2022, "see doctor");
    app2.getDefault(30, 5, 2022, "watch movie");
    app3.getDefault(29, 1, 2015, "go treking");
    addAppoinment(&head, &tail, app1);
    addAppoinment(&head, &tail, app2);
    addAppoinment(&head, &tail, app3);
    //***********************************
menu:
    // get current system date to display current month calender
    time_t getTime = time(0);
    tm *currentTime = localtime(&getTime);

    curDay = currentTime->tm_mday; // 1-31
    curMonth = currentTime->tm_mon; // 0-11
    curYear = currentTime->tm_year; // -1900, 2022-1900
    //*********************************************
    tookInput = FALSE;
    setTextColor(WHITE);
    system("cls");
    // print a vertical line between calendar and main menu
    if (printCalendar(4))
    {
        setCursor(25, 20);
    }
    else
    {
        // if it fails get a date again
        setCursor(25, 20);
        setTextColor(RED);
        cout << "Calendar dates wrong. ";
        cout << "Enter again...";
    }
    setTextColor(GREEN);
    for (int x = 2; x < 23; x++)
    {
        setCursor(x, 48);
        cout << "||";
    }
    setCursor(3, 56);
    // show the options menu
    setTextColor(MAGENTA);
    cout << "MAIN MENU";
    setCursor(5, 52);
    setTextColor(WHITE);
    cout << "1. ADD APPOINTMENT";
    setCursor(7, 52);
    cout << "2. VIEW ALL APPOINTMENTS IN A MONTH";
    setCursor(9, 52);
    cout << "3. UPDATE APPOINTMENT";
    setCursor(11, 52);
    cout << "4. DELETE APPOINTMENT";
    setCursor(13, 52);
    cout << "5. VIEW ALL APPOINTMENTS";
    setCursor(15, 52);
    cout << "6. QUIT";
    setCursor(17, 52);
    cout << "Enter Choice [1, 2, 3, 4, 5, 6]: ";
    int choice = getNumber();
    switch (choice)
    {
    // add an appointment
    case 1: 
    {
    getday:
        // get the date, month and year
        system("cls");
        setTextColor(WHITE);
        setCursor(3, 15);
        cout << "1.Enter note on todays date?";
        setCursor(5, 15);
        cout << "2.Enter note on specific date?";
        setCursor(7, 15);
        cout << "3.Go Back";
        setCursor(9, 15);
        cout << "Enter Choice: ";
        choice = getNumber();
        if (choice == 1)
        {
            newAppoint.dt.date = curDay;
            newAppoint.dt.month = curMonth + 1;
            newAppoint.dt.year = curYear + 1900;
            goto getText;
        }
        else if (choice == 2)
        {
            setCursor(3, 52);
            cout << "Enter day: ";
            day = newAppoint.dt.date = getNumber();
            getInput();
            newAppoint.dt.month = month + 1;
            newAppoint.dt.year = year + 1900;
            if(-1 == checkDate())
                goto dateInvalid;
            goto getText;
        }
        else if (choice == 3)
        {
            goto menu;
        }
        else
        {
            setCursor(25, 20);
            setTextColor(RED);
            cout << "WRONG INPUT!! PLEASE TRY AGAIN...";
            getch();
            goto getday;
        }
    // get the notes text
    getText:
        setCursor(9, 52);
        cout << "Enter text[50-characters]: ";
        cin.ignore();
        getline(cin, newAppoint.info);
        // trim to text into 50 characters
        if (newAppoint.info.size() > 50)
        {
            newAppoint.info.erase(50);
        }
        addAppoinment(&head, &tail, newAppoint);
        system("cls");
        // print calendar
        if (printCalendar(4))
        {
            // show appointments
            printAppointments();
            setCursor(25, 20);
            setTextColor(GREEN);
            cout << "Record Added Successfully...";
            getch();
            goto menu;
        }
        else
        {
            dateInvalid:
            // if errors, ask dates again
            setCursor(25, 20);
            setTextColor(RED);
            cout << "Calendar date is invalid. ";
            cout << "Enter again...";
            getch();
            setTextColor(WHITE);
            goto getday;
        }
    }
    // view all appointments in a month
    case 2:
    {
    getmon:
        system("cls");
        // get month, year from the user
        getInput();
        system("cls");
        // print calendar
        if (printCalendar(4))
        {
            // show appointments
            printAppointments();
            setCursor(25, 20);
            setTextColor(GREEN);
            cout << "Done. Press any key...";
            // pause so that the user
            // can see the message
            getch();
            setTextColor(WHITE);
            goto menu;
        }
        else
        {
            // if it fails get a date again
            setCursor(25, 20);
            setTextColor(RED);
            cout << "Calendar date is invalid. ";
            cout << "Enter again...";
            // pause so that the user
            // can see the message
            getch();
            setTextColor(WHITE);
            goto getmon;
        }
    }
    // update record
    case 3:
    {
    getUpDate:
        // get the date, month and year
        system("cls");
        setTextColor(WHITE);
        setCursor(3, 15);
        cout << "1.Update note from today's date?";
        setCursor(5, 15);
        cout << "2.Update note from specific date?";
        setCursor(7, 15);
        cout << "3.Go Back";
        setCursor(9, 15);
        cout << "Enter Choice: ";
        choice = getNumber();
        if (choice == 1)
        {
            newAppoint.dt.date = curDay;
            newAppoint.dt.month = curMonth + 1;
            newAppoint.dt.year = curYear + 1900;
            updateAppointment(&head, newAppoint);
        }
        else if (choice == 2)
        {
            setCursor(3, 52);
            cout << "Enter day: ";
            day = newAppoint.dt.date = getNumber();
            getInput();
            newAppoint.dt.month = month + 1;
            newAppoint.dt.year = year + 1900;
            if(-1 == checkDate())
                goto wrongDate;
            updateAppointment(&head, newAppoint);
        }
        else if (choice == 3)
        {
            goto menu;
        }
        else
        {
            setCursor(25, 20);
            setTextColor(RED);
            cout << "WRONG INPUT!! PLEASE TRY AGAIN...";
            getch();
            goto getUpDate;
        }
        setTextColor(MAGENTA);
        setCursor(23, 15);
        cout << "Press 'Y' to print calender";
        setCursor(24, 30);
        setTextColor(WHITE);
        cout << "(OR)";
        setCursor(25, 15);
        setTextColor(MAGENTA);
        cout << "Press any key to go to main menu";
        if (toupper(getch()) == 'Y')
            goto printUpCal;
        else
            goto menu;
    printUpCal:
        system("cls");
        setTextColor(WHITE);
        // print calendar
        if (printCalendar(4))
        {
            // show appointments
            printAppointments();
            setCursor(25, 20);
            setTextColor(GREEN);
            cout << "Done. Press any key...";
            getch();
            goto menu;
        }
        else
        {
            wrongDate:
            // if errors, ask dates again
            setCursor(25, 20);
            setTextColor(RED);
            cout << "Calendar date is invalid. ";
            cout << "Enter again...";
            getch();
            setTextColor(WHITE);
            goto getUpDate;
        }
    }
    // delete a appoinment
    case 4:
    {
    getDelDate:
        // get the date, month and year
        system("cls");
        setTextColor(WHITE);
        setCursor(3, 15);
        cout << "1.Delete node from todays date?";
        setCursor(5, 15);
        cout << "2.Delete note from specific date?";
        setCursor(7, 15);
        cout << "3.Go Back";
        setCursor(9, 15);
        cout << "Enter Choice: ";
        choice = getNumber();
        if (choice == 1)
        {
            newAppoint.dt.date = curDay;
            newAppoint.dt.month = curMonth + 1;
            newAppoint.dt.year = curYear + 1900;
            deleteAppointment(&head, newAppoint.dt);
        }
        else if (choice == 2)
        {
            setCursor(3, 52);
            cout << "Enter day: ";
            day = newAppoint.dt.date = getNumber();
            getInput();
            newAppoint.dt.month = month + 1;
            newAppoint.dt.year = year + 1900;
            if(-1 == checkDate())
                goto invalDate;
            deleteAppointment(&head, newAppoint.dt);
        }
        else if (choice == 3)
        {
            goto menu;
        }
        else
        {
            setCursor(25, 20);
            setTextColor(RED);
            cout << "WRONG INPUT!! PLEASE TRY AGAIN...";
            getch();
            goto getDelDate;
        }
        setTextColor(MAGENTA);
        setCursor(23, 15);
        cout << "Press 'Y' to print calender";
        setCursor(24, 30);
        setTextColor(WHITE);
        cout << "(OR)";
        setCursor(25, 15);
        setTextColor(MAGENTA);
        cout << "Press any key to go to main menu";
        if (toupper(getch()) == 'Y')
            goto printDelCal;
        else
            goto menu;
    printDelCal:
        system("cls");
        setTextColor(WHITE);
        // print calendar
        if (printCalendar(4))
        {
            // show appointments
            printAppointments();
            setCursor(25, 20);
            setTextColor(GREEN);
            cout << "Done. Press any key...";
            getch();
            goto menu;
        }
        else
        {
            invalDate:
            // if errors, ask dates again
            setCursor(25, 20);
            setTextColor(RED);
            cout << "Calendar date is invalid. ";
            cout << "Enter again...";
            getch();
            setTextColor(WHITE);
            goto getDelDate;
        }
    }
    // display all notes date wise
    case 5:
    {
        // get the year to be checked
        printAllAppointments(head);
        setTextColor(WHITE);
        // pause so that the user
        // can see the message
        getch();
        goto menu;
    }
    case 6:
    {
        system("cls");
        break;
    }
    default:
    {
        setCursor(25, 20);
        setTextColor(RED);
        cout << "WRONG INPUT!! PLEASE TRY AGAIN...";
        getch();
        goto menu;
    }
    }
    // reset console settings so that the console returns back to the default state
    SetConsoleTextAttribute(h, csbInfo.wAttributes);
    return 0;
}

// get month and year
void getInput()
{
    tookInput = TRUE;
    setCursor(5, 52);
    cout << "Month?[1-12]: ";
    month = getNumber();
    month -= 1;
    setCursor(7, 52);
    cout << "Year?[1970-3000]: ";
    year = getNumber();
    year -= 1900;
}

// verify the data as a date is valid is not
int checkDate()
{
    tm t = {0, 0, 0, day, month, year};
    // mktime() validates the combination of day, month, year as a valid date
    if (mktime(&t) < 0)
    {
        return -1;
    }
    // error in day? return failed
    if (day != t.tm_mday)
    {
        return -1;
    }
    // error in month? return failed, range 0 - 11
    if (month != t.tm_mon)
    {
        return -1;
    }
    // error in year? return failed, range of year 1970 - 3000
    if (year != t.tm_year)
    {
        return -1;
    }
    // return day of the week, return values 0-6, 0 for sunday
    return t.tm_wday;
}

// check leap year
BOOL checkLeap()
{
    day = 29;
    month = 1;
    return checkDate() >= 0;
}

// get name of month by month number
string getMonthName(int monthNumber)
{
    string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    return (months[monthNumber - 1]);
}

// display dates of a month that have notes saved in linked list
void displayAppDates(notes *temp)
{
    while (temp != NULL)
    {
        if (temp->dt.year == (year + 1900))
        {
            if (temp->dt.month == (month + 1))
            {
                if (temp->dt.date == day)
                {
                    setTextColor(YELLOW);
                }
            }
        }
        temp = temp->next;
    }
}

// print calender function
BOOL printCalendar(int fromRow)
{
    day = 1;
    // if month and year is not taken by user, print current calender
    if (!tookInput)
    {
        month = curMonth;
        year = curYear;
    }
    // if the date is invalid dont display the calender
    if (-1 == checkDate())
    {
        return FALSE;
    }
    int row = fromRow;
    setCursor(row, 29);
    cout << getMonthName(month + 1) << "-";
    cout << year + 1900;
    row += 2;
    setCursor(row, 20);
    cout << "Su  Mo  Tu  We  Th  Fr  Sa";
    row += 2;
    setCursor(row, 20);
    cout << "==========================";
    row += 1;
    while (true)
    {
        switch (checkDate()) // 0-6
        {
        // mon
        case 1:
        {
            setCursor(row, 24);
        }
        break;
        // tue
        case 2:
        {
            setCursor(row, 28);
        }
        break;
        // wed
        case 3:
        {
            setCursor(row, 32);
        }
        break;
        // thu
        case 4:
        {
            setCursor(row, 36);
        }
        break;
        // fri
        case 5:
        {
            setCursor(row, 40);
        }
        break;
        // sat
        case 6:
        {
            setCursor(row, 44);
            // after saturday next row
            row += 2;
        }
        break;
        // sun
        case 0:
        {
            // cyan color for sunday
            setTextColor(CYAN);
            setCursor(row, 20);
        }
        break;
        default:
        {
            // calendar displayed successfully
            return TRUE;
        }
        }
        // yellow color for notes
        displayAppDates(head);
        if (curDay == day && curMonth == month && curYear == year)
        {
            setTextColor(RED);
        }
        printf("%2d", day++);
        setTextColor(WHITE);
        // so that it print current month calender unless asked for input
        tookInput = FALSE;
    }
}

// display dates and note of a month that have notes saved in Linked list
void displayAppInfo(notes *temp, int rw, int cl)
{
    while (temp != NULL)
    {
        if (temp->dt.year == (year + 1900))
        {
            if (temp->dt.month == (month + 1))
            {
                setCursor(rw, cl);
                cout << temp->dt.date;
                cout << " - " << temp->info;
                rw += 2;
            }
        }
        temp = temp->next;
    }
}

// displays all the appointments present
void printAppointments()
{
    setTextColor(GREEN);
    // print a vertical line between calender and appointments
    for (int x = 2; x < 23; x++)
    {
        setCursor(x, 48);
        cout << "||";
    }
    setTextColor(MAGENTA);
    setCursor(4, 56);
    cout << "APPOINTMENTS";
    setCursor(6, 52);
    cout << "==========================";
    setTextColor(YELLOW);
    displayAppInfo(head, 8, 52);
    setTextColor(WHITE);
}

//get a number as input from user
int getNumber()
{
    int temp;
    cin >> temp;
    return temp;
}
