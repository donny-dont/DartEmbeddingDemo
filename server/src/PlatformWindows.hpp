/**
 * \file PlatformWindows.hpp
 *
 * \section COPYRIGHT
 *
 * Dart Embedding Example
 *
 * ---------------------------------------------------------------------
 *
 * Copyright (c) 2012 Don Olmstead
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 * 
 *   3. This notice may not be removed or altered from any source
 *   distribution.
 */

#ifndef DART_EMBED_PLATFORM_WINDOWS_HPP_INCLUDED
#define DART_EMBED_PLATFORM_WINDOWS_HPP_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <XInput.h>

#include <cstdint>

/// Classname for the application
#define WINDOW_CLASS_NAME "Dart Embed Application"

/**
 * Update game pads.
 */
void updateGamePads();

#endif // end DART_EMBED_PLATFORM_WINDOWS_HPP_INCLUDED
