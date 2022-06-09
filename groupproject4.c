/**
*ICS 462-50: Operating Systems
*Group 6: Project 4 Thread Synchronization (Group Project)
*Contributors' Names: Neil Haggerty, Punyapat Rabalert, Zakariya Mehmud
*StarID:
*
*Program Description:
*The Program is the multithreaded program to simulate the bridge with 2 lanes, having
*the bridge's traffic policies and restrictions for a vehicle to perform its routine:
*arrive, cross, and leave, so the vehicle would be waiting upon the arrival before 
*it sends across the bridge, then it leaves the bridge while following the bridge policies and
*restrictions such as the 1200 unit weight limit, the two lanes being occupied by the vehicle(s)
*with two different directions or the same direction for 2 lanes if the two lanes are empty 
*for at least 2 vehicle(s) with the same direction at the moment, and more polices and restrictions.
*
*Resources:
*https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html
*https://stackoverflow.com/questions/26895245/pthread-join-for-asynchronous-threads
*https://stackoverflow.com/questions/4926622/how-to-generate-different-random-numbers-in-a-loop-in-c
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Define important values
#define NONE 0
#define NORTHBOUND 1
#define SOUTHBOUND 2
#define MAX_WEIGHT 1200
#define flag(x) puts(x);fflush(stdout);

//bridge variables
int onBridge, total_weight, number;
int carsOnNorth = 0, carsOnSouth = 0;
static int idCounter = 1;

char * dName[]={"","NORTHBOUND","SOUTHBOUND"};
// consider lane1 mainly for northbound and lane2 mainly for southbound

//number of waiting vehicles to go north on lane1
int waitingOnLane1;

//number of waiting vehicles to go south using lane2
int waitingOnLane2;

//keep track of number of vehicles that are crossing lane1
//lane1Vehicles[1] =number of vehicles NORTHBOUND in lane1
//lane1Vehicles[2] =number of vehicles Southbound in lane1
int lane1Vehicles[3];
int lane2Vehicles[3];

pthread_mutex_t bridgeLock = PTHREAD_MUTEX_INITIALIZER; //Initialize the pthread mutex lock
pthread_cond_t bridgeLane = PTHREAD_COND_INITIALIZER; //Initialize the pthread condition for wait and signal

//structure for vehicles
typedef struct{
    int id;
    char *type;
    int weight;
    int direction;
    pthread_t internalThread;
} Vehicle;

#define MAX_ARRAY_LEN 50
//vehicles passing lane1 and lane2
Vehicle lane1VehiclesArr[MAX_ARRAY_LEN];
Vehicle lane2VehiclesArr[MAX_ARRAY_LEN];

//to keep track of the waiting vehicles
Vehicle waitingList[3][500];

//Vehicle Routine function
void *vehicleRoutine(void *arg);
void arrive(Vehicle thisCar);
void sendAcross(Vehicle thisCar); //cross
void leave(Vehicle thisCar);

//Printing output of the bridge's status
void printBridgeStatus(){

    printf("\n\nBridge Status:\n");
    //vehicles on lane1
    printf("Lane 1: ");
    int first=1;
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane1VehiclesArr[i].id){
            if(first){
                printf("%s [ %s #%d",dName[lane1VehiclesArr[i].direction],lane1VehiclesArr[i].type,lane1VehiclesArr[i].id);
                first=0;
            }
            else printf(", %s #%d",lane1VehiclesArr[i].type,lane1VehiclesArr[i].id);

        }
    }
    //no vehicles were there
    if(first)printf("EMPTY []\n");
    else printf(" ]\n");
    
    //vehicles on lane1
    printf("Lane 2: ");
    first=1;
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane2VehiclesArr[i].id){
            if(first){
                printf("%s [ %s #%d",dName[lane2VehiclesArr[i].direction],lane2VehiclesArr[i].type,lane2VehiclesArr[i].id);
                first=0;
            }
            else printf(", %s #%d",lane2VehiclesArr[i].type,lane2VehiclesArr[i].id);

        }
    }
    //no vehicles were there
    if(first)printf("EMPTY []\n");
    else printf(" ]\n");


    //print the waiting list
    printf("Waiting queue (NORTHBOUND): ");
    first=1;
    for (int i = 0; i < 500; ++i)
    {
        if(waitingList[1][i].id){
            if(first){
                printf("[ %s #%d",waitingList[1][i].type,waitingList[1][i].id);
                first=0;
            }
            else printf(", %s #%d",waitingList[1][i].type,waitingList[1][i].id);

        }
    }
    //no vehicles were there
    if(first)printf("EMPTY []\n");
    else printf(" ]\n");


    //print the waiting list
    printf("Waiting queue (SOUTHBOUND): ");
    first=1;
    for (int i = 0; i < 500; ++i)
    {
        if(waitingList[2][i].id){
            if(first){
                printf("[ %s #%d",waitingList[2][i].type,waitingList[2][i].id);
                first=0;
            }
            else printf(", %s #%d",waitingList[2][i].type,waitingList[2][i].id);

        }
    }
    //no vehicles were there
    if(first)printf("EMPTY []\n");
    else printf(" ]\n");

    puts("\n");
}

//Add the v vehicle to the waiting
void addToWaitingList(Vehicle v){
    for (int i = 0; i < 500; ++i)
    {
        if(waitingList[v.direction][i].id==0){
            waitingList[v.direction][i]=v;
            return;
        }
    }
}

//Remove the v vehicle to the waiting
void removeFromWaitingList(Vehicle v){
    for (int i = 0; i < 500; ++i)
    {
        if(waitingList[v.direction][i].id==v.id){
            waitingList[v.direction][i].id=0;
            return;
        }
    }
}

//Add to the Lane 1 of the bridge
void addToLane1(Vehicle v){
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane1VehiclesArr[i].id<1){
            lane1VehiclesArr[i]=v;
            break;
        }
    }
}

//returns 1 if remved
int removeFromLane1(Vehicle v) {
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane1VehiclesArr[i].id==v.id){
            lane1VehiclesArr[i].id=0;
            return 1;
        }
    }
    return 0;
}

//Add to the Lane 2 of the bridge
void addToLane2(Vehicle v){
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane2VehiclesArr[i].id<1){
            lane2VehiclesArr[i]=v;
            break;
        }
    }
}

//returns 2 if found
int removeFromLane2(Vehicle v) {
    for (int i = 0; i < MAX_ARRAY_LEN; ++i)
    {
        if(lane2VehiclesArr[i].id==v.id){
            lane2VehiclesArr[i].id=0;
            return 2;
        }
    }
    return 0;
}


//method to verify vehicle is safe to cross
int checkWeight(Vehicle thisCar) {
    if((total_weight + thisCar.weight)<=1200){
        return 1;
    } else {
        return 0;
    }
}

//Northbound cars will use lane1 by default. southbound cars will use lane2 by default
//but sometimes it can happen that there are some cars going south using lane1 or north using lane2
//in that case thisCar has to wait thouhg the bridge can take its load
int canGoOverDefaultLane(Vehicle thisCar){

     if(thisCar.direction==NORTHBOUND){
        //check if there are some southbound cars on lane1
        if(lane1Vehicles[SOUTHBOUND])return 0;
        return 1;
     }
     if(thisCar.direction==SOUTHBOUND){
       //check if there are some northbound cars on lane2
        if(lane2Vehicles[NORTHBOUND])return 0;
        return 1;  
     }
}

//Like a ready state for vehicle
void arrive(Vehicle thisCar){
    
    pthread_mutex_lock(&bridgeLock);

 
    printf("%s #%d (%s) arrived\n", thisCar.type,thisCar.id ,dName[thisCar.direction]);
   
    
    //if the bridge cant take load the vehicle should wait on their default lane
    if(thisCar.direction==NORTHBOUND)waitingOnLane1++;
    else waitingOnLane2++;
    addToWaitingList(thisCar);
    printBridgeStatus();

    while(checkWeight(thisCar)==0 || !canGoOverDefaultLane(thisCar)) {
        pthread_cond_wait(&bridgeLane, &bridgeLock);
    }
    pthread_cond_signal(&bridgeLane);
 
    //vehicle will get out from their waiting state 
    if(thisCar.direction==NORTHBOUND)waitingOnLane1--;
    else waitingOnLane2--;
    
    removeFromWaitingList(thisCar);

    pthread_mutex_unlock(&bridgeLock);
    pthread_cond_signal(&bridgeLane);
  
}

//method that will handle majority of the logic. Each time you call it, a new thread will be created.
void sendAcross(Vehicle thisCar) {
    pthread_mutex_lock(&bridgeLock);
    
    total_weight = total_weight + thisCar.weight;
    printf("%s #%d crossing the bridge\n", thisCar.type,thisCar.id);
    
    if(thisCar.direction==NORTHBOUND){
        //first check if it can be sent over lane2 (lane1 is default for northbound)
        //check no cars are waiting on lane2 and no southbound cars passing on lane2
        if(waitingOnLane2==0 && lane2Vehicles[SOUTHBOUND]==0 && waitingOnLane1!=0){
            lane2Vehicles[NORTHBOUND]++;
            addToLane2(thisCar);
            //thisCar.lane=2;
        }
        else {
            lane1Vehicles[NORTHBOUND]++;
            addToLane1(thisCar);
        }
    }
    else{
        //first check if it can be sent over lane1 (lane2 is default for southbound)
        //check no cars are waiting on lane1 and no northbound cars passing on lane1
        if(waitingOnLane1==0 && lane1Vehicles[NORTHBOUND]==0 && waitingOnLane2!=0){
            lane1Vehicles[SOUTHBOUND]++;
            addToLane1(thisCar);
        }
        else {
            lane2Vehicles[SOUTHBOUND]++;
            addToLane2(thisCar);
        }   
    }

    printBridgeStatus();
    pthread_mutex_unlock(&bridgeLock);
    pthread_cond_signal(&bridgeLane);
    sleep(3);

}

//a terminated state for vehicle(Remove the car from the bridge routine)
void leave(Vehicle thisCar) {
    pthread_mutex_lock(&bridgeLock);

    total_weight = total_weight - thisCar.weight;
    

    //try to remove from both of the lane as we dont know which lane the car was in
    int x1=removeFromLane1(thisCar);
    int x2=removeFromLane2(thisCar);
    
    int lane = x1==0?x2:x1;

    if(lane==1){
        //this car finished passing
        lane1Vehicles[thisCar.direction]--;
         
    }
    else {
        lane2Vehicles[thisCar.direction]--;
    }

    printf("%s #%d exited the bridge\n", thisCar.type,thisCar.id);
    printBridgeStatus();
    pthread_mutex_unlock(&bridgeLock);
    pthread_cond_signal(&bridgeLane);
}

Vehicle all_vehicles[500]; //Initialize the array of vehicle

Vehicle getTheVehicle(int id){
    for (int i = 0; i < 500; ++i)
    {
        if(all_vehicles[i].id==id){
            return all_vehicles[i];
        }
    }
    
}
//returns an available vehicle index reference
int getAnVehicle(){
    for (int i = 0; i < 500; ++i)
    {
        if(all_vehicles[i].id<1){
            return i;;
        }
    }
    return -1;
}

//remove vehicle
void removeVehicle(int id){
 for (int i = 0; i < 500; ++i)
    {
        if(all_vehicles[i].id==id){
            all_vehicles[i].id=0;
            break;
        }
    }   

}


//vehicle routine: arrive, cross, leave
void *vehicleRoutine(void *arg) {
    int id=*((int *)arg);

    Vehicle v=getTheVehicle(id);
    arrive(v);
    sendAcross(v);
    leave(v);
    
    return NULL;
}

//method to initialize vehicle. pass vehicle type as param.
int newVehicle(char *type, int direction) {
    int vIndex=getAnVehicle();

    all_vehicles[vIndex].id = idCounter++;
    all_vehicles[vIndex].type = type;
    all_vehicles[vIndex].direction = direction;

    if(strcmp(type, "Car") == 0 )
        all_vehicles[vIndex].weight = 200;
    else if(strcmp(type, "Van") == 0)
        all_vehicles[vIndex].weight = 300;

    pthread_create(&all_vehicles[vIndex].internalThread, NULL, vehicleRoutine, (void *)&all_vehicles[vIndex].id);

     return vIndex;
}

//Creating Group type
typedef struct{
    int total;
    double nProbability,sProbability;
    int delay;
} Group;


Group arr[50]; //Initialize the array of group

//Add new group
void addGroup(Group g){

    srand((unsigned)time(NULL));
    
    int indices[50];
    int idx=0;
    
    for (int i = 0; i < g.total; ++i)
    {
        int carOrVan= rand()%1000;
        char *type;

        if(carOrVan<500)type="Car";
        else type="Van";

        int northOrSouth = (rand()%1000);
        int nProbability= g.nProbability*1000;
        int direction;

        if(northOrSouth<=nProbability)direction = NORTHBOUND;
        else direction=SOUTHBOUND;

        int index = newVehicle(type,direction);

        indices[idx++]=index;
    
    }

    for (int i = 0; i < idx; ++i)
    {
        pthread_join(all_vehicles[indices[i]].internalThread,NULL); 
    }


}

//Function to Run the scheduling
void driver(){
    int numOfGroups;
    printf("Total number of group: ");
    scanf("%d",&numOfGroups);
    for (int i = 0; i < numOfGroups; ++i)
    {  
        printf("Group %d: \n",i+1);
        printf("Total vehicles: ");
        scanf("%d",&arr[i].total);

        printf("[N/S] (input should be in '.23/.77' format): ");
        scanf("%lf/%lf",&arr[i].nProbability,&arr[i].sProbability);

        if(i<numOfGroups-1){
            printf("DELAY: ");
            scanf("%d",&arr[i].delay);
        }
    
    }


    for (int i = 0; i < numOfGroups; ++i)
    {

        addGroup(arr[i]);

        if(i<numOfGroups-1){
            sleep(arr[i].delay);
        }
    }
}

//Run the main program
int main(void) {
    total_weight = 0;
    onBridge = 0;

    
    driver();
    return 0;
}