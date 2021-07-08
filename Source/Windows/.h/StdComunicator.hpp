#pragma once
#include "IRunnable.hpp"
#include "Settings.hpp"
#include "GetObject.hpp"
#include "ChannelObservable.hpp"

namespace cm
{
    class StdComunicator final : public ChannelObservable, public IRunnable
    {
    public:
        static std::unique_ptr<StdComunicator> getObject(const StdComunicatorSettings &settings, IChannelEventLoop *eventLoop);

        StdComunicator(const StdComunicatorSettings &settings) { _settings = settings; }
        ~StdComunicator() {}

        void run() override;

    private:
        StdComunicatorSettings _settings;
    };
}