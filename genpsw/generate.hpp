//
//  generate.hpp
//  genpsw
//
//  Created by DE4ME on 01.07.2025.
//

#ifndef generate_hpp
#define generate_hpp


#include <stdlib.h>


enum {
    
    PasswordFlagNumbers =               1 << 0,
    PasswordFlagLetters =               1 << 1,
    PasswordFlagUpperLetters =          1 << 2,
    PasswordFlagSymbols =               1 << 3,
    PasswordFlagExtendedSymbols =       1 << 4,
    PasswordFlagHexNumbers =            1 << 5,
    
    PasswordFlagNoRepeat =              1 << 16,
    PasswordFlagNoSimilar =             1 << 17,
    
    PasswordFlagDefaultChars = PasswordFlagNumbers | PasswordFlagLetters | PasswordFlagUpperLetters | PasswordFlagSymbols,
    PasswordFlagAllChars = PasswordFlagNumbers | PasswordFlagLetters | PasswordFlagUpperLetters | PasswordFlagSymbols | PasswordFlagExtendedSymbols | PasswordFlagHexNumbers
};


void mixchars(char* buffer, size_t length, int count);
void mixcpy(char* destantion, const char* source, size_t length, int count = 12);
size_t remchar(char* buffer, size_t length, char ch);
size_t randcpy(char* dest, size_t destLength, const char* source, size_t sourceLength, bool noRepeat);
bool generate_password(char* password, size_t length, int flags);


#endif /* generate_hpp */
