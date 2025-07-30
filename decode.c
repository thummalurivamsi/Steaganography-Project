/*
Documentation
Name        : Vamsi T
Date        : 30/7/25
Description : Steaganography project
*/

#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Perform the decoding */
/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\n## DECODING OPERATION STARTED ##\n\n");
    //  Open stego image file
    if (open_image_file(decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to open stego image file\n");
        return e_failure;
    }
    printf("✅ Image file opened successfully\n");
    printf("✅ Done\n\n");

    // Decode magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Magic string decoding failed\n");
        return e_failure;
    }
    printf("✅ Magic string decoded successfully\n");
    printf("✅ Done\n\n");

    //  Decode secret file extension size
    if (decode_secret_file_extn_size(&(decInfo->size_secret_file_extn), decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to decode secret file extension size\n");
        return e_failure;
    }
    printf("✅ Secret file extension size decoded successfully\n");
    printf("✅ Done\n\n");

    // Decode secret file extension
    if (decode_secret_file_extn(decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to decode secret file extension\n");
        return e_failure;
    }
    printf("✅ Secret file extension decoded successfully\n");
    printf("✅ Done\n\n");

    // Decode secret file size
    if (decode_secret_file_size(decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to decode secret file size\n");
        return e_failure;
    }
    printf("✅ Secret file size decoded successfully\n");
    printf("✅ Done\n\n");

    // Decode secret file data
    if (decode_secret_file_data(decInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to decode secret file data\n");
        return e_failure;
    }
    printf("✅ Secret file data decoded successfully\n");
    printf("✅ Done\n\n");

    return e_success;
}
