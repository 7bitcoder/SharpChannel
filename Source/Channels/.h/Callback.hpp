#pragma once
#include <iostream>
#include "IUnsubscribable.hpp"
#include "Control.hpp"
#include "IReadOnlyChannel.hpp"

namespace cm {
    class Callback final: public IUnsubscribable {
        public:
            // getInfo
            bool isData() { return bool(_onDataReceived); }
            bool isMessage() { return bool(_onMessageReceived); }
            bool isComplete() { return bool(_onCompleted); }
            // callable
            Control onMessageReceived(const std::string& msg);
            Control onDataReceived(const std::vector<char>& data);
            void complete();

            // initialization
            Callback(const std::function<bool()>& unsubscriber, const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted);

            Callback(const std::function<bool()>& unsubscriber, const OnDataReceived& onDataReceived, const onCompleted& onCompleted);

            bool unsunscribe();

            virtual ~Callback() {}
        private:
            OnMessageReceived _onMessageReceived;
            OnDataReceived _onDataReceived;
            onCompleted _onCompleted;
            const std::function<bool()>& _unsubscriber;
    };
}