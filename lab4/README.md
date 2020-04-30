DINING PHILOSOPHERS - LAB 4


4/17/20

Implemented basic eat and think functions.

Made the function wait for a random time.

Included semaphores but they broke a bunch of stuff so idk.

Implemented a way to keep track of cycle counts.

4/22/20

Stuck with a bug where the script launches a few philosopher processes but

for whatever reason most of them just die and only one or two are thinking.

Been stuck on this bug for days now.

4/24/20

Figured out it was a stupid flag causing the bug when doing sem open.

Program runs fine now just need to take care of deadlocking.

4/28/20

Added a semaphore to take care of deadlocking issue.

Program runs fine no issues with deadlocking.

Took care of some minor bugs causing weird freezes.

Created a termReceived function that correctly destroys all the semaphores once were done.

4/29/20

Created a function that sets the pgid and writes it into a text document so that we can

terminate all of the processes using just one term signal.

Finished up commenting and cleaning up code, ready to submit.
