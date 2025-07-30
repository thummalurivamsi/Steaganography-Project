/*
Documentation
Name        : Vamsi T
Date        : 30/7/25
Description : Steaganography project
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

OperationType check_operation_type(char *argv[])
{
    if (argv[1] == NULL)
    {
        return e_unsupported;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Initialize all file pointers to NULL to prevent undefined behavior
    encInfo->fptr_src_image = NULL;
    encInfo->fptr_secret = NULL;
    encInfo->fptr_stego_image = NULL;

    printf("🔍 Validating encoding arguments...\n");

    // Validate source image file (.bmp)
    char *bmp = strstr(argv[2], ".bmp");
    if (bmp != NULL && strcmp(bmp, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
        printf("✅ Source image validated: %s\n", argv[2]);
    }
    else
    {
        printf("❌ Invalid source image file. File must be <.bmp image>\n");
        return e_failure;
    }

    // Extract file extension
    char *ext = strrchr(argv[3], '.');

    if (ext != NULL)
    {
        if (strcmp(ext, ".c") == 0 || strcmp(ext, ".sh") == 0 || strcmp(ext, ".txt") == 0)
        {
            encInfo->secret_fname = argv[3];
            strcpy(encInfo->extn_secret_file, ext);
            printf("✅ Secret file validated: %s\n", argv[3]);
        }
        else
        {
            printf("❌ Invalid secret file extension\n");
            printf("📌 Allowed: .txt, .c, .sh\n");
            return e_failure;
        }
    }
    else
    {
        printf("❌ Secret file has no extension\n");
        printf("📌 Allowed: .txt, .c, .sh\n");
        return e_failure;
    }

    // Validate or set default output stego image file
    if (argv[4] == NULL)
    {
        printf("⚠️ Output file not mentioned. Creating stego.bmp as default...\n");
        encInfo->stego_image_fname = "stego.bmp";
    }
    else
    {
        char *bmp_ext = strstr(argv[4], ".bmp");
        if (bmp_ext != NULL && strcmp(bmp_ext, ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];
            printf("✅ Output image validated: %s\n", argv[4]);
        }
        else
        {
            printf("❌ Invalid output image file. File must be <output_file_name.bmp>\n");
            return e_failure;
        }
    }

    printf("✅ All encode arguments validated successfully!\n");
    printf("✅ Done\n\n");
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    printf("🔍 Checking for %s capacity to handle %s...\n", encInfo->src_image_fname, encInfo->secret_fname);

    // Get total capacity of the image in bytes
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    // Calculate required size for encoding: magic string + extension + file size + actual data
    int encoding_size = 54 + (((strlen(MAGIC_STRING)) + 4 + (strlen(encInfo->extn_secret_file)) + 4 + get_file_size(encInfo->fptr_secret)) * 8);

    if (encInfo->image_capacity >= encoding_size)
    {
        return e_success;
    }
    return e_failure;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // Move the file pointer to the end of the file
    return ftell(fptr);       // Return the size of the file
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    printf("🗂️  Copying BMP Header from source to destination...\n");

    // Ensure reading from start of the source image
    rewind(fptr_src_image);

    char str[54]; // BMP header is 54 bytes

    // Read BMP header from source image
    if (fread(str, 54, 1, fptr_src_image) != 1)
    {
        perror("❌ Error reading BMP header");
        return e_failure;
    }

    // Write BMP header to destination (stego) image
    if (fwrite(str, 54, 1, fptr_dest_image) != 1)
    {
        perror("❌ Error writing BMP header");
        return e_failure;
    }

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("🔐 Encoding Magic String into image...\n");

    if (encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        printf("❌ Failed to encode magic string into the image...\n");
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    printf("🔐 Encoding secret file extension size into image...\n");

    // Read 32 bytes from the source image
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        perror("❌ Error reading from source image during file size encoding");
        // perror explain why it failed —  was the file missing ? no permissions ? corrupted ? return e_failure;
    }

    // Encode the 32-bit file size into LSBs
    if (encode_int_to_lsb(file_size, buffer) != e_success)
    {
        fprintf(stderr, "❌ Failed to encode file size into image buffer\n");
        return e_failure;
    }

    // Write 32 bytes to the stego image
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        perror("❌ Error writing encoded file size to stego image");
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    printf("🔐 Encoding secret file extension into image...\n");

    // Encode the file extension into the image
    if (encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        return e_failure;

    return e_success;
}
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    printf("🔐 Encoding secret file size into image...\n");
    encInfo->size_secret_file = file_size;
    char buffer[32];

    // Read 32 bytes from the source image
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        perror("❌ Error reading from source image while encoding secret file size");
        return e_failure;
    }

    // Encode the 32-bit file size into LSBs
    if (encode_int_to_lsb(file_size, buffer) != e_success)
    {
        fprintf(stderr, "❌ Failed to encode secret file size into image buffer\n");
        return e_failure;
    }
    // printf("%ld\n", encInfo->size_secret_file);

    // Write 32 bytes to the stego image
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        perror("❌ Error writing encoded secret file size to stego image");
        return e_failure;
    }

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("🔐 Encoding secret file data into image...\n");

    if (fseek(encInfo->fptr_secret, 0, SEEK_SET) != 0)
    {
        perror("Error: fseek failed on secret file");
        return e_failure;
    }

    char *buffer = (char *)malloc(encInfo->size_secret_file);
    if (!buffer)
    {
        printf("Error: Memory allocation failed for secret file buffer\n");
        return e_failure;
    }

    if (fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret) != 1)
    {
        perror("Error reading from secret file");
        free(buffer);
        return e_failure;
    }

    buffer[encInfo->size_secret_file] = '\0';
    if (encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        printf("Error: Failed to encode secret file data into image\n");
        free(buffer);
        return e_failure;
    }

    free(buffer);
    return e_success;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];

    for (int i = 0; i < size; i++)
    {
        // Read 8 bytes from the source image
        if (fread(buffer, 8, 1, fptr_src_image) != 1)
        {
            perror("Error: Failed to read 8 bytes from source image");
            return e_failure;
        }

        // Encode 1 byte of data into those 8 bytes
        if (encode_byte_to_lsb(data[i], buffer) != e_success)
        {
            fprintf(stderr, "Error: Failed to encode byte %d (char: '%c') to LSB\n", i, data[i]);
            return e_failure;
        }

        // Write modified 8 bytes to stego image
        if (fwrite(buffer, 8, 1, fptr_stego_image) != 1)
        {
            perror("Error: Failed to write encoded bytes to stego image");
            return e_failure;
        }
    }

    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++) // Loop over 8 bits
    {
        // Extract bit from MSB to LSB and set it in the LSB of image_buffer[i]
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}
Status encode_int_to_lsb(int data, char *image_buffer)
{
    for (int i = 0; i < 32; i++) // Loop over 32 bits from MSB to LSB
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (31 - i)) & 1);
    }
    return e_success;
}

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    printf("📦 Copying remaining image data to output image...\n");

    char ch;
    // EOF is -1, but fread() returns an unsigned type (size_t), so:
    // Comparing to EOF(-1) will always be false or invalid

    while (fread(&ch, 1, 1, encInfo->fptr_src_image) == 1) // fread returns number of items read
    {
        if (fwrite(&ch, 1, 1, encInfo->fptr_stego_image) != 1)
        {
            perror("❌ Error: Failed to write remaining byte to stego image");
            return e_failure;
        }
    }

    if (ferror(encInfo->fptr_src_image))
    {
        perror("❌ Error: Failed to read remaining data from source image");
        return e_failure;
    }

    return e_success;
}
