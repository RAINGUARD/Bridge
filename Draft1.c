
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
#include <stdbool.h>
  
int total_weight = 0;
int southboundqueue = 0;
int northboundqueue = 0;

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

int checkWeight(struct Vehicle *thisCar) {
    if((total_weight + thisCar->weight)<=1200){
        return 0;
    } else {
        return 1;
    }
}

void *sendAcross(struct Vehicle *thisCar) {
    if(checkWeight(thisCar) == 0) {
        total_weight = total_weight + thisCar->weight;
        northboundqueue++;
        sleep(3);
        total_weight = total_weight - thisCar->weight;
        northboundqueue--;
    }
    return NULL;
}

int main() {
    struct Vehicle *v1;
    *v1 = newVehicle("Van");
    sendAcross(v1);
    
}