// genpsw.cpp : Defines the entry point for the console application.
//


#include <string>


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


void print_help(const char* name) {
    
    printf("USE: %s [flags] [password length] [count]\n", name);
    printf("Example: %s nlL 8 2\n", name);
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
                return current;
            default:
                break;
        }
    }
    return path;
}


int main(int argc, const char* args[]) {
    
    const char* flags_string = nullptr;
    size_t password_length = 32;
    int password_count = 1;
    int password_flags = 0;
    const char* name = appname(args[0]);
    
    switch (argc) {
        case 1:
            break;
            
        case 2:
        {
            bool arg1_flags = is_flags(args[1]);
            
            if (arg1_flags) {
                flags_string = args[1];
            }
            else {
                password_length = atoi(args[1]);
            }
            break;
        }
            
        case 3:
        {
            bool arg1_flags = is_flags(args[1]);
            bool arg2_number = is_number(args[2]);
            
            if (arg1_flags && arg2_number) {
                flags_string = args[1];
                password_length = atoi(args[2]);
                break;
            }
            if (!arg1_flags && arg2_number) {
                password_length = atoi(args[1]);
                password_count = atoi(args[2]);
                break;
            }
            print_help(name);
            return -1;
        }
            
        case 4:
        {
            bool arg1_flags = is_flags(args[1]);
            bool arg2_number = is_number(args[2]);
            bool arg3_number = is_number(args[3]);
            
            if (arg1_flags && arg2_number && arg3_number) {
                flags_string = args[1];
                password_length = atoi(args[2]);
                password_count = atoi(args[3]);
                break;
            }
            print_help(name);
            return -1;
        }
            
        default:
            print_help(name);
            return -1;
    }
    
    if (password_length == 0 || password_count == 0) {
        return 0;
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
    
    char* password = reinterpret_cast<char*>(malloc(password_length + 1));
    
    while (password_count-- > 0) {
        if (generate_password(password, password_length, password_flags)) {
            printf("%s\n", password);
            continue;
        }
        break;
    }
    
    free(password);
    return 0;
}

