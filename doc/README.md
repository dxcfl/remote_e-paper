**Simple remote controllable e-paper display**
==============================================

**Things used in this project**
-------------------------------
## Hardware components

* Particle Photon
* Waveshare 200x200, 1.54inch E-Ink display module
* 18650-Type Lithium Ion Battery (generic)
* 18650-Type Battery Holder (generic)

## Software apps and platforms

* Particle Console
* Particle Desktop IDE (Dev)

## Introduction & Disclaimer

This is just a demo, a prototype, a short sketch or quick recipe, how to
control an e-paper display connected to a Particle Photon.

If you look for more comprehensive information about

-   the [Particle Photon,](https://www.particle.io/products/hardware/photon-wifi)

-   [the Particle Device Cloud,](https://www.particle.io/products/software/device-cloud/) or

-   how use a [Waveshare 1.54 inch e-Paper Module](http://www.waveshare.com/1.54inch-e-Paper-Module.htm)

you are encouraged to read the associated documentation for this
platforms/products.

## Prerequisites

-   Hardware components (see BOM)

-   Particle Photon device connected and setup (see
    [here](https://docs.particle.io/guide/getting-started/start/photon/))

-   Particle Build and or Particle Desktop IDE (Dev) setup (see
     [here](https://docs.particle.io/guide/tools-and-features/dev/))

## Waveshare 1.54 inch e-Paper Module

The [Waveshare 1.54 inch e-Paper Module ](http://www.waveshare.com/1.54inch-e-Paper-Module.htm)"*is an
E-paper device adopting the image display technology of
Microencapsulated Electrophoretic Display, MED.*"

The e-paper screen display patterns by reflecting ambient light and does
not require a background light requirement. Once the display pattern is
set/updated the module has an extreme low power consumption in standby
mode (5 uA = 0.000005 A)

To connect the display module to the Photon, you can basically follow
the instructions for “Working with Arduino” from the vendors
[documentation](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module#Working_with_Arduino) and consider the differences
regarding the pin layout and the build environment / platform.

The default **SPI** pins on the Photon are **A5 (MOSI)** and **A3
(SCK).** Following the instructions from the [Waveshare Wiki Page
](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module#Working_with_Arduino)the two SPI signals SCK and MOSI must be remapped
to A3 and A5 for the Photon. The remaining signals like CS, DC, RST and
BUSY can be freely mapped to the other digital signals of the Photon.

### Wiring

This is the mapping table for wiring the module with the Photon:

![](https://raw.githubusercontent.com/dxcfl/remote_e-paper/master/doc/Particle%20Photon%20%2B%20Waveshare%201.54%20inch%20e-Paper%20Module%20%20-%20Pins%20%26%20Wires.png)

and this is the final wiring:

![](https://github.com/dxcfl/remote_e-paper/blob/master/doc/Particle%20Photon%20+%20Waveshare%20e-paper%20module%20-%20Wiring.png?raw=true)

### Library

To integrate the vendor provided library into your own Particle project
some minor changes to the provided library code are necessary:

- Download code and library from [https://www.waveshare.com/wiki/File:1.54inch\_e-Paper\_Module\_code.7z](https://www.waveshare.com/wiki/File:1.54inch_e-Paper_Module_code.7z) and extract the package.
- Copy the files from thedirectory ```arduino/libraries``` of the demo package to ```/lib/ep1in54/src```in your project directory.
- In the files ```/lib/ep1in54/epdif.h``` and ```/lib/ep1in54/epdif.h``` make the following changes:

In file ```epdif.h``` replace

> \#include &lt;arduino.h&gt;

by

> \#include "application.h"
>
> \#include "Particle.h"

In file ```epdif.cpp``` replace

> \#include &lt;spi.h&gt;

by

> \#include "Particle.h"
>
> \#include "Arduino.h"

and

> \#define RST_PIN 8
>
> \#define DC_PIN 9
>
> \#define CS_PIN 10
>
> \#define BUSY_PIN 7

by

> \#define RST_PIN 4
>
> \#define DC_PIN 5
>
> \#define CS_PIN 6
>
> \#define BUSY_PIN 3

## Photon - Application

The application itself  is basically a simple use case combination of two example sketches: The *Web-Connected LED* example provided int
Particle Build web platform and the ```ep1in54-demo``` sketch provided by
Waveshare.

For this demo we basically:

- define/register a Particle function and handler to control the
text message to be displayed

- display the given text  as shown in the *ep1in54-demo* example.

The complete code is [here](https://raw.githubusercontent.com/dxcfl/remote_e-paper/master/doc/src/RemoteLabel.ino).

After being compiled and flashed a new function **show** should be
available on your device and being shown at the Particle console:

![](https://github.com/dxcfl/remote_e-paper/blob/master/doc/screenshots/Screenshot%20=%20Example%20=%20Particle%20console.jpg?raw=true)

Calling this function with parameter "CLEAR" will clear the e-papper.
Any other parameter will be treated as text to be displayed. 
The text can be split into multiple lines with the ';' character.

The **debug** function can be used to toggle the DEBUG events on/off.

## Particle project RemoteLabel

Every new Particle project is composed of 3 important elements that you'll see have been created in your project directory for IoSP-DoorSignDemo.

#### ```/src``` folder:  
This is the source folder that contains the firmware files for your project. It should *not* be renamed.
Anything that is in this folder when you compile your project will be sent to our compile service and compiled into a firmware binary for the Particle device that you have targeted.

If your application contains multiple files, they should all be included in the `src` folder. If your firmware depends on Particle libraries, those dependencies are specified in the `project.properties` file referenced below.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device. It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++. For more information about using the Particle firmware API to create firmware for your Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/) section of the Particle documentation.

#### ```project.properties``` file:  
This is the file that specifies the name and version number of the libraries that your project depends on. Dependencies are added automatically to your `project.properties` file when you add a library to a project using the `particle library add` command in the CLI or add a library in the Desktop IDE.

## Adding additional files to your project

#### Projects with multiple sources
If you would like add additional files to your application, they should be added to the `/src` folder. All files in the `/src` folder will be sent to the Particle Cloud to produce a compiled binary.

#### Projects with external libraries
If your project includes a library that has not been registered in the Particle libraries system, you should create a new folder named `/lib/<libraryname>/src` under `/<project dir>` and add the `.h` and `.cpp` files for your library there. All contents of the `/lib` folder and subfolders will also be sent to the Cloud for compilation.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`
