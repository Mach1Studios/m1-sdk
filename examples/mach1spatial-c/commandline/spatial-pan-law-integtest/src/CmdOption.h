//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.
//  Special Thanks to iain on StackOverflow

#ifndef __CMDOPTION_H_INCLUDED
#define __CMDOPTION_H_INCLUDED

#include <string>
#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

#endif
