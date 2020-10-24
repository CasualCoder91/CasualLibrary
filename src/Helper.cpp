#include "Helper.h"

bool Helper::is64(const HANDLE pHandle){
    IMAGE_NT_HEADERS* headers = ImageNtHeader(pHandle);
    return headers->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;
}
