

# ESP32 Scailable KPN Things Demo

### Connect the ESP32 TTGO Camera Plus with the Arduino MKR WAN 1310 over I2C

| <img src="docs/img/esp32.jpg" alt="esp32" style="zoom: 50%;" /> | <img src="docs/img/arduino.jpg" alt="arduino" style="zoom: 50%;" /> |
| ------------------------------------------------------------ | ------------------------------------------------------------ |



```

ESP32 TTGO Camera Plus          Arduino MKR WAN 1310

Ground                  <--->   Ground
SDA                     <--->   SDA
SDA                     <--->   SCL

```



### Install ESP32 Scailable Runtime firmware

Install the `ttgocameraplus.bin` using:

https://github.com/espressif/esptool

On boot, the ESP32 will retrieve the emotion detection model available at https://cdn.sclbl.net:8000/file/a3dc49b2-b01b-11eb-af86-9600004e79cc.wasm, or fallback to a locally available version when it fails to connect.

The ESP32 then retrieves a frame from the ESP32's camera, runs it through the Scailable model, and sends the prediction over I2C to the Arduino MKR WAN. It will then retrieve the next frame, and so on.

### Install Arduino Arduino MKR WAN 1310 firmware

Install the Arduino IDE. Setup according to:

https://kpn-iot.github.io/thingsml-c-library/?c--arduino#installation

Load, compile and flash `arduino_mkrwan_slave.ino`.

The Arduino MKR WAN will now receive predictions over I2C, and send them over Lora to KPN Things.


