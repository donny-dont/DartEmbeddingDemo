/**
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

class GamePadState
{
  Point2D _leftThumbstick;
  Point2D _rightThumbstick;
  GamePadState()
    : _leftThumbstick = new Point2D(0, 0)
    , _rightThumbstick = new Point2D(0, 0);
  
  bool get isConnected() => true;
  bool get up() => true;
  bool get down() => true;
  bool get left() => true;
  bool get right() => true;
  bool get back() => true;
  bool get start() => true;
  bool get leftShoulder() => true;
  bool get rightShoulder() => true;
  bool get leftStick() => false;
  bool get rightStick() => false;
  bool get x() => true;
  bool get y() => true;
  bool get a() => true;
  bool get b() => true;
  
  Point2D get leftThumbstick() => _leftThumbstick;
  Point2D get rightThumbstick() => _rightThumbstick;
}

class GamePad
{
  
}
