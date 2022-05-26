#include "WebComm.hpp"

void WebComm::createWebCommListenerThread(MsgQueue* msgQueue){
    WebCommListenerThread lt(msgQueue);
    Thread* thread_listener = new Thread(&lt); 
    thread_listener->start();
    std::cout << "Started WebCommListenerThread\n";
    //thread_listener->detach();
    sleep(1);
}

void WebComm::WebCommListenerThread::handleResponse(std::string response){
    responseString* msg = new responseString;
    if(response == "allow"){
        string resp("STAE");
        msg->response = resp;
        msgQueue_->send(TA, msg);
    }
    else if(response == "denied"){
        string resp("STDE");
        msg->response = resp;
        msgQueue_->send(TD, msg);
    }
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
        if(answer != "UNDEF"){
            handleResponse(answer);
        }
        i++;
        sleep(5);
    }
    std::cout << "Broken!!\n";
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
        std::string resp = std::string{response.body.begin(), response.body.end()} + '\n'; // print the result
        if(resp == "allow" || resp == "denied"){
            std::cout << "BINGO\n";
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