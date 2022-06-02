    #include "SystemManager.hpp"
    // 
    #define PATHNAME "/home/stud/git/SemesterProjekt3/RPI/dat/settings.dat"
    #define WATCH_FLAGS                 IN_CLOSE_WRITE
    #define EVENT_SIZE                  (sizeof(struct inotify_event))
    #define WATCH_DELAY_MS               100
    #define BUFFER_SIZE                 500

    void SystemManager::runMain(){
        MainThread * mt = new MainThread();   
        mt_ = new Thread(mt);
        mt_->start();
        mt_->join();
    }

    void SystemManager::MainThread::listenSettingsUpdate(MsgQueue * msgQueue, Setting * cs){
        ListenerThread *listener_ = new ListenerThread(cs, msgQueue);   
        lt_ = new Thread(listener_);
        lt_->start();
        std::cout << "SettingsListener Started\n";
    }

    void SystemManager::MainThread::waitFeedingTime(MsgQueue * msgQueue, Setting * cs){
        TimeThread* timeThread = new TimeThread(cs, msgQueue);   
        tt_ = new Thread(timeThread);
        //
        tt_->start();
        std::cout << "TimeThread Started\n";
    }
    //main Thread run()
    void SystemManager::MainThread::run(){
        
        //CommandController
        CommandController cmdCtrl;

        //Settings Struct
        Setting currentSetting = {
            12,      // Hour
            42,     // Minute
            100,    // Amount
            3,      // TreatLimit
            true,   // Enable Requests
            true    // Enable Treats
        };
        //Create Threads
        listenSettingsUpdate(cmdCtrl.getMessageQueue(), &currentSetting);
        waitFeedingTime(cmdCtrl.getMessageQueue(), &currentSetting);

        std::cout << "MAIN LOOP ONLINE\n";

        std::cout << "Requesting Treat\n";
        cmdCtrl.requestTreat();
        osapi::sleep(2000);
        cmdCtrl.getTreatRequestAnswer();
        //main loop
        
        while(true){
            cmdCtrl.dispatchCommand();
        }
    }

    // Check https://linuxhint.com/inotify_api_c_language/ for good example of using inotify
    void SystemManager::MainThread::ListenerThread::run(){
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
                    
                    //MSQUEUE TEST
                    std::cout << "TESTING QUEUE\n";
                    std::string test("TEST");
                    UartString* msg = new UartString;
                    msg->response = test;
                    msgQueue->send(2, msg);
                    std::cout << "MESSAGE SENT\n";
                }
                
                i = i + EVENT_SIZE + event->len;
            }
            
            i = 0;
            osapi::sleep(WATCH_DELAY_MS);
        }
    }


    void SystemManager::MainThread::TimeThread::passToCommandController(std::string command){
        std::cout << "CREATING MESSAGE\n";
        UartString* msg = new UartString;
        std::cout << "SETTING RESPONSE\n";
        msg->response = command;
        std::cout << "SENDING MESSAGE\n";
        msgQueue->send(1, msg);
        std::cout << "MESSAGE SENT\n";
    }

    void SystemManager::MainThread::TimeThread::run(){

        while(true){
            std::cout << "TIME THREAD STARTING\n";
            int feedingHour = curSet->feedingHour;
            int feedingMinute = curSet->feedingMinute;
            int foodAmount = curSet->foodAmount;
            std::cout << "CURRENT SETTING : { Hour: " << std::to_string(feedingHour) << " Minute: " << std::to_string(feedingMinute)<< " Amount: " << std::to_string(foodAmount) << " } \n";
        
            time_t t = time(NULL);
            struct tm *tmp = gmtime(&t);    
            bool isFeedingTime = false;
            int currentHour = tmp->tm_hour;
            int currentMinute = tmp->tm_min;
        
            while(!isFeedingTime){

                t = time(NULL);
                tmp = gmtime(&t);
                currentHour = tmp->tm_hour;
                currentMinute = tmp->tm_min;
                std::cout << "{ Hour: " << std::to_string(tmp->tm_hour) << " Minute: " << std::to_string(tmp->tm_min) << " } \n";
                if(feedingHour == currentHour){
                    if(feedingMinute == currentMinute){
                        isFeedingTime = true;
                    }
                }
                if(!isFeedingTime){
                    osapi::sleep(30000);
                }
            }
            std::cout << "FEEDING TIME\n";
            string food = "F";
            string amountString = std::to_string(foodAmount);
            passToCommandController(food+amountString);
            osapi::sleep(60000);    
        }

    }

    int SystemManager::MainThread::ListenerThread::init_inot(){
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