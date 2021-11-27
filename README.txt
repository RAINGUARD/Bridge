ICS 462 Operating Systems
Group 6: Project 4 Thread Synchronization (Group Project)
Contributor(s) Name: Neil Haggerty, Punyapat Rabalert. Zakariya Mehmud
StarID(Respectively to the Names): fi2002ae, ih2153wu, rj3591tf

Program Description:
The Program is the multithreaded program to simulate the bridge with 2 lanes, having
the bridge's traffic policies and restrictions for a vehicle to perform its routine on
the bridge, which are: arrive, cross, and leave the bridge on the correct direction
while following the bridge's policies and restrictions such as 1200 unit weight limit,
two empty lanes can be used for the same directions vehicles if there is no waiting/crossing 
vehicle at the moment, and more.

File(s):
   -groupproject4.c:
      -The program simulates the bridge with 2 lanes by having the bridge's variables and pthread_cond and mutex_lock
        to lock or release a vehicle's thread.
      -The program create a schedule for running the group(s) of vehicle to perform the vehicle routine.
      -The program asks for inputs to create schedule.
      -Such as Number of Group, Number of Vehicle(s) in the Group, Probability of Northbound/Southbound 
       Direction, and Delay (if Multiple Groups)
      -The program create a new vehicle with unique id, appropriate weight and type, and random direction based
       on given probability, adding to the allVehicle array, then the vehicle creates pthread to perform its vehicle routine.
      -The vehicle would arrive and wait in its lane queue if the vehicles on the bridge would surpass the weight limit.
      -The vehicle would cross toward its lane toward the targeted direction for 3 seconds.
      -The vehicle would leave and exit the bridge from its lane.

Compiling the code:
prompt>$ gcc groupproject4.c -o gp -pthread
prompt>$./gp

  -Running the program and How to enter a schedule.
     Total number of group:(*Enter the Schedule's Number of Group.
     Group 1: 
     Total vehicles:(*Enter the Number of Vehicle(s) of the Group.)
     [N/S] (input should be in '.23/.77' format):(*Enter your N/S Probability Following the Format.)
     DELAY:(*Enter the Delay time in s Note: Ask When your group is > 1.)
     Group 2:(Note: Output if your input group is > 1.)
     Total vehicles: (*Enter the Number of Vehicle(s) of the Group.)
     [N/S] (input should be in '.23/.77' format): (input should be in '.23/.77' format):(*Enter your Formatted N/S Probability)

Example Input/Output:
 For Schedule, 10 [N/S: 0.5/0.5] : DELAY(10) : 15 [N/S: 1.0/0.0]
  -Asking for Input
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
     ...
     
  -The screenshots of the required schedules are in the schedule output's folders.
     
Recurring Problems and Challenges in the Implementation:
     During the implementation and tests, we used to have recurring issues: the schedule finishing before all
     the vehicles complete its routines, some vehicles not performing their routines, and the waiting queues
     not dequeuing correctly since there was a problem with index for queue and locking/ releasing the mutex_lock 
     at wrong time or position. However, the current draft no longer has the problems.    
     
Citing Resources:
https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
https://stackoverflow.com/questions/26895245/pthread-join-for-asynchronous-threads
https://stackoverflow.com/questions/4926622/how-to-generate-different-random-numbers-in-a-loop-in-c
