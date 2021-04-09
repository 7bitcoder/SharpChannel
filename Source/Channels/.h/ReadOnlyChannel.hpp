#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <mutex>
#include "Callback.hpp"
#include "IReadOnlyChannel.hpp"
#include "IChannelEventLoop.hpp"

namespace cm {
    class ReadOnlyChannel: public virtual IReadOnlyChannel {
        public:
            ReadOnlyChannel() { _callbacks = std::make_shared<CallbacksMap>(); }

            std::shared_ptr<IUnsubscribable> subscribe(const onCompleted& onCompleted) final;

            std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived& onMessageReceived) final;
            std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived& onDataReceived) final;

            std::shared_ptr<IUnsubscribable> subscribe(const OnMessageReceived& onMessageReceived, const onCompleted& onCompleted) final;
            std::shared_ptr<IUnsubscribable> subscribe(const OnDataReceived& onDataReceived, const onCompleted& onCompleted) final;

            std::shared_ptr<IUnsubscribable> subscribe(IMessageObserver& observer) final;
            std::shared_ptr<IUnsubscribable> subscribe(IDataObserver& observer) final;
            std::shared_ptr<IUnsubscribable> subscribe(IObserver& observer) final;

            virtual ~ReadOnlyChannel() {}
        protected:
            void setChannelEventLoop(IChannelEventLoop& eventLoop); 
            void completeAll();
            void nextAll(const std::string& msg);
            void nextAll(const char* data, size_t lenght);
            CallbacksMap& getCallbacks() { return *_callbacks; }
        private:
            void completeAllImpl();
            void nextAllImpl(const std::string& msg);
            void nextAllImpl(const char* data, size_t lenght);
            std::shared_ptr<IUnsubscribable> subscribeImpl(std::shared_ptr<Callback> callback);
            std::shared_ptr<CallbacksMap> _callbacks;
            size_t _indexCnt = 0;
            IChannelEventLoop* _eventLoop = nullptr;
    };
}