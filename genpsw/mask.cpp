//
//  mask.cpp
//  genpsw
//
//  Created by DE4ME on 24.11.2025.
//

#include "mask.hpp"


#include <string>


void uuid_update(char*, size_t, int);
void mac_update(char*, size_t, int);


const MaskInfo mask_array[] = {
    {"strong", "??????-??????-??????", "Strong password", nullptr, PF_NUMBERS | PF_LETTERS | PF_UPPER_LETTERS | PF_NO_SIMILAR},
    {"good", "????-????-????", "Good password", nullptr, PF_NUMBERS | PF_LETTERS | PF_UPPER_LETTERS | PF_NO_SIMILAR},
    {"weak", "????-????", "Weak password", nullptr, PF_NUMBERS | PF_LETTERS | PF_NO_SIMILAR},
    {"wifi", "????-???-????", "Wi-Fi password", nullptr, PF_NUMBERS | PF_LETTERS | PF_UPPER_LETTERS | PF_NO_SIMILAR},
    {"mac", "??:??:??:??:??:??", "A unique identifier assigned to a network interface controller", &mac_update, PF_HEX_NUMBERS},
    {"uuid", "????????-????-????-????-????????????", "128-bit number designed to be a unique identifier for objects in computer systems", &uuid_update, PF_HEX_NUMBERS},
    {"clsid", "{????????-????-????-????-????????????}", "Globally Unique Identifier that identifies a COM class object", &uuid_update, PF_HEX_NUMBERS}
};


char get_mask_char() {
    
    return '?';
}


int get_masks_count() {
    
    return sizeof(mask_array)/sizeof(mask_array[0]);
}


const MaskInfo& get_mask(int index) {
    
    return mask_array[index];
}


const std::optional<MaskInfo> get_mask(const char* name) {
    
    for (const auto& mask_info : mask_array) {
        
        if (strcmp(name, mask_info.name) == 0 ) {
            return  mask_info;
        }
    }

    return std::nullopt;
}


size_t get_mask_char_count(const char* string, char mask_char) {
    
    size_t result = 0;
    
    while(auto ch = *string++) {
        
        if (ch == mask_char) {
            result++;
        }
    }
    
    return result;
}


char hexchar_to_number(char ch) {
    
    return ch < 'A' ? ch - '0' : ch - ('A' - 0xA);
}


char number_to_hexchar(char ch) {
    
    return ch < 0xA ? '0' + ch : 'A' + (ch - 0xA);
}


//xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
//M - version (4: random)
//N - variant (1: 10xx, range: 8..B)

void uuid_update(char* uuid, size_t length, int flags) {
    
    if ((flags & PF_ALL_CHARS) != PF_HEX_NUMBERS) {
        return;
    }
    
    int offset;
    
    switch (length) {
        case 36: //uuid
            offset = 14;
            break;
        case 38: //clsid
            offset = 15;
            break;
        default:
            return;
    }
    
    uuid[offset] = '4';
    offset += 5;
    auto result = hexchar_to_number(uuid[offset]);
    result = 0x8 | (result & 0x3);
    uuid[offset] = number_to_hexchar(result);
}


// xN:xx:xx:xx:xx:xx
// N: xxBA
// A: 0 - unicast, 1 - multicast
// B: 0 - globally unique, 1 - locally administered
// N: 2: AAI: Can be randomly or arbitrarily assigned to devices
//    6: Reserved for future use, but may be used similarly to AAI

void mac_update(char* mac, size_t length, int flags) {
    
    if ((flags & PF_ALL_CHARS) != PF_HEX_NUMBERS) {
        return;
    }
    
    if (length != 17) {
        return;
    }
    
    auto result = hexchar_to_number(mac[1]);
    result = (result & 0x4) | 0x2;
    mac[1] = number_to_hexchar(result);
}
