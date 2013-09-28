//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 5: interpret an Array
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 5", "interpret an Array");

var wheelCount = Application.car.wheels.length;
Debugger.trace("Info", "There're " + wheelCount + " wheels on this car.");

for (var i = 0; i < wheelCount; i++)
{
	var brand = Application.car.wheels[i].brand;
	var color = Application.car.wheels[i].color;
	Debugger.trace("Info", "Index: " + i + " Brand: " + brand + " Color: " + color);
}