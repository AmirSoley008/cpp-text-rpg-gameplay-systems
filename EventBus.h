//
// Created by parsian on 9/20/2026.
//

#ifndef UNTITLED3_EVENTBUS_H
#define UNTITLED3_EVENTBUS_H


#include <functional>
#include <any>
#include <unordered_map>
#include <typeindex>
#include <vector>

class EventBus {
private:
    using Callback = std::function<void(const std::any&)>;

    std::unordered_map<std::type_index,std::vector<Callback>>callbacks;
public:
    template<typename Event>
    void subscribe(std::function<void(const Event &)> callback) {
        callbacks[typeid(Event)].push_back(
                [callback](const std::any& event)
                {
                    callback(std::any_cast<const Event&>(event));
                }
        );
    }

    template<typename Event>
    void publish(const Event &event) {
        auto it = callbacks.find(typeid(Event));

        if (it == callbacks.end()) return ;

        std::any storedEvent = event;

        for (auto callback : it->second) {
            callback(storedEvent);
        }
    }
};


#endif //UNTITLED3_EVENTBUS_H
