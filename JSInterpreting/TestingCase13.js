//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 13: dynamic properties (IDispatchEx)
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 13", "dynamic properties (IDispatchEx)");

try
{
	var wheel1 = Application.car.createWheel("KUMHO", "red");
	wheel1.price = 500;
}
catch (e)
{
	Debugger.trace("Exception", e.message);
}

var wheel2 = Application.car.createDynamicWheel("KUMHO", "red");
Debugger.trace("Info", "The wheel's brand is " + wheel2.brand);
Debugger.trace("Info", "The wheel's color is " + wheel2.color);
Debugger.trace("Info", "The wheel's (original) price is " + wheel2.price);
wheel2.price = 500;
Debugger.trace("Info", "The wheel's (new) price is " + wheel2.price);