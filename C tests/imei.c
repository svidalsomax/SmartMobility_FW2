#include "imei.h"
#include <stdlib.h>

#include "Binary.h"

void imei_init(Imei * imei, char* imei_str){ 
    imei_loadText(imei, imei_str); 
}

void imei_loadText(Imei * imei, char * imei_str){

    imei->imei_ = strtoll(imei_str, NULL, 10);
}

void imei_binary(Imei * imei, char * encoded){
    encode(imei->imei_, 7, encoded); 
}

unsigned long long imei_value(Imei * imei){
    return imei->imei_;
}

//faltan los operators. No se usará el operator= que es básicamente hacer una asignación. 

bool imei_is_equal(Imei * imei1, Imei * imei2) {
    if (imei1 && imei2)
    {
       return (imei1->imei_ == imei2->imei_);
    }
    else {
        return false; 
    }
}

bool imei_is_not_equal(Imei * imei1, Imei * imei2){ 
    if (imei1 && imei2)
    {
       return !(imei1->imei_ == imei2->imei_);
    }
    else {
        return false; 
    }
}

