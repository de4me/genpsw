//
//  generate.cpp
//  genpsw
//
//  Created by DE4ME on 01.07.2025.
//

#include "generate.hpp"


#include <random>
#include <string>


using namespace std;


const char numbersChars[] = {
    
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};


const char lettersChars[] = {
    
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};


const char upperLettersChar[] = {
    
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};


const char symbolsChars[] = {
    
    '~', '!', '@', '#', '$', '%', '^', '&', '(', ')', '-', '=',
    '_', '+', '[', ']', ';', '{', '}'
};


const char extendedSymbolsChars[] = {
    
    '\\', '|', '/', '?', ',', '.', '<', '>', ':', '*'
};


const char hexChars[] = {
    
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
};


const char similarChars[] = {
    
    'O', '0', 'o',
    '1', 'I', 'l', 'i', '!', '|',
    '.', ',',
    ':', ';'
};


#define ALLCHAR_LENGTH \
    sizeof(numbersChars) + \
    sizeof(lettersChars) + sizeof(upperLettersChar) + \
    sizeof(symbolsChars) + sizeof(extendedSymbolsChars) + \
    sizeof(hexChars)


size_t remchar(char* buffer, size_t length, char ch) {
    
    char* current = buffer;
    char* end = buffer + length;
    size_t count = 0;
    do {
        current = reinterpret_cast<char*>(memchr(current, ch, end - current));
        if (current == nullptr) {
            break;
        }
        count++;
        char* next = current + 1;
        if (next == end) {
            break;
        }
        memcpy(current, next, end - current - count);
    } while (true);
    return length - count;
}


void mixchars(char* buffer, size_t length, int count) {
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, static_cast<int>(length - 1));
    
    while (count-- > 0) {
        
        int loff = dist(gen);
        int roff = dist(gen);
        
        char tmp = buffer[loff];
        buffer[loff] = buffer[roff];
        buffer[roff] = tmp;
    }
}


void mixcpy(char* destantion, const char* source, size_t length, int count) {
    
    memcpy(destantion, source, length);
    
    while (count-- > 0) {
        mixchars(destantion, length, count);
    }
}


size_t randcpy(char* dest, size_t destLength, const char* source, size_t sourceLength, int flags) {
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, static_cast<int>(sourceLength - 1));
    
    bool can_repeat;
    if (flags & PasswordFlagNoRepeat) {
        can_repeat = false;
        if (destLength > sourceLength) {
            destLength = sourceLength;
        }
    } else {
        can_repeat = true;
    }
    
    size_t buffer_length = sourceLength;
    char* buffer = reinterpret_cast<char*>(calloc(1, sourceLength));
    memcpy(buffer, source, sourceLength);
    
    for (size_t index = 0; index < destLength; index++) {
        size_t off = dist(gen);
        if (off >= buffer_length) {
            off %= buffer_length;
        }
        auto ch = buffer[off];
        dest[index] = ch;
        if (can_repeat) {
            continue;
        }
        buffer_length = remchar(buffer, buffer_length, ch);
    }
    
    free(buffer);
    return destLength;
}


bool generate_password(char* password, size_t length, int flags) {
    
    size_t buffer_length = 0;
    char buffer[ALLCHAR_LENGTH];
    
    if (flags & PasswordFlagNumbers) {
        mixcpy(&buffer[buffer_length], numbersChars, sizeof(numbersChars));
        buffer_length += sizeof(numbersChars);
    }
    
    if (flags & PasswordFlagLetters) {
        mixcpy(&buffer[buffer_length], lettersChars, sizeof(lettersChars));
        buffer_length += sizeof(lettersChars);
    }
    
    if (flags & PasswordFlagUpperLetters) {
        mixcpy(&buffer[buffer_length], upperLettersChar, sizeof(upperLettersChar));
        buffer_length += sizeof(upperLettersChar);
    }
    
    if (flags & PasswordFlagSymbols) {
        mixcpy(&buffer[buffer_length], symbolsChars, sizeof(symbolsChars));
        buffer_length += sizeof(symbolsChars);
    }
    
    if (flags & PasswordFlagExtendedSymbols) {
        mixcpy(&buffer[buffer_length], extendedSymbolsChars, sizeof(extendedSymbolsChars));
        buffer_length += sizeof(extendedSymbolsChars);
    }
    
    if (flags & PasswordFlagHexNumbers) {
        mixcpy(&buffer[buffer_length], hexChars, sizeof(hexChars));
        buffer_length += sizeof(hexChars);
    }
    
    if (buffer_length == 0) {
        return false;
    }
    
    if (flags & PasswordFlagNoSimilar) {
        for (auto ch : similarChars) {
            buffer_length = remchar(buffer, buffer_length, ch);
        }
    }
    
    mixchars(buffer, buffer_length, 100);
    length = randcpy(password, length, buffer, buffer_length, flags);
    password[length] = 0;
    return true;
}
