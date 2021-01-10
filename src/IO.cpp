//
// Created by Yushi Yang on 10/01/2021.
//
#include "IO.h"
using namespace std;

int test_2d(){
    cout << ">>> 2D Testing <<<" << endl;
    uint32 width, length;
    TIFF *tif = TIFFOpen("../img-2d.tif", "r");
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length);
    uint32 n_pixels = width * length;
    // the follwoing two lines are the only way to get data out of libtiff
    uint32* raster;
    raster = (uint32 *) _TIFFmalloc(n_pixels * sizeof(uint32));

    bool is_successful = TIFFReadRGBAImage(tif, width, length, raster, 0);
    uint32 sum = 0;
    for (int i = 0; i < n_pixels; i++){
        uint32 red = TIFFGetR(raster[i]);
        uint32 green = TIFFGetG(raster[i]);
        uint32 blue = TIFFGetB(raster[i]);
        sum += red + green + blue;
    }
    cout << "loading 2D image, shape: (" << length << ", "  << width << ") " << endl;
    cout << "loading RGBA succeed? " << is_successful << endl;
    cout << "total brightness: " << sum << endl;
    _TIFFfree(raster);
    TIFFClose(tif);
    return is_successful;
}


int test_3d(){
    cout << ">>> 3D Testing <<<" << endl;
    uint32 width, length;
    TIFF *tif = TIFFOpen("../img-3d.tif", "r");
    vector<uint32*> rasters;

    bool all_succeed = true;
    int z = 0;
    uint32 sum = 0;

    do {
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &length);
        uint32 n_pixels = width * length;
        rasters.push_back((uint32 *) _TIFFmalloc(n_pixels * sizeof(uint32)));
        bool is_successful = TIFFReadRGBAImage(tif, width, length, rasters[z], 0);
        for (int i = 0; i < n_pixels; i++){
            sum += rasters[z][i];
            uint32 red = TIFFGetR(rasters[z][i]);
            uint32 green = TIFFGetG(rasters[z][i]);
            uint32 blue = TIFFGetB(rasters[z][i]);
            sum += red + green + blue;
        }
        all_succeed *= is_successful;
        z++;
    } while (TIFFReadDirectory(tif));

    for (auto raster : rasters){
        _TIFFfree(raster);
    }
    TIFFClose(tif);

    cout << "loading 3D image, shape: ("
         << length << ", "  << width << ", " << z << ") " << endl;
    cout << "loading all stacks succeed? " << all_succeed << endl;
    cout << "total brightness: " << sum << endl;

    return all_succeed;
}


int main(){
    test_2d();
    test_3d();
}