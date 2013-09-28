//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 12: support some kind of timer for scripts
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 12", "support some kind of timer for scripts");

Application.createTimer(1000, false, timerProc1);
Application.createTimer(2000, true, timerProc2);

function timerProc1()
{
	Debugger.trace("Info", "timerProc1() is executed!");
}

function timerProc2()
{
	Debugger.trace("Info", "timerProc2() is executed!");
}