#ifndef PROFILE_H
#define PROFILE_H

#include <chrono>

class Profile
{
public:
    Profile();
    static std::chrono::nanoseconds durationRunAll;
    static std::chrono::nanoseconds first_part;
    static std::chrono::nanoseconds pseudo_generation;
    static std::chrono::nanoseconds filter_pseudos;
    static std::chrono::nanoseconds filter_legal_check;
    static std::chrono::nanoseconds parse_san_fast;
    static std::chrono::nanoseconds node_new;
    static std::chrono::nanoseconds node_regexp;

};

#endif // PROFILE_H
