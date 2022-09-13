# Read Geotiff with GDAL
Read Geotiff using Gdal C++ API and get value with specified band no, image or projection coordinate parameters. The code was developed and tested on Windows.

### **Parameter Description**  
The console application takes following parameters:	

- **input:** Takes name or full path of input Geotiff file.
- **bandNo:** Takes band number of input Geotiff to get value from.
- **i:** Takes i value of Image coordinate system.
- **j:** Takes j value of Image coordinate system.
- **X:** Takes X value of Projeted coordinate system.
- **Y:** Takes Y value of Projeted coordinate system.
		
For Image coordinate system, upper left corner of the image is origin. i value increases from left to right, j value increases from up to bottom. Projection coordinate system parameters are stored in Geotiff. Generally, while X value is increasing from left to right, Y values increase from bottom to up side of the image.
		
### **Examples**
After compilation of the code, executable program can be run with following parameteres from console.

Using projection coordinates:

	yourBinary.exe input:/../sample.tif X:432109.876 Y:4567890.987 bandNo:1

Using image coordinates:

	yourBinary.exe input:sample.tif i:10 j:15 bandNo:2
