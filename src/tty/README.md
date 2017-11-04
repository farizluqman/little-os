## What is TTY?
It's the Teletype Terminal. The functionality is like the real teletype machine, pressing a key sends a character down the serial link. Incoming characters on the serial link are printed on the printer. In the real case, it is printed on the screen instead. Before anything is written on the screen, it is the tty that handles the input from the keyboard.

Here is where we will have different tty channels, with different use cases. This time, we only have one tty, the tty0 used to handle all applications.
