// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"
#include <SDL_log.h>
#include <tracy/Tracy.hpp>

#include "Strings/String.h"


namespace Log {
	template<typename ...Args>
	inline void Message(const TCHAR* format, Args... args) {
		if constexpr (sizeof...(args) > 0)
		{
			String msg = CString::Printf(format, eastl::forward<Args>(args)...).c_str();
			TracyMessage(msg.c_str(), msg.size()); // Send to profiler
			SDL_Log(msg.c_str()); // Send to console
		}
		else
		{
			TracyMessage(format, std::strlen(format)); // Send to profiler
			SDL_Log(format); // Send to console
		}
	}

	inline void Log(const String msg) {
		Log(msg.c_str());
	}

	template<typename ...Args>
	inline void Error(const TCHAR* format, Args... args) {
		if constexpr (sizeof...(args) > 0)
		{
			String msg{ "Error> " };
			msg.append_sprintf(format, eastl::forward<Args>(args)...).c_str();
			TracyMessage(msg.c_str(), msg.size()); // Send to profiler
			SDL_Log(msg.c_str()); // Send to console
		}
		else
		{
			String msg{ "Error> " };
			msg.append(format);
			TracyMessage(msg.c_str(), msg.size()); // Send to profiler
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, msg.c_str()); // Send to console
		}
	}

	inline void Error(const String msg) {
		Error(msg.c_str());
	}
};
