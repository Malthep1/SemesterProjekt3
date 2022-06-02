#include "WebComm.hpp"

void WebComm::createWebCommListenerThread(MsgQueue* msgQueue){
    id++;
    WebCommListenerThread * lt = new WebCommListenerThread(msgQueue, id);
    Thread* thread_listener = new Thread(lt); 
    threadStruct tStruct = {
        lt,
        thread_listener
    };
    threadPool[id] = tStruct;
    thread_listener->start();
    std::cout << "Started WebCommListenerThread, ID = " << std::to_string(id) <<"\n";
}

void WebComm::WebCommListenerThread::handleResponse(std::string response){
    responseString* msg = new responseString;
    if(response.find("allow")){
        string resp("TA");
        msg->response = resp;
        msgQueue_->send(3, msg);
    }
    else if(response.find("deny")){
        string resp("TD");
        msg->response = resp;
        msgQueue_->send(3, msg);
    }
    osapi::sleep(1000);
    responseString* msgDelete = new responseString;
    std::cout << "delete thread\n";
    string resp(std::to_string(id));
    msgDelete->response = resp;
    msgQueue_->send(9, msgDelete);
}

void WebComm::deleteThread(int id){
    threadStruct ts = threadPool[id];
    delete ts.tp;
    delete ts.wchread;
}

void WebComm::WebCommListenerThread::run(){
    
    string answer("UNDEF");
    int i = 0;
    while(answer == "UNDEF"){
        std::cout << "WEBCOMMLISTENER: Checking for answer\n";
        if(i == 10){
            break;
        }
        answer = getRequestAnswer();
        std::cout << answer;
        if(answer.find("allow" || answer.find("deny"))){
            handleResponse(answer);
        }
        i++;
        if(i == 7){
            break;
        }
        osapi::sleep(10000);
    }
}

void WebComm::postTreatRequest(int devid){
    try{
        http::Request request{"http://foodgiver.hopto.org:8080/req_treat/420"};
        const string body = "foo=test";
        const auto response = request.send("POST", body, {
        {"Content-Type", "application/x-www-form-urlencoded"}
    });
        std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
    }
    catch (const std::exception& e){
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
}

void WebComm::postNotification(string type){
    //TODO
}

string WebComm::WebCommListenerThread::getRequestAnswer(){
    string undef("UNDEF");
    try{
        http::Request req{"http://foodgiver.hopto.org:8080/req_treat/420"};
        const auto response = req.send("GET");
        std::string resp = std::string{response.body.begin(), response.body.end()} + '\n'; // Save request to variable
        if(resp.find("deny")){
            return resp;
        }
        else if(resp.find("allow")){
            return resp;
        }
        else {
            return undef;
        }
    }
    catch (const std::exception& e){    
        std::cerr << "Request failed, error: " << e.what() << '\n';
        return undef;
    }
}