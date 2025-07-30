/*
Documentation
Name        : Vamsi T
Date        : 30/7/25
Description : Steaganography project
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status do_encoding(EncodeInfo *encInfo)
{
    printf("## ENCODING OPERATION STARTED ##\n\n");
    // Open source and destination files
    if (open_files(encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to open files\n");
        return e_failure;
    }
    printf("✅ All Files opened successfully\n");
    printf("✅ Done\n\n");

    //  Check image capacity
    if (check_capacity(encInfo) != e_success)
    {
        printf("❌ Error: %s does not have enough capacity to store %s\n", encInfo->src_image_fname, encInfo->secret_fname);
        return e_failure;
    }
    printf("✅ Image has sufficient capacity\n");
    printf("✅ Done\n\n");

    // Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to copy BMP header\n");
        return e_failure;
    }
    printf("✅ BMP header copied successfully\n");
    printf("✅ Done\n\n");

    //  Encode magic string
    if (encode_magic_string(MAGIC_STRING, encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to encode magic string\n");
        return e_failure;
    }
    printf("✅ Magic string encoded successfully\n");
    printf("✅ Done\n\n");

    //  Encode secret file extension size
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to encode secret file extension size\n");
        return e_failure;
    }
    printf("✅ Secret file extension size encoded successfully\n");
    printf("✅ Done\n\n");

    // Encode secret file extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to encode secret file extension\n");
        return e_failure;
    }
    printf("✅ Secret file extension encoded successfully\n");
    printf("✅ Done\n\n");

    // Encode secret file size
    if (encode_secret_file_size(get_file_size(encInfo->fptr_secret), encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to encode secret file size\n");
        return e_failure;
    }
    printf("✅ Secret file size encoded successfully\n");
    printf("✅ Done\n\n");

    // Encode secret file data
    if (encode_secret_file_data(encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to encode secret file data\n");
        return e_failure;
    }
    printf("✅ Secret file data encoded successfully\n");
    printf("✅ Done\n\n");

    //  Copy remaining image data
    if (copy_remaining_img_data(encInfo) != e_success)
    {
        fprintf(stderr, "❌ Error: Failed to copy remaining image data\n");
        return e_failure;
    }
    printf("✅ Remaining image data copied successfully\n");
    printf("✅ Done\n\n");

    return e_success;
}
