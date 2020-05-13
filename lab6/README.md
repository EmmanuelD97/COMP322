Wack a Mole - Lab 6

5/9/20

First commit, wrote the makefile and started the mole and spiritd files, but havent added much.

5/10/20

Started forking for daemon, and then killing the parent.

Made it so it changes directory using chdir("/");

Made a separate function which forks the daemon and then registers the signals before 

redirecting the file descriptor using dup2.

Created the signal handler which takes care of USR1 USR2 and TERM signals, if USR1 or 2

is received it kills the respective mole and creates a new random mole.

Having an issue where moles are not dying so we have infinite moles.

Added some exit status flags that take care of certain errors.

5/11/20

Experimenting with killing the mole in different ways to try and cap the number of moles to 2.

running into an issue where killing one mole kills all the moles.

I think its because the process that is sending the term signal is also receiving it.

5/12/20

Fixed the problem where moles were not dying, and fixed the issue where killing one mole killed

every process.

Reorganized code and renamed firstMole and secondMole to mole1 and mole2 to make it easier to

write into the log file.

Still not writing things into the log file correctly.

Commented on code and going to submit as is but will keep working on it to try and fix the

issue with the log file.


