#include <iostream>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#else
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "webview/webview.h"

#include <httplib.h>
#include <subprocess.h>

subprocess_s server_p;
webview::webview w(true, nullptr);

#if defined(_WIN32)
BOOL WINAPI ConsoleHandler(DWORD dwType)
{
    switch(dwType) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
        if (subprocess_alive(&server_p)) {
            std::cout << "Closing server" << std::endl;
            int res = subprocess_terminate(&server_p);
            if (res != 0) {
                std::cout << "Error in closing server" << std::endl;
            }
        }

        w.terminate();
        ExitProcess(0);
        break;
    default:
        printf("Other event\n");
    }
    return TRUE;
}
#else
static void signal_handler(int id) {
    if (subprocess_alive(&server_p)) {
        std::cout << "Closing server" << std::endl;
        int res = subprocess_terminate(&server_p);
        if (res != 0) {
            std::cout << "Error in closing server" << std::endl;
        }
    }

    w.terminate();
    exit(1);
}
#endif

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No channel name specified" << std::endl;
        return 1;
    }

#if _WIN32
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
        fprintf(stderr, "Unable to install handler!\n");
        return 1;
    }
#else
    signal(SIGINT, signal_handler);
#endif

    char p_stdout_buffer[256] = { 0 };

    const char *command_line[] = {"stream-server", argv[1], NULL};
    std::cout << "Starting server" << std::endl;
    int result = subprocess_create(command_line, subprocess_option_enable_async | subprocess_option_inherit_environment | subprocess_option_no_window, &server_p);
    if (0 != result) {
        std::cout << "Error in starting server" << std::endl;
        return 1;
    }
    std::cout << "Finished starting server" << std::endl;

    try {
        w.set_title("player");
        w.set_size(1280, 720, WEBVIEW_HINT_NONE);

        httplib::Client client("127.0.0.1", 8080);
        httplib::Result res = client.Get("/");
        while (res.error() == httplib::Error::Connection) {
            std::cout << "Trying to connect" << std::endl;
            res = client.Get("/");
        }

        std::cout << "Connected to http://127.0.0.1:8080/" << std::endl;

        res.value().body.clear();

        client.stop();

        //w.set_html(res.value().body);
        w.navigate("http://127.0.0.1:8080/");

        w.run(); // main loop
    } catch (const webview::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (subprocess_alive(&server_p)) {
        std::cout << "Closing server" << std::endl;
        int res = subprocess_terminate(&server_p);
        if (res != 0) {
            std::cout << "Error in closing server" << std::endl;
        }
    }

    std::cout << "Closing client" << std::endl;

    return 0;
}