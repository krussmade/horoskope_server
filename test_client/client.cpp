#include "../libs/httplib.h"
#include <string>
#include <fstream>

int main() {
    httplib::Client client("localhost", 6666);
    std::string sign_sun_req = "{\"dateOfBirth\": \"2002-05-23\"}";

    std::string horoscope_basics_req = "{\"dateOfBirth\": \"2003-01-21\",\"period\": 1}";

    std::string horoscope_compatibility = "{\"manSign\": 2, \"womanSign\": 6}";

    auto sign_sun_result = client.Post("/sign/sun/",
                                       sign_sun_req.c_str(), sign_sun_req.size(), "application\\json");

    auto horoscope_basics_result = client.Post("/horoscope/basic/",
                                               horoscope_basics_req.c_str(), horoscope_basics_req.size(),
                                               "application\\json");

    auto horoscope_compatibility_result = client.Post("/horoscope/compatibility/",
                                                      horoscope_compatibility.c_str(), horoscope_compatibility.size(),
                                                      "application\\json");

    std::ofstream sign_sun_response("sign_sun_response.txt");
    sign_sun_response << sign_sun_result->body;
    sign_sun_response.close();

    std::ofstream horoscope_basics_response("horoscope_basics_response.txt");
    horoscope_basics_response << horoscope_basics_result->body;
    horoscope_basics_response.close();

    std::ofstream horoscope_compatibility_response("horoscope_compatibility_response.txt");
    horoscope_compatibility_response << horoscope_compatibility_result->body;
    horoscope_compatibility_response.close();
    return 0;
}

