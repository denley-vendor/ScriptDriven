//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 2: Methods and Properties
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 2", "Methods and Properties");

var msg;
msg  = "Media Player, Version " + MediaPlayer.majorVersion + "." + MediaPlayer.minorVersion;
msg += ", Vendor: " + MediaPlayer.vendor;
Debugger.trace("Info", msg);
Debugger.trace("Info", "Current position: " + MediaPlayer.currentPosition);

MediaPlayer.play("c:\\HelloWorld.mpg");
MediaPlayer.currentPosition = 888;
Debugger.trace("Info", "Current position: " + MediaPlayer.currentPosition);

MediaPlayer.pauseOn();
MediaPlayer.pauseOff();
MediaPlayer.Stop();