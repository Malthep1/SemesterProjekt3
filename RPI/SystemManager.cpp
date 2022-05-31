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
    //cmdCtrl.setupCommunicationModules();
    //cmdCtrl.getTreatRequestAnswer();
    //std::cout << "AFTER PSOC LISTENER\n";

        
    while(true){    

        std::cout << "MAIN LOOP ONLINE\n";
        osapi::sleep(5000);
        std::cout << "DISPATCHING COMMAND\n";
        cmdCtrl.dispatchCommand();
        std::cout << "COMMAND DISPATCHED\n";
        //cmdCtrl.requestTreat();
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
    TimeThread timeThread(cs, cmdCtrl.getMessageQueue());   
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
                //SPLIT CSV
                std::string settingString(buffer);
                boost::split(settings, settingString, boost::is_any_of(","));
                //LOAD INTO SETTINGS STRUCT
                //KILL TIMER THREAD AND START A NEW ONE

            }
            
            i = i + EVENT_SIZE + event->len;
        }
        
        i = 0;
        osapi::sleep(WATCH_DELAY_MS);
    }
}

void SystemManager::TimeThread::passToCommandController(std::string command){
    CommandString* msg = new CommandString;
    msg->response = command;
    msgQueue->send(123123, msg);
}

void SystemManager::TimeThread::run(){
    std::cout << "TIME THREAD ONLINE\n";
    std::cout << "CURRENT SETTING : { Hour: " << std::to_string(curSet->feedingHour) << " Minute: " << std::to_string(curSet->feedingMinute) << " } \n";
    
    time_t t = time(NULL);
    struct tm *tmp = gmtime(&t);    
    bool isFeedingTime = false;
    std::cout << "Hour: " << std::to_string(tmp->tm_hour) << " Minute: " << std::to_string(tmp->tm_min) << "\n";
    while(!isFeedingTime){
        t = time(NULL);
        tmp = gmtime(&t);
        if(curSet->feedingHour == tmp->tm_hour){
            if(curSet->feedingMinute == tmp->tm_min){
                std::cout << "Bingo \n";
                isFeedingTime = true;
            }
        }
        osapi::sleep(30000);
        std::cout << "{ Hour: " << std::to_string(tmp->tm_hour) << " Minute: " << std::to_string(tmp->tm_min) << " } \n";
    }
    std::cout << "FEEDING TIME\n";
    string food = "F";
    string amount = std::to_string(curSet->foodAmount);
    passToCommandController(food+amount);
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