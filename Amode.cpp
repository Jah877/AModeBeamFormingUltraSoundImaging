#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

float **createDataMatrix(int numElement, int numSample)
{
    // Dynamically create a 2D data matrix of size numElement and numSample
    float **RFData; // create double pointer of float type
    RFData = new float* [numElement]; // allocate float pointer of size numElement
    for (int i = 0; i < numElement; i++){ // for each float pointer of size numElement
        RFData[i] = new float [numSample]; // allocate a new float array of length numSample
    }
    return RFData; // returns a numElement x numSample matrix
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{
    // Open the text file fileName, read the data and store into RFData
    cout << "Processing File..." << endl << endl; // checkpoint for user
    ifstream infile (fileName); // load the file via ifstream

    if (!infile.good()){ // if the data fails to load, terminate program
        cerr << "File could not be processed.";
        return -1;
    }

    char lineStream[100];

    for (int i = 0; i < numElement; i++){ // loads file element by element into RFData matrix
        for (int j = 0; j < numSample; j++){
            infile.getline(lineStream, 100); //uses the getline function to copy line by line, taking up to 100 characters
            RFData[i][j] = atof(lineStream); // converts argument to float
        }
    }
    infile.close(); // close the input file
    return 0; // returns 0, if data is correctly loaded
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    float ImgDepth; // get imaging depth input from user
    cout << "Enter Imaging Depth: ";
    cin >> ImgDepth;
    cout << "The scanline will be generated with a depth of " << ImgDepth << "m" << endl << endl;

    cout << "Enter Number of Pixels: "; // get number of pixels from user
    cin >> numPixel;
    cout << "The scanline will be generated with " << numPixel << " pixels." << endl << endl;

    float *scanlineLocation = new float[numPixel]; // create 1D array for scanlineLocation, distance of each pixel
    for (int i = 0; i < numPixel; i++){ // iterates through array elements
        scanlineLocation[i] = i * (ImgDepth/(numPixel - 1)); // populates as per 0 - to - imaging depth
    }

/* // output array to console for debugging
    for (int i=0; i < numPixel; i++){
        if (scanlineLocation[i] != '\n'){
        cout << i << ":" << scanlineLocation[i] << setw(5);
        }
    }
    cout << endl;
*/
    return scanlineLocation;
}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH)
{
   float N = numElement; // handle any possible math errors, by establishing as float

   float *eleLocation = new float[numElement]; // 1D array containing physical location in x-direction of each transducer element
   for (int n = 0; n < numElement; n++){  // populates each element in the array as per given formula
    eleLocation[n] = (n-((N-1)/2))*PITCH; 
   }

/* // output array to console for debugging
    cout << "Now printing Element Locations..." << endl;

    for (int i=0; i < numElement; i++){
        if (eleLocation[i] != '\n'){
        cout << "Location " << i << ": " << eleLocation[i] << endl;
        }
    }
    cout << endl;
*/
   return eleLocation;
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel)
{
    float *scanline = new float[numPixel]; // allocate a float array to store beamfrom results
    return scanline;
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{
    float *tForward = new float[numPixel]; // calculates the forward time of flight for each position, using 1D array
    for (int i =0; i < numPixel; i++){
        tForward[i] = (scanlinePosition[i]/SoS); // applies given formula
    } 

    float **tBackward = new float*[numPixel]; // calculates backwards flight time for wave to go from ith elemetn back to kth element of transducer
    for (int i = 0; i < numPixel; i++) { // uses a 2d array to perform calculations for each element
        tBackward[i] = new float[numElement];
        for (int k = 0; k < numElement; k++) {
            tBackward[i][k] = sqrt(pow(scanlinePosition[i],2) + pow(elementPosition[k],2)) / SoS; // applies given formula
    }
    }

    float **tTotal = new float*[numPixel]; // 2D for total time of flight from the tranducer to the ith location and kth element
    for (int i = 0; i < numPixel; i++){
        tTotal[i] = new float[numElement];
        for (int k =0; k < numElement; k++){
            tTotal[i][k] = tForward[i] + tBackward[i][k]; // applies given formula
        }
    }

    int **s = new int*[numPixel]; // used to calculate a sample s from kth transducer element matches with the ith scanline location 
    for (int i = 0; i < numPixel; i++){ //uses a 2d array to perform calculatios for each element
        s[i] = new int[numElement];
        for (int k = 0; k < numElement; k++){
            s[i][k] = floor(tTotal[i][k] * FS); // applies given formula
        }
    }

    float *Preal = new float[numPixel]; //data extraction for the ith scanline location from kth element at given s[i][k]
    for (int i =0; i < numPixel; i++){
        Preal[i] = 0;
    for (int k =0; k < numElement; k++){
        Preal[i] += realRFData[k][s[i][k]];
        }
    }

    float *Pimag = new float[numPixel]; //data extraction for the ith scanline location from kth element at given s[i][k]
    for (int i =0; i < numPixel; i++){
        Pimag[i] = 0;
    for (int k =0; k < numElement; k++){
        Pimag[i] += imagRFData[k][s[i][k]];
        }
    }

    for (int i =0; i < numPixel; i++){
        scanline[i] = sqrt(pow(Preal[i],2) + pow(Pimag[i],2)); // place the magnitude calculation in the scanline
    }
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{
    cout << "Outputting file..." << endl;

    ofstream outfile (fileName); // use the ofstream, to create an output file

    if (!outfile.good()){ // if file fails to open, terminate program
        cerr << "File could not be processed.";
        return -1;
    }
    
    // generates a header line
    outfile << "Position, Value" << endl;

    // set precision of the floating-point output
    outfile << fixed << setprecision(6);

    for (int i =0; i < numPixel; i++){ // for each line print the desired result to the csv file
        outfile << scanlinePosition[i] << "," << scanline[i] << endl;
    }
    cout << fileName << endl; // output the file to console
    outfile.close(); // close the output file
    return 0;
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
    // delete all single pointer allocated memory

    delete[] scanline;
    delete[] scanlinePosition;
    delete[] elementPosition;

    // delete all double pointer arrays, release for both dimensions of matrix

    for (int i =0 ; i < numElement; i++) { // release allocated memory for each float pinter
        delete[] realRFData[i];
        delete[] imagRFData[i];
    }

    // delete allocated memory for single pointer
    delete[] realRFData;
    delete[] imagRFData;
}