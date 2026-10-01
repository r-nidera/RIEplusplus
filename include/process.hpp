#pragma once

#define PROGRAM_VERSION "0.0.1"

namespace process {

enum ArgActionType {
    Nothing,
    Compile,
};

class ArgumentHandler {
    int argc_ = 0;
    char **argv_ = nullptr;

    ArgActionType action_ = Nothing;
    const char *output_string_ = nullptr;

  public:
    ArgumentHandler(int argc, char **argv) : argc_(argc), argv_(argv) {}
    int parse(void);

    ArgActionType action() const { return action_; }
    const char *output_string() const { return output_string_; }
};

} // namespace process
