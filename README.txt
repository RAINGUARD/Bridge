ICS 462 Operating Systems
Group 6: Project 4 Thread Synchronization (Group Project)
Contributor(s) Name: Neil Haggerty, Punyapat Rabalert. Zakariya Mehmud
StarID(Respectively to the Names): fi2002ae, ih2153wu, rj3591tf

Program Description:
The Program is the multithreaded program to simulate the bridge with 2 lanes, having
the bridge's traffic policies and restrictions for a vehicle to perform its routine on
the bridge, which are: arrive, cross, and leave the bridge on the correct direction
while following the bridge's policies and restrictions such as 1200 unit weight limit,
two empty lanes can be used for

Write-Up Implementation:

File(s): groupproject4.c

Example Input/Output for Group 1 with 10 vehicles and Group 2 with 15 vehicles delaying 10s between 1st and 2nd groups:
  -Asking for input
     Total number of group: 2
     Group 1: 
     Total vehicles: 10
     [N/S] (input should be in '.23/.77' format): 0.5/0.5
     DELAY: 10
     Group 2: 
     Total vehicles: 15
     [N/S] (input should be in '.23/.77' format): 1.0/0.0
 -Output
     Van #1 (SOUTHBOUND) arrived

     Bridge Status:
     Lane 1: EMPTY []
     Lane 2: EMPTY []
     Waiting queue (NORTHBOUND): EMPTY []
     Waiting queue (SOUTHBOUND): [ Van #1 ]

     Van #1 crossing the bridge

     Bridge Status:
     Lane 1: EMPTY []
     Lane 2: SOUTHBOUND [ Van #1 ]
     Waiting queue (NORTHBOUND): EMPTY []
     Waiting queue (SOUTHBOUND): EMPTY []

     Car #3 (NORTHBOUND) arrived

     Bridge Status:
     Lane 1: EMPTY []
     Lane 2: SOUTHBOUND [ Van #1 ]
     Waiting queue (NORTHBOUND): [ Car #3 ]
     Waiting queue (SOUTHBOUND): EMPTY []

     Car #3 crossing the bridge

     Bridge Status:
     Lane 1: NORTHBOUND [ Car #3 ]
     Lane 2: SOUTHBOUND [ Van #1 ]
     Waiting queue (NORTHBOUND): EMPTY []
     Waiting queue (SOUTHBOUND): EMPTY []
     ...
     Car #3 exited the bridge

     Bridge Status:
     Lane 1: NORTHBOUND [ Car #5 ]
     Lane 2: SOUTHBOUND [ Van #1, Car #10 ]
     Waiting queue (NORTHBOUND): [ Van #9, Van #6, Van #7 ]
     Waiting queue (SOUTHBOUND): [ Van #4, Car #2 ]

     Van #1 exited the bridge

     Bridge Status:
     Lane 1: NORTHBOUND [ Car #5 ]
     Lane 2: SOUTHBOUND [ Car #10 ]
     Waiting queue (NORTHBOUND): [ Van #9, Van #6, Van #7 ]
     Waiting queue (SOUTHBOUND): [ Van #4, Car #2 ]

Citing Resources:
https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
https://stackoverflow.com/questions/26895245/pthread-join-for-asynchronous-threads
https://stackoverflow.com/questions/4926622/how-to-generate-different-random-numbers-in-a-loop-in-c
