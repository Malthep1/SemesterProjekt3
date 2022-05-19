#include "ServTCP.hpp"

int main(void){

    ServTCP serv;
    printf("Created serv tf\n");
    Thread servT(&serv);
    servT.start();
    printf("Started server\n");
    servT.join();
    printf("Server finished\n");
    return 0;
}