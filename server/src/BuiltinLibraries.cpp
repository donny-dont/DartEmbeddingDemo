/**
 * \file BuiltinLibraries.cpp
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

#include "BuiltinLibraries.hpp"
#include "ScriptLibrary.hpp"
using namespace DartEmbed;

//---------------------------------------------------------------------
// Loads the builtin Dart libraries
//
// Currently there is no way to query the source for the builtin
// libraries. Should probably be an extern value or an aux library
// should provide a hook into it.
//
// The application is compiled into the snapshot so these values
// are set to null.
//---------------------------------------------------------------------

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createJsonLibrary()
{
	return new ScriptLibrary("dart:json", 0);
}

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createUriLibrary()
{
	return new ScriptLibrary("dart:uri", 0);
}

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createCryptoLibrary()
{
	return new ScriptLibrary("dart:crypto", 0);
}

//---------------------------------------------------------------------

ScriptLibrary* BuiltinLibraries::createUtfLibrary()
{
	return new ScriptLibrary("dart:utf", 0);
}
