# Computer Graphics OpenGL project, Kristina Milosevic 127/2018

## Description
Pyramid objects have directional and spotlight lighting - by pressing B on your keyboard, 
the lighting changes from Phong to Blinn-Phong reflection model.

The object of Finn the Human has a point light source of lighting. The model doesn't have a specular component so
the Phong reflection model is used.


The fire portal is the example of using blending, specifically discarding fragments.


The bacon road and the portal aren't lit. 

The desert background is an implementation of SkyBox(Cubemaps).


The brick archway is used to implement normal mapping. It is combined with blending.




### Controls
| Key | Description                                                      |
| :---  |:-----------------------------------------------------------------|
| ```W``` , ```A```, ```S```, ```D``` | Move the camera                                                  |
| ```Mouse left/right/up/down``` | Change the camera angle                                          |
| ```Mouse scroll``` | Zoom the camera                                                  |
| ```ESC``` | Exit the scene                                                   |
|```L```| Move light source position for the archway to the front          |
|```O```| Move light source position for the archway backwards             |
|```F```| Move light source position for Finn to the left                  |
|```R```| Move light source position for Finn to the right                 |
|```B```| Change reflection model from Phong to Blinn-Phong and vice versa |
