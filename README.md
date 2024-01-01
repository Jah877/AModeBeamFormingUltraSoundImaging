# AModeBeamFormingUltraSoundImaging
amode-beamforming-ultrasound-imaging

Overview
This C++ program is designed to perform A-mode beamforming in ultrasound imaging, simulating the process of generating a scanline from ultrasound data. 
The program involves reading pre-processed ultrasound data from text files, calculating transducer element locations and scanline locations, implementing the A-mode beamforming algorithm, and outputting the results to a CSV file for visualization, as per instructions given in BME 121: Digital Computation.


Key Features
1. Multiple File Compilation: The program is structured using multiple files - main.cpp, Amode.h, and AMode.cpp. The modular approach allows for easier code organization and maintenance. As an in-class final assignment, I developed the Amode.cpp file, given the main.cpp and Amode.h files.

2. Dynamic Memory Allocation: The program dynamically allocates memory for data matrices and arrays using pointers. Memory is appropriately released using the delete keyword in the destroyAllArrays function.

3. File Operations: The program handles file operations, reading data from text files ("realRFData.txt" and "imagRFData.txt") and writing results to an output CSV file ("output.csv").

4. Function Libraries: A custom function library is created to encapsulate specific functionalities, enhancing code readability and modularity.
