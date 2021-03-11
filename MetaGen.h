#ifndef _META_GEN_H
#define _META_GEN_H

#define TRUE (1)
#define FALSE (0)
#define EmptyDefaultCase default:{}break

enum token_type
{
    Token_Unknown,
    Token_Identifier,
    Token_OpenBracket,
    Token_CloseBracket,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_OpenParenthesis,
    Token_CloseParenthesis,
    Token_Comma,
    Token_Colon,
    Token_SemiColon,
    Token_Period,
    Token_Space,
    Token_Tab,
    Token_LineFeed,
    Token_CarriageReturn,
    Token_Pound,
    Token_BackSlash,
    Token_ForwardSlash,
    Token_Asterisk,
    Token_Plus,
    Token_Equal,
    Token_Dash,
    Token_Quote,
    Token_UnderScore,
    Token_StreamEnd
};

struct token
{
    token_type Type;
    int TextLength;
    char* Text;
};

struct tokenizer
{
    char* At;
};

struct linked_node
{
    void* Data;
    linked_node* Next;
};

struct struct_member
{
    int IsConst;
    int IsPointer;
    token Type;
    token Name;
};

struct struct_meta
{
    token Name;
    linked_node* Members;
};

static const char* META_TYPE_PREFIX = "MetaType_";

#endif //_META_GEN_H
