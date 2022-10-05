#ifndef HOROSCOPE_SERVER_MODELS_H
#define HOROSCOPE_SERVER_MODELS_H

#include <string>
#include "../libs/json.hpp"

namespace horoscope {

    struct base_model {
        int status{};
        std::string message{};

        virtual void serialize(nlohmann::json &json) const {
            json["status"] = status;
            json["message"] = message;
        }
    };

    struct prediction : public base_model {
        std::string text{};

        void serialize(nlohmann::json &json) const override {
            base_model::serialize(json);
            json["data"]["text"] = text;
        }
    };

    struct sign_info : public base_model {
        std::string sign_name{};
        std::string sign_element{};
        std::string sign_desk{};

        void serialize(nlohmann::json &json) const override {
            base_model::serialize(json);
            json["data"]["signName"] = sign_name;
            json["data"]["signElement"] = sign_element;
            json["data"]["signDesc"] = sign_desk;
        }
    };

    struct compatibility_info : public base_model {
        int percentage{};
        std::string happiness_in_marriage{};
        std::string good_luck_compatibility{};
        std::string sexual_compatibility{};
        std::string family_compatibility{};
        std::string for_kids_compatibility{};

        void serialize(nlohmann::json &json) const override {
            base_model::serialize(json);
            json["data"]["percentage"] = percentage;
            json["data"]["happinessInMarriage"] = happiness_in_marriage;
            json["data"]["goodLuckCompatibility"] = good_luck_compatibility;
            json["data"]["sexualCompatibility"] = sexual_compatibility;
            json["data"]["familyCompatibility"] = family_compatibility;
            json["data"]["forKidsCompatibility"] = for_kids_compatibility;
        }
    };

}


#endif //HOROSCOPE_SERVER_MODELS_H
