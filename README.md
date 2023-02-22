#  Content Based Retrieval
Chandler Smith
No links or URLs

- What operating system and IDE you used to run and compile your code.

I used xcode to run and compile my code. This required command line arguements to be passed via the schema. Please reach out directly if you need assistance with this. 

- Instructions for running your executables.

tl;dr:
- ensure olympus folder is in the local directory
- createFeatureDatabase and readfiles requires manual declaration of file path
- TextureAndColor.csv takes a while to get through all the images, but after a coupel of mins it will return! 

Lots of notes here. This code is messy! I repeatedly tried to create helper functions to lower the number of lines of code bus consistently required little tweaks and, therefore, got errors. 

The overall layout is main file calls pipeline. In main, the specific matching function to be applied is dictated by the commented/uncommented matching code. To toggle between the matching functions, simply comment in the matching declaration you want. 

From there we call pipeline. The maine pipline file is at the end of pipeline.cpp. Pipeline, depending on the csv passed creates a csv file, then runs a full analysis. The csv can be used to understand the specifics of the results. Note, a manual declaration of filepath is required here. Another note about this function, textureAndColor section is set to break after one loop to prevent increase time delays. Uncomment it if you want to create a csv but it won't impact the overall functionality. 

The next two functions are divided into two categories - single vector functions, and two fector functions. These differe in the parameters they take in and produce. Some additional helper functions are called in these two categories and they are in the matching.cpp file. 

TextureAndColor.csv suite of functions take a while to run, but they will get there in a couple of minutes!

Lastly, you need to manually pass your directory path in in readfiles, line 42. 

Other than that this should be good to go. Please don't hesitate to reach out with any comments or concerns. Sorry for the messy code!

Instructions for testing any extensions you completed.

No extensions, this project was turned in late. 

Whether you are using any time travel days and how many

No time travel days used
.

