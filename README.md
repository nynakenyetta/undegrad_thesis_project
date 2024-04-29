# Tactile Array Sensor (Pressure Matrix) for Myoelectric A Prosthetic Hand
For my undergraduate project at Queen Mary University, I am working in a group to create an affordable myoelectric prosthetic hand for a trans-radial amputee. Many of the options currently available on the market are extremely expensive. We aim to create an alternative that is not only less expensive but gives users a sense of touch. This tactile sensor is made cheaply and can be easily replaced if issues arise.

<img src="picture-of-sensor-and-arduino.PNG" width="300">
_Figure 1. Picture of the full system which includes the sensor and Arduino used._

## Initial experiments

<img src="circuit-of-arduino-and-fsr.png
" width="300">
_Figure 2. (A) The FSR circuit (B) The sourced schematic [1]_

Before discovering the pressure matrix, I initially tried using an FSR. Force sensor resistors (FSR) are commonly used in tactile sensing in a variety of forms. FSRs are a polymer thick film (PTF) device that record decreases in resistance when a force is applied to its active surface, see Figure 3. These sensors can be either square or circular, for my application I used a small, circular one. They are cheap and quite robust but can only be used on flat surfaces. Applying them to a curved surface may cause pre-loading that will need to be measured and accounted for. Similar to a pressure sensor, they can only sense how much force they have received and not where. A further disadvantage is that they can only sense changes in force in a one dimensional space. Not at an angle and only from one surface.

## Constructing the pressure matrix

<img src="constructing-prototype.png
" width="300">
_Figure 3. Construction and layers of the pressure matrix_

Following an online tutorial [2], I constructed my original prototype of the sensor out of copper tape, Velostat, and paper. In Arduino, the code provided allowed the Arduino to receive 9 sensor values and print them in a line before repeating. This solution worked quite well but was difficult to integrate with the haptic system due to the values being in a loop and difficult to call. The main algorithms of this code are explained in the mermaind flowchart below. Please click the image to be taken to the code.

## Data Acquisition and Data Visualization

In the final Arduino code, I focused on data acquisition that would be useful not only for my system but also for integrating with the haptic feedback system. I needed to receive all 9 sensor values so that the haptic system could use it for the DC motor and vibration motor modules. To do this, I updated the code the read and store each value in an array. The values are then concatenated into string values. This was done so that MATLAB would be able to easily receive the data and later convert it back into an integer value. The mermaid flowchart below explains the main algorithms. Please click the image to be taken to the code.

<p align="center"><img src="constructing-prototype.png
" width="300"></p>

Finally, I used MATLAB for data visualization. The code runs in a cyclic manner and begins by clearing the Command Window in order to establish a fresh connection with my Arduino. The code functions using a loop that monitors for new data form the Arduino. Once it receives it, it converts it from a string to an integer which is then parsed and distributed the originally initialized 3x3 matrix. The matrix is updated with each loop and allows for real-time visualization. The mermaid flowchart below explains the main algorithms. Please click the image to be taken to the code.

## Future work
Going forward, I am working on using the 3x3 matrices generated through my MATLAB visualization code to train a convolutional neural network (CNN). Through this, I hope to create a separate system for object classification that can later be used to improve the user's experience.



##References
1. In-Depth: Interfacing Force Sensing Resistor (FSR) with Arduino [Internet]. Last Minute Engineers. 2020. Available from: https://lastminuteengineers.com/fsr-arduino-tutorial/#:~:text=4.9V-
2. HOW TO GET WHAT YOU WANT [Internet]. Available from: https://www.kobakant.at/DIY/?p=7443
