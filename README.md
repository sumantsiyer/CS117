LUX 1.0

The program, created by Swetha Akella, Sumant Iyer and Melissa Peng, utilizes two APIs, FastLED and the Adafruit Bluefruit LE nRF51, in order to simplify the connection between the Arduino and light array, and a mobile device and the Arduino respectively. Our own software is implemented in the file LUX.ino, where we define our own functions for utilizing the other APIs. Some challenges faced in the development were limited space for the program, as the Arduino has a relatively small flash memory allocation, thus requiring the conservation of resources. Additionally, we were limited by a very narrow interface provided by the Adafruit app. 

We had a few goals for the program:
	* Prove feasiblity for an app based control over LEDs that can be easily expanded to accomodate different light strips and Arduino types
	* Incorporation of existing APIs to streamline the process of transmitting data from a mobile device to the Arduino and finally to the Light array
	* Make the entire product easy to use for a common user

We delivered on the first two goals, using the FastLED and Adafruit APIs as indicated previously. The FastLED library allows the interchanging of light strips, though changing Arduino type is more complicated. The third goal is not quite developed, as we don't currently have a method to easily program their own lighting combinations without some prior knowledge of coding. We do believe that this capability can be added given more time.

