# Arduino-MicroWifi

System Arduino Login Wifi & MQTT messages to prae.

> Hardware system requirement: [Node MCU](https://en.wikipedia.org/wiki/NodeMCU)

### Step Install Software Basic:
1. Instarll Arduino IDE By [Download Tools](https://www.arduino.cc/en/Main/Software)
![Step1](https://i.imgur.com/Hu2mGe1.png)

2. When Install Arduino IDE success & open program Arduino IDE.
![Step2](https://i.imgur.com/YYMRgwD.png)

3. Go to Menu File >> Preferences >> Add URL >> Addition Board Manager URLs : http://arduino.esp8266.com/stable/package_esp8266com_index.json
![Step3](https://i.imgur.com/BiQKg5M.png)

4. Then OK, and Go to Menu Tools >> Board:”xxxxxx” >> Board Manager…
![Step4](https://i.imgur.com/SoqdNBm.png)

5. Type is Contributed. Find and select ESP8266 and Cilck Install.
![Step5](https://i.imgur.com/oJRtwOE.png)

6. Finished, you can select Menu Tools >> Board: NodeMCU1.0”
![Step6](https://i.imgur.com/EuBfAoa.png)

## Step Install Module Basic:
---
1. Go to Menu Tools  >> Library Manager ::
![Step8](https://i.imgur.com/UXUe3Mb.png)

## Library
- [x] [WiFiManager](https://github.com/tzapu/WiFiManager) Author: tzapu

- [x] [PubSubClient](https://github.com/knolleary/pubsubclient) Author: Nick O'Leary

- [x] [ArduinoJson version::5 ](https://github.com/bblanchon/ArduinoJson) Author: Benoit Blanchon

- [x] [EasyButton](https://github.com/evert-arias/EasyButton) Author: Evert Arias

## Deployment System
2. Deploy File code :: [Smart-wifi.ino]() on Node-MCU.