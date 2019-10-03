# LightCtrl
Light Controller for Egret TenV4 electric scooter

The goal of this project is to power the scooter's front light from the main LiPo battery instead of using throw-away AAA batteries.
Further, it allows the light to be switched on/off conveniently via the throttle controller's power button (same as the rear light)
or have the light switched on/off automatically when it gets dark.

The PCB needs to be installed into the front light battery compartment. The on/off switch is no longer needed and can be replaced by a photo transistor.


# Required Skills
Before you consider building your own, please be aware that you'll need some skills here:

* You should be able to solder SMD! I've chosen large components (1206) for the PCB, but of course there is the 44-pin package of the ATMEGA, which may need some practice and a decent solder iron/station. More importantly it needs the right solder tip: Do not use a needle style tip! This is how it's done: https://www.youtube.com/watch?v=nyele3CIs-U

* You'll have to open the scooter's throttle controller (which is glued with silicone and therefore needs some mechanical skills), remove the USB socket (glued with even more silicone, big mess!), open the bridge between the USB data pins, unsolder the throttle controller's UART Tx cable and wire it through the USB data pins (I'm going to post detailed instructions with photos in the wiki soon).

* You'll need to drill a hole into the front light's plastic case and the battery compartment (for the USB cable).

* You'll need to remove (unsolder) the on/off switch from the front light's PCB (to make room for the light sensor) and solder a small wire between the voltage regulators enable pin (again SMD) and ground.

* You'll need to crimp some cables (sounds like a simple task, but I've seen plenty of people failing)

* You'll need to use an ISP-programmer for flashing the firmware

No, this project is not suitable for beginners and of course I won't take any liability if you break your scooter.


# Bill of Material (BOM)
The PCB can be ordered at OSH Park (they ship worldwide):

* https://oshpark.com/shared_projects/PSo9nJN7

I had all components laying around from other projects already, so I didn't had to order any.
However, for convenience I've created a virtual shopping basket at Reichelt Elektronik:

* https://www.reichelt.de/my/1627216

Unfortunately, the AQY212EH isn't available at Reichelt, but you can order it from Buerklin,
Order-No.: 29G8736

* https://www.buerklin.com/de/Produkte/Elektromechanik/Relais-und-Sch%C3%BCtze/PhotoMOS-Relais/AQY-212-EHA%2C-SMD-Relais/p/29G8736

Further, I ordered the following USB cables from Amazon:

* https://www.amazon.de/gp/product/B01I1HXRK8/

* https://www.amazon.de/gp/product/B00P0ES0VC/

The UGreen USB3 extension cable is of decent quality and can be crimped perfectly into the Micromatch connector.
(Yes, it needs to be cut into halves, sorry. The female connector is needed for the front light, the male can be used to build an adapter for the ISP programmer)

# Building and Flashing the Firmware
You need to have an AVR toolchain installed (make, gcc, binutils)

On Arch Linux simply install the following packages:

`# pacman -S community/avr-gcc community/avr-binutils`

I'm still using my good old AVR Dragon (unfortunately, it seems to be out of production), which needs to have avrdude installed:

`# pacman -S avrdude`

If you own a different programmer there is a good chance it's also supported by avrdude, otherwise just install whatever software it needs and don't forget to adopt the program/fuses section of the Makefile accordingly.

To build the firmware change into the `src` directory and run:

`# make`

...then connect the board to your ISP programmer (it needs to be powered through the programmer!) and flash the fuses:

`# make fuses`

...and firmware:

`# make program`

Done.
