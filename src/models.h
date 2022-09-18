#ifndef HOROSCOPE_SERVER_MODELS_H
#define HOROSCOPE_SERVER_MODELS_H

#include <string>

namespace horoscope {

    struct base_model {
        int status{};
        std::string message{};
    };

    struct prediction : public base_model {
        std::string text{};
    };

    struct sign_info : public base_model {
        std::string sign_name{};
        std::string sign_element{};
        std::string sign_desk{};
    };

    struct compatibility_info : public base_model {
        int percentage{};
        std::string happiness_in_marriage{};
        std::string good_luck_compatibility{};
        std::string sexual_compatibility{};
        std::string family_compatibility{};
        std::string for_kids_compatibility{};
    };

}


#endif //HOROSCOPE_SERVER_MODELS_H
