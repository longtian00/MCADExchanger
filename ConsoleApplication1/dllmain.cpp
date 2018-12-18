#include"dllmain.h"


static uiCmdAccessState TestAccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}

extern "C" int user_initialize(
	int argc,
	char *argv[],
	char *version,
	char *build,
	wchar_t errbuf[80])
{
	std::ofstream log("logMain.log", std::ofstream::app);  //// Log-Datei zur Fehlersuche

	uiCmdCmdId cmd_id;

	ProError error;


	////////////////////// Button für Import in Creo


	// Festlegen,welche Funktion durch den Button aufgerufen wird
	error = ProCmdActionAdd(
		"Parameter_import",
		(uiCmdCmdActFn)Parameter_import,
		uiProe2ndImmediate,
		TestAccessDefault,
		PRO_B_TRUE,
		PRO_B_TRUE,
		&cmd_id
	);
	if (error != 0)
	{
		log << "ProCmdActionAdd for Parameter returned " << error << std::endl;
	}

	///// Beschriftung des Buttons
	error = ProCmdDesignate(
		cmd_id,
		"import1",
		"import2",
		"import3",
		L"menu.txt"
	);
	if (error != 0)
	{
		log << "ProCmdDesignate for Parameter_import returned " << error << std::endl;
	}


	//////////////////////// Button für Export aus Creo
	error = ProCmdActionAdd(
		"Parameter_export",
		(uiCmdCmdActFn)Parameter_export,
		uiProe2ndImmediate,
		TestAccessDefault,
		PRO_B_TRUE,
		PRO_B_TRUE,
		&cmd_id
	);
	if (error != 0)
	{
		log << "ProCmdActionAdd for Parameter returned " << error << std::endl;
	}
	error = ProCmdDesignate(
		cmd_id,
		"export1",
		"export2",
		"export3",
		L"menu.txt"
	);
	if (error != 0)
	{
		log << "ProCmdDesignate for Parameter_export returned " << error << std::endl;
	}

	

	return 0;
}

extern "C" void user_terminate()
{
}

