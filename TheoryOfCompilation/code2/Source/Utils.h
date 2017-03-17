//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH

#include <fstream>
#include <stdarg.h>
//---------------------------------------------------------------------------
namespace ESL
{                        
//---------------------------------------------------------------------------
class CLogFile
{
private:

    std::ofstream  _LogFile;     //�������� ����� ������, � ������� ������� ���

    unsigned  _ErrorsCount;
    unsigned  _WarningsCount;

    //�������� ����������� ������ � ������ ������� ����
    void  StartupBanner(const char*);
    //�������� ����������� ������ � ����� ������� ����
    void  EndBanner(void);

    //����� � ��� ������� �����
    void  WriteTime(void);
    //����� � ��� ��������� ������ ��������� ������
    void  Write(const char*, const char*, va_list);

public:

    std::ofstream& LogFile(void)           {return _LogFile;} 

    //����� � ��� ������������ ���������
    void  WriteMessage(const char*, ...);
    //����� � ��� ��������������
    void  WriteWarning(const char*, ...);
    //����� � ��� ��������� �� ������
    void  WriteError(const char*, ...);

    //� ������������ ��������� ��� � ���� ���������� ����������� ������
    CLogFile(const char*, const char*);
    //� ����������� ��� ����������� � �����������
    ~CLogFile(void);
};

extern CLogFile ESLLog;

#ifndef _ESLNDEBUG
  /*�������, ��������� ���� ������������� ��� ����� ������� ������
  ��������� �� ������ � ��� � �������� ����������.*/
  //err - ����� ������, ret - ������������ ���������
  #define MACRO_ERROR_RET(err,ret) {ESLLog.WriteError(##err##);return(##ret##);}
  //err - ����� ������; �������������� ����� �� ���������� �������
  #define MACRO_ERROR_RET_VOID(err) {ESLLog.WriteError(##err##);return;}
#else
  #define MACRO_ERROR_RET(err,ret)  {return(##ret##);}
  #define MACRO_ERROR_RET_VOID(err) {return;}
#endif
//---------------------------------------------------------------------------
}




#endif
