Before using tinycog functionalities be sure to

  1. Install OpenCog and its dependencies 
  2. Install TinyCog and its dependencies
  3. Enable the camera via raspi-config
  
To test the c++ detector functions that currently have a scheme wrapper
  1. cd to TinyCog/build and open a guile shell
  2. load the dr_roboto library
    (load-extension "libdr_roboto" "init_dr_roboto")
  3. Run some available detector functions
  
    * (det-face)------------>To detect face.
    It returns the size and positions of faces detected
    
    * (det-face-smaile)------>To detect smile.
    It returns the size and positions of smiles detected
    
    * (det-salient-point)----->To detect a saliency.
    It returns the position where saliency is detected
    
    * (act-say "your-input-text")------->To make your speaker repeat your input text.
    It won't return a value but command your device speaker to say your input word
    
