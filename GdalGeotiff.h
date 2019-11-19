#ifndef GDALGEOTIFF_H
#define GDALGEOTIFF_H

#include "iostream"
#include "gdal_priv.h"

using namespace std;

class GdalGeotiff {
private:
  //Basic parameters of input Geotiff 
  int blockSizeX;
  int blockSizeY;
  double geoTransform[6];
  GDALDataset* ds;
  template<typename Type>
  Type getValueByCoordinate(int i, int j, int bandNo);

public:
  int height, width, bandCount;
  double extend[4];

  GdalGeotiff(GDALDataset* dataset);
  ~GdalGeotiff();
  double getValueByCoordinate(int i, int j, int bandNo); 
  int* projectionToImageCoordinates(double X, double Y);
};

#endif //GDALGEOTIFF_H
