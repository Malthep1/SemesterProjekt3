#include "SystemManager.hpp"
// 
#define PATHNAME "/home/stud/git/SemesterProjekt3/RPI/dat/settings.dat"
#define WATCH_FLAGS                 IN_CLOSE_WRITE
#define EVENT_SIZE                  (sizeof(struct inotify_event))
#define WATCH_DELAY_MS              100
#define BUFFER_SIZE                 500

void SystemManager::runMain(){
    
    while(true){
        //cmdCtrl.dispatchCommand();
        osapi::sleep(200);
        cmdCtrl.dispatchUartCommand("W350");
        cmdCtrl.dispatchUartCommand("W200");
        cmdCtrl.dispatchUartCommand("F200");
        cmdCtrl.dispatchUartCommand("F400");
        cmdCtrl.dispatchUartCommand("T020");
        cmdCtrl.dispatchUartCommand("F040");
        break;
    }
}

void SystemManager::listenSettingsUpdate(){
    ListenerThread listener_;   
    lt_ = new Thread(&listener_);

    lt_->start();

    // DEBUG:
    lt_->join();
}

// Check https://linuxhint.com/inotify_api_c_language/ for good example of using inotify
void SystemManager::ListenerThread::run(){
    printf("Made a new listener thread :)) \n");

    init_inot();

    int i = 0, length;
    char buffer[BUFFER_SIZE];

    for(;;){
        length = read(inotify_fd, buffer, BUFFER_SIZE);
        while(i < length){  
            struct inotify_event* event = (struct inotify_event*) &buffer[i];
            // DEBUG:
            //Print the mask for debug and check with inotify.h 
            //printf("Mask: %d \n", event->mask );
            //printf("%d\n", event->mask & IN_MODIFY);
            //printf("%d\n", event->mask & IN_CLOSE_WRITE | IN_MODIFY);
            
            if((event->mask & IN_CLOSE_WRITE | IN_MODIFY) == 10){
                printf("File was written to and has been closed\n");
                // Do something (fx send Msg to main thread to update the settings)
            }
            
            i = i + EVENT_SIZE + event->len;
        }
        
        i = 0;
        osapi::sleep(WATCH_DELAY_MS);
    }
}

int SystemManager::ListenerThread::init_inot(){
    inotify_fd = inotify_init();
    if(inotify_fd < 0){
        printf("Error initializing inotify instance\n");
    }
    printf("%d\n", inotify_fd);

    if(fcntl(inotify_fd, F_SETFL, O_NONBLOCK) < 0){
        printf("fcntl error\n");
    }
    // IN_CLOSE_WRITE signifies that a file that was open for writing has been closed
    // IN_MODIFY signifies a file has been written to (or otherwise modified)
    // See more about masks on https://man7.org/linux/man-pages/man7/inotify.7.html
    inotify_watch_fd = inotify_add_watch(inotify_fd, PATHNAME, IN_MODIFY | IN_CLOSE_WRITE);
    
    if(inotify_watch_fd < 0){
        printf("Error initializing inotify watch\n");
    }

    printf("Inotify initialize finished\n");
    return 0;

}