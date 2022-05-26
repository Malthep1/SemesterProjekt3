#include "SystemManager.hpp"
// 
#define PATHNAME "/home/stud/git/SemesterProjekt3/RPI/dat/settings.dat"
#define WATCH_FLAGS                 IN_CLOSE_WRITE
#define EVENT_SIZE                  (sizeof(struct inotify_event))
#define WATCH_DELAY_MS               100
#define BUFFER_SIZE                 500

void SystemManager::runMain(){

    //osapi::sleep(10000);
    //std::cout << "PSOC LISTENER\n";
    cmdCtrl.setupCommunicationModules();
    //cmdCtrl.getTreatRequestAnswer();
    //std::cout << "AFTER PSOC LISTENER\n";
    while(true){    
        std::cout << "main loop sending UART\n";
        unsigned char w[5];
        w[0] = 'W';
        w[1] = '1';
        w[2] = '0';
        w[3] = '0';
        w[4] = '\0';
        unsigned char f[5];
        f[0] = 'F';
        f[1] = '1';
        f[2] = '0';
        f[3] = '0';
        f[4] = '\0';
        unsigned char t[5];
        t[0] = 'T';
        t[1] = '0';
        t[2] = '2';
        t[3] = '0';
        t[4] = '\0';
        unsigned char* wptr = &w[0];
        cmdCtrl.dispatchUartCommand(wptr);
        osapi::sleep(10000);
        unsigned char* tptr = &t[0];
        cmdCtrl.dispatchUartCommand(tptr);
        osapi::sleep(10000);
        unsigned char* fptr = &f[0];
        cmdCtrl.dispatchUartCommand(fptr);
        osapi::sleep(10000);
        
        //std::cout << "Requesting Treat\n";
        //cmdCtrl.requestTreat();
        osapi::sleep(5000);
    }
}

void SystemManager::listenSettingsUpdate(){
    Setting * cs =  &currentSetting;
    ListenerThread listener_(cs);   
    lt_ = new Thread(&listener_);
    //
    lt_->start();

    // DEBUG:
    //lt_->detach();

    sleep(3);
}

void SystemManager::waitFeedingTime(){
    Setting * cs =  &currentSetting;
    TimeThread timeThread(cs);   
    tt_ = new Thread(&timeThread);
    //
    tt_->start();

    sleep(1);
}

// Check https://linuxhint.com/inotify_api_c_language/ for good example of using inotify
void SystemManager::ListenerThread::run(){
    printf("Made a new listener thread :)) \n");
    std::vector<string> settings;
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
                std::string settingString(buffer);
                boost::split(settings, settingString, boost::is_any_of(","));
            }
            
            i = i + EVENT_SIZE + event->len;
        }
        
        i = 0;
        osapi::sleep(WATCH_DELAY_MS);
    }
}

void SystemManager::TimeThread::run(){
    std::tm feedingTime, currentTime, timeDiff;
    time_t now = time(0);
    currentTime = *localtime(&now);
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