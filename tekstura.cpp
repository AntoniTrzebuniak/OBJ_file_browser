#include "tekstura.h"
#include <algorithm>

GLuint  num_texture=-1;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint   WczytajTeksture (const char *filename)
{
    // Pomocnicze
	int i, j=0;
    FILE *l_file;
    unsigned char *l_texture; 
    BITMAPFILEHEADER fileheader; 
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;
    glGenTextures(1, &num_texture);

	fopen_s(&l_file, filename, "rb");
    if( l_file==NULL) return (-1);
    // Nag³ówek:
	fread(&fileheader, sizeof(fileheader), 1, l_file); 
    fseek(l_file, sizeof(fileheader), SEEK_SET); 
    fread(&infoheader, sizeof(infoheader), 1, l_file); 
	// Alokacja pamiêci
    l_texture = (unsigned char *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
 
    // Wczytywanie pikseli
	for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {            
            fread(&rgb, sizeof(rgb), 1, l_file); 
            l_texture[j+0] = rgb.rgbtRed; 
            l_texture[j+1] = rgb.rgbtGreen; 
            l_texture[j+2] = rgb.rgbtBlue; 
            l_texture[j+3] = 255; // Alpha
            j += 4; 
    }
	fclose(l_file); 
     
    glBindTexture(GL_TEXTURE_2D, num_texture); // Zg³oszenie tekstury (poprzez jej unikatowe ID)
    // Ustawienie parametrów rekstury
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Tiling tekstury, gdy za ma³a
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 


    // Stwórz taka teksture w OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture); 

    return (num_texture); // Zwróæ identyfikator OpenGL ID tekstury 
}

GLuint WczytajTekstureJPG(const char *filename)
{
    int width = 0, height = 0, channels = 0;
    // Force 4 components (RGBA)
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        // failed to load
        return (GLuint)-1;
    }

    // Generate and bind texture ID
    glGenTextures(1, &num_texture);
    glBindTexture(GL_TEXTURE_2D, num_texture);

    // Texture parameters (same as BMP loader)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    // Upload to GPU as RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free CPU memory
    stbi_image_free(data);

    return num_texture;
}

GLuint WczytajTeksturePNG(const char *filename)
{
    int width = 0, height = 0, channels = 0;
    // force 4 components (RGBA)
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        // failed to load
        return (GLuint)-1;
    }

    // Generate and bind texture ID
    glGenTextures(1, &num_texture);
    glBindTexture(GL_TEXTURE_2D, num_texture);

    // Texture parameters (match BMP/JPG loaders)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    // Upload to GPU as RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free CPU memory
    stbi_image_free(data);

    return num_texture;
}



unordered_map<string, GLuint> textureCache;

void addText2Mesh(GPUmesh& m){
    const std::string& texPath = m.material.texturePath;

    if (!texPath.empty())
    {
        if (textureCache.count(texPath))
        {
            m.textureID = textureCache[texPath];
        }
        else
        {
            GLuint tex = LoadTextureAuto(texPath);
            //GLuint tex = LOAD_JPG(texPath);
            
            textureCache[texPath] = tex;
            m.textureID = tex;
        }
    }

}

ImageExt getExtension( const std::string& filename )
{
    auto pos = filename.find_last_of( '.' );
    if (pos == std::string::npos)
        return ImageExt::UNKNOWN;

    std::string ext = filename.substr( pos + 1 );
    std::transform( ext.begin(), ext.end(), ext.begin(), ::tolower );

    if (ext == "png")  return ImageExt::PNG;
    if (ext == "bmp")  return ImageExt::BMP;
    if (ext == "jpg")  return ImageExt::JPG;
    if (ext == "jpeg") return ImageExt::JPEG;

    return ImageExt::UNKNOWN;
}

GLuint LoadTextureAuto( const std::string& path )
{
    switch (getExtension( path )) {
    case ImageExt::PNG:
        return LOAD_PNG(path);
    case ImageExt::BMP:
        return LOAD_BMP( path );
    case ImageExt::JPG:
    case ImageExt::JPEG:
        return LOAD_JPG( path );
    default:
        printf( "Nieznane rozszerzenie: %s\n", path.c_str() );
        return 0;
    }
}