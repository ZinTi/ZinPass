#ifndef PROMPT_H
#define PROMPT_H

#include <string>

namespace zinpass::cli{

class Prompt {
private:
    std::string currentUser;
    std::string prompt;

public:
    Prompt();

    Prompt(const std::string& prompt);

    ~Prompt();

    std::string getPrompt();

    void setPrompt(const std::string& prompt);

    std::string getCurrentUser();

    void setCurrentUser(const std::string& currentUser);

    void print() const;

};

}

#endif //PROMPT_H
