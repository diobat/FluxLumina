#pragma once

#define BOOST_USE_WINAPI_VERSION 0x0601     // Must be here or doesn't compile, blame boost

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks.hpp>

struct 
{
    /* data */
};


class Logger
{
public:
    static Logger& Instance();

    void init();

    void test();

    void tic();
    void toc(const std::string& msg);

    void drawCall(int numInstances = 1);
    void instancedDrawCall(int numInstances = 1);
    void resetDrawCalls();

private:
    Logger();
    
};