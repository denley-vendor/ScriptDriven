//
// A Practical Guide to Script-Driven Software Development
//

Application.onApplicationLoaded = applicationLoaded;
addEventListener("ev_ButtonClicked", buttonClicked, false);

// This function will be invoked just after the application is loaded.
function applicationLoaded()
{
	mainWindow.buttons["btn_Reload"].enabled = false;
	mainWindow.buttons["btn_Invert"].enabled = false;
	mainWindow.buttons["btn_Greyscale"].enabled = false;
	mainWindow.buttons["btn_Save"].enabled = false;
}

// Handle the button click event
function buttonClicked(ev)
{
	switch (ev.id)
	{
		case "btn_Open":
			getImageFile(getImageFileCompleted);
			break;

		case "btn_Reload":
			imageProcessor.Reload();
			refreshDisplay();
			break;

		case "btn_Invert":
			imageProcessor.invert();
			refreshDisplay();
			break;

		case "btn_Greyscale":
			imageProcessor.greyscale();
			refreshDisplay();
			break;

		case "btn_Save":
			saveImageFile(saveImageFileCompleted);
			break;

		case "btn_About":
			showAboutDialog();
			break;

		case "btn_Min":
			minimizeMainWindow();
			break;

		case "btn_Exit":
			exitApplication();
			break;
	}
}

function getImageFileCompleted(status, filename)
{
	if (status == SUCCEEDED)
	{
		imageProcessor.load(filename);
		updateButtonStatus();
		refreshDisplay();
	}
}

function saveImageFileCompleted(status, filename)
{
	if (status == SUCCEEDED)
	{
		imageProcessor.saveAs(filename);
	}
}

function updateButtonStatus()
{
	if (imageProcessor.isReady())
	{
		mainWindow.buttons["btn_Reload"].enabled = true;
		mainWindow.buttons["btn_Invert"].enabled = true;
		mainWindow.buttons["btn_Greyscale"].enabled = true;
		mainWindow.buttons["btn_Save"].enabled = true;
	}
	
}