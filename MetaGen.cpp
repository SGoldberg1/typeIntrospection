
#include "stdio.h"
#include "stdlib.h"
#include <time.h>

#include "MetaGen.h"

char*
ReadEntireFile(char* fileName)
{
    char* result = 0;
    int fileSize = 0;
    FILE* file = fopen(fileName, "r");
    
    if(file)
    {
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result = (char*)calloc(fileSize + 1, 1);
        
        if(result)
        {
            if(fread(result, fileSize, 1, file) != 1)
            {
                free(result);
                result = 0;
                fprintf(stderr, "ERROR: MetaGen.cpp->main(int, char**) fread failure\n");
            }
        }
        
        fclose(file);
    }
    
    return(result);
}

inline int
IsAlpha(char c)
{
    int result = ((('a' <= c) && (c <= 'z')) ||
                  (('A' <= c) && (c <= 'Z')));
    return(result);
}

inline int
IsNumber(char c)
{
    int result = (('0' <= c) && (c <= '9'));
    return(result);
}

inline int
IsLineEnd(char c)
{
    int result = ((c == '\r') ||
                  (c == '\n'));
    return(result);
}

inline int
IsStreamEnd(char c)
{
    int result = (c == 0);
    return(result);
}

inline int
IsWhiteSpace(char c)
{
    int result = ((c == ' ') ||
                  (c == '\t') ||
                  IsLineEnd(c));
    return(result);
}

inline void
IgnoreWhiteSpace(tokenizer* tokenizer)
{
    while(IsWhiteSpace(tokenizer->At[0]))
    {
        ++tokenizer->At;
    }
}

inline void
IngoreLine(tokenizer* tokenizer)
{
    while((IsStreamEnd(tokenizer->At[0]) == FALSE) &&
          (IsLineEnd(tokenizer->At[0]) == FALSE))
    {
        ++tokenizer->At;
    }
}

inline void
GetIdentifier(tokenizer* tokenizer,
              token* result)
{
    while(IsAlpha(tokenizer->At[0]) ||
          IsNumber(tokenizer->At[0]) ||
          (tokenizer->At[0] == '_'))
    {
        ++tokenizer->At;
    }
    
    result->Type = Token_Identifier;
    result->TextLength = (tokenizer->At - result->Text);
}

token
GetNextToken(tokenizer* tokenizer)
{
    IgnoreWhiteSpace(tokenizer);
    
    token result;
    result.Type = Token_Unknown;
    result.TextLength = 1;
    result.Text = tokenizer->At;
    
    switch(tokenizer->At[0])
    {
        case '\0': { result.Type = Token_StreamEnd; }break;
        case '{': { result.Type = Token_OpenBrace; ++tokenizer->At; }break;
        case '}': { result.Type = Token_CloseBrace; ++tokenizer->At; }break;
        case '[': { result.Type = Token_OpenBracket; ++tokenizer->At; }break;
        case ']': { result.Type = Token_CloseBracket; ++tokenizer->At; }break;
        case '(': { result.Type = Token_OpenParenthesis; ++tokenizer->At; }break;
        case ')': { result.Type = Token_CloseParenthesis; ++tokenizer->At; }break;
        case ',': { result.Type = Token_Comma; ++tokenizer->At; }break;
        case '.': { result.Type = Token_Period; ++tokenizer->At; }break;
        case ' ': { result.Type = Token_Space; ++tokenizer->At; }break;
        case '#': { result.Type = Token_Pound; ++tokenizer->At; }break;
        case '\\': { result.Type = Token_BackSlash; ++tokenizer->At; }break;
        case '/': { result.Type = Token_ForwardSlash; ++tokenizer->At; }break;
        case '*': { result.Type = Token_Asterisk; ++tokenizer->At; }break;
        case '=': { result.Type = Token_Equal; ++tokenizer->At; }break;
        case '+': { result.Type = Token_Plus; ++tokenizer->At; }break;
        case '-': { result.Type = Token_Dash; ++tokenizer->At; }break;
        case '"': { result.Type = Token_Quote; ++tokenizer->At; }break;
        case ';': { result.Type = Token_SemiColon; ++tokenizer->At; }break;
        case '_': { result.Type = Token_UnderScore; ++tokenizer->At; }break;
        default:
        {
            if(IsAlpha(tokenizer->At[0]))
            {
                GetIdentifier(tokenizer, &result);
            }
            else
            {
                ++tokenizer->At;
            }
        }break;
    }
    
    return(result);
}

inline int
RequisiteToken(tokenizer* tokenizer, token_type type)
{
    int result = (GetNextToken(tokenizer).Type == type);
    return(result);
}

inline void
IgnoreCodeComments(tokenizer* tokenizer)
{
    token nextToken = GetNextToken(tokenizer);
    
    if(nextToken.Type == Token_ForwardSlash)
    {
        IngoreLine(tokenizer);
    }
    else if(nextToken.Type == Token_Asterisk)
    {
        while((IsStreamEnd(tokenizer->At[0]) == FALSE) &&
              (tokenizer->At[0] != '*') &&
              (tokenizer->At[1] != '/'))
        {
            ++tokenizer->At;
        }
        
        tokenizer->At += 2;
    }
}

inline int
TokenEquals(token token, const char* match)
{
    int result = FALSE;
    
    for(int i = 0; i < token.TextLength; ++i)
    {
        if(*match != token.Text[i]) { break; }
        ++match;
    }
    
    if(*match == 0)
        result = TRUE;
    
    return(result);
}

int
GetIntrospecitonParamValue(tokenizer* tokenizer,
                           token* token)
{
    int result = FALSE;
    
    if(RequisiteToken(tokenizer, Token_Equal) &&
       RequisiteToken(tokenizer, Token_Quote))
    {
        *token = GetNextToken(tokenizer);
        
        if(token->Type == Token_Identifier)
        {
            result = TRUE;
        }
        else
        {
            fprintf(stderr, "MetaGen.cpp->ParseIntrospectionParamValue(tokenizer*, token*): "
                    "Invalid Syntax for Introspection, expected identier\n");
        }
    }
    
    return(result);
}

int
ParseIntrospectionParams(tokenizer* tokenizer)
{
    int parsing = TRUE;
    int result = TRUE;
    
    if(RequisiteToken(tokenizer, Token_OpenParenthesis))
    {
        while(parsing && result)
        {
            token token = GetNextToken(tokenizer);
            
            switch(token.Type)
            {
                case Token_StreamEnd:
                {
                    result = FALSE;
                    fprintf(stderr, "MetaGen.cpp->ParseIntrospectionParams(tokenizer*): "
                            "Invalid Syntax for Introspection, expected symbol ')'\n");
                }break;
                case Token_CloseParenthesis:{ parsing = FALSE; }break;
                case Token_Identifier:
                {
                    if(TokenEquals(token, "category"))
                    {
                        if(GetIntrospecitonParamValue(tokenizer, &token))
                        {
                            // TODO(Stephen): PROCESS INTROSPECTION PARAMETERS
                            //printf("Category: %.*s\n", token.TextLength, token.Text);
                        }
                    }
                    
                }break;
                EmptyDefaultCase;
                
            }//switch(token.Type)
        }//while(parsing && result)
    }//if(RequisiteToken(tokenizer, Token_OpenParenthesis))
    else
    {
        fprintf(stderr, "MetaGen.cpp->ParseIntrospectionParams(tokenizer*): "
                "Invalid Syntax for Introspection, expected symbol '('\n");
    }
    
    return(result);
}

struct_member
ParseStructMember(tokenizer* tokenizer, token memberType)
{
    int parsing = TRUE;
    struct_member result = {};
    
    if(TokenEquals(memberType, "const"))
    {
        result.IsConst = TRUE;
        result.Type = GetNextToken(tokenizer);
    }
    else
        result.Type = memberType;
    
    while(parsing)
    {
        token token = GetNextToken(tokenizer);
        
        switch(token.Type)
        {
            case Token_SemiColon:
            case Token_StreamEnd: { parsing = FALSE; }break;
            case Token_Asterisk: { result.IsPointer = TRUE; }break;
            case Token_Identifier: { result.Name = token; }break;
            EmptyDefaultCase;
        }
    }
    
    return(result);
}


#define Linked_PushStruct(list, type) (type*)_push_node_(list, sizeof(type))
#define Linked_PushSize(list, type, count) (type*)_push_node_(list, (sizeof(type) * count))
void*
_push_node_(linked_node** list,
            int dataSize)
{
    linked_node* next = (linked_node*)calloc(sizeof(linked_node), 1);
    next->Data = calloc(dataSize, 1);
    next->Next = (*list);
    (*list) = next;
    return(next->Data);
}

int
TokenEquals(token a, token b)
{
    int result = TRUE;
    
    if(a.TextLength == b.TextLength)
    {
        for(int i = 0; i < a.TextLength; ++i)
        {
            if(a.Text[i] != b.Text[i])
            {
                result = FALSE;
                break;
            }
        }
    }
    else
    {
        result = FALSE;
    }
    
    return(result);
}

void
ParseStruct(tokenizer* tokenizer,
            linked_node** structMetaList,
            linked_node** metaTypesList)
{
    token structName = GetNextToken(tokenizer);
    
    if(RequisiteToken(tokenizer, Token_OpenBrace))
    {
        struct_meta* structMeta = Linked_PushStruct(structMetaList, struct_meta);
        structMeta->Name = structName;
        
        int parsing = TRUE;
        
        while(parsing)
        {
            token memberType = GetNextToken(tokenizer);
            
            if((memberType.Type == Token_CloseBrace) ||
               (memberType.Type == Token_StreamEnd))
            {
                parsing = FALSE;
            }
            else
            {
                struct_member* member = Linked_PushStruct(&(structMeta->Members), struct_member);
                *member = ParseStructMember(tokenizer, memberType);
                
                int hasMetaType = FALSE;
                linked_node* nextMetaType = *metaTypesList;
                
                while(nextMetaType)
                {
                    token* check = (token*)nextMetaType->Data;
                    
                    if(TokenEquals(*check, member->Type))
                    {
                        hasMetaType = TRUE;
                        break;
                    }
                    
                    nextMetaType = nextMetaType->Next;
                }
                
                if(hasMetaType == FALSE)
                {
                    token* metaType = Linked_PushStruct(metaTypesList, token);
                    *metaType = member->Type;
                }
            }
        }//while(parsing)
    }
    else
    {
        fprintf(stderr, "MetaGen.cpp->ParseStruct(tokenizer*): "
                "Invalid Syntax for Introspection, expected symbol '{'\n");
    }
}

void
ParseIntrospection(tokenizer* tokenizer,
                   linked_node** structMetaList,
                   linked_node** metaTypes)
{
    if(ParseIntrospectionParams(tokenizer))
    {
        token token = GetNextToken(tokenizer);
        
        if(token.Type == Token_Identifier)
        {
            if(TokenEquals(token, "struct"))
            {
                ParseStruct(tokenizer,
                            structMetaList,
                            metaTypes);
            }
        }
        else
        {
            fprintf(stderr, "MetaGen.cpp->ParseIntrospection(tokenizer*): "
                    "Invalid Syntax for Introspection, expected identier\n");
        }
    }
}

void
Ouput_StructMeta(linked_node* structMetaList)
{
    linked_node* nextStruct = structMetaList;
    
    printf("\nstruct meta_struct\n"
           "{\n"
           "\tint IsConstant;\n"
           "\tint IsPointer;\n"
           "\tint Offset;\n"
           "\tmeta_type Type;\n"
           "\tconst char* MemberName;\n"
           "};\n");
    
    while(nextStruct)
    {
        struct_meta* structMeta = ((struct_meta*)nextStruct->Data);
        printf("\nmeta_struct MetaMembersOf_%.*s[] = \n"
               "{\n",
               structMeta->Name.TextLength, structMeta->Name.Text);
        
        linked_node* nextMember = structMeta->Members;
        
        while(nextMember)
        {
            struct_member* member = (struct_member*)nextMember->Data;
            nextMember = nextMember->Next;
            
            printf("\t{ %d, %d, (unsigned int)&((%.*s*)0)->%.*s, %s%.*s, \"%.*s\" },\n",
                   member->IsConst,
                   member->IsPointer,
                   structMeta->Name.TextLength, structMeta->Name.Text,
                   member->Name.TextLength, member->Name.Text,
                   META_TYPE_PREFIX,
                   member->Type.TextLength, member->Type.Text,
                   member->Name.TextLength, member->Name.Text);
        }
        
        printf("};\n");
        
        nextStruct = nextStruct->Next;
    }
}

void
Ouput_TypeMeta(linked_node* metaTypeList)
{
    linked_node* nextMetaType = metaTypeList;
    char metaTypeBuffer[256] = {};
    
    
    printf("\nenum meta_type\n"
           "{\n");
    while(nextMetaType)
    {
        token* metaType = ((token*)nextMetaType->Data);
        
        int length = sprintf(metaTypeBuffer,
                             "%s%.*s",
                             META_TYPE_PREFIX,
                             metaType->TextLength, metaType->Text);
        printf("\t%s,\n", metaTypeBuffer);
        
        metaTypeBuffer[length] = 0;
        nextMetaType = nextMetaType->Next;
    }
    printf("};\n");
    
}

void
Ouput_Comments(void)
{
    time_t currentTime;
    time(&currentTime);
    printf("\n//This program was computer generated\n"
           "//See MetaGen.cpp for details\n"
           "//Generated: %s\n",
           ctime(&currentTime));
}

int
main(int argc, char** args)
{
    int parsing = TRUE;
    tokenizer tokenizer = { };
    tokenizer.At = ReadEntireFile("./main.h");
    
    linked_node* structMetaList = 0;
    linked_node* typeMetaList = 0;
    
    while(parsing)
    {
        token token = GetNextToken(&tokenizer);
        
        switch(token.Type)
        {
            case Token_Identifier:
            {
                if(TokenEquals(token, "Introspection"))
                {
                    ParseIntrospection(&tokenizer,
                                       &structMetaList,
                                       &typeMetaList);
                }
            }break;
            case Token_Pound:
            {
                IngoreLine(&tokenizer);
            }break;
            case Token_ForwardSlash:
            {
                IgnoreCodeComments(&tokenizer);
            }break;
            
            case Token_StreamEnd:
            {
                parsing = FALSE;
            }break;
            EmptyDefaultCase;
        }
    }
    
    printf("#ifndef _ENGINE_META_GENERATED_H\n"
           "#define _ENGINE_META_GENERATED_H\n");
    
    Ouput_Comments();
    Ouput_TypeMeta(typeMetaList);
    Ouput_StructMeta(structMetaList);
    
    printf("\n#endif //_ENGINE_META_GENERATED_H\n");
    
    return(0);
}
