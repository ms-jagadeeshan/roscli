#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define REFRESH_INTERVEL 1
#define CONFIGDIR getenv("HOME") "/.config/roscli"
void workspace(WINDOW *win, char[]);

void buildstatus(WINDOW *win, char[]);

void message(WINDOW *win, char[]);

void package(WINDOW *win, char[]);
void pidttyname(WINDOW *win, char fname[]);
void freepts(WINDOW *win, char fname[]);
int doexit(char[]);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{
    sleep(1);
    char *HOME_PATH = getenv("HOME");
    char CONFIG_DIR[2048];
    strcpy(CONFIG_DIR, HOME_PATH);
    strcat(CONFIG_DIR, "/.config/roscli");
    char workspace_conf[2048];
    strcpy(workspace_conf, CONFIG_DIR);
    strcat(workspace_conf, "/workspace");

    char buildstatus_conf[2048];
    strcpy(buildstatus_conf, CONFIG_DIR);
    strcat(buildstatus_conf, "/buildstatus");

    char message_conf[2048];
    strcpy(message_conf, CONFIG_DIR);
    strcat(message_conf, "/message");

    char doexit_conf[2048];
    strcpy(doexit_conf, CONFIG_DIR);
    strcat(doexit_conf, "/doexit");

    char package_conf[2048];
    strcpy(package_conf, CONFIG_DIR);
    strcat(package_conf, "/package");

    char pidttyname_conf[2048];
    strcpy(pidttyname_conf, CONFIG_DIR);
    strcat(pidttyname_conf, "/pidttyname");

    char freepts_conf[2048];
    strcpy(freepts_conf, CONFIG_DIR);
    strcat(freepts_conf, "/freepts");

    WINDOW *win[4];
    int i;

    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);

    if (COLS < 66)
    {
        while (true)
        {

            win[0] = newwin(LINES, COLS, 0, 0);
            print_in_middle(win[0], (LINES - 3) / 2, 0, COLS, "Window size not sufficient", COLOR_PAIR(4));
            print_in_middle(win[0], (LINES - 1) / 2, 0, COLS, "Needs atleast 66 columns", COLOR_PAIR(4));

            if (COLS >= 66)
            {
                delwin(win[0]);
                refresh();
                break;
            }
            wrefresh(win[0]);
            sleep(1);
        }
    }

    /* Create the window to be associated with the menu */
    win[0] = newwin(3, COLS, 0, 0);
    win[1] = newwin(LINES - 3, (COLS + 1) / 2, 3, COLS / 2);
    win[2] = newwin(4, (COLS - 1) / 2, 3, 0);
    win[3] = newwin(LINES - 7, (COLS - 1) / 2, 7, 0);

    /* Print a border around the main window and print a title */
    box(win[0], 0, 0);
    box(win[1], 0, 0);
    box(win[2], 0, 0);
    box(win[3], 0, 0);
    refresh();
    while (TRUE)
    {
        delwin(win[0]);
        delwin(win[1]);
        delwin(win[2]);
        delwin(win[3]);
        /* Create the window to be associated with the menu */
        win[0] = newwin(3, COLS, 0, 0);
        win[1] = newwin(LINES - 3, (COLS + 1) / 2, 3, COLS / 2);
        win[2] = newwin(4, (COLS - 1) / 2, 3, 0);
        win[3] = newwin(LINES - 7, (COLS - 1) / 2, 7, 0);

        /* Print a border around the main window and print a title */
        box(win[0], 0, 0);
        box(win[1], 0, 0);
        box(win[2], 0, 0);
        box(win[3], 0, 0);

        /* Function to call for each window */
        workspace(win[0], workspace_conf);
        buildstatus(win[0], buildstatus_conf);
        package(win[1], package_conf);
        message(win[2], message_conf);
        pidttyname(win[3], pidttyname_conf);
        freepts(win[3], freepts_conf);
        if (doexit(doexit_conf))
        {
            pid_t pid = fork();
            if (!pid)
                execl("/bin/rm", "rm", "-f", workspace_conf, buildstatus_conf, package_conf, message_conf, doexit_conf, (char *)NULL);
            break;
        }

        /* refreshing window for every two seconds */
        wrefresh(win[0]);
        wrefresh(win[1]);
        wrefresh(win[2]);
        wrefresh(win[3]);
        refresh();
        sleep(REFRESH_INTERVEL);
    }
    delwin(win[0]);
    delwin(win[1]);
    delwin(win[2]);
    endwin();
    return 0;
}

void workspace(WINDOW *win, char fname[])
{
    int c;
    int y = 1, x = 1;
    char msg[] = "Workspace : ";
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, y, x, msg);
    wattroff(win, COLOR_PAIR(6));
    if (access(fname, F_OK) != 0)
    {
        return;
    }

    x += strlen(msg);
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, x, "Can't able to access");
        wattroff(win, COLOR_PAIR(1));
        return;
    }

    c = fgetc(fptr);
    wattron(win, COLOR_PAIR(2));
    while (c != EOF && c != '\n')
    {
        mvwaddch(win, y, x++, c);
        c = fgetc(fptr);
    }
    wattroff(win, COLOR_PAIR(2));
    fclose(fptr);
}

void buildstatus(WINDOW *win, char fname[])
{
    int c;
    int y = 1, x = 1;
    char msg[] = "Build Status: ";
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, y, COLS - 24, msg);
    wattroff(win, COLOR_PAIR(6));
    if (access(fname, F_OK) != 0)
    {
        return;
    }

    x += strlen(msg);
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, COLS - 24 + x, "NA");
        wattroff(win, COLOR_PAIR(1));
        return;
    }

    c = fgetc(fptr);

    while (c != EOF && c != '\n')
    {
        if (c == '0')
        {
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, 1, COLS - 24 + x, "Success");
            wattroff(win, COLOR_PAIR(2));
        }
        else
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 1, COLS - 24 + x, "Failed");
            wattroff(win, COLOR_PAIR(1));
        }
        c = fgetc(fptr);
    }
    fclose(fptr);
}

void message(WINDOW *win, char fname[])
{
    int c;
    int y = 1;
    int x = 1;
    char msg[] = "Message: ";
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, y, x, msg);
    wattroff(win, COLOR_PAIR(3));
    if (access(fname, F_OK) != 0)
    {
        return;
    }
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        wattron(win, COLOR_PAIR(1));
        wprintw(win, "Can't able to access");
        wattroff(win, COLOR_PAIR(1));
        return;
    }

    c = fgetc(fptr);
    x += strlen(msg);
    wattron(win, COLOR_PAIR(4));
    while (c != EOF && c != '\n')
    {
        if (x >= (COLS - 5) / 2)
        {
            y++;
            x = 1;
        }
        if (y < 3)
        {
            mvwaddch(win, y, x++, c);
        }
        c = fgetc(fptr);
    }
    wattroff(win, COLOR_PAIR(4));
    fclose(fptr);
}

void package(WINDOW *win, char fname[])
{
    int c, y = 0, x = 1;

    char msg[] = "{Packages in current workspace}";
    int lenght = (COLS - 3) / 2;
    int msg_len = strlen(msg);
    int no_of_hyphen = (lenght - msg_len + 1) / 2;

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, y++, x + no_of_hyphen, msg);
    wattroff(win, COLOR_PAIR(3));

    if (access(fname, F_OK) != 0)
    {
        return;
    }
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, x, "Can't able to access");
        wattroff(win, COLOR_PAIR(1));
        return;
    }

    c = fgetc(fptr);
    wattron(win, COLOR_PAIR(4));
    while (c != EOF)
    {
        if (c == '\n')
        {
            y++;
            x = 1;
            c = fgetc(fptr);
            continue;
        }
        mvwaddch(win, y, x++, c);
        c = fgetc(fptr);
    }
    wattroff(win, COLOR_PAIR(4));
    fclose(fptr);
}

void pidttyname(WINDOW *win, char fname[])
{
    int c, y = 0, x = 1;

    char msg[] = "{PID}--{TTY}--{NAME}";
    int lenght = (COLS - 3) / 2;
    int msg_len = strlen(msg);
    int no_of_hyphen = (lenght - msg_len + 1) / 2;

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, y++, x + no_of_hyphen, msg);
    wattroff(win, COLOR_PAIR(3));

    if (access(fname, F_OK) != 0)
    {
        return;
    }
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, y, x, "Can't able to access");
        wattroff(win, COLOR_PAIR(1));
        return;
    }

    c = fgetc(fptr);
    wattron(win, COLOR_PAIR(4));
    while (c != EOF)
    {
        if (c == '\n')
        {
            y++;
            x = 1;
            c = fgetc(fptr);
            continue;
        }
        mvwaddch(win, y, x++, c);
        c = fgetc(fptr);
    }
    wattroff(win, COLOR_PAIR(4));
    fclose(fptr);
}

void freepts(WINDOW *win, char fname[])
{
    int c, y = LINES -9, x = 1;

    char msg[] = "Freepts: ";

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win,y, 1, msg);
    wattroff(win, COLOR_PAIR(3));

    if (access(fname, F_OK) != 0)
    {
        return;
    }
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        return;
    }
    x += strlen(msg);
    c = fgetc(fptr);
    wattron(win, COLOR_PAIR(4));
    while (c != EOF && c != '\n')
    {
        mvwaddch(win, y, x++, c);
        c = fgetc(fptr);
    }
    wattroff(win, COLOR_PAIR(4));
    fclose(fptr);
}

int doexit(char fname[])
{
    int c;
    if (access(fname, F_OK) != 0)
    {
        return 0;
    }
    FILE *fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        return 0;
    }

    c = fgetc(fptr);
    fclose(fptr);
    if (c == '1')
        return 1;
    else
        return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}