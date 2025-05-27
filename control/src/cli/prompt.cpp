#include "prompt.h"

#include <iostream>

#define CURRENT_USER_DEFAULT "root"
#define PROMPT_DEFAULT "$ "

namespace zinpass::cli {

    Prompt::Prompt() {
        this->currentUser = CURRENT_USER_DEFAULT;
        this->prompt = PROMPT_DEFAULT;
    }

    Prompt::Prompt(const std::string& prompt) {
        this->currentUser = CURRENT_USER_DEFAULT;
        this->prompt = prompt;
    }

    Prompt::~Prompt() {}

    std::string Prompt::getPrompt() { return this->prompt; }

    void Prompt::setPrompt(const std::string& prompt) { this->prompt = prompt; }

    std::string Prompt::getCurrentUser() { return this->currentUser; }

    void Prompt::setCurrentUser(const std::string& currentUser) { this->currentUser = currentUser; }

    void Prompt::print() const {
        std::cout << "[" << this->currentUser << "]" << std::endl;
        std::cout << this->prompt;
    }

} // namespace zinpass::cli
