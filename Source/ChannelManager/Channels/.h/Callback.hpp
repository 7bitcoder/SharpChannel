#pragma once
#include <iostream>
#include <map>
#include "IUnsubscribable.hpp"
#include "Control.hpp"
#include "IReadOnlyChannel.hpp"

namespace cm {
    class Callback;
    using CallbacksMap = std::map<size_t, std::shared_ptr<Callback>>;

    class Callback final: public IUnsubscribable {
        public:
            // getInfo
            bool isData() { return bool(_onDataReceived); }
            bool isMessage() { return bool(_onMessageReceived); }
            bool isComplete() { return bool(_onComplete); }
            // callable
            Control onMessageReceived(const std::string& msg);
            Control onDataReceived(const char* data, size_t lenght);
            void complete();

            // initialization
            Callback(std::shared_ptr<CallbacksMap> map, const OnComplete& onComplete);
            
            Callback(std::shared_ptr<CallbacksMap> map, const OnMessageReceived& onMessageReceived);
            Callback(std::shared_ptr<CallbacksMap> map, const OnMessageReceived& onMessageReceived, const OnComplete& onComplete);

            Callback(std::shared_ptr<CallbacksMap> map, const OnDataReceived& onDataReceived);
            Callback(std::shared_ptr<CallbacksMap> map, const OnDataReceived& onDataReceived, const OnComplete& onComplete);

            void setIdentifier(size_t identifier) { _identifier = identifier; }
            
            bool unsunscribe();

            virtual ~Callback() {}
        private:
            size_t _identifier;
            OnMessageReceived _onMessageReceived;
            OnDataReceived _onDataReceived;
            OnComplete _onComplete;
            std::shared_ptr<CallbacksMap> _map;
    };
}