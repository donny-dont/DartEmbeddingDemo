/**
 * \file ScriptLibrary.cpp
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

#include "ScriptLibrary.hpp"
using namespace DartEmbed;

//----------------------------------------------------------------------

ScriptLibrary::ScriptLibrary()
: _library(0)
, _name(0)
, _hashedName(0)
, _source(0)
, _resolver(0)
{ }

//----------------------------------------------------------------------

ScriptLibrary* ScriptLibrary::fromSource(const char* name, const char* source)
{
	ScriptLibrary* library = new ScriptLibrary();

	library->setName(name);
	library->setSource(source);

	return library;
}
