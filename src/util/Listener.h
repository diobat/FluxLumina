#pragma once

// STD includes
#include <vector>
#include <functional>
#include <memory>
#include <map>

class ModelObject;

class Broadcaster
{
public:
    using ListenerHandle = std::shared_ptr<std::function<void(std::shared_ptr<ModelObject>, int)>>;
    ListenerHandle addListener(std::function<void(std::shared_ptr<ModelObject>, int)> listener)
    {
        auto handle = std::make_shared<std::function<void(std::shared_ptr<ModelObject>, int)>>(listener);
        _listeners[handle] = listener;
        return handle;
    }

    void removeListener(ListenerHandle handle)
    {
        for (auto &listener : _listeners)
        {
            if (listener.first == handle)
            {
                _listeners.erase(listener.first);
                return;
            }
        }

    }

    void trigger(std::shared_ptr<ModelObject> modelObject, int arg)
    {
        for (auto &listener : _listeners)
        {
            listener.second(modelObject, arg);
        }
    }

private:
    std::map<ListenerHandle, std::function<void(std::shared_ptr<ModelObject>, int)>> _listeners;
};
