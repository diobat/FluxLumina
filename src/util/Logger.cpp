#include "util/Logger.hpp"


namespace
{
    
    long int timeStart;

    int drawCalls = 0;
    int instancedDrawCalls = 0;

}


Logger::Logger()
{
    //boost::log::add_console_log(std::cout, boost::log::keywords::format = "%TimeStamp%: %Message%");
    //boost::log::add_common_attributes();
    //boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}

Logger& Logger::Instance()
{
    static Logger instance;
    return instance;
}

void Logger::init()
{
    boost::log::add_file_log("debug.log", boost::log::keywords::format = "[%TimeStamp%]: %Message%");
}

void Logger::test()
{
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
}

void Logger::tic()
{
    timeStart = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();
}

void Logger::toc(const std::string& msg)
{
    if(timeStart == 0)
    {
        BOOST_LOG_TRIVIAL(debug) << "Logger::tic() was not called before Logger::toc()";
        return;
    }

    double timeEnd = boost::posix_time::microsec_clock::local_time().time_of_day().total_milliseconds();
    double timeElapsed = timeEnd - timeStart;
    BOOST_LOG_TRIVIAL(info) << msg << " took " << timeElapsed << " ms";
}

void Logger::drawCall(int numInstances)
{
    drawCalls+=numInstances;
}

void Logger::instancedDrawCall(int numInstances)
{
    instancedDrawCalls+=numInstances;
}

void Logger::resetDrawCalls()
{
    BOOST_LOG_TRIVIAL(info) << drawCalls << " draw calls were made this run";
    BOOST_LOG_TRIVIAL(info) << instancedDrawCalls << " instanced draw calls were made this run";
    drawCalls = 0;
    instancedDrawCalls = 0;
}

void Logger::logGLErrors(const std::string &msg)
{
    bool firstTime = true;
    GLenum glError;
    do
    {
        glError = glGetError();

        switch(glError)
        {
            case GL_NO_ERROR:
                if(firstTime)
                {
                    BOOST_LOG_TRIVIAL(info) << msg << " GL_NO_ERROR";    
                }
                break;
            case GL_INVALID_ENUM:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                BOOST_LOG_TRIVIAL(warning) << msg << " GL_STACK_OVERFLOW";
                break;
            default:
                break;
        }

        firstTime = false;

    }while(glError != GL_NO_ERROR);
}