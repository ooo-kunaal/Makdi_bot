Version 4 uses a PCA9685 module to control and power the servo motors because the esp32 expansion board was insufficient to power them. PCA9685 communicates with the ESP32 using I2C protocol, so the code was updated accordingly. This is an adaptation of the "combo" code from before.

![pca9685_module](https://github.com/user-attachments/assets/bbe897d8-270b-4381-984c-2959a30a4cd0)
