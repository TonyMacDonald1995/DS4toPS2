# DS4toPS2

DS4toPS2 is a project designed for the Raspberry Pi Pico W, enabling you to control a PlayStation 2 console using a PlayStation 4 controller.

## Features

- Control a PlayStation 2 console with a PlayStation 4 controller over Bluetooth
- Designed for Raspberry Pi Pico W microcontroller
- Supports various functions of the PlayStation 4 controller, including buttons, analog sticks, and vibration feedback

## Getting Started

To get started with DS4toPS2, follow the steps below:

### Prerequisites

- Raspberry Pi Pico W microcontroller
- PlayStation 2 console
- PlayStation 4 controller
- USB cable (to connect Raspberry Pi Pico W to PlayStation 2 console)

### Installation

1. Clone this repository to your local machine:

   ```
   git clone https://github.com/TonyMacDonald1995/DS4toPS2.git
   ```

2. Connect the Raspberry Pi Pico W to your computer using a USB cable.

3. Build and flash the firmware onto the Raspberry Pi Pico W.

4. Connect the Raspberry Pi Pico W to the desired controller port on the PlayStation 2.

5. Power on the PlayStation 2 console.

6. Ensure that the PlayStation 4 controller is fully charged and turned off.

7. If the Playstation 4 controller has not already been paired to the Raspberry Pi Pico W, press and hold the "PS" and "Share" buttons on the PlayStation 4 controller simultaneously until the LED on the controller starts double flashing. This initiates the pairing process with the Raspberry Pi Pico W.

8. Once the pairing is successful, the Raspberry Pi Pico W will start emulating the PlayStation 2 controller input, allowing you to control the console using the PlayStation 4 controller.

### Configuration

The DS4toPS2 project includes the following configuration files:

- `controller_simulator.c`: This file contains the MAC address of the PlayStation 4 controller, which is hardcoded. Make sure to update this file with the correct MAC address of your controller before building and flashing the firmware.

- `psxSPI.pio`: This file includes the GPIO pin assignments for the Raspberry Pi Pico. If you need to modify the pin assignments, make the necessary changes in this file before building and flashing the firmware.

- `btstack/src/classic/hid_host.c`: This file in the BTStack library. You need to modify the hardcoded value of the define `MAX_ATTRIBUTE_VALUE_SIZE` to 512. Locate the line in the file and make the necessary change to ensure compatibility with the PlayStation 4 controller.

### Acknowledgments

The DS4toPS2 project utilizes the `psxSPI.pio` file from the [PicoMemcard](https://github.com/dangiu/PicoMemcard) project. I would like to extend my gratitude to the developers of the PicoMemcard project for providing this essential component.

## License

This project is licensed under the [GNU Public License Version 3](LICENSE).

## Contributing

Contributions to DS4toPS2 are welcome! If you have any ideas, suggestions, or improvements, please feel free to open an issue or submit a pull request.

## Support

If you encounter any issues or have any questions regarding the DS4toPS2 project, please [open an issue](https://github.com/TonyMacDonald1995/DS4toPS2/issues) on the GitHub repository.

## Disclaimer

DS4toPS2 is a single developer project and is not affiliated with or endorsed by Sony Interactive Entertainment or any other related parties. Use this project at your own risk. I am not responsible for any damage caused to your PlayStation 2 console, PlayStation 4 controller, Raspberry Pi Pico W, or any other hardware or software components.

---

I hope you find DS4toPS2 useful for your PlayStation 2 gaming experience. Enjoy playing your favorite games with the convenience of a PlayStation 4 controller!
