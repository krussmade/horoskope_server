#include "repository.h"

namespace horoscope {
    repository::repository()
            :
            db_(DB_PATH),
            client_(HORO_MAIL_SERVER) {
        sqlitepp::query predictions_query(db_, "CREATE TABLE IF NOT EXISTS predictions_db "
                                               "(id INT,today_date VARCHAR, today_prediction VARCHAR, "
                                               "tomorrow_date VARCHAR, tomorrow_prediction VARCHAR, "
                                               "week_date VARCHAR, week_prediction VARCHAR,"
                                               "month_date VARCHAR, month_prediction VARCHAR,"
                                               "year_date VARCHAR, year_prediction VARCHAR);");

        sqlitepp::query signs_query(db_, "CREATE TABLE IF NOT EXISTS signs_info "
                                         "(id INT, sign_name VARCHAR, sign_element VARCHAR, sign_desk VARCHAR);");

        sqlitepp::query compatibility_query(db_, "CREATE TABLE IF NOT EXISTS compatibility_db "
                                                 "(id INT, percentage INT, "
                                                 "happiness_in_marriage VARCHAR, "
                                                 "good_luck_compatibility VARCHAR, "
                                                 "sexual_compatibility VARCHAR, "
                                                 "family_compatibility VARCHAR, "
                                                 "for_kids_compatibility VARCHAR);");
        signs_query.exec();
        predictions_query.exec();
        compatibility_query.exec();
    }

    void repository::getSignInfo(date date_of_birth, sign_info *result) {
        zodiac_sign sign = date_of_birth.to_zodiac_sign();
        sqlitepp::query query(db_, "SELECT * FROM signs_info WHERE id = " + std::to_string(sign.sign) + ";");
        sqlitepp::result res = query.store();
        result->sign_name.append(res[0]["sign_name"]);
        result->sign_element.append(res[0]["sign_element"]);
        result->sign_desk.append(res[0]["sign_desk"]);
    }

    void
    repository::getPrediction(date date_of_birth, date_category category_of_date, prediction *result) {
        this->predictionParse(date_of_birth, category_of_date, result);
    }

    void repository::predictionParse(date date_of_birth, date_category category_of_date, prediction *result) {
        zodiac_sign sign = date_of_birth.to_zodiac_sign();
        auto result_by_server = client_.Get("/prediction/" + sign.to_str() + "/" + category_of_date.to_str() + "/");
        std::string html_code = result_by_server->body;
        HtmlParser parser;
        shared_ptr<HtmlDocument> doc = parser.Parse(html_code.c_str(), html_code.size());
        std::vector<shared_ptr<HtmlElement>> elements_by_class_name = doc->GetElementByClassName(
                "article__item article__item_alignment_left article__item_html");
        for (int i = 0; i < elements_by_class_name.size(); ++i) {
            for (auto j = elements_by_class_name[i]->ChildBegin(); j != elements_by_class_name[i]->ChildEnd(); ++j) {
                for (auto k = (*j)->ChildBegin(); k != (*j)->ChildEnd(); ++k) {
                    result->text += (*k)->GetValue() + "\n";
                }
            }
        }
    }

    void repository::getCompatibility(zodiac_sign man_sign, zodiac_sign woman_sign, compatibility_info *result) {
        int combination = compatibility(woman_sign, man_sign);
        sqlitepp::query query(db_, "SELECT * FROM compatibility_db WHERE id = " + std::to_string(combination) + ";");
        sqlitepp::result res = query.store();
        if (res.empty()) {
            this->parseCompatibility(man_sign, woman_sign, result);
            std::string query_str = "INSERT INTO compatibility_db "
                                    "(id, "
                                    "percentage, "
                                    "happiness_in_marriage, "
                                    "good_luck_compatibility, "
                                    "sexual_compatibility, "
                                    "family_compatibility, "
                                    "for_kids_compatibility) "
                                    "VALUES( '" + std::to_string(combination) + "', "
                                    + "'" + std::to_string(result->percentage) + "', "
                                    + "'" + result->happiness_in_marriage + "', "
                                    + "'" + result->good_luck_compatibility + "', "
                                    + "'" + result->sexual_compatibility + "', "
                                    + "'" + result->family_compatibility + "', "
                                    + "'" + result->for_kids_compatibility + "');";
            sqlitepp::query query(db_, query_str);
            query.exec();
        } else {
            result->percentage = res[0]["percentage"];
            result->happiness_in_marriage.append(res[0]["happiness_in_marriage"]);
            result->good_luck_compatibility.append(res[0]["good_luck_compatibility"]);
            result->sexual_compatibility.append(res[0]["sexual_compatibility"]);
            result->family_compatibility.append(res[0]["family_compatibility"]);
            result->for_kids_compatibility.append(res[0]["for_kids_compatibility"]);
        }
    }

    void repository::parseCompatibility(zodiac_sign man_sign, zodiac_sign woman_sign, compatibility_info *result) {
        int combination = compatibility(woman_sign, man_sign);
        auto result_by_server = client_.Get("/compatibility/zodiac/" + std::to_string(combination) + "/");
        std::string html_code = result_by_server->body;
        HtmlParser parser;
        shared_ptr<HtmlDocument> doc = parser.Parse(html_code.c_str(), html_code.size());
        std::vector<shared_ptr<HtmlElement>> compatibility_items = (*doc->GetElementByClassName(
                "article__item article__item_alignment_left article__item_html").front()).GetElementByTagName("p");

        result->happiness_in_marriage = (*(compatibility_items[0]->ChildBegin()))->GetValue();
        result->good_luck_compatibility = (*(compatibility_items[1]->ChildBegin()))->GetValue();
        result->sexual_compatibility = (*(compatibility_items[2]->ChildBegin()))->GetValue();
        result->family_compatibility = (*(compatibility_items[3]->ChildBegin()))->GetValue();
        result->for_kids_compatibility = (*(compatibility_items[4]->ChildBegin()))->GetValue();

        std::vector<shared_ptr<HtmlElement>> percentage = doc->GetElementByClassName(
                "p-item__left-icon-text");

        std::string percentage_str = (*(percentage.front()->ChildBegin()))->GetValue();
        percentage_str.pop_back();

        result->percentage = std::stoi(percentage_str);
    }
}