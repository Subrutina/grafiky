# Computer Graphics OpenGL project, Kristina Milosevic, Faculty of Mathematics, University of Belgrade

This project showcases several basic and advanced techniques in OpenGL to demonstrate various lighting models, shading techniques, blending, and texturing.

## Components Overview

### 1. **Pyramid Objects**
- **Lighting:** Directional and spotlight lighting.
- **Shading Model:** By pressing **B**, you can switch between the **Phong** and **Blinn-Phong** reflection models, giving you the opportunity to explore different shading effects on the pyramid objects.

### 2. **Finn the Human Model**
- **Lighting:** Point light source.
- **Shading Model:** Uses the **Phong reflection model** since the model doesn't include a specular component. This highlights how point lighting interacts with the surface for diffuse reflections.

### 3. **Fire Portal**
- **Blending:** Implements fragment discarding for creating a blending effect, which is commonly used for transparent or semi-transparent effects.

### 4. **Bacon Road and Portal**
- **Lighting:** These objects are **unlit**, emphasizing their texture or appearance without the impact of lighting.

### 5. **Desert Background (SkyBox)**
- **Cubemaps:** The desert background is implemented as a **SkyBox**, using cubemaps to render a 360-degree environment.

### 6. **Brick Archway**
- **Normal Mapping:** This object demonstrates **normal mapping** to give the surface more detail by simulating small surface bumps.
- **Blending:** Combined with blending to enhance visual effects.




## Controls
Once the project is built and running, use the following commands to explore the functionalities:
| Key | Description                                                     |
| :---  |:----------------------------------------------------------------|
| ```W``` , ```A```, ```S```, ```D``` | Move the camera                                                 |
| ```Mouse left/right/up/down``` | Change the camera angle                                         |
| ```Mouse scroll``` | Zoom the camera                                                 |
| ```ESC``` | Exit the scene                                                  |
|```L```| Move light source position for the archway to the front         |
|```O```| Move light source position for the archway backwards            |
|```F```| Move light source position for Finn to the left                 |
|```R```| Move light source position for Finn to the right                |
|```B```| Changereflection model from Phong to Blinn-Phong and vice versa |
