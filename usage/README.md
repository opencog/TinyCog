Before using tinycog functionalities be sure to

  1. Install OpenCog and its dependencies 
  2. Install TinyCog and its dependencies
  3. Enable the camera via raspi-config
  
To test the c++ detector functions that currently have a scheme wrapper
  1. cd to TinyCog/build and open a guile shell
  2. load the dr_roboto library
    (load-extension "./libdr_roboto" "init_dr_roboto")
  3. Run some available detector functions
  
    * (det-face)------------>To detect face.
    It returns the size and positions of faces detected
    
    * (det-face-smile)------>To detect smile.
    It returns the size and position of a face along with a true/false if the face is smiling or not.
    
    * (det-salient-point)----->To detect a saliency.
    It returns the point of saliency
    
    * (act-say "your-input-text")------->To make your speaker say the text
    It does TTS.
    
