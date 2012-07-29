
/**
 * Status of the server connection.
 */
class ConnectionStatus
{
  /// Connected to the server.
  static final ConnectionStatus Connected = const ConnectionStatus(0);
  /// Connecting to the server.
  static final ConnectionStatus Connecting = const ConnectionStatus(1);
  /// Disconnected from the server.
  static final ConnectionStatus Disconnected = const ConnectionStatus(2);
  
  final int value;
  
  const ConnectionStatus(int this.value);
}

/**
 * Visual indicator of the server connection.
 */
class ConnectionStatusIndicator
{
  /// Element displaying the status.
  Element _element;
  /// Current status.
  ConnectionStatus _status;
  
  /**
   * Creates an instance of the ConnectionStatusIndicator class.
   */
  ConnectionStatusIndicator(String id)
  {
    _element = document.query(id);
    status = ConnectionStatus.Disconnected;
  }
  
  /**
   * The status of the server connection.
   */
  ConnectionStatus get status() => _status;
  set status(ConnectionStatus value)
  {
    _status = value;
    
    if (_status == ConnectionStatus.Connected)
      _setStyle('connected');
    else if (_status == ConnectionStatus.Connecting)
      _setStyle('connecting');
    else
      _setStyle('disconnected');
  }
  
  void _setStyle(String style)
  {
    _element.classes.clear();
    _element.classes.add(style);
  }
}

/**
 * Error message for connection issues.
 */
class ErrorMessage
{
  /// Element displaying the message.
  Element _element;
  
  /**
   * Create an instance of the ErrorMessage class.
   */
  ErrorMessage(String id)
  {
    _element = document.query(id);
  }
  
  /**
   * Displays the error [message].
   */
  void showError(String message)
  {
    _element.innerHTML = message;
    
    _element.style.animation = 'show 2s';
  }
  
  /**
   * Hides the error message.
   * 
   * This will immediately remove the error message.
   */
  void hideError()
  {
    _element.style.animation = '';
  }
}

/**
 * Controller vibration controls
 */
class VibrationControls
{
  /// Maximum value of the slider
  static double _maxValue = 65535.0;
  
  /// Left motor element
  Element _leftMotorElement;
  /// Right motor element
  Element _rightMotorElement;
  /// Left motor input element
  InputElement _leftMotorInputElement;
  /// Right motor input element
  InputElement _rightMotorInputElement;
  /// Left motor values
  List<double> _leftMotorValues;
  /// Right motor values
  List<double> _rightMotorValues;
  
  /// The index of the player being displayed
  int _playerIndex; 
  
  VibrationControls(String leftId, String rightId)
  {
    _playerIndex = 0;

    // Setup the left motor
    Element leftMotorRoot = document.query(leftId);
    _leftMotorElement = leftMotorRoot.query('.motorValue');
    _leftMotorInputElement = leftMotorRoot.query('input');
    
    _leftMotorValues = [ 0.0, 0.0, 0.0, 0.0 ];
    
    _setupCallback(_leftMotorInputElement, _leftMotorElement, _leftMotorValues);
    
    // Setup the right motor
    Element rightMotorRoot = document.query(rightId);
    _rightMotorElement = rightMotorRoot.query('.motorValue');
    _rightMotorInputElement = rightMotorRoot.query('input');
    
    _rightMotorValues = [ 0.0, 0.0, 0.0, 0.0 ];
    
    _setupCallback(_rightMotorInputElement, _rightMotorElement, _rightMotorValues);
  }
  
  set enabled(bool value)
  {
    bool disabled = !value;
    
    _leftMotorInputElement.disabled = disabled;
    _rightMotorInputElement.disabled = disabled;
  }
  
  int get playerIndex() => _playerIndex;
  set playerIndex(int value)
  {
    _playerIndex = value;
    
    _updateView();
  }
  
  void reset()
  {
    _leftMotorValues[0] = 0.0;
    _leftMotorValues[1] = 0.0;
    _leftMotorValues[2] = 0.0;
    _leftMotorValues[3] = 0.0;
    
    _rightMotorValues[0] = 0.0;
    _rightMotorValues[1] = 0.0;
    _rightMotorValues[2] = 0.0;
    _rightMotorValues[3] = 0.0;
    
    _updateView();
  }
  
  void _updateView()
  {
    double leftMotor = _leftMotorValues[_playerIndex];
    double rightMotor = _rightMotorValues[_playerIndex];
    
    _leftMotorElement.innerHTML = leftMotor.toStringAsPrecision(3);
    _rightMotorElement.innerHTML = rightMotor.toStringAsPrecision(3);
    
    _leftMotorInputElement.valueAsNumber = leftMotor * _maxValue;
    _rightMotorInputElement.valueAsNumber = rightMotor * _maxValue;
  }
  
  void _setupCallback(InputElement input, Element display, List<double> list)
  {
    input.on.change.add((e) {
      // Store information
      double value = input.valueAsNumber / _maxValue;
      list[_playerIndex] = value;
      
      display.innerHTML = value.toStringAsPrecision(3);
      
      // Notify that a value has changed
      _onValueChanged();
    });
  }
  
  void _onValueChanged()
  {
    double leftMotor = _leftMotorValues[_playerIndex];
    double rightMotor = _rightMotorValues[_playerIndex];
    
    GamePad.setVibration(_playerIndex, leftMotor, rightMotor);
  }
}
