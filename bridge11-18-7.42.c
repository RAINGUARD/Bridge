
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>

//Define important values
#define MAX_VEHICLE 6 //1200/200 = 6 is maxmum number for all cars with lowest weight
#define NONE 0
#define NORTHBOUND 1
#define SOUTHBOUND 2
#define MAX_WEIGHT 1200

//bridge variable
int onBridge;
int number;
int carsCount = 0 ;
int carsOnNorth = 0, carsOnSouth = 0;
int total_weight, waitingForSouthbound = 0, waitingForNorthbound = 0;
//static int currentDirection = 0;
static int idCounter = 0;
pthread_mutex_t bridgeLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bridgeLane = PTHREAD_COND_INITIALIZER;

//structure for vehicles
typedef struct{
    int id;
    char *type;
    int weight;
    int direction;
    pthread_t internalThread;
} Vehicle;

Vehicle vehicleHeadsNorth[MAX_VEHICLE];
Vehicle vehicleHeadsSouth[MAX_VEHICLE];
void *vehicleRoutine(Vehicle *v);
void arrive(Vehicle *thisCar);
void sendAcross(Vehicle *thisCar);
void leave(Vehicle *thisCar);

//method to initialize vehicle. pass vehicle type as param.
Vehicle newVehicle(char *type, int direction) {

    Vehicle newVehicle;
    idCounter += 1;
    newVehicle.id = idCounter;
    newVehicle.type = type;
    if(strcmp(type, "Car") == 0 )
        newVehicle.weight = 200;
    else if(strcmp(type, "Van") == 0)
        newVehicle.weight = 300;

    if(direction == NORTHBOUND) {
        newVehicle.direction = direction;
        vehicleHeadsNorth[carsOnNorth] = newVehicle;
        pthread_create(&newVehicle.internalThread, NULL ,vehicleRoutine(&newVehicle), (void*)&vehicleHeadsNorth[carsOnNorth]);
        //(void*)&vehicleHeadsNorth[carsOnNorth] vehicleRoutine
        carsOnNorth++;

    } else if(direction == SOUTHBOUND){
        newVehicle.direction = direction;
        vehicleHeadsSouth[carsOnSouth] = newVehicle;
        pthread_create(&newVehicle.internalThread, NULL ,vehicleRoutine(&newVehicle), (void*)&vehicleHeadsSouth[carsOnSouth]);
        //(void*)&vehicleHeadsSouth[carsOnSouth]
        carsOnSouth++;
    }

    return newVehicle;
}


//method to verify vehicle is safe to cross
int checkWeight(Vehicle *thisCar) {
    if((total_weight + thisCar->weight)<=1200){
        return 0;
    } else {
        return 1;
    }
}

//method that will handle majority of the logic. Each time you call it, a new thread will be created.
void sendAcross(Vehicle *thisCar) {
    pthread_mutex_lock(&bridgeLock);
    if(checkWeight(thisCar) == 0) {
        total_weight = total_weight + thisCar->weight;
        printf("Current weight = %d\n", total_weight);
        onBridge++;
        printf("Cars on bridge: %d\n", onBridge);
        sleep(3);
        total_weight = total_weight - thisCar->weight;
        onBridge--;
        printf("Current weight = %d\n", total_weight);
        printf("Cars on bridge: %d\n", onBridge);
    }
    pthread_mutex_unlock(&bridgeLock);
}

//Like a ready state for vehicle
void arrive(Vehicle *thisCar){
    pthread_mutex_lock(&bridgeLock);
    if(NORTHBOUND == thisCar->direction){
        waitingForNorthbound++;
    }
    if(SOUTHBOUND == thisCar->direction){
        waitingForSouthbound++;
    }

    while(checkWeight(thisCar) == 1){
        pthread_cond_wait(&bridgeLane,&bridgeLock);
    }

    if(thisCar->direction == NORTHBOUND){
        printf("Arrived Vehicle: %d Type: %s Travel To: %s\n", thisCar->id, thisCar->type, "NORTHBOUND");
    } else if(thisCar->direction == SOUTHBOUND) {
        printf("Arrived Vehicle: %d Type: %s To: %s\n", thisCar->id, thisCar->type, "SOUTHBOUND");
    }
    onBridge++;
    pthread_cond_signal(&bridgeLane);
    pthread_mutex_unlock(&bridgeLock);
}

//Like a running state for vehicle
void cross(Vehicle *thisCar){
    pthread_mutex_lock(&bridgeLock);

    
    pthread_mutex_unlock(&bridgeLock);
}

//Like a terminated state for vehicle(Remove the car from the bridge routine)
void leave(Vehicle *thisCar) {
    pthread_mutex_lock(&bridgeLock);

    pthread_mutex_unlock(&bridgeLock);
}

//vehicle routine: arrive, cross, leave
void *vehicleRoutine(Vehicle *v) {

  printf("Vehicle Routine: %d to %d \n", v->id, v->direction);
  arrive(v);
  sleep(1); // calling interleavings
  sendAcross(v);
  sleep(1); // calling interleavings
  leave(v);
  sleep(1); // calling interleavings
  pthread_exit(NULL);
}

int main() {
    total_weight = 0;
    onBridge = 0;

    Vehicle v1;
    v1 = newVehicle("Van", NORTHBOUND);
    printf("%d, %s, %d\n",v1.id,v1.type,v1.weight);
    Vehicle v2;
    v2 = newVehicle("Car", SOUTHBOUND);
    printf("%d, %s, %d\n",v2.id,v2.type,v2.weight);

  //  pthread_t newthread;

  //  pthread_create(&newthread, NULL, sendAcross(&v1), NULL);
  //  pthread_create(&newthread, NULL, sendAcross(&v2), NULL);
    
}