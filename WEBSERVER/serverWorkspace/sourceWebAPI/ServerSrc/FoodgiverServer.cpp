#include "FoodgiverServer.hpp"

// Simple restinio server
// Updated to use websockets!



using traits_t = 
            restinio::traits_t<
                restinio::asio_timer_manager_t,
                restinio::single_threaded_ostream_logger_t,
                router_t>; 

/* This handler checks if a connection with dev_id written in parameter is present
*  and sends back an appropriate message */
 auto FgHandler::onGetCheckId(const restinio::request_handle_t& req, rr::route_params_t params){
     auto resp = init_resp(req->create_response());
     
     try{
        // Convert request parameter to an integer used to look for the correct websocket connection (mobile app)
        auto p_dev_id = restinio::utils::unescape_percent_encoding(params["dev_id"]);
        int dev_id = std::stoi(p_dev_id);
        bool found = false;
        // Look for the correct entry in the websocket connection registry
        for(auto [k, v] : m_registry){
            if(v.fg_id == dev_id){
                resp.set_body("Mobile app present\n");
                found = true;
            }
        }
        if(!found){
            resp.set_body("Mobile app not found");
        }  
     }
     catch(const std::exception &)
    {
        mark_as_bad_request(resp);
    }
    return resp.done();
 }

/* This handler checks if a connection with dev_id written in parameter is present
*  If so, it sends a treat request message. Otherwise sends "No mobile app found" response */
auto FgHandler::onPostReqTreat(const restinio::request_handle_t& req, rr::route_params_t params){
    auto resp = init_resp(req->create_response());
try{
        auto p_dev_id = restinio::utils::unescape_percent_encoding(params["dev_id"]);
        int dev_id = std::stoi(p_dev_id);
        bool found = false;
        for(auto [k, v] : m_registry){
            if(v.fg_id == dev_id){
                found = true;
                rws::message_t msg(rws::final_frame, rws::opcode_t::text_frame);
                msg.set_payload("treat request");

                v.ws_handle->send_message(msg);
            }
            if(found == false){
                resp.set_body("No mobile app with this id found");
            }
        }
     }
     catch(const std::exception &)
    {
        mark_as_bad_request(resp);
    }
    return resp.done();
}
/* Websocket message handler: determines what to do after a message from 
*  mobile app has been received*/
std::string FgHandler::handleWsMessage(ws_handle wsh, std::string payload){
    std::cout << "PAYLOAD RECEIVED: " << payload << "\n";
    // Read 3 characters after '_' to figure out the fg_id : id of the device to set data for
    size_t pos = payload.find_first_of('_') + 1;
    int fg_id = std::stoi(payload.substr(pos, pos + 2));

    if(payload.find("auth") != std::string::npos){
        // If "auth" received, change connection fg_id to the number specified after '_' sign

        auto data = m_registry.find(wsh->connection_id());
        if(data != m_registry.end()){
            data->second.fg_id = fg_id;
        }
        std::cout << "Changed data of connection, fg_id : " << data->second.fg_id << "\n";
        std::string reply = payload;
        return reply;     
    }
    else if(payload.find("treat ack") != std::string::npos){
        auto data = m_registry.find(wsh->connection_id());
        if(data != m_registry.end()){
            data->second.reply = REPLY_ALLOW;
        }
        return("Treat acknowledges, relaying msg to device\n");
    }
    else if(payload.find("treat nack") != std::string::npos){
        auto data = m_registry.find(wsh->connection_id());
        if(data != m_registry.end()){
            data->second.reply = REPLY_DENY;
        }
        return("Treat not acknowledged, relaying msg to device\n");
    }
    else
        return(payload);
}

// This
auto FgHandler::onWebsocketConnect(const restinio::request_handle_t& req, rr::route_params_t params){
    if(restinio::http_connection_header_t::upgrade == req->header().connection())
    {
        std::cout << "RECEIVED WS\n";
        int fg_id;
        auto wsh = rws::upgrade<traits_t>(*req, rws::activation_t::immediate,
        [this](auto wsh, auto m){
            if(rws::opcode_t::text_frame == m->opcode() ||
                rws::opcode_t::binary_frame == m->opcode() ||
                rws::opcode_t::continuation_frame == m->opcode())
                {
                    std::string response = handleWsMessage(wsh, m->payload());
                    std::cout << response;
                    
                    m->set_payload(response);
                    wsh->send_message(*m);
                }
                else if(rws::opcode_t::ping_frame == m->opcode())
                {
                    std::cout << "ping frame\n";

                    auto resp = *m;
                    resp.set_opcode(rws::opcode_t::pong_frame);
                    wsh->send_message(resp);
                }
                else if(rws::opcode_t::connection_close_frame == m->opcode())
                {
                    std::cout << "close frame\n";
                    m_registry.erase(wsh->connection_id());
                }

        });
        fgSocketData_t data = {-1, wsh, REPLY_UNDEF};
        m_registry.emplace(wsh->connection_id(), data);

        init_resp(req->create_response()).done();

        return restinio::request_accepted();
    }
    return restinio::request_rejected();
}

// Unimplemented options , PATCH, DELETE, PUT
auto FgHandler::options(restinio::request_handle_t req, restinio::router::route_params_t){
    printf("request options called!!!\n");
    const auto methods = "OPTIONS, GET, POST";
    auto resp = init_resp(req->create_response());
    resp.append_header(restinio::http_field::access_control_allow_methods, methods);
    resp.append_header(restinio::http_field::access_control_allow_headers, "content-type");
    resp.append_header(restinio::http_field::access_control_max_age, "86400");
    return resp.done();
}




// Returns all weather data from a specific date
/*
auto FgHandler::on_get_data(const restinio::request_handle_t& req, rr::route_params_t params) const
{
    auto resp = init_resp(req->create_response());
    if(m_wData.empty()){
        resp.set_body("No weather data on the server!\n");
        return resp.done();
    }
    
    try{
        auto date = restinio::utils::unescape_percent_encoding(params["Date"]);

        resp.set_body("Weather data for the selected date: \n");

        for(unsigned int i = 0; i < m_wData.size(); i++){
            if(date == m_wData[i].date_){
                append_data(resp, i);
            }
        }
    }
    catch(const std::exception &)
    {
        mark_as_bad_request(resp);
    }
    return resp.done();
}
*/


// Binds the handler functions to HTTP requests!
auto server_handler()
{
    auto router = std::make_unique<router_t>();
    auto handler = std::make_shared<FgHandler>();

    auto by = [&] (auto method){
        using namespace std::placeholders;
        return std::bind(method, handler, _1, _2);
    };

    auto method_not_allowed = [] (const auto & req, auto){
        return req->create_response(restinio::status_method_not_allowed())
            .connection_close()
            .done();
    };

    // Handler for '/' path
    router->add_handler(restinio::http_method_options(), "/", by(&FgHandler::options));
    // router->add_handler(restinio::)
    
    router->http_get("/chat", by(&FgHandler::onWebsocketConnect));
    router->http_get("/check/:dev_id", by(&FgHandler::onGetCheckId));
    router->http_post("/req_treat/:dev_id", by(&FgHandler::onPostReqTreat));
    //router->http_get("/Date/:Date", by(&FgHandler::on_get_data));
    
    // Disable other methods for '/'
    router->add_handler(
        restinio::router::none_of_methods(
            restinio::http_method_get(), restinio::http_method_post()),
            "/", method_not_allowed);
    return router;
}

int run(){
    using namespace std::chrono;
    

    
try{    
        restinio::run(
            restinio::on_this_thread<traits_t>()
                .address( "192.168.1.20" )
                .request_handler(server_handler())
                .read_next_http_message_timelimit( 10s )
                .write_http_response_timelimit( 1s )
                .handle_request_timeout( 1s )
        );
    }
    catch( const std::exception & ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}