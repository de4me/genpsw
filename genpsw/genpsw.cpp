// genpsw.cpp : Defines the entry point for the console application.
//


#include <string>


#include "config.h"
#include "generate.hpp"
#include "mask.hpp"


using namespace std;


#ifdef WIN32
#define DIRECTORY_SLASH '\\'
#else
#define DIRECTORY_SLASH '/'
#endif


bool is_number(const char* string) {
    
    while (*string != 0) {
        if (isdigit(*string++)) {
            continue;
        }
        return false;
    }
    return true;
}


bool is_flags(const char* string) {
    
    if (!is_number(string)) {
        return true;
    }
    switch (atoi(string)) {
        case 1:
            return true;

        default:
            return false;
    }
}


bool is_mask(const char* string) {
    
    return strchr(string, CONF_MASK_CHAR) != nullptr || get_mask(string);
}


void print_help(const char* name) {
    
    printf("USE: %s [flags] [[password length] | [mask]] [count]\n", name);
    printf("Example:\n");
    printf("\t%s nlL 8 2\n", name);
    printf("\t%s h \"??-??-??-??\" 2\n", name);
    printf("\t%s strong 2\n", name);
    printf("WHERE flags:\n");
    printf("\tn\t\tUse decimal numbers\n");
    printf("\tl\t\tUse lowercase letters\n");
    printf("\tL\t\tUse uppercase letters\n");
    printf("\ts\t\tUse symbols\n");
    printf("\tS\t\tUse extended symbols\n");
    printf("\th\t\tUse hex numbers\n");
    printf("\tr\t\tRemove similar letters\n");
    printf("\t1\t\tDo not repeat letters\n");
    printf("PREDEFINED masks:\n");
    for (int i = 0; i < get_masks_count(); i++) {
        const auto& mask_info = get_mask(i);
        printf("\t%s \t%s\n", mask_info.name, mask_info.desc);
    }
}


const char* appname(const char* path) {
    
    auto appname = strrchr(path, DIRECTORY_SLASH);
    return appname != nullptr ? appname + 1 : path;
}


void print_password(int password_flags, size_t password_length, int password_count) {
    
    char* password = reinterpret_cast<char*>(malloc(password_length + 1));
    
    if (password == nullptr) {
        return;
    }

    while (password_count-- > 0) {
        
        if (generate_password(password, password_length, password_flags)) {
            printf("%s\n", password);
            continue;
        }
        break;
    }
    
    free(password);
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
    
    char* password = reinterpret_cast<char*>(malloc(password_length + 1));
    if (password == nullptr) {
        return;
    }

    char* password_masked = reinterpret_cast<char*>(malloc(mask_length + 1));
    if (password_masked != nullptr) {
        
        if (password_flags == 0) {
            password_flags = mask_info.flags;
        }
        
        while (password_count-- > 0) {
            
            if (generate_password(password, password_length, password_flags)) {
                auto password_ptr = password;
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
                    mask_info.update(password_masked, current_mask_length, password_flags);
                }
                printf("%s\n", password_masked);
                continue;
            }
            break;
        }
        
        free(password_masked);
    }
    
    free(password);
}


int main(int argc, const char* args[]) {
    
    const char* flags_string = nullptr;
    size_t password_length = CONF_PASSWORD_LENGTH;
    int password_count = CONF_PASSWORD_COUNT;
    int password_flags = 0;
    const char* name = appname(args[0]);
    
    int flags_index = 0;
    int length_index = 0;
    int count_index = 0;
    int mask_index = 0;
    
    switch (argc) {
        case 1:
            break;
            
        case 2:
        {
            auto arg1 = args[1];
            
            if (is_mask(arg1)) {
                mask_index = 1;
                break;
            }
            
            if (is_flags(arg1)) {
                flags_index = 1;
                break;
            }
            
            if (is_number(arg1)) {
                length_index = 1;
                break;
            }
            
            print_help(name);
            return -1;
        }
            
        case 3:
        {
            auto arg1 = args[1];
            
            if (is_mask(arg1)) {
                mask_index = 1;
            } else if (is_flags(arg1)) {
                flags_index = 1;
            } else if (is_number(arg1)) {
                length_index = 1;
            } else {
                print_help(name);
                return -1;
            }
            
            auto arg2 = args[2];
            
            if (mask_index == 0 && is_mask(arg2)) {
                mask_index = 2;
            } else if (is_number(arg2)) {
                if (mask_index == 0 && length_index == 0) {
                    length_index = 2;
                } else {
                    count_index = 2;
                }
            } else {
                print_help(name);
                return -1;
            }
            break;
        }
            
        case 4:
        {
            if (is_flags(args[1])) {
                flags_index = 1;
            } else {
                print_help(name);
                return -1;
            }
            
            auto arg2 = args[2];
            
            if (is_mask(arg2)) {
                mask_index = 2;
            } else if (is_number(arg2)) {
                length_index = 2;
            } else {
                print_help(name);
                return -1;
            }
            
            if (is_number(args[3])) {
                count_index = 3;
            } else {
                print_help(name);
                return -1;
            }

            break;
        }
            
        default:
            print_help(name);
            return -1;
    }
    
    if (flags_index != 0) {
        flags_string = args[flags_index];
    }
    
    if (length_index != 0) {
        password_length = atoi(args[length_index]);
        if (password_length == 0) {
            return EXIT_SUCCESS;
        }
    }
    
    if (count_index != 0) {
        password_count = atoi(args[count_index]);
        if (password_count == 0) {
            return EXIT_SUCCESS;
        }
    }
    
    if (flags_string != nullptr) {
        const char* flag_curent = flags_string;
        int flags_count = static_cast<int>(strlen(flags_string));
        while (flags_count-- > 0) {
            char ch = *flag_curent;
            bool first_flag = strchr(flags_string, ch) == flag_curent;
            flag_curent++;
            if (!first_flag) {
                continue;
            }
            switch (ch) {
                case 'n':
                    password_flags |= PasswordFlagNumbers;
                    break;
                case 'h':
                    password_flags |= PasswordFlagHexNumbers;
                    break;
                case 'l':
                    password_flags |= PasswordFlagLetters;
                    break;
                case 'L':
                    password_flags |= PasswordFlagUpperLetters;
                    break;
                case 's':
                    password_flags |= PasswordFlagSymbols;
                    break;
                case 'S':
                    password_flags |= PasswordFlagExtendedSymbols;
                    break;
                case '1':
                    password_flags |= PasswordFlagNoRepeat;
                    break;
                case 'r':
                    password_flags |= PasswordFlagNoSimilar;
                    break;
                default:
                    print_help(name);
                    return -1;
            }
        }
    }
    
    if (mask_index == 0) {
        
        if ((password_flags & PasswordFlagAllChars) == 0) {
            password_flags |= PasswordFlagDefaultChars;
        }
        
        print_password(password_flags, password_length, password_count);
    } else {
        
        auto mask_string = args[mask_index];
        password_length = get_mask_char_count(mask_string, CONF_MASK_CHAR);
        if (password_length == 0) {
            
            const auto& mask_info = get_mask(mask_string);
            if (!mask_info) {
                
                print_help(name);
                return -1;
            }
            
            if (password_flags != 0 && (password_flags & PasswordFlagAllChars) == 0) {
                password_flags |= PasswordFlagDefaultChars;
            }
            
            print_password_masked(password_flags, password_count, get_mask_char(), *mask_info);
        } else {
            
            if ((password_flags & PasswordFlagAllChars) == 0) {
                password_flags |= PasswordFlagDefaultChars;
            }
            
            if (strlen(mask_string) != password_length) {
                
                MaskInfo custom_mask_info;
                
                memset(&custom_mask_info, 0, sizeof(custom_mask_info));
                custom_mask_info.mask = mask_string;
                
                print_password_masked(password_flags, password_count, CONF_MASK_CHAR, custom_mask_info);
            } else {
                
                print_password(password_flags, password_length, password_count);
            }
        }
    }

    return EXIT_SUCCESS;
}

