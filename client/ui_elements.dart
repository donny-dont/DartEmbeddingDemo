
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
  
}
