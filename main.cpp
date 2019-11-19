main.cpp
Dün
Sal 19:49
t
Bir öğe yüklediniz
Metin
main.cpp
#include "GdalGeotiff.h"
#include <cstring>
#include <limits>
#include <iomanip>

#define CLOCKS_PER_SEC 1000

int main(int argc, char **argv) {
  
  clock_t t_Start = clock();
  string fileName;
  int i, j, bandNo;
  double X, Y;
  bool is_fileName = false;
  bool is_i = false; 
  bool is_j = false;
  bool is_X = false;
  bool is_Y = false;
  bool is_bandNo = false;

  //Parsing console parameters : fileName, bandNo, i & j or X & Y
  #pragma region ParseConsole
  for (int k = 1; k < argc; ++k) {
    
    string argStr = argv[k];
    size_t sepIdx = argStr.find_first_of(":");
    string key = argStr.substr(0, sepIdx);
    string val = argStr.substr(sepIdx + 1, argStr.size() - sepIdx);
    
    if (!is_fileName)
    {
      if (key.compare("input") == 0)
      {
        fileName = val;
        is_fileName = true;
      }
    }
    else if (!is_i && !is_X)
    {
      if (key.compare("i") == 0)
      {
        i = atoi(val.c_str());
        is_i = true;
      }
      if (key.compare("X") == 0)
      {
        X = atof(val.c_str());
        is_X = true;
      }
    }
    else if (!is_j && !is_Y)
    {
      if (key.compare("j") == 0)
      {
        j = atoi(val.c_str());
        is_j = true;
      }
      if (key.compare("Y") == 0)
      {
        Y = atof(val.c_str());
        is_Y = true;
      }
    }
    else if (!is_bandNo)
    {
      if (key.compare("bandNo") == 0)
      {
        bandNo = atoi(val.c_str());
        is_bandNo = true;
      }
    }
  }
  clock_t t_ConsoleParse = clock();
  #pragma endregion

  if (is_fileName && is_bandNo && ((is_i && is_j) || (is_X && is_Y)))
  {
    //Reading Geotiff dataset
    GDALAllRegister();
    GDALDataset *dataset;
    dataset = (GDALDataset*)GDALOpen(fileName.c_str(), GA_ReadOnly);
    clock_t t_ReadDataset = clock();

    //Get value with image coordinate and band no
    if (dataset != NULL)
    {
      GdalGeotiff gdalGeotiff(dataset);

      if ((is_X && is_Y) && !(is_i && is_j))
      {
        int* pixelCoordinates = gdalGeotiff.projectionToImageCoordinates(X, Y);
        i = pixelCoordinates[0];
        j = pixelCoordinates[1];
      }

      if (bandNo > 0 && bandNo <= gdalGeotiff.bandCount)
      {
        if (i >= 0 && i < gdalGeotiff.width && j >= 0 && j < gdalGeotiff.height)
        {
          double retVal = gdalGeotiff.getValueByCoordinate(i, j, bandNo);
          clock_t t_getValue = clock();

          //Write to console
          if (is_i && is_j)
          {
            cout << "Value at i: " << i << ", j: " << j << ", Band: " << bandNo << " is: " << retVal << endl << endl;
          }
          if (is_X && is_Y)
          {
            cout << setprecision(10) << "Value at X: " << X << ", Y: " << Y << ", Band: " << bandNo << " is: " << retVal << endl << endl;
          }
          cout << "Process times:" << endl;
          cout << "Parsing console parameters :  " << (double)(t_ConsoleParse - t_Start) / CLOCKS_PER_SEC << " sec." << endl;
          cout << "Reading Geotiff dataset :  " << (double)(t_ReadDataset - t_ConsoleParse) / CLOCKS_PER_SEC << " sec." << endl;
          cout << "Reading value from Geotiff :  " << (double)(t_getValue - t_ReadDataset) / CLOCKS_PER_SEC << " sec." << endl;
        }
        else
        {
          cout << "Input coordinate is invalid. Coordinate range:" << endl;
          if (is_i && is_j)
          {
            cout << "i : 0 - " << gdalGeotiff.width - 1 << endl;
            cout << "j : 0 - " << gdalGeotiff.height - 1 << endl;
          }
          else if (true)
          {
            cout << setprecision(10) << "X : "<< gdalGeotiff.extend[0] << " -> " << gdalGeotiff.extend[2] << endl;
            cout << "Y : " << gdalGeotiff.extend[1] << " -> " << gdalGeotiff.extend[3] << endl;
          }
        }
      }
      else
      {
        cout << "Band number is invalid." << endl;
        cout << "bandNo : 1 - " << gdalGeotiff.bandCount << endl;
      }

      //Clean memory
      gdalGeotiff.~GdalGeotiff();
    }
  }
  else
  {
    if (!is_fileName)
    {
      cout << "Missing parameter : input" << endl;
    }

    if (!is_bandNo)
    {
      cout << "Missing parameter : bandNo" << endl;
    }
    if (!((is_i && is_j) || (is_X && is_Y)))
    {
      cout << "Missing parameters : i,j or X,Y" << endl;
    }
  }
  cin.get();
  return 0;
}
