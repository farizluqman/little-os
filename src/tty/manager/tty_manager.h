#ifndef TTY_MANAGER_H
#define TTY_MANAGER_H

static int current_tty;

void initialize_tty();
void display_current_tty();
void set_current_tty(int i);
void process_tty_command();
void fetch_tty_input(char *c);
int get_current_tty();

#endif
