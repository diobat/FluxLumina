#pragma once

#define BOOST_USE_WINAPI_VERSION 0x0601     // Must be here or doesn't compile, blame boost
// this is a "temporary fix", 
// see https://github.com/microsoft/vcpkg/discussions/22762 for more info

#include "rendering/GLFW_Wrapper.hpp"

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks.hpp>

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

    void logGLErrors(const std::string& msg = "");

private:
    Logger();
    
};