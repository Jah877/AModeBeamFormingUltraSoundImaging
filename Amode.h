#ifndef AMODE_H
#define AMODE_H

// createDataMatrix - Create a 2D array, numElement x numSample
//      numElement: Number of desired elements
//      numSample: Number of desired samples
float ** createDataMatrix(int numElement, int numSample);

// loadRFData - Populate the RFData array with data from input file
//      **RFData: Input pointer to 2D data array
//      *fileName: Input pointer to fileName array
//      numElement: Number of desired elements
//      numSample: Number of desired samples
int loadRFData(float **RFData, const char *fileName, int numElement, int numSample);

// genScanlineLocation - Create an array containing the depth location (in z-direction) for each pixel on the scanline
// &numPixel: Address location for number of pixels
float *genScanlineLocation(int &numPixel);

// genElementLocation - Create an array containing the element location (in x-direction) of the ultrasound transducer
//      numElement: Number of desired elements
//      PITCH: PITCH of transducer (spacing of elements)
float *genElementLocation(int numElement, float PITCH);

// createScanline - Allocate memory to store the beamformed scanline
//      numPixel: Number of desired pixels
float *createScanline(int numPixel) ;

// beamform - Beamform the A-mode scanline
//      *scanline: Input pointer to scanline array
//      **realRFData: Input pointer to real RF data array
//      **imagRFData: Input pointer to iamg RF data array
//      *scanlinePosition: Input pointer to scanline position array
//      *elementPosition: Input pointer to scanline position array
//      numElement: Number of desired elements
//      numSample: Number of desired samples
//      numPixel: Number of desired pixels
//      FS: Number of desired pixels
//      SOS: Number of desired pixels
void beamform(float *scanline, float ** realRFData, float ** imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SOS);

// outputScanline - Write the scanline to a csv file
// *fileName: Input pointer to filename character array
// *scanlinePosition: Input pointer to scanline position array
// *scanline: Input pointer to scanline array
// numPixel: Number of desired pixels
int outputScanline(const char * fileName, float *scanlinePosition, float *scanline, int numPixel);

// destroyAllArrays - Destroy all the allocated memory
//      *scanline: Input pointer to scanline array
//      **realRFData: Input pointer to real RF data array
//      **imagRFData: Input pointer to iamg RF data array
//      *scanlinePosition: Input pointer to scanline position array
//      *elementPosition: Input pointer to scanline position array
//      numElement: Number of desired elements
//      numSample: Number of desired samples
//      numPixel: Number of desired pixels
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel);

#endif