#pragma once

#include "restinio/all.hpp"
#include <restinio/websocket/websocket.hpp>
#include <map>
#include "string.h"

namespace rr = restinio::router;
namespace rws = restinio::websocket::basic;

// Replies for treat requests 
#define REPLY_UNDEF "no reply yet"
#define REPLY_DENY  "deny"
#define REPLY_ALLOW "allow"

// For such a small project the id, ws connection handle and reply data can be stored in the same struct
struct fgSocketData_t{
    int fg_id;
    rws::ws_handle_t ws_handle;
    std::string reply;
};

using router_t = rr::express_router_t<>;
using ws_handle = restinio::websocket::basic::ws_handle_t;
using ws_registry_t = std::map<std::uint64_t, fgSocketData_t>;


class FgHandler{
public:
    explicit FgHandler()
    {}

    FgHandler(const FgHandler &) = delete;
    FgHandler(FgHandler &&) = delete;

    auto onPostReqTreat(const restinio::request_handle_t& req, rr::route_params_t);
    auto onGetCheckId(const restinio::request_handle_t& req, rr::route_params_t params);
    auto onGetReqTreat(const restinio::request_handle_t& req, rr::route_params_t params);
    auto on_update(const restinio::request_handle_t& req, rr::route_params_t params) const;
    auto onWebsocketConnect(const restinio::request_handle_t& req, rr::route_params_t params);
    auto options(restinio::request_handle_t req, restinio::router::route_params_t);


private:

    std::string handleWsMessage(ws_handle wsh, std::string payload);
    ws_registry_t m_registry;

    void sendMessage(int fg_id, std::string message){
        for(auto [k,v] : m_registry){
            if(v.fg_id == fg_id){
                v.ws_handle->send_message(rws::final_frame, rws::opcode_t::text_frame, message);
            }
        }     
    }
    int append_data(restinio::response_builder_t<restinio::restinio_controlled_output_t> &resp, int i = -1) const;

    template <typename RESP>
    static RESP
    init_resp(RESP resp)
    {
        resp
            .append_header("Server", "Foodgiver Server /v.0.9")
            .append_header_date_field()
            .append_header("Content-Type", "application/json")
            .append_header(restinio::http_field::access_control_allow_origin, "*");

            return resp;
    }

    template <typename RESP>
    static void
    mark_as_bad_request (RESP& resp)
    {
        resp.header().status_line(restinio::status_bad_request());
    }
};

// Might need to refactor these
auto server_handler();

int run();