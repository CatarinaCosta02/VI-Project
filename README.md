# VI
Visualization and Lighting ( Visualização e Iluminação)

This project aimed to gather knowledge about visualization and lighting by building a renderer able to render a 3D image, based on a previously developed project. 
This project was composed of three phases, being the third the most critical.


## Phase 3

In this phase, we were proposed to choose two topics from a given list. We chose "Parallel Multithreading" and "Output JPG/PFM/OpenEXR images" since we found those the more appealing.

### Output JPG/PFM/OpenEXR images

We chose this theme, so we could be able to save the rendering image in various formats. For that, we use the help of the library OpenCV and its features, as well as other tools to save the picture in the mentioned formats successfully.
We implemented an option display so the user could choose which type they wanted the image to be saved when running the program.

### Parallel Multithreading

Since rendering the images without any type of parallelism applied takes a long time to do, we decided to choose this theme so we could better the performance of our renderer by using multithreading, shared variables, and other features of the omp library to help the calculation of the pixels.
