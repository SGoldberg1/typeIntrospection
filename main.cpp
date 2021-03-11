
#include "stdio.h"

#include "main.h"
#include "Meta_Generated.h"

#define EmptyDefaultCase default: {} break
#define ArrayLength(array) (sizeof(array) / sizeof(array[0]))

#define META_STRUCT_TYPE(type) MetaMembersOf_##type
#define DumpStruct(structPtr, structType) dump_struct_(structPtr, \
META_STRUCT_TYPE(structType), \
ArrayLength(META_STRUCT_TYPE(structType)))

void
dump_struct_(void* structPtr,
             meta_struct* metaData,
             int memberCount)
{
    for(int i = 0; i < memberCount; ++i)
    {
        meta_struct member = metaData[i];
        void* data = ((char*)structPtr + member.Offset);
        
        printf("\n%s: ", member.MemberName);
        
        switch(member.Type)
        {
            case MetaType_v2:
            {
                DumpStruct(data, v2);
            }break;
            case MetaType_char:
            {
                char* value = (char*)data;
                printf("%c\n", *value);
            }break;
            case MetaType_type_flag:
            {
                type_flag* value = (type_flag*)data;
                printf("%d\n", *value);
            }break;
            case MetaType_int:
            {
                int* value = (int*)data;
                printf("%d\n", *value);
            }break;
            case MetaType_float:
            {
                float* value = (float*)data;
                printf("%f\n", *value);
            }break;
            EmptyDefaultCase;
        }
    }//for(int i = 0; i < memberCount; ++i)
}

int
main()
{
    
    return(0);
}