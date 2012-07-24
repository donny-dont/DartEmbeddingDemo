/**
 * \file InputScriptLibrary.cpp
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

#include "InputScriptLibrary.hpp"
#include "GamePad.hpp"
#include "dart_api.h"
using namespace DartEmbed;

#include <vector>


namespace
{
	/// Name for the library
	const char* __libraryName = "embed:input";

	struct NativeResolver
	{
		std::int32_t hash;
		Dart_NativeFunction function;
	} ;

	std::vector<NativeResolver> __resolvers;

	void __addResolver(const char* name, Dart_NativeFunction function)
	{
		NativeResolver resolver;
		resolver.hash = fnv1aHash(name);
		resolver.function = function;

		__resolvers.push_back(resolver);
	}

	/// Source code for the library
	const char* __script =
		"#library('embed:input');\n"
		"\n"
		"class GamePadState\n"
		"{\n"
		"    static GamePadState create() native 'GamePadState_New';\n"
		"    GamePadState clone() native 'GamePadState_clone';\n"
		"    double get leftTrigger()\n"
		"    { return getLeftTrigger(this); }\n"
		"    void cloneTo(GamePadState state) native 'GamePadState_cloneTo';\n"
		"    static bool isConnected(GamePadState state) native 'GamePadState_isConnected';\n"
		"    static double getLeftTrigger(GamePadState state) native 'GamePadState_getLeftTrigger';\n"
		"    static double getRightTrigger(GamePadState state) native 'GamePadState_getRightTrigger';\n"
		"}\n"
		""
		"class GamePad\n"
		"{"
		"    static void getState(int index, GamePadState state) native 'GamePad_getState';\n"
		"    static void setVibration(int index, double leftMotor, double rightMotor) native 'GamePad_setVibration';\n"
		"}";

	//----------------------------------------------------------------------

	PlayerIndex::Enum __toPlayerIndex(Dart_Handle handle)
	{
		if (Dart_IsInteger(handle))
		{
			std::int64_t index = 0;
			Dart_IntegerToInt64(handle, &index);

			return (PlayerIndex::Enum)index;
		}

		return PlayerIndex::One;
	}

	//----------------------------------------------------------------------

	float __toFloat(Dart_Handle handle)
	{
		double value = 0.0;

		if (::Dart_IsDouble(handle))
		{
			Dart_DoubleValue(handle, &value);
		}

		return (float)value;
	}

	//----------------------------------------------------------------------

	GamePadState* __toGamePadState(Dart_Handle handle)
	{
		bool weak = Dart_IsWeakPersistentHandle(handle);
		std::int64_t ptr = 0;
		Dart_Handle result = Dart_IntegerToInt64(handle, &ptr);

		if (!Dart_IsError(result))
		{
			return (GamePadState*)ptr;
		}

		return 0;
	}

	//----------------------------------------------------------------------

	void GamePadState_Delete(Dart_Handle handle, void* peer)
	{
		GamePadState* state = (GamePadState*)peer;

		delete state;
	}

	//----------------------------------------------------------------------

	void GamePadState_New(Dart_NativeArguments args)
	{
		GamePadState* state = new GamePadState();

		Dart_Handle handle = Dart_NewInteger((std::int64_t)state);
		Dart_Handle result = Dart_NewWeakPersistentHandle(handle, state, GamePadState_Delete);

		Dart_SetReturnValue(args, result);
	}

	//----------------------------------------------------------------------

	void GamePadState_isConnected(Dart_NativeArguments args)
	{
		int argc = Dart_GetNativeArgumentCount(args);

		if (argc == 1)
		{
			GamePadState* state = __toGamePadState(Dart_GetNativeArgument(args, 0));

			Dart_SetReturnValue(args, Dart_NewBoolean(state->isConnected()));
		}
	}

	//----------------------------------------------------------------------
	
	void GamePadState_getLeftTrigger(Dart_NativeArguments args)
	{
		int argc = Dart_GetNativeArgumentCount(args);

		if (argc == 1)
		{
			GamePadState* state = __toGamePadState(Dart_GetNativeArgument(args, 0));

			Dart_SetReturnValue(args, Dart_NewDouble(state->getLeftTrigger()));
		}
	}

	//----------------------------------------------------------------------

	void GamePadState_getRightTrigger(Dart_NativeArguments args)
	{
		int argc = Dart_GetNativeArgumentCount(args);

		if (argc == 1)
		{
			GamePadState* state = __toGamePadState(Dart_GetNativeArgument(args, 0));

			Dart_SetReturnValue(args, Dart_NewDouble(state->getRightTrigger()));
		}
	}

	//----------------------------------------------------------------------

	void GamePad_getState(Dart_NativeArguments args)
	{
		int argc = Dart_GetNativeArgumentCount(args);

		if (argc == 2)
		{
			PlayerIndex::Enum index = __toPlayerIndex(Dart_GetNativeArgument(args, 0));
			GamePadState* state = __toGamePadState(Dart_GetNativeArgument(args, 1));

			*state = GamePad::getState(index);
		}
	}

	//----------------------------------------------------------------------

	void GamePad_setVibration(Dart_NativeArguments args)
	{
		int argc = Dart_GetNativeArgumentCount(args);
		
		if (argc == 3)
		{
			PlayerIndex::Enum index = __toPlayerIndex(Dart_GetNativeArgument(args, 0));
			float leftMotor = __toFloat(Dart_GetNativeArgument(args, 1));
			float rightMotor = __toFloat(Dart_GetNativeArgument(args, 2));

			GamePad::setVibration(index, leftMotor, rightMotor);
		}
	}

	//----------------------------------------------------------------------

	Dart_NativeFunction __resolveInput(Dart_Handle name, int argc)
	{
		const char* functionName = 0;
		Dart_Handle result = Dart_StringToCString(name, &functionName);

		if (!Dart_IsError(result))
		{
			std::int32_t hash = fnv1aHash(functionName);
			std::size_t size = __resolvers.size();

			for (std::size_t i = 0; i < size; ++i)
			{
				if (hash == __resolvers[i].hash)
					return __resolvers[i].function;
			}
		}

		// Unknown function
		return 0;
	}
}

//----------------------------------------------------------------------

InputScriptLibrary::InputScriptLibrary()
{
	setName(__libraryName);
	setSource(__script);
	setNativeResolver(__resolveInput);

	if (__resolvers.empty())
	{
		__addResolver("GamePadState_New", GamePadState_New);
		__addResolver("GamePadState_isConnected", GamePadState_isConnected);
		//__addResolver("GamePadState_clone", GamePadState_clone);
		//__addResolver("GamePadState_cloneTo", GamePadState_cloneTo);
		__addResolver("GamePadState_getLeftTrigger", GamePadState_getLeftTrigger);
		__addResolver("GamePadState_getRightTrigger", GamePadState_getRightTrigger);
		__addResolver("GamePad_getState", GamePad_getState);
		__addResolver("GamePad_setVibration", GamePad_setVibration);
	}
}
