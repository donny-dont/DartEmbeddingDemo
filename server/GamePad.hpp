/**
 * \file GamePad.hpp
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

#ifndef DART_EMBED_GAME_PAD_HPP_INCLUDED
#define DART_EMBED_GAME_PAD_HPP_INCLUDED

#include <cstdint>

namespace DartEmbed
{
	/**
	 * Specifies the game controller associated with a player.
	 */
	namespace PlayerIndex
	{
		/// An enumerated type
		enum Enum
		{
			/// The first controller
			One,
			/// The second controller
			Two,
			/// The third controller
			Three,
			/// The fourth controller
			Four,
			/// The number of enumerations
			Size

		} ; // end enum Enum

	} // end namespace PlayerIndex

	/**
	 * Represents information about the state of an Xbox 360 Controller.
	 */
	class GamePadState
	{
		//----------------------------------------------------------------------
		// Construction
		//----------------------------------------------------------------------

		public:

			/**
			 * Creates an instance of the GamePadState class.
			 */
			GamePadState();

			/**
			 * Initializes an instance of the GamePadState class.
			 *
			 * \param copy The instance to copy.
			 */
			GamePadState(const GamePadState& copy);

		//----------------------------------------------------------------------
		// Assignment
		//----------------------------------------------------------------------

		public:

			/**
			 * Copies the values from the given instance.
			 *
			 * \param copy The instance to copy.
			 */
			GamePadState& operator= (const GamePadState& copy);

		//----------------------------------------------------------------------
		// Properties
		//----------------------------------------------------------------------

		public:

			inline bool isConnected() const
			{
				return _connected;
			}

			void setConnected(bool value)
			{
				_connected = value;
			}

			inline float getLeftTrigger() const
			{
				return _leftTrigger;
			}

			inline float getRightTrigger() const
			{
				return _rightTrigger;
			}

			inline std::int32_t getPacketNumber() const
			{
				return _packetNumber;
			}

			inline void setPacketNumber(std::int32_t value)
			{
				_packetNumber = value;
			}

			inline void setLeftTrigger(float value)
			{
				_leftTrigger = value;
			}

			inline void setRightTrigger(float value)
			{
				_rightTrigger = value;
			}

		//----------------------------------------------------------------------
		// Member variables
		//----------------------------------------------------------------------

		private:

			/// Whether the controller is connected
			bool _connected;
			/**
			 * The packet number for the controller.
			 *
			 * Whenever the state changes the packet number is changed.
			 * If the packet number hasn't changed then the values don't
			 * need to be copied again.
			 */
			std::int32_t _packetNumber;
			float _leftTrigger;
			float _rightTrigger;

	} ; // end class GamePadState

	/**
	 * Allows retrieval of user interaction with an Xbox 360 controller.
	 */
	namespace GamePad
	{
		/**
		 * Gets the current state of a game pad controller.
		 *
		 * \returns The current state of a game pad controller.
		 */
		const GamePadState& getState(PlayerIndex::Enum player);

		/**
		 * Sets the virbration motor speeds of an Xbox 360 controller.
		 *
		 * \param leftMotor The speed of the low-frequency left motor.
		 * \param rightMoto The speed of the high-frequency right motor.
		 */
		void setVibration(PlayerIndex::Enum player, const float leftMotor, const float rightMotor);

	} // end namespace GamePad
}

#endif // end DART_EMBED_GAME_PAD_HPP_INCLUDED
