//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 4: pass scripting objects to the C++ side
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 4", "pass scripting objects to the C++ side");

var kumhoWheel = Application.car.createWheel("KUMHO", "black");
Application.car.installWheel(kumhoWheel, 0);


//////////////////////////////////////////////////////////////////////////////
// Define a JavaScript object
function Wheel(brand, color)
{
	this.brand = brand;
	this.color = color;
}

Wheel.prototype.rotate = function()
{
	Debugger.trace("Function", "Wheel.prototype.rotate called.");
}

Wheel.prototype.adjustParameters = function(param1, param2, param3)
{
	var msg = "Wheel.prototype.adjustParameters called. Adjusted to: ";
	msg += param1 + ", " + param2 + ", " + param3;
	Debugger.trace("Function", msg);
}

var jsWheel = new Wheel("MICHELIN", "gray");
Application.car.installWheel(jsWheel, 1);