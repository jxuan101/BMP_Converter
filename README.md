# :camera:BMP Converter (BMP Negative Converter)
is a non-destructive image processing program that takes in a .BMP image file and creates a new .BMP image file with the photo negative filter on. This program uses a GUI built using [wxWidgets](https://www.wxwidgets.org) for C++. 

NOTE: Currently, this program has only been tested for windows. Feel free to try and compile the source by yourself on Linux and Mac systems since this program was made with wxWidgets, a C++ library made for cross platform applications.

To use this program, either download the release or use the building instructions [here](#build-from-source).

This program assumes the following about the input:
1. Must be a .BMP image file
2. No compression was used on the image
3. Bits per pixel = 24

Below is a short technical explanation of the program, if you would like to get to the instructions on how to use this program, click [here](#user-instructions).

## Brief Technical Background of the Program
### Bitmaps (BMP)
The BMP file format also known as a bitmap image file, device independent bitmap (DIB) file format and bitmap, is a raster graphics image file format used to store bitmap digital images.

For the purposes of this program, a valid BMP file consists of several parts:
1. Bitmap file header - 14 bytes
2. DIB header - 40 bytes, 108 bytes, 124 bytes
3. Pixel array - depends on the size of the image

This program will read in all the data and edit only the pixel array because we only want to change the RGB values of the image and leave everything else untouched because there's a high chance of corrupting the resulting image.

To learn more about the BMP file format, click [here](https://en.wikipedia.org/wiki/BMP_file_format).

### Photo Negative
In simple terms, the photo negative or negative of an image is when the darkest areas become the lightest and the lightest areas become the darkest.

Since the max value for any RGB value is 255, the photo negative of a RGB value is obtained with $255-currentValue$. 

For example, if a pixel's RGB value is (100, 111, 50), the photo negative would be (155, 144, 205).

A negative of a negative will create a positive meaning the photo negative of (155, 144, 205) would be (100, 111, 50) which was our original values.

This program will traverse through the pixel array of a BMP file and perform this operation to every RGB value.

### Tech Used
This program was developed using C++ using the latest version of Visual Studio (v17.1.6). The GUI is also written in C++ using the lastest version of [wxWidgets](https://www.wxwidgets.org) (v3.1.6), a C++ library that lets developers create cross platform applications.

## User Instructions
Install the Visual C++ 2015-2022 Redistributable if your system does not have them already. The redistributable will be made available for you in the redist folder or you can download them from Microsoft directly. Start the application. 
 
<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171955358-d7eeb924-8337-49c2-829b-3c272fc055de.png">
</p>

Input the path of the target .BMP image file to be altered either using the text prompt or the "Open File" button.  
Note: Make sure your input file fits the assumptions above or you'll get an nice error message later when you try to generate the photo negative image but good news is, you can change it anytime.

<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171959355-767f3552-8ca8-4352-a394-ef9bb5900866.png">
</p>

By default, the program will output the new image file with the negative filter in the same directory as your input file. 

Optionally, you can also change the path of the directory in which you prefer your output file to be created at by clicking the "Change Output Destination" button. A new window will open where you can set your output destination by entering your path in the text prompt or by using the "Select Folder" button. 

To save your choice, simply click the "Save" button. 

Note: The paths will be validated upon save and will output a successful message along with your new output destination or an error message informing you what went wrong. If you get an error, simply retry and save again.

<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171958655-8e2468ba-0972-49e0-9581-31ea0561c55f.png">
</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171959146-84c37977-a71a-4d70-b988-ccaee4db942f.png">
</p>

When you're satisfied with all your inputs, you can press the "Apply Filter" button to generate a new .BMP file that has your input image with a photo negative filter on it. 

If you're successful, a message will tell you the location of your output file. If you're not successful, there will be also be an error message informing you why.

<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171960014-1ffd0f94-2781-4d38-a46f-cd3e93a49ccb.png">
</p>

If you need a brief explanation of what the program is or a brief refresher of the instructions, you can click the "Help" button and a new window will popup with a summary of this guide. There will also be a "Learn More" button that will lead you here.

<p align="center">
  <img src="https://user-images.githubusercontent.com/35944722/171960201-8743a9e4-3722-4f9b-98f8-05c64bc3ced8.png">
</p>

## Build From Source
### Requirements (Preferably the last versions)
Visual Studio (v17.1.6)

Microsoft Visual C++ 2015-2022 Redistributable (x64 or x86)

wxWidgets (v3.1.6)

### Building
Get the requirements if you don't have them already. 

Download the repository.

Open the Visual Studio solution file included with the repository, should be named "BMP_Converter.sln".

Choose the release x64 configuration in Visual Studio for the project.

Go to Build then Compile.

BMP_Converter is ready to be used!



