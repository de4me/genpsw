//
//  mask.hpp
//  genpsw
//
//  Created by DE4ME on 24.11.2025.
//

#ifndef mask_hpp
#define mask_hpp


#include <optional>
#include "generate.hpp"


typedef void(update_proc)(char*, size_t, int);


struct MaskInfo {
    const char* name;
    const char* mask;
    const char* desc;
    update_proc* update;
    int flags;
};


char get_mask_char();
int get_masks_count();

const MaskInfo& get_mask(int index);
const std::optional<MaskInfo> get_mask(const char* name);

size_t get_mask_char_count(const char* string, char mask_char);


#endif /* mask_hpp */
