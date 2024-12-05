//
// Created by ginko on 12/2/24.
//

#ifndef LOGGING_H
#define LOGGING_H


namespace Log {
    void Instantiate();

    template<typename... T>
    std::basic_string<char> runtime(std::string_view fmt, T &&... args);
}


#endif //LOGGING_H
