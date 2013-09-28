//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

Debugger.trace("Info", GetInfo());

// Testing Case 1: Named Item and Global Members
Debugger.trace("Testing Case 1", "Named Item and Global Members");

Application.alert("Called by Application.alert().");
alert("Called by alert() -- Global Members.");

function GetInfo()
{
	var d, s;

	d = new Date();

	s = "You are running ";
	s += ScriptEngine() + " Version ";
	s += ScriptEngineMajorVersion() + ".";
	s += ScriptEngineMinorVersion() + ".";
	s += ScriptEngineBuildVersion() + " at ";
	s += d.getHours() + ":";
	s += d.getMinutes() + ":";
	s += d.getSeconds() + ":";
	s += d.getMilliseconds() + " ";
	s += (d.getMonth() + 1) + "/";
	s += d.getDate() + "/";
	s += d.getYear() + "\n";

	return s;
}