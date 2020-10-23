#include "Helper.h"

bool Helper::is64(HANDLE pHandle){
    IMAGE_NT_HEADERS* headers = ImageNtHeader(pHandle);
    if (headers->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64){
        return true;
    }
    return false;
}
