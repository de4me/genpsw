//
//  Options.hpp
//  genpsw
//
//  Created by DE4ME on 28.11.2025.
//

#ifndef Options_hpp
#define Options_hpp


#include <stdlib.h>
#include <optional>


#include "mask.hpp"


class Options {
    
private:
    
    const char* _appName;
    const char* _passwordFlags;
    const char* _passwordLength;
    const char* _passwordCount;
    const char* _passwordMask;
    
    bool _showHelp;
    
    const char* getAppName(const char* path) const;
    bool isFlags(const char* string) const;
    bool isMask(const char* string) const;
    bool isNumber(const char* string) const;
    
public:
    
    Options();
    
    const char* getAppName() const { return _appName; }
    
    bool hasPasswordMask() const { return _passwordMask != nullptr; }
    bool needShowHelp() const { return _showHelp; }
    
    char getMaskChar() const;
    size_t getPasswordMaskLength() const;
    std::optional<MaskInfo> getPasswordMask() const;

    size_t getPasswordLength() const;
    int getPasswordCount() const;
    int getPasswordFlags() const;
    
    void parse (int argc, const char* args[]);
    
};


#endif /* Options_hpp */
