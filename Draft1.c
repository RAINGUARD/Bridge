
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>
  
int total_weight = 0;
int southboundqueue = 0;
int northboundqueue = 0;

//structure for vehicles
struct Vehicle {
    char *type;
    int weight;
};

//method to initialize vehicle. pass vehicle type as param.
struct Vehicle Vehicle_init(char *type){
    struct Vehicle newVehicle;
    newVehicle.type = type;
    int result = strcmp(type, "Car");
    if(result == 0)
        newVehicle.weight = 200;
    else
        newVehicle.weight = 300;
    return newVehicle;
}

void *newVehicle(struct Vehicle *thisCar)
{
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}


int main()
{
    struct Vehicle v1;
    v1 = Vehicle_init("Van");
    
}