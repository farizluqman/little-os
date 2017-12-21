#include "tty/calc/calc.h"


static int current_tty;

char cbuffer_str[100];
int cwrite_flag = 0; // the flag used whether to write output on console or not
int cleft = 0; // number of characters typed by the user
int cexit = 0;

void calc_main()
{
    while (cexit == 0) {
			char c = get_ascii_char(); // this may provide some weird bugs, need to
																 // implement our own scanf function
			calc_fetch_input(&c);
    }
}

void process_calc_command()
{
    int found = 0;
    int clear = 0; // if requested to clear screen

    if (strcmp(cbuffer_str, "clear") == 0) {
        found = 1;
        clear = 1;
        clear_screen();
    }

		if (strcmp(cbuffer_str, "plus") == 0) {
				found = 1;
				int first_get = 0;
				printf("Plus > Enter First Number: ");

				while(first_get == 0){
					char c = get_ascii_char();
					printf("\n%d\n", (int)(c - '0'));
				}
		}

    if (found == 0 && cleft > 0) {
        // user typed something but command not found
        printf("Command not found\n");
    } else if (found == 0 && cleft == 0) {
        // user did nothing but only enter
    } else if (clear == 0) {
        // command is found and application is executed
        console_writestring("\n");
    }
}

void calc_fetch_input(char *c)
{
    switch (*c)
    {
	    case '\b': // backspace
	        if(cleft == 0) { // protect from "over deleting" characters
	            cbuffer_str[0] = '\0'; // clear buffer
	        } else {
	            console_putchar(*c); // only "delete" if there are characters on the left
	            cleft--;
	            cbuffer_str[cleft] = '\0'; // clear character from buffer
	        }
	        break;
	    case '\n': // newline
	    case '\r': // carriage return
					printf("\n");
	        process_calc_command();
	        //process_tty_command();
	        cbuffer_str[0] = '\0';
	        cleft = 0;
					printf("Calc > ");
	        break;
	    default:
	        console_putchar(*c);
	        cbuffer_str[cleft] = *c;
	        cleft++;
	        cbuffer_str[cleft] = '\0';
    }
}
