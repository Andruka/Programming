//---------------------------------------------------------------------------

#include <ctype.h>
#include <stdlib.h>
#include <algorithm>

#pragma hdrstop

#include "Lexer.h"
#include "Utils.h"
//---------------------------------------------------------------------------

namespace ESL
{
//---------------------------------------------------------------------------
CToken ErrorToken(ttUnknown, "ErrorToken");
//---------------------------------------------------------------------------

CToken& CToken::operator=(const CToken& other)
{
    _Type = other._Type;
    _Text = other._Text;

    return *this;
}

//---------------------------------------------------------------------------

bool CToken::operator==(const CToken& other)const
{
    return _Type==other._Type && _Text==other._Text;
}

//---------------------------------------------------------------------------

CToken::CToken(void): _Type(ttUnknown) {};

//---------------------------------------------------------------------------

CToken::CToken(const CToken& other): _Type(other._Type), _Text(other._Text) {};

//---------------------------------------------------------------------------

CToken::CToken(E_TOKEN_TYPES type, const string& text):
    _Type(type), _Text(text) {};

//---------------------------------------------------------------------------

void CLexer::DefineTokenType(CToken& token)const
{
    //����� � ������� ������������
    if(_Deviders.find(token.Text())!=_Deviders.npos)
        token._Type = ttDevider;
    //����� � ������� ����������
    else if(_Operators.find(token.Text())!=_Operators.npos)
        token._Type = ttOperator;
    //����� � ������� ����������������� ����
    else if(find(_ResWordsArray.begin(),_ResWordsArray.end(),token.Text())!=
                                                          _ResWordsArray.end())
        token._Type = ttResWord;
    //����� � ������� �������
    else if(find(_FunctionsArray.begin(),_FunctionsArray.end(),token.Text())!=
                                                          _FunctionsArray.end())
        token._Type = ttFunction;
    //��������: �������� �� ����� ����� ������
    else
    {
        unsigned i;
        for(i=0;i<token._Text.length();++i)
            if(!isdigit(token.Text()[i]))
                break;

        if(i==token.Text().length())//���� ��� ������� ������ ������ - �����
            token._Type = ttIntConstant;
        /*������� ������������ �� �������, ������ ����� - �������� �����-����
        ����������. ���� ��� �� ��� (�.�. ����� ������ ����������� - ��������
        �������� ��� �������� �������������� ����������), �� ������ �����
        �������� �� ������ ���������� ������ ����������� �������*/
        else
            token._Type = ttVariable;
    }
}

//---------------------------------------------------------------------------

CToken CLexer::SkanStringConstant(const string& text)
{
    //_OffSet ������ ��������� � ������� ������ �������
    //����� ������ ����������� �������
    const unsigned pos = text.find_first_of('"',++_OffSet);

    if(pos==text.npos)//���� ����������� ������� �� �������
        MACRO_ERROR_RET("CLexer::SkanStringConstant: '' expected",ErrorToken);

    //��� � �������, ������� �������
    const unsigned begin = _OffSet;

    _OffSet = pos+1; //����������� ������� �� �������

    return CToken(ttStrConstant, string(text, begin, pos-begin));
}

//---------------------------------------------------------------------------

CToken CLexer::SkanToken(const string& text)
{
    //���� � ������� ������� �������, ������ ����� ������� ��������� ���������
    if(text[_OffSet]=='"')
        return SkanStringConstant(text);
    //������� ����������� ���� ����
    if(text[_OffSet]=='%')
        SkipComment(text);

    SkipSpacing(text);//���������� ������������

    CToken ret;
    //������, ���������� ����������� �������
    static const string delim = _Deviders + _Operators;
    //������� ���������� ������������ �������
    const unsigned delimpos = text.find_first_of(delim, _OffSet);

    //���� ��� ��������� ������� � text
    //�� �������� �������� ������ ����� �������� � ������ �����
    if(delimpos==text.npos)
        ret._Text = text.substr(_OffSet);
    //������� �� ���������, �� ���� �������� �����, ����������� �����
    //������� �������� � ��������� ����������� ��������
    else
    {
        //����, ���� ��������� �� ����������� �������� ���
        ret._Text = text.substr(_OffSet,max(delimpos-_OffSet,unsigned(1)));

        //����������� �� ��������� �������
        _OffSet = max(delimpos,_OffSet+1);
    }

    DefineTokenType(ret);//����������� ���� ������, �� ������ � ���
    return ret;
}

//---------------------------------------------------------------------------

void CLexer::SkipSpacing(const string& text)
{
    //���� ������� ������� �� �����������
    if(!text[_OffSet] || !strchr(" \t\r\n", text[_OffSet]))
        return;//�� ���������� �� � ������

    //����������� ������� �� ��������� ����������� ������
    _OffSet = text.find_first_not_of(" \t\r\n", _OffSet) - 1;
}

//---------------------------------------------------------------------------

void CLexer::SkipComment(const string& text)
{
    while(text[_OffSet]=='%')
        _OffSet = text.find_first_of("\n", _OffSet) + 1;
}

//---------------------------------------------------------------------------

void CLexer::SaveTokens(ostream& os)const
{
    if(os.bad())
        MACRO_ERROR_RET_VOID("CLexer::SaveTokens error");

    string types[] = {"ttResWord",     "ttOperator", "ttStrConstant",
                      "ttIntConstant", "ttVariable", "ttFunction",
                      "ttDevider",     "ttUnknown"};

    for(unsigned i=0;i<_TokensBuffer.size();++i)
        os<<types[_TokensBuffer[i].Type()]<<
            ": '"<<_TokensBuffer[i].Text()<<"'"<<endl;
}

//---------------------------------------------------------------------------

bool CLexer::Lex(const string& text)
{
    _OffSet = 0;
   _TokensBuffer.clear();

    //���� ����� text ���� - �����
    if (text.empty())
        MACRO_ERROR_RET("CLexer::Lex tryng to lex empty text", false);

    unsigned prev_offset=-1;//����������� �������; ��� ������������ ��������
    CToken   token;//��������� �����
    //��������� ������, ���� �� ������ �� ����� �������
    do
    {
        //���� �������� �� ��������� ������� �� �����-���� ������� �� ���������
        if(prev_offset==_OffSet)
            MACRO_ERROR_RET("CLexer::Lex error. Possibly end missed",false);

        prev_offset = _OffSet;
        token       = SkanToken(text);

        if(token==ErrorToken)return false;//������ ��� ������������ ������

        _TokensBuffer.push_back(token);
    }
    while(token.Text()!="end");

    return true;
}

//---------------------------------------------------------------------------

void  CLexer::Init(//��������� �� ������ ������� ������� �����-������
                   const string* data,
                   unsigned rwc,  //����� ����������������� ����
                   unsigned opc,  //����� ����������
                   unsigned fnc,  //����� �������
                   unsigned dvc)  //����� ������������
{
    unsigned i;

    //����������� ����������������� ����
    _ResWordsArray.resize(rwc);
    const string* rwend = data + rwc;
    copy(data,rwend,_ResWordsArray.begin());

    //����������� ����������
    const string* opend = rwend + opc;
    for(i=0;i<opc;++i)
        _Operators += rwend[i];

    //����������� �������
    _FunctionsArray.resize(fnc);
    const string* fnend = opend + fnc;
    copy(opend,fnend,_FunctionsArray.begin());

    //����������� ������������
    for(i=0;i<dvc;++i)
        _Deviders += fnend[i];
}

//---------------------------------------------------------------------------

void CLexer::AddFunctions(const string* data, unsigned fnc)
{
    _FunctionsArray.insert(_FunctionsArray.end(),data,data+fnc);
}

//---------------------------------------------------------------------------
}
