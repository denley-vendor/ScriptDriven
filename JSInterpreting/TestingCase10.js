//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 10: retrieve variables and invoke functions of scripts from the C++ side
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 10", "set/get value to script variables");
Debugger.trace("Testing Case 10", "invoke script functions");

var BOOK_TITLE = "A Practical Guide to Script-Driven Software Development";
var bookCount  = 5000;

function showMessage(msg)
{
	Debugger.trace("showMessage", msg);
}

Application.reviewScripts();