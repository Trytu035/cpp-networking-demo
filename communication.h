#pragma once
// #include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <string>

typedef struct query{
    enum action_type{GET, SET} action;
    int key;
    std::string password;

    query(){
        password = "";
        key = 0;
        action = action_type::GET;
    }

    query(action_type action, int key){
        query::password = "";
        query::key = key;
        query::action = action;
    }

    query(action_type action, int key, std::string password){
        query::password = password;
        query::key = key;
        query::action = action;
    }
} query;
NLOHMANN_JSON_SERIALIZE_ENUM(query::action_type, {
    {query::action_type::GET, "get"},
    {query::action_type::SET, "set"},
})
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(query, action, key)

typedef struct answer{
    enum status_type{ERR, OK, CREATED} status;
    int key;
    bool exist;
    std::string message;

    answer(){
        exist = false;
        key = 0;
        message = "";
        status = status_type::ERR;
    }

    answer(status_type status, int key, bool exist = false, std::string message = ""){
        answer::exist = exist;
        answer::key = key;
        answer::message = message;
        answer::status = status;
    }
} answer;
NLOHMANN_JSON_SERIALIZE_ENUM( answer::status_type, {
    {answer::status_type::ERR, "error"},
    {answer::status_type::OK, "ok"},
    {answer::status_type::CREATED, "created"},
})

namespace nlohmann {
    template <>
    struct adl_serializer<answer> {
        static void to_json(json& j, const answer& opt) {
            switch (opt.status){
                case answer::status_type::ERR:
                    j["status"] = opt.status;
                    j["message"] = opt.message;
                    break;
                case answer::status_type::OK:
                    j["status"] = opt.status;
                    j["key"] = opt.key;
                    j["exist"] = opt.exist;
                    break;
                case answer::status_type::CREATED:
                    j["status"] = opt.status;
                    j["key"] = opt.key;
                    j["message"] = opt.message;
                    break;
                default:
                    j["status"] = answer::status_type::ERR;
                    j["message"] = "serialization error";
            }
        }

        static void from_json(const json& j, answer& opt) {
            opt = {};
            switch(j["status"].get<answer::status_type>()){
                case answer::status_type::ERR:
                    j["status"].get_to(opt.status);
                    j["message"].get_to(opt.message);
                    break;
                case answer::status_type::OK:
                    j["status"].get_to(opt.status);
                    j["key"].get_to(opt.key);
                    j["exist"].get_to(opt.exist);
                    break;
                case answer::status_type::CREATED:
                    j["status"].get_to(opt.status);
                    j["key"].get_to(opt.key);
                    j["message"].get_to(opt.message);
                    break;
                default:
                    opt.status = answer::status_type::ERR;
                    opt.message = "deserialization error";
            }
        }
    };
}