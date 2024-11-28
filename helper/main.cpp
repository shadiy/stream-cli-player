#include <iostream>
#include <string>

#include <subprocess.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No channel name specified" << std::endl;
        return 1;
    }

    char p_stdout_buffer[256] = { 0 };

    subprocess_s player_p;
    const char *command_line[] = {"player", argv[1], NULL};
    int result = subprocess_create(command_line, subprocess_option_enable_async | subprocess_option_inherit_environment, &player_p);
    if (0 != result) {
        std::cout << "Error in starting player" << std::endl;
        return 1;
    }

    return 0;
}