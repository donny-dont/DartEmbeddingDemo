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

class Vector2D implements Point
{
  num x;
  num y;
  
  Vector2D(this.x, this.y);
  
  void cloneTo(Vector2D copy)
  {
    copy.x = x;
    copy.y = y;
  }
  
  void setValues(num xValue, num yValue)
  {
    x = xValue;
    y = yValue;
  }
}

class GamePadState
{
  static final int _dPadUp        = 1 <<  0;
  static final int _dPadLeft      = 1 <<  1;
  static final int _dPadRight     = 1 <<  2;
  static final int _dPadDown      = 1 <<  3;
  static final int _start         = 1 <<  4;
  static final int _back          = 1 <<  5;
  static final int _leftThumb     = 1 <<  6;
  static final int _rightThumb    = 1 <<  7;
  static final int _leftShoulder  = 1 <<  8;
  static final int _rightShoulder = 1 <<  9;
  static final int _aButton       = 1 << 11;
  static final int _bButton       = 1 << 12;
  static final int _xButton       = 1 << 13;
  static final int _yButton       = 1 << 14;
  
  /// Whether the controller is connected
  bool _connected;
  /// The state of the left thumbstick
  Vector2D _leftThumbstick;
  /// The state of the right thumbstick
  Vector2D _rightThumbstick;
  /// The state of the left trigger
  double _leftTrigger;
  /// The state of the right trigger
  double _rightTrigger;
  /// The state of the buttons
  int _buttons;
  
  GamePadState()
    : _connected = false
    , _leftThumbstick = new Vector2D(0, 0)
    , _rightThumbstick = new Vector2D(0, 0)
    , _leftTrigger = 0.0
    , _rightTrigger = 0.0
    , _buttons = 0;
  
  bool get isConnected()   => _connected;
  bool get up()            => _isFlagSet(_dPadUp);
  bool get down()          => _isFlagSet(_dPadDown);
  bool get left()          => _isFlagSet(_dPadLeft);
  bool get right()         => _isFlagSet(_dPadRight);
  bool get back()          => _isFlagSet(_back);
  bool get start()         => _isFlagSet(_start);
  bool get leftShoulder()  => _isFlagSet(_leftShoulder);
  bool get rightShoulder() => _isFlagSet(_rightShoulder);
  bool get leftStick()     => _isFlagSet(_leftThumb);
  bool get rightStick()    => _isFlagSet(_rightThumb);
  bool get x()             => _isFlagSet(_xButton);
  bool get y()             => _isFlagSet(_yButton);
  bool get a()             => _isFlagSet(_aButton);
  bool get b()             => _isFlagSet(_bButton);
  
  double get leftTrigger() => _leftTrigger;
  double get rightTrigger() => _rightTrigger;
  Vector2D get leftThumbstick() => _leftThumbstick;
  Vector2D get rightThumbstick() => _rightThumbstick;
  
  void cloneTo(GamePadState state)
  {
    state._connected = _connected;
    state._buttons = _buttons;
    state._leftTrigger = _leftTrigger;
    state._rightTrigger = _rightTrigger;
    
    _leftThumbstick.cloneTo(state._leftThumbstick);
    _rightThumbstick.cloneTo(state._rightThumbstick);
  }
  
  void reset()
  {
    _connected = false;
    _buttons = 0;
    _leftTrigger = 0.0;
    _rightTrigger = 0.0;
    
    _leftThumbstick.setValues(0.0, 0.0);
    _rightThumbstick.setValues(0.0, 0.0);
  }
  
  bool _isFlagSet(int flag)
  {
    return (_buttons & flag) == flag;
  }
}

class GamePad
{
  /// The port to connect on
  static final int _port = 8000;
  /// Socket connection to the server
  static WebSocket _connection;
  /// Whether the connection has been made
  static bool _connected;
  /// List of game pads
  static List<GamePadState> _gamePads;
  
  static bool get isConnected() => _connected;
  
  static void onInitialize()
  {
    _gamePads = new List<GamePadState>();
    _gamePads.add(new GamePadState());
    _gamePads.add(new GamePadState());
    _gamePads.add(new GamePadState());
    _gamePads.add(new GamePadState());
    
    _connected = false;
  }
  
  static void getState(int index, GamePadState state)
  {
    if ((_connection != null) && (_connected))
    {
      int player = index + 1;
      _connection.send('player$player');
    }
    
    _gamePads[index].cloneTo(state);
  }
  
  static void setVibration(int index, double leftMotor, double rightMotor)
  {
    
  }
  
  static void connectToServer(String ip, EventListener onOpen, EventListener onClose)
  {
    if (_connection != null)
      disconnectFromServer;

    // Setup the connection
    _connection = new WebSocket('ws://$ip:$_port/ws');
    
    // Connect to the open event
    _connection.on.open.add((e) {
      print("Connected!");
      _connected = true;
    });
    
    _connection.on.open.add(onOpen);
    
    // Connect to the close event
    _connection.on.close.add((CloseEvent e) {
      print("Disconnected! ${e.code} ${e.reason}");
      _connected = false;
      
      for (GamePadState gamePad in _gamePads)
        gamePad.reset();
    });
      
    _connection.on.close.add(onClose);
    
    // Receive the game pad updates
    _connection.on.message.add((MessageEvent e) {
      _receiveMessage(e.data);
    });
  }
  
  static void disconnectFromServer()
  {
    if (_connection != null)
    {
      _connection.close();
      _connection = null;
    }
  }
  
  static void _receiveMessage(String message)
  {
    Map json = JSON.parse(message);
    
    int index = json['index'];
    GamePadState gamePad = _gamePads[index];
    
    gamePad._connected = json['connected'];
    gamePad._leftTrigger = json['leftTrigger'];
    gamePad._rightTrigger = json['rightTrigger'];
    gamePad._buttons = json['buttons'];
    
    num x;
    num y;
    Map thumbstick;
    
    thumbstick = json['leftThumbstick'];
    x = thumbstick['x'];
    y = thumbstick['y'];
    gamePad._leftThumbstick.setValues(x, y);
    
    thumbstick = json['rightThumbstick'];
    x = thumbstick['x'];
    y = thumbstick['y'];
    gamePad._rightThumbstick.setValues(x, y);
  }
}
