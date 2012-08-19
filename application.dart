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

#import('dart:html');
#import('dart:json');
#source('game_pad.dart');
#source('game_pad_view.dart');
#source('ui_elements.dart');

class Application
{
  /// Whether the server has been connected to.
  bool _connected;
  /// Error message
  ErrorMessage _errorMessage;
  /// Element containing the url to connect to.
  InputElement _url;
  /// Button to initiate the server connection.
  InputElement _connectButton;
  /// Status indicator for the server connection.
  ConnectionStatusIndicator _statusIndicator;
  /// The currently selected tab.
  Element _selectedTab;
  /// Canvas that the game pad's state is displayed on.
  GamePadView _gamePadCanvas;
  /// Vibration controls
  VibrationControls _vibrationControls;

  //---------------------------------------------------------------------
  // Initialization
  //---------------------------------------------------------------------

  Application()
  {
    _connected = false;

    // Create the error message
    _errorMessage = new ErrorMessage('#errorMessage');

    // Setup the gamepad canvas
    _gamePadCanvas = new GamePadView('#gamepad');

    // Setup the tabbed UI
    _selectedTab = document.query('#player1');
    _setupTab('#player1', 0);
    _setupTab('#player2', 1);
    _setupTab('#player3', 2);
    _setupTab('#player4', 3);

    // Setup the connection button
    _connectButton = document.query('#connect');
    _connectButton.on.click.add(_onButtonClicked);
    _url = document.query('#url');

    // Setup the status indicator
    _statusIndicator = new ConnectionStatusIndicator('#status');

    // Setup the vibration controls
    _vibrationControls = new VibrationControls('#leftMotor', '#rightMotor');
  }

  void _setupTab(String id, int index)
  {
    Element tab = document.query(id);

    tab.on.click.add((e) {
      // Remove the selected class
      _selectedTab.classes.remove('selected');

      // Set the tab as selected
      tab.classes.add('selected');
      _selectedTab = tab;

      // Notify that the tab was clicked
      _onTabClicked(index);
    });
  }

  //---------------------------------------------------------------------
  // Update methods
  //---------------------------------------------------------------------

  void update()
  {
    _gamePadCanvas.update();
  }

  //---------------------------------------------------------------------
  // Connection methods
  //---------------------------------------------------------------------

  /**
   * Callback to initiate/terminate the server connection.
   */
  void _onButtonClicked(_)
  {
    if (_connected)
    {
      // Close the connection
      // UI will be updated in the close callbck
      GamePad.disconnectFromServer();
    }
    else
    {
      // Initiate the connection
      GamePad.connectToServer(_url.value, _onConnectionOpened, _onConnectionClosed);

      // Update the UI
      _onConnectionInitiated();
    }
  }

  /**
   * Callback for when the server connection is initiated.
   */
  void _onConnectionInitiated()
  {
    // Disble the button
    _connectButton.disabled = true;

    // Hide the error message
    _errorMessage.hideError();

    // Disble the url field
    _url.disabled = true;

    // Change the status indicator
    _statusIndicator.status = ConnectionStatus.Connecting;
  }

  /**
   * Callback for when the server connection is opened.
   */
  void _onConnectionOpened(_)
  {
    // Connection was opened
    _connected = true;

    // Change the button to disconnect
    _connectButton.disabled = false;
    _connectButton.value = 'Disconnect';
    _connectButton.classes.clear();
    _connectButton.classes.add('disconnect');

    // Change the status indicator
    _statusIndicator.status = ConnectionStatus.Connected;

    // Enable vibration controls
    _vibrationControls.enabled = true;
  }

  /**
   * Callback for when the server connection is closed.
   */
  void _onConnectionClosed(CloseEvent e)
  {
    // Check for an abnomal close
    if (e.code == 1006)
    {
      String message = _connected ? "Server connection was lost" : "Could not connect to server";

      _errorMessage.showError(message);
    }

    // Connection was closed
    _connected = false;

    // Enable the url field
    _url.disabled = false;

    // Change the button to connect
    _connectButton.disabled = false;
    _connectButton.value = 'Connect';
    _connectButton.classes.clear();
    _connectButton.classes.add('connect');

    // Change the status indicator
    _statusIndicator.status = ConnectionStatus.Disconnected;

    // Disable vibration controls
    _vibrationControls.reset();
    _vibrationControls.enabled = false;
  }

  //---------------------------------------------------------------------
  // Tab methods
  //---------------------------------------------------------------------

  /**
   * Callback for when a tab is selected.
   */
  void _onTabClicked(int index)
  {
    _gamePadCanvas.playerIndex = index;
    _vibrationControls.playerIndex = index;
  }
}

/// The application
Application _application;

/**
 * Updates the canvas.
 */
bool update(int time)
{
  _application.update();

  window.requestAnimationFrame(update);
}

void main()
{
  // Setup the gamepad class
  GamePad.onInitialize();

  // Setup the application
  _application = new Application();

  // Start the canvas animation
  window.requestAnimationFrame(update);
}
