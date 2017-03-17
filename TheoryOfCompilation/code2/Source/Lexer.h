//---------------------------------------------------------------------------

#ifndef LexerH
#define LexerH

#pragma warning(disable:4786)
#include <string>
#include <vector>
using namespace std;

//---------------------------------------------------------------------------
namespace ESL
{
//---------------------------------------------------------------------------

//��������� ���� ������
enum E_TOKEN_TYPES
{
    ttResWord = 0, //�������� (�����������������) �����
    ttOperator,    //��������
    ttStrConstant, //��������� ���������
    ttIntConstant, //�������� ���������
    ttVariable,    //����������
    ttFunction,    //�������
    ttDevider,     //�����������
    ttUnknown      //��� �� ���������, ���� �� ��������
};

//---------------------------------------------------------------------------

class CToken
{
friend class CLexer;
private:

    E_TOKEN_TYPES  _Type;//��� �������
    string         _Text;//����� �������

public:

    E_TOKEN_TYPES  Type(void)const {return _Type;}
    string         Text(void)const {return _Text;}

    CToken&        operator= (const CToken&);
    bool           operator==(const CToken&)const;

    CToken(void);
    CToken(const CToken&);
    CToken(E_TOKEN_TYPES, const string&);
};

typedef vector<CToken> TokensArray;

//---------------------------------------------------------------------------

typedef vector<string> StringsArray;

//������������ ��� �������������� ���������� ������ � ����� (������) �������
class CLexer
{
private:

    //�������������� ����������� ������������...
    StringsArray  _ResWordsArray,  //����������������� �����
                  _FunctionsArray; //�������
    string        _Operators,      //���������
                  _Deviders;       //�����������

    TokensArray   _TokensBuffer; //������, ���������� ��� ���������
                                 //"������������" ������
    unsigned      _OffSet; //������� � ������� ����������� ������

    //���������� ��� ������, ���� �� ��� �� ���������
    void    DefineTokenType(CToken&)const;
    //��������� ��������� ��������� (�������� ������, ����������� � "")
    CToken  SkanStringConstant(const string&);
    //���������� ��������� �����, ���������� _ErrorToken � ������ ������
    CToken  SkanToken(const string&);

    //���� ������ ���� ��������� ��������, �� ��� ������� ���������� ������
    //�� ��������� �� ���
    void    SkipSpacing(const string&);
    //���� ������� ������� ��������� �� ������� ������ ����������� %, �� ���
    //������� "��������" �����������, �.�. ���������� ������� �� ����
    void    SkipComment(const string&);   

public:

    const TokensArray& GetTokens(void)const {return _TokensBuffer;}
    //��������� ����� ������� � ����
    void  SaveTokens(ostream& os)const;

    //"���������" ������������ �����. True � ������ ������
    bool  Lex(const string&);

    //�������������� ����������� ����������. ��� ������� ������ ���� �������
    //�� ������� "������������"
    void  Init(const string*, unsigned, unsigned, unsigned, unsigned);
    //��������� �������������� ������� � _FunctionsArray
    void  AddFunctions(const string*, unsigned);
};

//---------------------------------------------------------------------------
}


#endif
#pragma warning(default:4786)