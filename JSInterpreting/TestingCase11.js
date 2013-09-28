//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

// Testing Case 11: multi-thread issue for script interpreting
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 11", "multi-thread issue for script interpreting");

Application.copyFile("C:\\a.js", "C:\\b.js", fileCopyComplete);

function fileCopyComplete(srcFile, dstFile, status)
{
	var msg = "Copy " + srcFile + " to " + dstFile + " completed. "
	msg += "Status: " + status + ".";
	Debugger.trace("Testing Case 11", msg);
}