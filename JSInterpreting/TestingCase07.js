//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 7: pass a null parameter
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 7", "pass a null parameter");

try
{
	Application.car.installWheel(null, 0);
}
catch (e)
{
	Debugger.trace("Exception", "Failed to uninstall the wheel.");
	Debugger.trace("Exception", e + " " + e.message);
}

Application.car.installWheel2(null, 0);
