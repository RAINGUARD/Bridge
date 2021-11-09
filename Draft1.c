
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>

int total_weight, goingSouth, goingNorth, northboundQueue, southboundQueue;

//structure for vehicles
struct Vehicle {
    char *type;
    int weight;
};

//method to initialize vehicle. pass vehicle type as param.
struct Vehicle newVehicle(char *type) {
    struct Vehicle newVehicle;
    newVehicle.type = type;
    int result = strcmp(type, "Car");
    if(result == 0)
        newVehicle.weight = 200;
    else
        newVehicle.weight = 300;
    return newVehicle;
}

//method to verify vehicle is safe to cross
int checkWeight(struct Vehicle *thisCar) {
    if((total_weight + thisCar->weight)<=1200){
        return 0;
    } else {
        return 1;
    }
}

//method to ensure only 3 cars at a time.
int numberCheck() {
    if((goingSouth + goingNorth)<3) {
        return 0;
    } else {
        return 1;
    }
}

//method that will handle majority of the logic. Each time you call it, a new thread will be created.
void *sendAcross(struct Vehicle *thisCar) {
    if(checkWeight(thisCar) == 0) {
        total_weight = total_weight + thisCar->weight;
        printf("%d\n", total_weight);
        goingNorth++;
        printf("%d\n", goingNorth);
        sleep(3);
        total_weight = total_weight - thisCar->weight;
        goingNorth--;
        printf("%d\n", total_weight);
        printf("%d\n", goingNorth);
    }
    return NULL;
}

int main() {
    total_weight = 0;
    goingSouth = 0;
    goingNorth = 0;

    struct Vehicle v1;
    v1 = newVehicle("Van");
    sendAcross(&v1);
    
}