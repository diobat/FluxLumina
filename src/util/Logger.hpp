#pragma once

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
    void toc(const std::string& msg = "");

    void drawCall(int numInstances = 1);
    void instancedDrawCall(int numInstances = 1);
    void resetDrawCalls();

    void logGLErrors(const std::string& msg = "");

private:
    Logger();
    
};