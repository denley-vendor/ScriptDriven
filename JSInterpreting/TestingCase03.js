//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 3: pass C++ objects to scripts
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 3", "pass C++ objects to scripts");

var whatEngine = Application.car.engine.brand; // create objects
Debugger.trace("Info", "Got value of Application.car.engine.brand: " + whatEngine);
Application.car.engine.run();

var newWheel = Application.car.createWheel("KUMHO", "black"); // create a new object
newWheel.adjustParameters("new", 1, 2);