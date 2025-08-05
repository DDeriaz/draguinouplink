# Arduino + Dragino LA66 Shield to LoRaWAN (Swisscom) + Akenza

This project connects an **Arduino UNO** (or compatible board) with a **Dragino LA66 LoRaWAN Shield** to the **Swisscom LPN network**, and forwards messages to **Akenza.io** for IoT data visualization and processing.

## Features

* LoRaWAN activation using **OTAA** (Over-The-Air Activation)
* Periodic sending of a message (e.g., "Hello World") as **hex payload**
* Integration with **Swisscom LPN** and automatic data transfer to **Akenza.io**
* Custom **data decoder script** for processing the payload in Akenza

## Hardware

* Arduino UNO / Nano / Mega
* Dragino LA66 LoRaWAN Shield
* Power: 5V USB (or 7–12V via VIN)

## Setting up

**Step 1**: Connect the antenna to the Dragino LA66 shield before attaching it to the Arduino.
Only after the antenna is securely connected, insert the shield onto the Arduino board.

**Step 2**: Now upload the [lorawan.ino](./lorawan.ino) sketch to your Arduino using the Arduino IDE.

*Remark on the code line*: ```AT+SENDB=0,1,11,48656C6C6F20576F726C64```

  * `0` = Unconfirmed uplink
  * `1` = LoRaWAN port
  * `11` = Payload length in bytes

**Step 3**: Create an account on akenza.io, then add a new device using the following device settings:

```javascript
function consume(event) {
    var decoded = {};

    // Vérifie que payloadHex existe
    if (!event.data.payloadHex) {
        decoded.error = "Aucun payload reçu";
    } else {
        var hexString = event.data.payloadHex;
        var result = "";

        // Convertir chaque octet hex en caractère ASCII
        for (var i = 0; i < hexString.length; i += 2) {
            var hexByte = hexString.substr(i, 2);
            var charCode = parseInt(hexByte, 16);
            result += String.fromCharCode(charCode);
        }

        decoded.message = result; // ex: "Hello World"
        decoded.port = event.data.port; // utile si tu envoies sur plusieurs ports
    }

    emit('sample', { data: decoded, topic: "default" });
}

```

The expected output in Akenza for ```"48656C6C6F20576F726C64"``` (```Hello World```) should be :

```json
{
  "message": "Hello World",
  "port": 1
}
```

Step 4: Add your **new device** in Akenza and connect to the **Akenza Database**.
