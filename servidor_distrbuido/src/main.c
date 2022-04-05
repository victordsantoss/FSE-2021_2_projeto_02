#include <wiringPi.h>
#include "socket.h"

int main(){
    wiringPiSetup();
    mainSocket();
}