//---------------------------------------------------------------------------

#ifndef VarH
#define VarH
//---------------------------------------------------------------------------
namespace ESL
{
//---------------------------------------------------------------------------

//��������� ���� ������ �������
enum E_VAR_TYPES
{
    vtInt,
    vtString,  //������, ���� char[N]
    vtUnknown  //��� �����������/����������
};

//---------------------------------------------------------------------------

//������������ ��� �������� ������ ���������� ����
class CVar
{
private:

    E_VAR_TYPES _Type;

    union UData
    {
        int    IVal;
        char*  SVal;
    };

    UData       _Data;

    static  int  _ErrorsCount; //���������� ������ � ���� ������ c ��������� CVar

    //�������� ������ �� ������� ����������
    void  Copy(const CVar&);

public:

    E_VAR_TYPES  Type(void)const        {return _Type;}
    bool         WereErrors(void)const  {return _ErrorsCount>0;}

    //��������� ��������������:
    operator  int         ()const;
    operator  const char* ()const;

    CVar& operator= (const CVar&);
    bool  operator==(const CVar&);
    bool  operator!=(const CVar&);
    bool  operator> (const CVar&);
    bool  operator>=(const CVar&);
    bool  operator< (const CVar&);
    bool  operator<=(const CVar&);
    CVar& operator+ (const CVar&);
    CVar& operator+=(const CVar&);
    void  operator++(void);
    CVar& operator- (const CVar&);
    CVar& operator-=(const CVar&);
    void  operator--(void);
    CVar& operator* (const CVar&);
    CVar& operator*=(const CVar&);
    CVar& operator/ (const CVar&);
    CVar& operator/=(const CVar&);

    CVar(void);
    CVar(const int&);
    CVar(const char*);
    CVar(const CVar&);
    ~CVar(void);
};









//---------------------------------------------------------------------------
}

#endif
