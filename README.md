# Project Website & Repo for Brian Li & Marko Ristic's sound visualizer

Project updates will be posted every week

##  Week 1

This week we went through and bought all of our parts:
- 8x32 RGB LED Board: https://www.amazon.com/gp/product/B088BTXHRG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
- Wii Nunchuck: already owned
- Wii Nunchuck Adapter: https://www.amazon.com/gp/product/B00RK1VKUQ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
- MEMS PDM Microphone: https://www.amazon.com/gp/product/B07Z496ZMK/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1

We also made sure that we would be able to interface with our components. The LED's are controlled using WS2812b protocol, which we will iterface with using SPI. Our Nunchuck will use I2C protocol to interface with our microprocessor. Our microphone sends data using PDM, which we will interface with using SPI protocol.

##  Week 2

This week we went about structuring our code based off the block diagram of our project. We implemented a lot of the UART and SPI pin and clock initializations, as well as made a barebones main.c file. Additionally, the parts for our lab came in this week, so next week we will go about soldering all our parts together and physically building our project, as well as begining to handle interefacing with our peripherals within our code.

##  Week 3

This week we implemented interfacing with our microphone using SPI protocol. This was done through the use of a function to receive the 16-bit PDM value from our microphone, which we then converted to a corresponding magnitude (within [0,8]) for our 8x32 LED Board. Additionally, we implemented communication with our Wii Nunchuck using I2C protocol, enabling us to now receive control instructions to change the color and brightness of the LED's on the board.

## Final Update

Unfortunately, we were unable to correctly interface with our 8x32 LED board using SPI protocol. Despite rigorously examining the WS2812B datasheet for the LED, our efforts to correctly interface with the LED board proved unsuccessful. Additionally, Amazon misrouted our cable to connect to our pdm microphone to the wrong Amazon locker (Thanks Bezos!), so we were unable to establish any sort of communication with our PDM Microphone using SPI protocol. This forced us to pivot to a new idea:  A Backup Camera Aide, which would work as follows.

* We use the Nunchuck to simulate our gear shift: pressing c shifts us into reverse.
* When in reverse, we measure the distance from our ultrasonic sensor, and then convert that distance to an index out of 16 to which we will display a bar on our LCD display (i.e. the larger the bar, the closer the object is to the ultrasonic sensor). This bar simulates a distance monitor that would be on the dashboard of your car.
* If an object every gets too close to the sensor in reverese mode (within 0.1m), we output to termite using UART "Too close! Move Forward", to simulate a message that would be displayed on the car's dashboard when about to hit an object.

Protocols/Peripheral Devices:
* I2C: LCD Display & Nintendo Nunchuk,
* UART: Termite Display
* PDM: Ultrasonic Distance Sensor

We implemented writing to the LCD Display this week, and pulled our ultrasonic code from a previous lab to correctly measure the distance from the sensor. We then added state code which was controlled by the pressing the c-button on the Nunchuk, and implemented a conversion from the distance received by the sensor to a displayable index on our LCD screen. 
