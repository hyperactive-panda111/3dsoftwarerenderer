#include <stdio.h>
#include "texture.h"
 
tex2_t tex2_clone(tex2_t* coord) {
    tex2_t result = { coord->u, coord->v };
    return result;
}
