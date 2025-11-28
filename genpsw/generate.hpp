//
//  generate.hpp
//  genpsw
//
//  Created by DE4ME on 01.07.2025.
//

#ifndef generate_hpp
#define generate_hpp


#include <stdlib.h>


enum PasswordFlags {
    
    PF_NUMBERS =            1 << 0,
    PF_LETTERS =            1 << 1,
    PF_UPPER_LETTERS =      1 << 2,
    PF_SYMBOLS =            1 << 3,
    PF_EXTENDED_SYMBOLS =   1 << 4,
    PF_HEX_NUMBERS =        1 << 5,
    
    PF_NO_REPEAT =          1 << 16,
    PF_NO_SIMILAR =         1 << 17,
    
    PF_DEFAULT_CHARS = PF_NUMBERS | PF_LETTERS | PF_UPPER_LETTERS | PF_SYMBOLS,
    PF_ALL_CHARS = PF_NUMBERS | PF_LETTERS | PF_UPPER_LETTERS | PF_SYMBOLS | PF_EXTENDED_SYMBOLS | PF_HEX_NUMBERS
};


void mixchars(char* buffer, size_t length, int count);
void mixcpy(char* destantion, const char* source, size_t length, int count = 12);
size_t remchar(char* buffer, size_t length, char ch);
size_t randcpy(char* dest, size_t destLength, const char* source, size_t sourceLength, bool noRepeat);
bool generate_password(char* password, size_t length, int flags);


#endif /* generate_hpp */
