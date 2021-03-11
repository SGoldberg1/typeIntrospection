clear

clang++ -g -o metaGen MetaGen.cpp -Wno-writable-strings
./metaGen > Meta_Generated.h
rm metaGen

# NOTE(Stephen): -fms-extensions is for meta processing
# type cast error from pointer to u32
# 64 -> 32 is truncated, however we will never have a struct with a member offset > 2^32!!!!!!

clang++ -O3 -fms-extensions -S -masm=intel -o main.txt main.cpp 
clang++ -g -fms-extensions -o main main.cpp 