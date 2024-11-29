#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

#include <httplib.h>

std::string load_file(const std::string& path) {
    std::string content;

    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
        while(getline(file, line)) {
            content.append(line);
        }
        file.close();
        std::cout << "Loaded " << path << std::endl;
    } else {
        std::cout << "Cant open " << path << std::endl;
        return "";
    }

    return content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No Channel name specified" << std::endl;
        return 1;
    }

    std::string channel_name = argv[1];

    httplib::Server svr;

    svr.Get("/", [channel_name](const httplib::Request& req, httplib::Response& res) {
        std::cout << req.body << std::endl;
        res.status = 200;

        std::string new_html = load_file("public/index.html");
        size_t start = new_html.find("src=\"\"") + 5;

        std::string src = "https://player.twitch.tv/?channel=" + channel_name + "&parent=127.0.0.1";

        new_html.insert(start, src);

        res.set_content(new_html, "text/html");
    });

    svr.Get("/styles.css", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << res.body << std::endl;
        res.status = 200;
        std::string css = load_file("public/styles.css");
        res.set_content(css, "text/css");
    });

    svr.Get("/main.js", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << res.body << std::endl;
        res.status = 200;
        std::string js = load_file("public/main.js");
        res.set_content(js, "text/javascript");
    });

    svr.set_idle_interval(10);

    std::cout << "Listening on 127.0.0.1:8080" << std::endl;
    svr.listen("127.0.0.1", 8080);

    return 0;
}