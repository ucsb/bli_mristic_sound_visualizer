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
