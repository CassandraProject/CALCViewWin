/*
* CALCView is a Calc Block Editor
* Copyright (C) 2003
*
* Created by Tod Baudais
*
* This file is part of CALCView.
*
* CALCView is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* CALCView is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CALCView.  If not, see <http://www.gnu.org/licenses/>.
*/
// ConnectBox.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "ConnectBox.h"
#include ".\connectbox.h"

#include "Preferences.h"
#include <sstream>
#include <iostream>

#include "Constants.h"
#include "Mem_Status.h"

#include "StringUtils.h"

#include "CALCViewWinDoc.h"
#include <fstream>

//#include <foxapi.h>


// CConnectBox dialog

IMPLEMENT_DYNAMIC(CConnectBox, CDialog)
CConnectBox::CConnectBox(CWnd* pParent /*=NULL*/, Machine *m, CCALCViewWinDoc *doc)
	: CDialog(CConnectBox::IDD, pParent)
{
	m_machine = m;
	m_doc = doc;	
}

CConnectBox::~CConnectBox()
{
}

void CConnectBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cp);
	DDX_Control(pDX, IDC_EDIT1, m_compoundblock);
	DDX_Control(pDX, IDC_EDIT3, m_messages);
}


BEGIN_MESSAGE_MAP(CConnectBox, CDialog)
	ON_BN_CLICKED(IDC_GET_BLOCK, OnBnClickedGetBlock)
	ON_BN_CLICKED(IDC_ADD_BLOCK, OnBnClickedAddBlock)
	ON_BN_CLICKED(IDC_MODIFY_BLOCK, OnBnClickedModifyBlock)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConnectBox message handlers

void CConnectBox::OnBnClickedGetBlock()
{
	CString cp,cb;
	m_cp.GetWindowText(cp);
	m_compoundblock.GetWindowText(cb);

	std::stringstream instream, outstream, errorstream;
	
	instream << "OPEN " << LPCTSTR(cp) << " READ CALCView" << "\r\n";
	instream << "GET " << LPCTSTR(cb) << " ALL" << "\r\n";
	instream << "CLOSE" << "\r\n";
	instream << "EXIT" << "\r\n" << "\r\n";
	
	m_messages.SetWindowText(_T("Running ICC Driver Task..."));
	m_messages.RedrawWindow();
	
	// Run the ICC driver task
	RunICC (instream, outstream, errorstream);
	
	m_messages.SetWindowText(outstream.str().c_str());
	
	// Tokenize the output
	std::list<std::string>	lines = Tokenize(outstream.str(), false);
	std::list<std::string>::iterator lines_iter = lines.begin();
	
	for (lines_iter = lines.begin(); lines_iter != lines.end(); ++lines_iter) {
		std::string s = *lines_iter;
		std::string::size_type location = s.find_first_of("=");
		
		// If there is an equals sign
		if (location != std::string::npos) {
			std::string reg_name = Trim(s.substr(0,location));
			std::string reg_config = Trim(s.substr(location + 2));
							
			// We have to change the type of the block if we get the TYPE param
			if (reg_name == "TYPE") {
				if (reg_config == "CALC")		m_doc->OnControlsCalc();
				else if (reg_config == "CALCA")	m_doc->OnControlsCalca();
				else if (reg_config == "MATH")	m_doc->OnControlsMath();
				else if (reg_config == "LOGIC")	m_doc->OnControlsLogic();
			}
			
			// Set the configuration
			try {
				int i = m_machine->Name_To_Index_Memory(reg_name);
				Mem_Status *reg = m_machine->Index_To_Memory(i);
				reg->Set_Configuration(reg_config);
			}
			catch (...) {
			
			}
		}
	}
	
}

void CConnectBox::OnBnClickedAddBlock()
{
	CString cp,cb;
	m_cp.GetWindowText(cp);
	m_compoundblock.GetWindowText(cb);

	std::stringstream instream, outstream, errorstream;
	
	instream << "OPEN " << LPCTSTR(cp) << " ALL CALCView" << "\r\n";
	instream << "ADD " << LPCTSTR(cb) << "\r\n";
	
	for (int i = 0; i < NUM_REGISTERS; ++i) {
		// Don't need to add the NAME parameter
		if (i == NAME)	continue;
	
		try {
			std::string reg_name = m_machine->Index_To_Name_Memory(i);
			
			Mem_Status *reg = m_machine->Index_To_Memory(i);
			std::string reg_value = reg->Get_Configuration();
			
			// Make sure the parameter is configurable
			if (reg->Can_Config())
				instream << reg_name << " = " << reg_value << "\r\n";
			
		}
		catch (...) {
		
		}
	}
	
	instream << "END" << "\r\n";
	instream << "CLOSE" << "\r\n";
	instream << "EXIT" << "\r\n" << "\r\n";
	
	m_messages.SetWindowText(_T("Running ICC Driver Task...\r\nIt may take a few minutes because\r\na checkpoint is required."));
	m_messages.RedrawWindow();
	
	// Run the ICC driver task
	RunICC (instream, outstream, errorstream);
	
	m_messages.SetWindowText(outstream.str().c_str());
}

void CConnectBox::OnBnClickedModifyBlock()
{
	CString cp,cb;
	m_cp.GetWindowText(cp);
	m_compoundblock.GetWindowText(cb);

	std::stringstream instream, outstream, errorstream;
	
	instream << "OPEN " << LPCTSTR(cp) << " ALL CALCView" << "\r\n";
	instream << "MODIFY " << LPCTSTR(cb) << "\r\n";
	
	for (int i = 0; i < NUM_REGISTERS; ++i) {
		// Don't need to add the NAME or TYPE parameter
		if (i == NAME)	continue;
		if (i == TYPE)	continue;
	
		try {
			std::string reg_name = m_machine->Index_To_Name_Memory(i);
			
			Mem_Status *reg = m_machine->Index_To_Memory(i);
			std::string reg_value = reg->Get_Configuration();
			
			// Make sure the parameter is configurable
			if (reg->Can_Config())
				instream << reg_name << " = " << reg_value << "\r\n";
			
		}
		catch (...) {
		
		}
	}
	
	instream << "END" << "\r\n";
	instream << "CLOSE" << "\r\n";
	instream << "EXIT" << "\r\n" << "\r\n";
	
	m_messages.SetWindowText(_T("Running ICC Driver Task...\r\nIt may take a few minutes because\r\na checkpoint is required."));
	m_messages.RedrawWindow();
	
	// Run the ICC driver task
	RunICC (instream, outstream, errorstream);
	
	m_messages.SetWindowText(outstream.str().c_str());
}

void CConnectBox::OnBnClickedOk()
{
	CString cp,cb;
	m_cp.GetWindowText(cp);
	m_compoundblock.GetWindowText(cb);
	
	Prefs_Set_Last_CB(cb);
	Prefs_Set_Last_CP(cp);
	
	OnOK();
}

BOOL CConnectBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CRect bounds;
	m_cp.GetWindowRect(&bounds);
	bounds.bottom = bounds.top + (bounds.bottom - bounds.top) * 5;
	m_cp.SetWindowPos(NULL,0,0,bounds.Width(),bounds.Height(),SWP_NOMOVE | SWP_NOZORDER);
	
	std::ifstream f;
	f.open(LPCTSTR(Prefs_Get_CPLNS_Path()), std::ios::in);
	
	char buffer[1024];
	
	while (1) {
		f.getline(buffer, 1024);
		
		// Make sure the line was read
		if (f.eof() || f.bad() || f.fail())
			break;
		
		m_cp.AddString(_T(buffer));
	};
	
	f.close();

	m_cp.SetWindowText(Prefs_Get_Last_CP());
	m_compoundblock.SetWindowText(Prefs_Get_Last_CB());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*
Creating a Child Process with Redirected Input and Output

The example in this topic demonstrates how to create a child process from a console 
process. It also demonstrates a technique for using anonymous pipes to redirect the 
child process's standard input and output handles. Note that named pipes can also be 
used to redirect process I/O.

The CreatePipe function uses the SECURITY_ATTRIBUTES structure to create inheritable 
handles to the read and write ends of two pipes. The read end of one pipe serves as 
standard input for the child process, and the write end of the other pipe is the 
standard output for the child process. These pipe handles are specified in the 
SetStdHandle function, which makes them the standard handles inherited by the child 
process. After the child process is created, SetStdHandle is used again to restore 
the original standard handles for the parent process.

The parent process uses the other ends of the pipes to write to the child process's 
input and read the child process's output. The handles to these ends of the pipe are 
also inheritable. However, the handle must not be inherited. Before creating the 
child process, the parent process must use DuplicateHandle to create a duplicate of 
the application-defined m_hChildStdinWr global variable that cannot be inherited. It 
then uses CloseHandle to close the inheritable handle. For more information, see 
Pipes.

*/



void CConnectBox::RunICC (std::stringstream &in, std::stringstream &out, std::stringstream &error)
{
	const int BUFSIZE = 4096;


	SECURITY_ATTRIBUTES saAttr; 
	BOOL fSuccess; 

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 



	// Create a pipe for the child process's STDOUT. 
	if (! CreatePipe(&m_hChildStdoutRd, &m_hChildStdoutWr, &saAttr, 0)) {
		AfxMessageBox(_T("Stdout pipe creation failed"));
		return;
	}

	// Create noninheritable read handle and close the inheritable read handle. 
	fSuccess = DuplicateHandle(	GetCurrentProcess(), m_hChildStdoutRd,
								GetCurrentProcess(), &m_hChildStdoutRdDup , 0,
								FALSE,
								DUPLICATE_SAME_ACCESS);
	if( !fSuccess ) {
		AfxMessageBox(_T("DuplicateHandle failed"));
		return;
	}
		
	CloseHandle(m_hChildStdoutRd);



	// Create a pipe for the child process's STDIN. 
	if (! CreatePipe(&m_hChildStdinRd, &m_hChildStdinWr, &saAttr, 0)) {
		AfxMessageBox(_T("Stdin pipe creation failed"));
		return;
	}

	// Duplicate the write handle to the pipe so it is not inherited. 
	fSuccess = DuplicateHandle(	GetCurrentProcess(), m_hChildStdinWr, 
								GetCurrentProcess(), &m_hChildStdinWrDup, 0, 
								FALSE,                  // not inherited 
								DUPLICATE_SAME_ACCESS); 
	if (! fSuccess) {
		AfxMessageBox(_T("DuplicateHandle failed"));
		return;
	}

	CloseHandle(m_hChildStdinWr); 
	
	
	/*// Create a pipe for the child process's Error. 
	if (! CreatePipe(&m_hChildErrorRd, &m_hChildErrorWr, &saAttr, 0)) {
		AfxMessageBox(_T("Error pipe creation failed"));
		return;
	}

	// Create noninheritable read handle and close the inheritable read handle. 
	fSuccess = DuplicateHandle(	GetCurrentProcess(), m_hChildErrorRd,
								GetCurrentProcess(), &m_hChildErrorRdDup , 0,
								FALSE,
								DUPLICATE_SAME_ACCESS);
	if( !fSuccess ) {
		AfxMessageBox(_T("DuplicateHandle failed"));
		return;
	}
		
	CloseHandle(m_hChildErrorRd);*/


	// Now create the child process. 

	fSuccess = CreateChildProcess(m_hChildStdinRd, m_hChildStdoutWr, m_hChildErrorWr);
	if (! fSuccess) {
		AfxMessageBox(_T("Create process failed"));
		return;
	} 

	/*// After process creation, restore the saved STDIN and STDOUT. 
	if (! SetStdHandle(STD_INPUT_HANDLE, m_hSaveStdin)) {
		AfxMessageBox(_T("Re-redirecting Stdin failed"));
		return;
	} 

	if (! SetStdHandle(STD_OUTPUT_HANDLE, m_hSaveStdout)) {
		AfxMessageBox(_T("Re-redirecting Stdout failed"));
		return;
	} */

	// Write to pipe that is the standard input for a child process. 
	DWORD dwRead, dwWritten;
	WriteFile(m_hChildStdinWrDup, in.str().c_str(), in.str().size(), &dwWritten, NULL);
	
	// Close the pipe handle so the child process stops reading.
	if (!CloseHandle(m_hChildStdinWrDup)) {
		AfxMessageBox(_T("Close pipe failed"));
		return;
	} 

	//Sleep(2000);

	// Close the unneeded ends of the pipe
	if (!CloseHandle(m_hChildStdoutWr)) {
		AfxMessageBox(_T("Closing handle failed"));
		return;
	}
	if (!CloseHandle(m_hChildStdinRd)) {
		AfxMessageBox(_T("Closing handle failed"));
		return;
	}
	/*if (!CloseHandle(m_hChildErrorRd)) {
		AfxMessageBox(_T("Closing handle failed"));
		return;
	}*/


	// Read from pipe that is the standard output for child process. 
	CHAR chBuf[BUFSIZE]; 

	// Read output from the child process 
	while (1) { 
		if( !ReadFile( m_hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead, NULL) || dwRead == 0) 
			break; 
		
		// Get buffer as string
		std::string buf(chBuf,0,dwRead);	
		
		// Replace newlines with \r\n
		for (std::string::size_type i = buf.find_first_of('\n',0);	i != std::string::npos; i = buf.find_first_of('\n',i+2)) {
			buf.insert(i,"\r");
		}
		
		std::cout << buf;
		
		// Copy to out stringstream
		out << buf;
	}  

	// Close the pipe handle so the child process stops reading.
	if (!CloseHandle(m_hChildStdoutRdDup)) {
		AfxMessageBox(_T("Close pipe failed"));
		return;
	} 
	
	
	// Read errors from the child process 
	/*while (1) { 
		if( !ReadFile( m_hChildErrorRdDup, chBuf, BUFSIZE, &dwRead, NULL) || dwRead == 0) 
			break; 
		
		// Get buffer as string
		std::string buf(chBuf,0,dwRead);	
		
		// Replace newlines with \r\n
		for (std::string::size_type i = buf.find_first_of('\n',0);	i != std::string::npos; i = buf.find_first_of('\n',i+2)) {
			buf.insert(i,"\r");
		}
		
		std::cout << buf;
		
		// Copy to out stringstream
		error << buf;
	}  

	// Close the pipe handle so the child process stops reading.
	if (!CloseHandle(m_hChildErrorRdDup)) {
		AfxMessageBox(_T("Close pipe failed"));
		return;
	} */
	
	return; 
}

BOOL CConnectBox::CreateChildProcess(HANDLE hstdin, HANDLE hstdout, HANDLE herror) 
{ 
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE; 

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	
	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	siStartInfo.hStdError = hstdout; //herror;
	siStartInfo.hStdInput = hstdin;
	siStartInfo.hStdOutput = hstdout;
	siStartInfo.wShowWindow = false;
	
	char *iccpath = (char *) LPCTSTR(Prefs_Get_ICC_Path());

	// Create the child process. 
	bFuncRetn = CreateProcess(NULL, 
		iccpath,       // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0) {
		DWORD err = GetLastError();
		return 0;
	} else {
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		return bFuncRetn;
	}
}



