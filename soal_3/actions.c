// actions.c

#include <stdio.h>
#include <string.h>

char* Gap(float distance) {
    if (distance < 3.5)
        return "Gogogo";
    else if (distance > 3.5 && distance <= 10)
        return "Push";
    else
        return "Stay out of trouble";
}

char* Fuel(int fuel) {
    if (fuel > 80)
        return "Push Push Push";
    else if (fuel >= 50 && fuel <= 80)
        return "You can go";
    else
        return "Conserve Fuel";
}

char* Tire(int tire) {
    if (tire > 80)
        return "Go Push Go Push";
    else if (tire >= 50 && tire <= 80)
        return "Good Tire Wear";
    else if (tire >= 30 && tire < 50)
        return "Conserve Your Tire";
    else
        return "Box Box Box";
}

char* TireChange(char* current_tire) {
    if (strcmp(current_tire, "Soft") == 0)
        return "Mediums Ready";
    else if (strcmp(current_tire, "Medium") == 0)
        return "Box for Softs";
    else
        return "Unknown tire type";
}

