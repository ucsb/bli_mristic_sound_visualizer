# Project Website & Repo for Brian Li & Marko Ristic's sound visualizer

Project updates will be posted every week

##  Week 1

This week we went through and bought all of our parts:
- 8x32 RGB LED Board: https://www.amazon.com/gp/product/B088BTXHRG/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
- Wii Nunchuck: already owned
- Wii Nunchuck Adapter: https://www.amazon.com/gp/product/B00RK1VKUQ/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
- MEMS PDM Microphone: https://www.amazon.com/gp/product/B07Z496ZMK/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1

We also made sure that we would be able to interface with our components. The LED's are controlled using WS2812b protocol, which we will iterface with using SPI. Our Nunchuck will use I2C protocol to interface with our microprocessor. Our microphone sends data using PDM, which we will interface with using SAI protocol.
