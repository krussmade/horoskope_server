#include <iostream>
#include "src/utils.h"
#include "src/repository.h"
#include "libs/httplib.h"
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

    server.Post("/sign/sun/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::date date = horoscope::date::parse(json_tree["dateOfBirth"].get<std::string>());
        horoscope::sign_info sign_info_model;
        rep.getSignInfo(date, &sign_info_model);
        nlohmann::json result_tree;
        sign_info_model.serialize(result_tree);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.Post("/horoscope/basic/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::date date = horoscope::date::parse(json_tree["dateOfBirth"].get<std::string>());
        horoscope::date_category category{horoscope::date_category::date_categories(json_tree["period"].get<int>())};

        horoscope::prediction prediction_model;
        rep.getPrediction(date, category, &prediction_model);

        nlohmann::json result_tree;
        prediction_model.serialize(result_tree);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.Post("/horoscope/compatibility/", [&](const httplib::Request &req, httplib::Response &res) {
        nlohmann::json json_tree = nlohmann::json::parse(req.body);
        horoscope::zodiac_sign man_sign{horoscope::zodiac_sign::zodiac_signs(json_tree["manSign"].get<int>())};
        horoscope::zodiac_sign woman_sign{horoscope::zodiac_sign::zodiac_signs(json_tree["womanSign"].get<int>())};

        horoscope::compatibility_info compatibility_info_model;
        rep.getCompatibility(man_sign, woman_sign, &compatibility_info_model);

        nlohmann::json result_tree;
        compatibility_info_model.serialize(result_tree);
        res.set_content(result_tree.dump(), "text/plain");
    });

    server.listen(SERVER_IP, SERVER_PORT);
    return 0;
}
