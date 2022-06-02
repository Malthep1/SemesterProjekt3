#include "FoodgiverServer.hpp"
#include <iostream>

using std::cout;

int main(void){

    cout << "Main online\n";
    FgServer server;
    return server.run();
}


