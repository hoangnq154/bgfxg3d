#include "../core/Base.h"
#include "../core/FileSystem.h"
#include "../graphics/Image.h"

#include <bimg/decode.h>
#include <bx/allocator.h>

namespace gplay
{

Image* Image::create(const char* path)
{
    GP_ASSERT(path);

    // open the file
    std::unique_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open image file '%s'.", path);
        return NULL;
    }

    // read file data
    size_t dataSize = stream->length();
    char* data = new char[dataSize];
    if (stream->read(data, sizeof(char), dataSize) != dataSize)
    {
        GP_ERROR("Failed to load image file; file is missing data.");
        SAFE_DELETE_ARRAY(data);
        return nullptr;
    }

    // parse image
    bx::DefaultAllocator s_allocator;
    bimg::ImageContainer* imageContainer = bimg::imageParse(&s_allocator, data, dataSize);
    if(imageContainer == nullptr)
    {
        GP_ERROR("Failed to parse image data from file '%s'.", path);
        return nullptr;
    }

    // create image
    Image* image = new Image();
    image->_width = imageContainer->m_width;
    image->_height = imageContainer->m_height;
    image->_format = imageContainer->m_hasAlpha ? Format::RGBA : Format::RGB;

    image->_data = new unsigned char[imageContainer->m_size];
    memcpy(image->_data, imageContainer->m_data, imageContainer->m_size);

    // free image
    bimg::imageFree(imageContainer);

    return image;









/*

    //---


    GP_ASSERT(path);

    // Open the file.
    std::unique_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open image file '%s'.", path);
        return NULL;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        GP_ERROR("Failed to load file '%s'; not a valid PNG.", path);
        return NULL;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        GP_ERROR("Failed to create PNG structure for reading PNG file '%s'.", path);
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        GP_ERROR("Failed to create PNG info structure for PNG file '%s'.", path);
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        GP_ERROR("Failed to set up error handling for reading PNG file '%s'.", path);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    // Initialize file io.
    png_set_read_fn(png, stream.get(), readStream);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

    Image* image = new Image();
    image->_width = png_get_image_width(png, info);
    image->_height = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
    case PNG_COLOR_TYPE_RGBA:
        image->_format = Image::RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_format = Image::RGB;
        break;

    default:
        GP_ERROR("Unsupported PNG color type (%d) for image file '%s'.", (int)colorType, path);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    size_t stride = png_get_rowbytes(png, info);

    // Allocate image data.
    image->_data = new unsigned char[stride * image->_height];

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (unsigned int i = 0; i < image->_height; ++i)
    {
        memcpy(image->_data+(stride * (image->_height-1-i)), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);

    return image;*/
}

Image* Image::create(unsigned int width, unsigned int height, Image::Format format, unsigned char* data)
{
    /*GP_ASSERT(width > 0 && height > 0);
    GP_ASSERT(format >= RGB && format <= RGBA);

    unsigned int pixelSize = 0;
    switch(format)
    {
    case Image::RGB:
        pixelSize = 3;
        break;
    case Image::RGBA:
        pixelSize = 4;
        break;
    }

    Image* image = new Image();

    unsigned int dataSize = width * height * pixelSize;

    image->_width = width;
    image->_height = height;
    image->_format = format;
    image->_data = new unsigned char[dataSize];
    if (data)
        memcpy(image->_data, data, dataSize);

    return image;*/

    // todo: implement me with bimg
    GP_ASSERT(0);
    return nullptr;
}

Image::Image() : _data(NULL), _format(RGB), _width(0), _height(0)
{
}

Image::~Image()
{
    SAFE_DELETE_ARRAY(_data);
}

}