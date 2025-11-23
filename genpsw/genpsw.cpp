// genpsw.cpp : Defines the entry point for the console application.
//


#include <string>


#include "config.h"
#include "generate.hpp"


using namespace std;


bool is_number(const char* string) {
    
    if (string == nullptr || *string == 0) {
        return false;
    }
    
    while (*string != 0) {
        if (isdigit(*string++)) {
            continue;
        }
        return false;
    }
    return true;
}


bool is_flags(const char* string) {
    
    if (string == nullptr || *string == 0) {
        return false;
    }
    
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


int mask_char_count(const char* string) {
    
    int result = 0;
    
    while(auto ch = *string++) {
        
        if (ch == CONF_MASK_CHAR) {
            result++;
        }
    }
    
    return result;
}


bool is_mask(const char* string) {
    
    return strchr(string, CONF_MASK_CHAR) != nullptr;
}


void print_help(const char* name) {
    
    printf("USE: %s [flags] [[password length] | [password mask]] [count]\n", name);
    printf("Example:\n");
    printf("\t %s nlL 8 2\n", name);
    printf("\t %s h \"??-??-??-??\" 2\n", name);
    printf("WHERE flags:\n");
    printf(" n \t- Use decimal numbers\n");
    printf(" l \t- Use lowercase letters\n");
    printf(" L \t- Use uppercase letters\n");
    printf(" s \t- Use symbols\n");
    printf(" S \t- Use extended symbols\n");
    printf(" h \t- Use hex numbers\n");
    printf(" r \t- Remove similar letters\n");
    printf(" 1 \t- Do not repeat letters\n");
}


const char* appname(const char* path) {
    
    if (path == nullptr || *path == 0) {
        return nullptr;
    }
    int length = static_cast<int>(strlen(path));
    const char* current = &path[length];
    while (--current > path) {
        switch (*current) {
            case '\\':
            case '/':
                return current + 1;
            default:
                break;
        }
    }
    return path;
}


void print_password(int password_flags, size_t password_length, int password_count) {
    
    char* password = reinterpret_cast<char*>(malloc(password_length + 1));

    while (password_count-- > 0) {
        
        if (generate_password(password, password_length, password_flags)) {
            printf("%s\n", password);
            continue;
        }
        break;
    }
    
    free(password);
}


void print_password_masked(int password_flags, size_t password_length, int password_count, const char* mask_string, size_t mask_length) {
    
    char* password = reinterpret_cast<char*>(malloc(password_length + 1));
    
    if (password == nullptr) {
        return;
    }
        
    char* password_masked = reinterpret_cast<char*>(malloc(mask_length + 1));
    if (password_masked != nullptr) {
        
        while (password_count-- > 0) {
            
            if (generate_password(password, password_length, password_flags)) {
                auto password_ptr = password;
                for (int i = 0; i < mask_length; i++) {
                    
                    auto ch = mask_string[i];
                    password_masked[i] = ch == CONF_MASK_CHAR ? *password_ptr++ : ch;
                }
                password_masked[mask_length] = 0;
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
    const char* mask_string = nullptr;
    size_t password_length = CONF_PASSWORD_LENGTH;
    size_t mask_length = 0;
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
                length_index = 2;
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
    }
    
    if (count_index != 0) {
        password_count = atoi(args[count_index]);
    }
    
    if (mask_index != 0) {
        mask_string = args[mask_index];
        mask_length = strlen(mask_string);
        password_length = mask_char_count(mask_string);
        if (password_length == mask_length) {
            mask_string = nullptr;
            mask_length = 0;
        }
    }
    
    if (password_length == 0 || password_count == 0) {
        return EXIT_SUCCESS;
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
    
    if ((password_flags & PasswordFlagAllChars) == 0) {
        password_flags |= PasswordFlagDefaultChars;
    }
    
    if (mask_string == nullptr) {
        print_password(password_flags, password_length, password_count);
    } else {
        print_password_masked(password_flags, password_length, password_count, mask_string, mask_length);
    }
    
    return EXIT_SUCCESS;
}

