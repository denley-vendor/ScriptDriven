//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 8: automatic type conversion
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 8", "automatic type conversion");

// Test some built-in objects
var currentDate = new Date();
Debugger.trace("Info", currentDate);

// Test a user-defined JS object
function Color(r, g, b)
{
	this.r = r;
	this.g = g;
	this.b = b;
}

Color.prototype.toString = function()
{
	return "RGB(" + this.r + "," + this.g + "," + this.b + ")";
}

var myColor = new Color(128, 128, 128);
Debugger.trace("Info", myColor);

// Test our C++ created object
try
{
	var wheel1 = Application.car.createWheel("KUMHO", "red");
	Debugger.trace("Info", wheel1);
}
catch (e)
{
	Debugger.trace("Exception", e.message);
}

// Test our enhanced wheel object
try
{
	var wheel2 = Application.car.createEnhancedWheel("KUMHO", "red");
	Debugger.trace("Info", wheel2);
}
catch (e)
{
	Debugger.trace("Exception", e.message);
}
