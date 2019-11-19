#include "GdalGeotiff.h"

GdalGeotiff::GdalGeotiff(GDALDataset* dataset) {
  ds = dataset;
  width = GDALGetRasterXSize(ds);
  height = GDALGetRasterYSize(ds);
  bandCount = GDALGetRasterCount(ds);
  ds->GetRasterBand(1)->GetBlockSize(&blockSizeX, &blockSizeY);
  ds->GetGeoTransform(geoTransform);

  extend[0] = geoTransform[0]; //minX
  extend[1] = geoTransform[3] + geoTransform[5] * height; //minY
  extend[2] = geoTransform[0] + geoTransform[1] * width;//maxX
  extend[3] = geoTransform[3]; //maxY
}

GdalGeotiff::~GdalGeotiff() {
  GDALDestroyDriverManager();
}

  template<typename Type>
  Type GdalGeotiff::getValueByCoordinate(int i, int j, int bandNo) {

        //Prepare the buffer
        int buffSize = blockSizeX * blockSizeY;
        Type *buffer = (Type*)CPLMalloc(sizeof(Type)*buffSize);

        //Position of block in image
        int bufCol = i / blockSizeX;
        int bufRow = j / blockSizeY;

        //Position of pixel in block
        int iB = i % blockSizeX;
        int jB = j % blockSizeY;

        //Index of the pixel in buffer
        int id = iB + jB * blockSizeX;

        //Fill the buffer
        ds->GetRasterBand(bandNo)->ReadBlock(bufCol, bufRow, buffer);

        //Get value by index
        Type retVal = buffer[id];

        //Parse to int return value
        return (double)retVal;

        //Clean memory
        CPLFree(buffer);   
  }
    
  double GdalGeotiff::getValueByCoordinate(int i, int j, int bandNo) {
    switch (GDALGetRasterDataType(ds->GetRasterBand(bandNo))) {
    case 1:
      return getValueByCoordinate<unsigned char>(i, j, bandNo);
      break;
    case 2:
      return getValueByCoordinate<unsigned short>(i, j, bandNo);
      break;
    case 3:
      return getValueByCoordinate<short>(i, j, bandNo);
      break;
    case 4:
      return  getValueByCoordinate<unsigned int>(i, j, bandNo);
      break;
    case 5:
      return getValueByCoordinate<int>(i, j, bandNo);
      break;
    case 6:
      return getValueByCoordinate<float>(i, j, bandNo);
      break;
    case 7:
     return getValueByCoordinate<double>(i, j, bandNo);
      break;
    default:
      break;
    }
  }

  int* GdalGeotiff::projectionToImageCoordinates(double X, double Y) {
     
    double invGeoTransform[6];
    GDALInvGeoTransform(geoTransform, invGeoTransform);

    int i = (int)floor(invGeoTransform[0] + invGeoTransform[1] * X + invGeoTransform[2] * Y);
    int j = (int)floor(invGeoTransform[3] + invGeoTransform[4] * X + invGeoTransform[5] * Y);

    int pixelCoordinates[2] = {i, j};
    return pixelCoordinates;
  }
