#include <crow.h>
#include <map>
#include <string>

std::map<std::string, std::string> parseBody(const std::string& body)
{
    std::map<std::string, std::string> parsedBody;
    std::istringstream ss(body);
    std::string token;

    while(std::getline(ss, token, '&')) {
        auto equalPos = token.find('=');
        if (equalPos != std::string::npos) {
            std::string key = token.substr(0, equalPos);
            std::string value = token.substr(equalPos+1);
            parsedBody[key] = value;
        }
    }

    return parsedBody;
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        .methods("GET"_method, "POST"_method)
        ([](const crow::request& req) {
            if (req.method == "GET"_method) {
                return crow::response(200,
                                      "<html><body>"
                                      "<h1>Simple Form</h1>"
                                      "<form action=\"/\" method=\"POST\">"
                                      "Name: <input type=\"text\" name=\"name\"><br>"
                                      "Age: <input type=\"text\" name=\"age\"><br>"
                                      "<input type=\"submit\" value=\"Submit\">"
                                      "</form>"
                                      "</body></html>"
                                      );
            }
            else if (req.method == "POST"_method) {
                auto parsedBody = parseBody(req.body);
                std::string name = parsedBody.count("name") ? parsedBody["name"] : "[none]";
                std::string age = parsedBody.count("age") ? parsedBody["age"] : "[none]";

                std::ostringstream os;
                os << "Received POST: Name=" << name << ", Age=" << age;

                return crow::response(200, os.str());
            }

            return crow::response(404);
        });

    app.port(8080).multithreaded().run();

    return 0;
}
