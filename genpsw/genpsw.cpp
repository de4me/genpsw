// genpsw.cpp : Defines the entry point for the console application.
//


#include <string>
#include <stdexcept>
#include <vector>


#include "config.h"
#include "generate.hpp"
#include "mask.hpp"
#include "Options.hpp"


using namespace std;


void print_version(const char* name) {
    
    printf("%s version %i.%i\n", name, GENPSW_VERSION_HI, GENPSW_VERSION_LO);
}


void print_help(const char* name) {
    
    print_version(name);
    printf("SYNOPSIS:\n");
    printf("\t%s [-h] [-v] [flags] [[length] | [mask]] [quantity]\n", name);
    printf("WHERE flags:\n");
    printf("\tn\tUse decimal numbers\n");
    printf("\tl\tUse lowercase letters\n");
    printf("\tL\tUse uppercase letters\n");
    printf("\ts\tUse symbols\n");
    printf("\tS\tUse extended symbols\n");
    printf("\th\tUse hex numbers\n");
    printf("\tr\tRemove similar characters\n");
    printf("\t1\tNo duplicate characters\n");
    printf("PREDEFINED masks:\n");
    for (int i = 0; i < get_masks_count(); i++) {
        const auto& mask_info = get_mask(i);
        printf("\t%s \t%s\n", mask_info.name, mask_info.desc);
    }
    printf("EXAMPLES:\n");
    printf("\t%s nlL 8 2\n", name);
    printf("\t%s h \"??-??-??-??\" 2\n", name);
    printf("\t%s strong 2\n", name);
}


void print_use_for_help() {
    
    printf("%s", "Use -h or -? for help\n");
}


void print_password(int password_flags, size_t password_length, int password_count) {
    
    vector<char> password(password_length + 1);
    
    while (password_count-- > 0) {
        
        if (!generate_password(password.data(), password_length, password_flags)) {
            return;
        }
        printf("%s\n", password.data());
    }
}


void print_password_masked(int password_flags, int password_count, char mask_char, const MaskInfo& mask_info) {
    
    auto mask_string = mask_info.mask;
    if (mask_string == nullptr) {
        return;
    }
    
    auto mask_length = strlen(mask_string);
    if (mask_length == 0) {
        return;
    }
    
    auto password_length = get_mask_char_count(mask_string, mask_char);
    if (password_length ==0) {
        return;
    }
    
    if (password_flags == 0) {
        password_flags = mask_info.flags;
    }
    
    vector<char> password(password_length + 1);
    vector<char> password_masked(mask_length + 1);
    
    while (password_count-- > 0) {
        
        if (!generate_password(password.data(), password_length, password_flags)) {
            return;
        }
        
        auto password_ptr = password.data();
        auto current_mask_length = mask_length;
        
        for (size_t i = 0; i < current_mask_length; i++) {
            
            char ch = mask_string[i];
            if (ch == mask_char) {
                ch = *password_ptr++;
                if (ch == 0) {
                    current_mask_length = i;
                    break;
                }
            }
            
            password_masked[i] = ch;
        }
        
        password_masked[current_mask_length] = 0;
        if (mask_info.update != nullptr) {
            mask_info.update(password_masked.data(), current_mask_length, password_flags);
        }
        printf("%s\n", password_masked.data());
    }
}


void perform(const Options& options) {
    
    auto password_count = options.getPasswordCount();
    auto password_length = options.getPasswordLength();
    
    if (password_count == 0 || password_length == 0) {
        return;
    }
    
    auto password_flags = options.getPasswordFlags();
    auto mask_info = options.getPasswordMask();

    if (mask_info) {
        
        if ((password_flags != 0 || (*mask_info).flags == 0) && (password_flags & PF_ALL_CHARS) == 0) {
            password_flags |= PF_DEFAULT_CHARS;
        }
        
        print_password_masked(password_flags, password_count, options.getMaskChar(), *mask_info);
        
    } else {
        
        if ((password_flags & PF_ALL_CHARS) == 0) {
            password_flags |= PF_DEFAULT_CHARS;
        }
        
        print_password(password_flags, password_length, password_count);
    }
}


int main(int argc, const char* args[]) {
    
    Options options;
    
    try {
        options.parse(argc, args);
        
        if (options.needShowHelp()) {
            print_help(options.getAppName());
        } else if (options.needShowVersion())
            print_version(options.getAppName());
        else {
            perform(options);
        }
        
        return EXIT_SUCCESS;
    }
    
    catch(char ch) {
        printf("invalid flag: %c\n", ch);
        print_use_for_help();
        return EXIT_FAILURE;
    }
    
    catch(const invalid_argument& e) {
        printf("invalid argument: %s\n", e.what());
        print_use_for_help();
        return EXIT_FAILURE;
    }
    
    catch(const exception& e) {
        printf("error: %s\n", e.what());
        return EXIT_FAILURE;
    }
}
