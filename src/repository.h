#ifndef HOROSCOPE_SERVER_REPOSITORY_H
#define HOROSCOPE_SERVER_REPOSITORY_H

#include <string>
#include "config.h"
#include "utils.h"
#include "models.h"
#include "../libs/sqlitepp.h"
#include "../libs/httplib.h"
#include "../libs/html_parser.hpp"

namespace horoscope {
    class repository {
        sqlitepp::db db_;
        httplib::Client client_;

    public:
        repository();

        void getSignInfo(date, sign_info *);

        void getPrediction(date, date_category, prediction *);

        void getCompatibility(zodiac_sign, zodiac_sign, compatibility_info *);

    private:
        void predictionParse(date, date_category, prediction *);

        void parseCompatibility(zodiac_sign, zodiac_sign, compatibility_info *);
    };
}


#endif //HOROSCOPE_SERVER_REPOSITORY_H
