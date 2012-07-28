
/**
 * Status of the server connection.
 */
class ConnectionStatus
{
  /// Connected to the server.
  static final int Connected = const ConnectionStatus(0);
  /// Connecting to the server.
  static final int Connecting = const ConnectionStatus(1);
  /// Disconnected from the server.
  static final int Disconnected = const ConnectionStatus(2);
  
  int value;
  
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
  int get status() => _status;
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
    _element.styles.clear();
    _element.styles.add(style);
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
