#pragma once

// This is the 'implementation' class for the PIMPL
// or "pointer to implementation" pattern. 
// 
// This one is completely hidden by another class. 
// This is because C++ doesn't have the concept of 'partial classes'
// so we put all the 'guts' of the code, including private stuff, here. 
// 
// This way we can't chage this class all we want and not force a re-compile. 

#include <string>

class cAssimpHelper_Imp
{
public:
	cAssimpHelper_Imp();
	~cAssimpHelper_Imp();

	bool Load3DModelFile(std::string filename);
	void SetBasePath(std::string basepath_no_end_slash);

	std::string getLastError(bool bAndClearErrors /*=true*/);


private:
	std::string m_basePath_no_end_slash;

	void m_AppendErrorString(std::string errorString);
	std::string m_LastError;

};

