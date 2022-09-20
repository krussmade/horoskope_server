#include <iostream>
#include "src/utils.h"
#include "src/repository.h"
#include "libs/httplib.h"
#include "libs/html_parser.hpp"

#include "src/config.h"
#include <fstream>


#include "libs/sqlite/sqlite3.h"

#include "libs/json.hpp"

int main() {
    horoscope::repository rep;
    httplib::Server server;

//    horoscope::sign_info resulte;
//    rep.getSignInfo(horoscope::date{2000, 10, 10}, &resulte);
//
//    horoscope::compatibility_info compatibil;
//    rep.getCompatibility({horoscope::zodiac_sign::zodiac_signs::ARIES}, {horoscope::zodiac_sign::zodiac_signs::ARIES}, &compatibil);
//
//    horoscope::prediction prediction_model;
//    rep.getPrediction({2003, 1, 21}, {horoscope::date_category::date_categories::YEAR}, &prediction_model);

    server.Post("sign/sun/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::date date = horoscope::date::parse(json_tree["dateOfBirth"].get<std::string>());
        horoscope::sign_info sign_info_model;
        rep.getSignInfo(date, &sign_info_model);
        nlohmann::json result_tree;
        result_tree["status"] = sign_info_model.status;
        result_tree["message"] = std::move(sign_info_model.message);
        result_tree["data"]["signName"] = std::move(sign_info_model.sign_name);
        result_tree["data"]["signElement"] = std::move(sign_info_model.sign_element);
        result_tree["data"]["signDesc"] = std::move(sign_info_model.sign_desk);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.Post("horoscope/basic/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::date date = horoscope::date::parse(json_tree["dateOfBirth"].get<std::string>());
        horoscope::date_category category = horoscope::date_category::parse(json_tree["onDate"].get<std::string>());

        horoscope::prediction prediction_model;
        rep.getPrediction(date, category, &prediction_model);

        nlohmann::json result_tree;
        result_tree["status"] = prediction_model.status;
        result_tree["message"] = std::move(prediction_model.message);
        result_tree["data"]["text"] = std::move(prediction_model.text);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.Post("horoscope/compatibility/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::zodiac_sign man_sign{horoscope::zodiac_sign::zodiac_signs(json_tree["manSign"].get<int>())};
        horoscope::zodiac_sign woman_sign{horoscope::zodiac_sign::zodiac_signs(json_tree["womanSign"].get<int>())};

        horoscope::compatibility_info compatibility_info_model;
        rep.getCompatibility(man_sign, woman_sign, &compatibility_info_model);

        nlohmann::json result_tree;
        result_tree["status"] = compatibility_info_model.status;
        result_tree["message"] = std::move(compatibility_info_model.message);
        result_tree["data"]["percentage"] = compatibility_info_model.percentage;
        result_tree["data"]["happinessInMarriage"] = std::move(compatibility_info_model.happiness_in_marriage);
        result_tree["data"]["goodLuckCompatibility"] = std::move(compatibility_info_model.good_luck_compatibility);
        result_tree["data"]["sexualCompatibility"] = std::move(compatibility_info_model.sexual_compatibility);
        result_tree["data"]["familyCompatibility"] = std::move(compatibility_info_model.family_compatibility);
        result_tree["data"]["forKidsCompatibility"] = std::move(compatibility_info_model.for_kids_compatibility);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.listen(SERVER_IP, SERVER_PORT);
    return 0;
}
