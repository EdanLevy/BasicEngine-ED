git repository link: https://github.com/EdanLevy/BasicEngine-ED.git

### Changes to the codebase

#### Changes in scene.h and scene.cpp

- created a method for custom draw the textures on the desired shape in the viewport of the scene.
- each filter has its own function for applying the filter on the desired screen and the corresponding shape.
- in addition, there are several helper function like setting values to a specific pixel in all channels.
- We added constants for different magic number values like size of image in pixels, min and max values of pixels, and
  thresholds for edge detection.

#### Changes in main.cpp

- created 4 new shapes to draw textures on them and display them in the correct viewport.
- added the base textures we want to draw.
- setting the textures to shapes to be drawn.
- call custom draw function to apply the filter on the desired screen number and the corresponding shape.