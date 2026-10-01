#include <cstring>
#include <iostream>
#include <ostream>
#include <process.hpp>

using namespace process;

int ArgumentHandler::parse(void) {
    if (argc_ == 1)
        return 1;

    if (strcmp(argv_[1], "build") == 0) {
        if (argc_ < 3) {
            std::cerr << "Please provide at least 1 file." << std::endl;
            return 1;
        }

        // Otherwise, put the string in.
        // And for now, we won't support multiple files because the problem with that is we aren't really done
        // with the actual compiler, bro.
        output_string_ = argv_[2];
        action_ = Compile;
    }

    else if (strcmp(argv_[1], "version") == 0) {
        std::cout << "CRIE V" << PROGRAM_VERSION << "\n"
                  << "This program comes with absolutely NO warranty" << std::endl;
    }

    else {
        std::cout << "Unknown command argument '" << argv_[1] << "'" << std::endl;
    }

    return 0;
}
