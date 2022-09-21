#ifndef HOROSCOPE_SERVER_UTILS_H
#define HOROSCOPE_SERVER_UTILS_H

#include "string"
#include <ctime>

namespace horoscope {

    struct date_category {
        enum date_categories {
            ERROR = 0,
            TODAY = 1,
            TOMORROW,
            WEEK,
            MONTH,
            YEAR
        };

        date_categories category;

        std::string to_str() {
            switch (category) {
                case TODAY:
                    return "today";
                case TOMORROW:
                    return "tomorrow";
                case WEEK:
                    return "week";
                case MONTH:
                    return "month";
                case YEAR:
                    return "year";
                case ERROR:
                    return {};
            }
        }
    };


    struct zodiac_sign {
        enum zodiac_signs {
            ERROR = 0,
            ARIES = 1,
            TAURUS,
            GEMINI,
            CANCER,
            LEO,
            VIRGO,
            LIBRA,
            SCORPION,
            SAGITTARIUS,
            CAPRICORN,
            AQUARIUS,
            PISCES,
        };

        zodiac_signs sign;

        std::string to_str() {
            switch (sign) {
                case ARIES:
                    return "aries";
                    break;
                case TAURUS:
                    return "taurus";
                    break;
                case GEMINI:
                    return "gemini";
                    break;
                case CANCER:
                    return "cancer";
                    break;
                case LEO:
                    return "leo";
                    break;
                case VIRGO:
                    return "virgo";
                    break;
                case LIBRA:
                    return "libra";
                    break;
                case SCORPION:
                    return "scorpion";
                    break;
                case SAGITTARIUS:
                    return "sagittarius";
                    break;
                case CAPRICORN:
                    return "capricorn";
                    break;
                case AQUARIUS:
                    return "aquarius";
                    break;
                case PISCES:
                    return "pisces";
                    break;
                case ERROR:
                    return {};
                    break;
            }
        }
    };


    struct date {
        int year;
        int month;
        int day;

        static date parse(const std::string &date_str) {
            date result;
            std::string temp;
            int index = 0;
            // year
            while (date_str[index] != '-') {
                temp += date_str[index++];
            }
            index += 1;
            result.year = std::stoi(temp);
            temp.clear();

            // month
            while (date_str[index] != '-') {
                temp += date_str[index++];
            }
            index += 1;
            result.month = std::stoi(temp);
            temp.clear();

            // day
            while (date_str[index] != 'T' && index < date_str.length()) {
                temp += date_str[index++];
            }
            index += 1;
            result.day = std::stoi(temp);
            temp.clear();
            return result;
        }

        bool yearEquality(const date &other) const {
            return year == other.year;
        }

        bool monthEquality(const date &other) const {
            return year == other.year && month == other.month;
        }

        bool weekEquality(const date &other) const {
            return year == other.year && month == other.month && day / 7 == other.day / 7;
        }

        bool dayEquality(const date &other) const {
            return year == other.year && month == other.month && day == other.day;
        }

        std::string to_str() const {
            return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
        }

        static date getCurrentDate() {
            date result;
            time_t raw_time;
            tm *time_info;
            time(&raw_time);
            time_info = localtime(&raw_time);

            result.day = time_info->tm_mday;
            result.month = time_info->tm_mon;
            result.year = time_info->tm_year;

            return result;
        }

        zodiac_sign toZodiacSign() {
            if ((month == 3 && day >= 21) || (month == 4 && day <= 20)) {
                return zodiac_sign{zodiac_sign::ARIES};
            } else if ((month == 4 && day >= 21) || (month == 5 && day <= 21)) {
                return zodiac_sign{zodiac_sign::TAURUS};
            } else if ((month == 5 && day >= 22) || (month == 6 && day <= 21)) {
                return zodiac_sign{zodiac_sign::GEMINI};
            } else if ((month == 6 && day >= 22) || (month == 7 && day <= 22)) {
                return zodiac_sign{zodiac_sign::CANCER};
            } else if ((month == 7 && day >= 23) || (month == 8 && day <= 21)) {
                return zodiac_sign{zodiac_sign::LEO};
            } else if ((month == 8 && day >= 22) || (month == 9 && day <= 23)) {
                return zodiac_sign{zodiac_sign::VIRGO};
            } else if ((month == 9 && day >= 24) || (month == 10 && day <= 23)) {
                return zodiac_sign{zodiac_sign::LIBRA};
            } else if ((month == 10 && day >= 24) || (month == 11 && day <= 22)) {
                return zodiac_sign{zodiac_sign::SCORPION};
            } else if ((month == 11 && day >= 23) || (month == 12 && day <= 22)) {
                return zodiac_sign{zodiac_sign::SAGITTARIUS};
            } else if ((month == 12 && day >= 23) || (month == 1 && day <= 20)) {
                return zodiac_sign{zodiac_sign::CAPRICORN};
            } else if ((month == 1 && day >= 21) || (month == 2 && day <= 19)) {
                return zodiac_sign{zodiac_sign::AQUARIUS};
            } else if ((month == 2 && day >= 20) || (month == 3 && day <= 20)) {
                return zodiac_sign{zodiac_sign::PISCES};
            }
            return zodiac_sign{zodiac_sign::ERROR};
        }
    };

    inline int compatibility(zodiac_sign woman_zodiac_num, zodiac_sign man_zodiac_num) {
        return ((woman_zodiac_num.sign - 1) * 12) + man_zodiac_num.sign;
    }
}
#endif //HOROSCOPE_SERVER_UTILS_H
