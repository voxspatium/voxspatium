/**
 * @file    Logger.h
 * @brief   Logging functions
 *
 * Voxspatium, 3D game engine for creative space-themed games
 * Copyright (C) 2021  Evert "Diamond" Prants <evert.prants@lunasqu.ee>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef __LOG_H__
#define __LOG_H__

#include "Application.h"

#include <string>
#include <stdlib.h>

#define FPRNT(io,tag,message) fprintf(io, "[%s] %s\n", tag, message.c_str());

inline void fatalError(const std::string message)
{
	FPRNT(stderr, "FATAL ERROR", message);
	Application::getInstance().exit();
}

inline void logInfo(const std::string& message)
{
	FPRNT(stdout, "Info", message);
}

inline void logWarn(const std::string& message)
{
	FPRNT(stdout, "Warning", message);
}

inline void logError(const std::string& message)
{
	FPRNT(stdout, "Error", message);
}
#endif // __LOG_H__
