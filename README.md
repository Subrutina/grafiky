#Computer Graphics OpenGL project, Kristina Milosevic 127/2018

#Description
Pyramid objects have directional and spotlight lighting - by pressing B on your keyboard, 
the lighting changes from Phong to Blinn-Phong reflection model.

The object of Finn the Human has a point light source of lighting. The model doesn't have a specular component so
the Phong reflection model is used.


The fire portal is the example of using blending, specifically discarding fragments.


The bacon road and the portal aren't lit. 

The desert background is an implementation of SkyBox(Cubemaps).


The brick archway is used to implement normal mapping. It is combined with blending.




## Controls
| Key | Description                                                        |
| :---  |:-------------------------------------------------------------------|
| ```W``` , ```A```, ```S```, ```D``` | Moving the camera                                                  |
| ```Mouse left/right/up/down``` | Changing the camera angle                                          |
| ```Mouse scroll``` | Zooming the camera                                                 |
| ```ESC``` | Exiting the scene                                                  |
|```L```| Moving light source position for the archway to the front          |
|```O```| Moving light source position for the archway backwards             |
|```F```| Moving light source position for Finn to the left                  |
|```R```| Moving light source position for Finn to the right                 |
|```B```| Changing reflection model from Phong to Blinn-Phong and vice versa |
