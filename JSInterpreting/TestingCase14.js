//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 14: step into scripts for debugging
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 14", "step into scripts for debugging");

pauseForDebugger();

var curDate = new Date();
var msg = "Today is ";
msg += (curDate.getMonth() + 1) + "/";
msg += curDate.getDate() + "/";
msg += curDate.getYear() + ".";
Debugger.trace("Info", msg);

Debugger.trace("Info", NaN);
Debugger.trace("Info", undefined);
Debugger.trace("Info", null);