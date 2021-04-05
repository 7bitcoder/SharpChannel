#include "StdComunicator.hpp"
#include "OsFlags.hpp"


namespace pm {
    #ifdef WIN
    namespace {
        long runCommand(const std::string& command, pm::CallbacksMap& map);
        void sendDataToCommand(const std::string& data);
    }

    void StdComunicator::run() {
        runCommand(_settings.childProcessCommand, *_callbacks);
    }

    #include <windows.h> 
    #include <tchar.h>
    #include <stdio.h> 
    #include <strsafe.h>
    #define BUFSIZE 4096
    namespace {

        HANDLE g_hChildStd_IN_Rd = NULL;
        HANDLE g_hChildStd_IN_Wr = NULL;
        HANDLE g_hChildStd_OUT_Rd = NULL;
        HANDLE g_hChildStd_OUT_Wr = NULL;

        HANDLE g_hInputFile = NULL;
        CHAR chBuf[BUFSIZE]; 
        
        void CreateChildProcess(const std::string& command); 
        bool ReadFromPipe(std::string& data); 
        void ErrorExit(const char*); 
        
        long runCommand(const std::string& command, pm::CallbacksMap& map) { 
            SECURITY_ATTRIBUTES saAttr; 
            
            printf("\n->Start of parent execution.\n");

            // Set the bInheritHandle flag so pipe handles are inherited. 
            
            saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
            saAttr.bInheritHandle = TRUE; 
            saAttr.lpSecurityDescriptor = NULL; 

            // Create a pipe for the child process's STDOUT. 
            
            if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
                ErrorExit("StdoutRd CreatePipe"); 

            // Ensure the read handle to the pipe for STDOUT is not inherited.

            if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
                ErrorExit("Stdout SetHandleInformation"); 

            // Create a pipe for the child process's STDIN. 
            
            if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
                ErrorExit("Stdin CreatePipe"); 

            // Ensure the write handle to the pipe for STDIN is not inherited. 
            
            if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
                ErrorExit("Stdin SetHandleInformation"); 
            
            // Create the child process. 
            
            CreateChildProcess(command);

            // Get a handle to an input file for the parent. 
            // This example assumes a plain text file and uses string output to verify data flow. 
            
            if (command.empty()) 
                ErrorExit("Please specify an input file.\n"); 

            // Write to the pipe that is the standard input for a child process. 
            // Data is written to the pipe's buffers, so it is not necessary to wait
            // until the child process is running before writing data.
            while (true)
            {
                std::string msg;
                if(!ReadFromPipe(msg)) {
                    break;
                } 

                for(auto& next: map) {
                    auto result = next.second->next(msg);
                }
            }
            
            // The remaining open handles are cleaned up when this process terminates. 
            // To avoid resource leaks in a larger application, close handles explicitly. 

            return 0; 
        } 
        
        void CreateChildProcess(const std::string& command) {
            // Create a child process that uses the previously created pipes for STDIN and STDOUT.
            PROCESS_INFORMATION piProcInfo; 
            STARTUPINFO siStartInfo;
            BOOL bSuccess = FALSE; 
            
            // Set up members of the PROCESS_INFORMATION structure. 
            
            ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
            
            // Set up members of the STARTUPINFO structure. 
            // This structure specifies the STDIN and STDOUT handles for redirection.
            
            ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
            siStartInfo.cb = sizeof(STARTUPINFO); 
            siStartInfo.hStdError = g_hChildStd_OUT_Wr;
            siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
            siStartInfo.hStdInput = g_hChildStd_IN_Rd;
            siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
            
            // Create the child process. 
                
            bSuccess = CreateProcess(NULL, 
                (TCHAR*)command.c_str(),     // command line 
                NULL,          // process security attributes 
                NULL,          // primary thread security attributes 
                TRUE,          // handles are inherited 
                0,             // creation flags 
                NULL,          // use parent's environment 
                NULL,          // use parent's current directory 
                &siStartInfo,  // STARTUPINFO pointer 
                &piProcInfo);  // receives PROCESS_INFORMATION 
            
            // If an error occurs, exit the application. 
            if ( ! bSuccess ) 
                ErrorExit("CreateProcess");
            else 
            {
                // Close handles to the child process and its primary thread.
                // Some applications might keep these handles to monitor the status
                // of the child process, for example. 

                CloseHandle(piProcInfo.hProcess);
                CloseHandle(piProcInfo.hThread);
                
                // Close handles to the stdin and stdout pipes no longer needed by the child process.
                // If they are not explicitly closed, there is no way to recognize that the child process has ended.
                
                CloseHandle(g_hChildStd_OUT_Wr);
                CloseHandle(g_hChildStd_IN_Rd);
            }
        }
        
        void sendDataToCommand(const std::string& data) {

            // Read from a file and write its contents to the pipe for the child's STDIN.
            // Stop when there is no more data. 
            DWORD dwWritten; 
            BOOL bSuccess = FALSE;
            
            bSuccess = WriteFile(g_hChildStd_IN_Wr, data.c_str(), data.length(), &dwWritten, NULL);
        } 
            
        bool ReadFromPipe(std::string& data) {

            // Read output from the child process's pipe for STDOUT
            // and write to the parent process's pipe for STDOUT. 
            // Stop when there is no more data. 
            DWORD dwRead; 
            BOOL bSuccess = FALSE;

            bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            if( !bSuccess ) return false;

            chBuf[dwRead] = '\0';
            data = chBuf;
            return true;
        } 
        
        void ErrorExit(const char* lpszFunction) {

            // Format a readable error message, display a message box, 
            // and exit from the application.
            /*LPVOID lpMsgBuf;
            LPVOID lpDisplayBuf;
            DWORD dw = GetLastError(); 

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0, NULL );

            lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
                (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
            StringCchPrintf((LPTSTR)lpDisplayBuf, 
                LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                TEXT("%s failed with error %d: %s"), 
                lpszFunction, dw, lpMsgBuf); 
            MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

            LocalFree(lpMsgBuf);
            LocalFree(lpDisplayBuf);
            ExitProcess(1);*/
        }
    }
    #endif
}