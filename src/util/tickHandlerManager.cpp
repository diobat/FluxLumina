// CUrrently unused by the build

#include "util/tickHandlerManager.h"

//First party object only includes
#include "user_input/glfwUserInputScanner.h"

using namespace Utils::Ticker;

HandlerManager::HandlerManager(boost::posix_time::milliseconds period):
    _intervalPeriod(period),
    _tickTimer30(_ioService, boost::posix_time::milliseconds(33))
{

    _tickTimer30.async_wait(boost::bind(&HandlerManager::tick, this));
    _ioService.run();
}



void HandlerManager::tick()
{
    std::cout << "30 tick boys" << std::endl;

    // Do stuff here dude

    for (std::shared_ptr<Handler> callback : _allCallbacks)
    {
        callback->tickCallback();
    }

    // Redefine expiration timestamp
    _tickTimer30.expires_from_now(_intervalPeriod);
    // Reschedule timer event
    _tickTimer30.async_wait(boost::bind(&HandlerManager::tick, this));
}

void HandlerManager::registerCallback(std::shared_ptr<Handler> callback)
{
    _allCallbacks.push_back(callback);
}