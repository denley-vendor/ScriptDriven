//*****************************************************************************
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 5/1/2006
//*****************************************************************************

function UNMASK(number)
{
	return(number&0xffff);
}

function DistinguishErrorType(e)
{
	if (e instanceof EvalError)
	{
		Debugger.trace("Info", "catch a built-in EvalError.");
	}
	else if (e instanceof RangeError)
	{
		Debugger.trace("Info", "catch a built-in RangeError.");
	}
	else if (e instanceof ReferenceError)
	{
		Debugger.trace("Info", "catch a built-in ReferenceError.");
	}
	else if (e instanceof SyntaxError)
	{
		Debugger.trace("Info", "catch a built-in SyntaxError.");
	}
	else if (e instanceof TypeError)
	{
		Debugger.trace("Info", "catch a built-in TypeError.");
	}
	else if (e instanceof URIError)
	{
		Debugger.trace("Info", "catch a built-in URIError.");
	}
	else
	{
		Debugger.trace("Info", "catch a normal Error.");
	}
}

// Testing Case 6: Exception handling
Debugger.addBlankLine(1);
Debugger.trace("Testing Case 6", "Exception handling");

try 
{
	var a = b;
	Debugger.trace("Info", "After the statement which throws an exception.");
}
catch (e)
{
	Debugger.trace("Info", e + " " + e.message + ", code: " + e.number);
}
finally
{
	Debugger.trace("Info", "Reach finally block.");
}

// Testing all built-in error objects.
try
{
	var err;
//	err = new Error("built-in (Error)");
//	err = new EvalError("built-in (EvalError)");
//	err = new RangeError("built-in (RangeError)");
//	err = new ReferenceError("built-in (ReferenceError)");
//	err = new SyntaxError("built-in (SyntaxError)");
//	err = new TypeError("built-in (TypeError)");
	err = new URIError("built-in (URIError)");
	err.number = 121;
	throw err;
}
catch (e)
{
	DistinguishErrorType(e);
	Debugger.trace("Info", e + " " + e.message + ", code: " + e.number);
}

try
{
	Application.triggerException();
}
catch (e)
{
	Debugger.trace("Info", e + " " + e.message);
	Debugger.trace("Info", "Catch an exception, error code: " + UNMASK(e.number));
}