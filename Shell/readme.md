#Shell in C
Type in the command "make" and then type ./crash to execute

This is a simple shell implementation with 11 files. The main loop has logic for taking an input and then parsing it into different commands.

The command is then checked against a list of prebuilt commands and executed accordingly. The background processes are tracked through means of a linked list of their pid's

The second iteration added fg, kjobs, jobs, setenv, unsetenv, and CTRL+C

