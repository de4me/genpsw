//
//  Options.cpp
//  genpsw
//
//  Created by DE4ME on 28.11.2025.
//

#include "Options.hpp"


#include <string>
#include <stdexcept>


#include "config.h"
#include "mask.hpp"


#ifdef WIN32
#define DIRECTORY_SLASH '\\'
#else
#define DIRECTORY_SLASH '/'
#endif


Options::Options() :
_appName(nullptr),
_passwordFlags(nullptr),
_passwordLength(nullptr),
_passwordCount(nullptr),
_passwordMask(nullptr),
_showHelp(false)
{
    
}


bool Options::isNumber(const char* string) const {
    
    while (*string != 0) {
        if (isdigit(*string++)) {
            continue;
        }
        return false;
    }
    return true;
}


bool Options::isFlags(const char* string) const {
    
    if (!isNumber(string)) {
        return true;
    }
    switch (atoi(string)) {
        case 1:
            return true;

        default:
            return false;
    }
}


bool Options::isMask(const char* string) const {
    
    return strchr(string, CONF_MASK_CHAR) != nullptr || get_mask(string);
}


const char* Options::getAppName(const char* path) const {
    
    auto appname = strrchr(path, DIRECTORY_SLASH);
    return appname != nullptr ? appname + 1 : path;
}


char Options::getMaskChar() const {
    
    return _passwordMask == nullptr || get_mask_char_count(_passwordMask, CONF_MASK_CHAR) != 0 ? CONF_MASK_CHAR : get_mask_char();
}


std::optional<MaskInfo> Options::getPasswordMask() const {
    
    if (_passwordMask == nullptr) {
        return std::nullopt;
    }
    
    if (get_mask_char_count(_passwordMask, CONF_MASK_CHAR) != 0) {
        
        MaskInfo result = {};
        result.mask = _passwordMask;
        return result;
    } else {
        
        return get_mask(_passwordMask);
    }
}


size_t Options::getPasswordMaskLength() const {

    if (_passwordMask == nullptr) {
        return 0;
    }
    
    if (auto count = get_mask_char_count(_passwordMask, CONF_MASK_CHAR)) {
        return count;
    }
    
    if (auto mask_info = get_mask(_passwordMask)) {
        return get_mask_char_count((*mask_info).mask, get_mask_char());
    }
    
    return 0;
}


size_t Options::getPasswordLength() const {
    
    if (hasPasswordMask()) {
        return getPasswordMaskLength();
    } else {
        return _passwordLength != nullptr ? atoi(_passwordLength) : CONF_PASSWORD_LENGTH;
    }
}


int Options::getPasswordCount() const {
    
    return _passwordCount != nullptr ? atoi(_passwordCount) : CONF_PASSWORD_COUNT;
}


int Options::getPasswordFlags() const {
    
    if (_passwordFlags == nullptr) {
        return 0;
    }
    
    int result = 0;
    auto current = _passwordFlags;
    
    while (char ch = *current++) {
        
        switch (ch) {
            case 'n':
                result |= PF_NUMBERS;
                break;
            case 'h':
                result |= PF_HEX_NUMBERS;
                break;
            case 'l':
                result |= PF_LETTERS;
                break;
            case 'L':
                result |= PF_UPPER_LETTERS;
                break;
            case 's':
                result |= PF_SYMBOLS;
                break;
            case 'S':
                result |= PF_EXTENDED_SYMBOLS;
                break;
            case '1':
                result |= PF_NO_REPEAT;
                break;
            case 'r':
                result |= PF_NO_SIMILAR;
                break;
            default:
                throw ch;
        }
    }
    return result;
}


void Options::parse(int argc, const char* args[]) {
    
    _appName = getAppName(args[0]);
    for (int i = 1; i < argc;) {
        
        const char* current = args[i++];
        if (current[0] == '-') {
            switch (current[1]) {
                case 'h':
                case '?':
                    _showHelp = true;
                    break;
                    
                default:
                    throw std::invalid_argument(current);
            }
        } else if (_passwordMask == nullptr && isMask(current)) {
            _passwordMask = current;
        } else if (_passwordFlags == nullptr && isFlags(current) ) {
            _passwordFlags = current;
        } else if (_passwordLength == nullptr && _passwordMask == nullptr && isNumber(current)) {
            _passwordLength = current;
        } else if (_passwordCount == nullptr && isNumber(current)) {
            _passwordCount = current;
        } else {
            throw std::invalid_argument(current);
        }
    }
}
