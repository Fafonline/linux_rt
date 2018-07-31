# Description

Example to evaluate scheduling priority and policy.

The program create 3 threads with configurable scheduling policy and  priority.
ALl thread are infinite loop. A callback could be specify to  perform within the loop.

Finale purpose will be to achieve a fine tuning of real  times thread on a system.


# Scheduling policy and priority range

On linux system, priority range are 0 to 139. But range is split in two to distinguish real-time
and non-real-time thread. According to this, two value shall taken in consideration: Priority and niceness.

## Niceness (NI of ps command)

Niceness deals with non-real-time thread,i.e the one affected by schedule policy different of FIFO or RR.
Highest thread priority is -20 while 19 is the lowest. It maps prioritu range 100 to 139.

## Priority (PRI of ps command)

Priority deals with real-time thread, i.e the one affected by schedule policy FIFO and RR
