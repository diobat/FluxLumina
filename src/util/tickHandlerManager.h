#pragma once
// CUrrently unused by the build


// First-party includes 
#include "util/tickHandler.h"

// Third-party includes
#include "boost/asio.hpp"
#include <boost/bind.hpp>

namespace Utils
{ 
	namespace Ticker
	{

		class HandlerManager
		{
		public:
			HandlerManager(boost::posix_time::milliseconds period);
			void tick();

			void registerCallback(std::shared_ptr<Handler> callback);

		private:
			boost::posix_time::milliseconds _intervalPeriod;
			boost::asio::io_service _ioService;
			boost::asio::deadline_timer _tickTimer30;


			std::vector<std::shared_ptr<Handler>> _allCallbacks;
		};


	}
}