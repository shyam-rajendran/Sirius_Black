# Sirius Black - Flight Computer

Sirius Black is the 5th gen custom flight computer fully designed and developed in house for the sounding rocket built for IREC 25, Airavata. 

Sirius Black consists a multitude of sensors required but not limited for model rocketry, along with 2 integrated pyro channels and 4 servo channels for any actuator control operations. ESP32-S3-N4R8 was the chosen microcontroller due to its great functionalities and prior familiarity with the ESP32 family ICs.

The following write up gives a comprehensive data that could be used while operating with Sirius Black for future projects or serve as reference for any future developements. 

Throughout the developement of this, I documented my approach and considerations I took for most of the steps involved in component selection, schematic design, layout planning, RF considerations, soldering and post-testing results along with mistakes made and solution implemented. My documentation, albeit unstructured, can be found here: 
[Notion](https://chief-cat-5a9.notion.site/Flight-Computer-5-SIRIUS-1274bad4a99a81079046df186ca57ac9)


## Repo Structure
The PCB was designed entirely in Altium Designer. I've included the entire Altium project file including the design files, libraries, 3D models, documentations, schematics, gerber, NCC files. 

To replicate or view the project using Altium, download the files and open it within Altium. Specify the file location for each project file within the project tree

    1. Sirius FC.DsnWrk 
       The Design Workspace — manages the working environment setup, such as open documents and panels. 
       
    2. Sirius_Black.PrjPcbStructure
       Internal file that manages project structure data, not important.

    3. Sirius_Black.PrjPcb
       The main project file — links all source documents (schematics, PCBs, libraries) and manages project-wide settings.

    4. Sirius_Black.OutJob
       The Output Job file — configures and groups output generation tasks like Gerbers, BOMs, PDFs, etc.

    5. PCB1.PcbDoc
       The PCB layout file — contains the physical board design, footprints, and routing.

    6. Design File (Folder)
        a. MCU.SchDoc
        b. Power Distribution.SchDoc
        c. USB.SchDoc
        d. Sensor Suite 1.SchDoc
        e. Sensor Suite 2.SchDoc
        f. Custom Libraries (folder)
          - Contains all custom or outsourced libraries used within the project.

    7. Libraries (Folder)
        a. Sirius.PcbLib
        b. Sirius.SchLib
        c. Sirius.CMP

    8. Docs (Folder)
       All the datasheets or external PDFs referenced of every component used.

    9. 3D Models (Folder)
       STEP files used for every footprint.

    10. Project Outputs for Sirius_Black (Folder)
        a. Sirius_Black schematics.pdf
        b. Sirius Black.step
        c. BOM.xlsx
        d. Design Rule Check - PCB1.drc
        e. Fabrication Files (Folder)
            i. Final Gerber file used for fabrication
        f. NC Drill (Folder)
            i. Drill files
        g. Gerber (Folder)
            i. Contains the Gerber for all layers individually

    11. Test Codes (Folder)
        All the test codes used while testing.



## Sensor ICs used
- BMI088 (6 Axis Inertial Measurement Unit)
- LSM6DSL (6 Axis Inertial Measurement Unit)
- HMC5883 (3 Axis Magnetometer)
- LIS3MDLTR (3 Axis Magnetometer)
- MS5611 (Temperature and Pressure)
- BMP280 (Temperature and Pressure)
- NEOM8N (Gloabal Navigation Sattelite System)
- DS3231 (Real Time Clock)
- RFM95W (LoRa Module @ 915MHz)


## Pinout and Address

Communication Interface:
| **SDA** | **SCL** | **MOSI** | **MISO** | **SCK** |  
| ------------- | ------------- | -- | -- | -- |
| GPIO4  | GPIO5  | GPIO12 | GPIO13 | GPIO14 | 


I2C Peripheral addresses:
| **MS5611** | **BMP280** | **BMI088** | **DS3231RTC** | **LSM6DSL** |  **LIS3MDLTR** | **HMC5883L**|
| ------------- | ------------- | -- | -- | -- |-- |--|
| 0x77  | 0x76  | Accel: 0x19, Gyro: 0x69 | 0x68 | 0x6A | 0x1C| 0x1E|

SPI Peripherals

| **RFM95W (LoRa)** | **GPIO ** | **Micro SD Card** | **GPIO** | **W25Q128JVSQ (Flash)** | **GPIO** |
|:-----------------:|:---------:|:-----------------:|:--------:|:-----------------------:|:--------:|
|        SCK        |   GPIO14  |        SCK        |  GPIO14  |           SCK           |  GPIO14  |
|        MISO       |   GPIO13  |        MISO       |  GPIO13  |           MISO          |  GPIO13  |
|        MOSI       |   GPIO12  |        MOSI       |  GPIO12  |           MOSI          |  GPIO12  |
|        DI0        |   GPIO08  |         CS        |  GPIO15  |            CS           |  GPIO11  |
|       RESET       |   GPIO09  |         -         |     -    |            -            |     -    |
|        NSS        |   GPIO10  |         -         |     -    |            -            |     -    |





## Mistakes in Design
1. Made a mistake while creating the footprint for WS2812B LED, where the pins are flipped, essentially rendering this LED as useless.

> No possible solution other that hardwiring it, which I’d rather not do.


2. SD Card not working consistently.
> I’ve implemented every possible solution I could think of yet I couldn’t get it to work properly. The footprint is correct and there is good continuity between all the pins. The SPI channel is healthy as well and does respond to other SPI peripherals.

> What I noticed was the the assigned CS pin fixed at 3V3. Ideally whenever SD card is accessed, CS should be pulled down but for some reason that wasn’t happening. There was no problem with the ESP32 S3 IC itself and the SD card mount was working properly as well.

> Despite checking multiple times for soldering issue or continuity issue I couldn’t find any. However, I noticed that one shell pin (GND) of the mount was not soldered, so I applied solder to it and the SD card started to work only to fail after reaching the US. Unsoldered GND cannot be the issue as out of the 5 GND pins, 4 were properly soldered and there was stable 3V3 supply.


3. W25Q128JVSQ External Flash not responding
> The device address that the flash was returning didn’t correspond to the address as described by the manufacturer. It’s likely a counterfeit that’s why it didn’t work properly.


## Troubleshoot
1. The resistors used across comparator within the pyro channels were modified to change the reference voltage. The capacitor was replaced with a resistor. Input values changed to meet the common mode input voltage requirement. (Check [Notion Page](https://chief-cat-5a9.notion.site/Flight-Computer-5-SIRIUS-1274bad4a99a81079046df186ca57ac9?source=copy_link) for config details)
- Capacitor is changed as because of it the value of IN- doesn’t vary despite changing the resistor in series. This is because the resistor doesn’t have any reference to the ground as the capacitor is in the middle. Changed capacitor to 5.1k



