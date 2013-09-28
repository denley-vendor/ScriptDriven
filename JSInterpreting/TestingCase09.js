//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 9: call back to scripts
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 9", "call back to scripts");

Application.getInformation("weather", myCallback);

function myCallback(key, value, errorCode)
{
	Debugger.trace("Function", "script myCallback() called.");
	Debugger.trace("Info", "key: " + key + ", value: " + value + ", error code: " + errorCode);
}