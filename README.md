# LightCtrl

![cover image](https://github.com/mlohse/LightCtrl/blob/master/img/cover.jpg?raw=true)

Light Controller for Egret TenV4 electric scooter

The goal of this project is to power the scooter's front light from the main LiPo battery instead of using 4x throw-away alkaline batteries (AAA).
Further, it allows the light to be switched on/off conveniently via the throttle controller's power button (same as the rear light)
or have the light switched on/off automatically when it's getting dark (e.g. in tunnels or at night).

The PCB needs to be installed into the front light's battery compartment. The on/off switch is no longer needed and can be replaced by a photo transistor.

A short demo video (German) is available on vimeo: https://vimeo.com/364582430


# Required Skills
Before you consider building your own, please be aware that you'll need some skills here:

* You should be able to solder SMD! I've chosen large components (1206) for the PCB, but of course there is the 44-pin package of the ATMEGA, which may need some practice and a decent solder iron/station.

* The PCB and front light is powered via the 5V USB supply of the scooters throttle controller and needs to be connected in series to the throttle controller's tx data line (white cable).
  This can be accomplished by either...
  a) opening the throttle controller (which is glued with silicone), remove the USB socket (glued with even more silicone, big mess!), open the bridge between the USB data pins, unsolder the throttle controller's UART Tx cable and wire it through the USB data pins or...
  b) carefully removing the outer insulation of the data cable and connecting the PCB directly

* The front light's plastic case needs some mechanical modification to fit a cable through the housing (drill a hole) and fit PCB into the battery compartment.

* You'll need to remove the front light's on/off switch and solder wire between the voltage regulators enable pin (again SMD) and ground.

* You'll need an ISP-programmer for flashing the firmware (those are available on Ebay for ~15 Euro)

This project is certainly not suitable for beginners and of course I won't take any liability if you break your scooter or any components of it.


# Bill of Material (BOM)
The PCB can be ordered at OSH Park (they ship worldwide):

* https://oshpark.com/shared_projects/PSo9nJN7

It doesn't really matter of which brand/type you order resistors and capacitors, nor does this board require any tight tolerances. You can purchase whatever your local hardware dealer has on stock fitting the value/package described by the schematic.
However, for convenience I've created a virtual shopping basket at Reichelt Elektronik:

* https://www.reichelt.de/my/1627216

Unfortunately, the AQY212EH isn't available at Reichelt, but you can order it from Buerklin,
Order-No.: 29G8736

* https://www.buerklin.com/de/Produkte/Elektromechanik/Relais-und-Sch%C3%BCtze/PhotoMOS-Relais/AQY-212-EHA%2C-SMD-Relais/p/29G8736

Further, I've tried several (low-cost) usb cables from Amazon which didn't quite fit my needs, until I came across these 2:

* https://www.amazon.de/gp/product/B01I1HXRK8/ (this is actually much shorter than 40cm, I measured only 34cm!)

* https://www.amazon.de/gp/product/B00P0ES0VC/

The UGreen USB3 extension cable is of decent quality and can be crimped perfectly into the Micromatch connector.
(Yes, it needs to be cut into halves: The female connector is needed for the front light, the male can be used to build an adapter for the ISP programmer)

# Building and Flashing the Firmware
You need to have an AVR toolchain installed (make, gcc, binutils)

On Arch Linux simply install the following packages:

`# pacman -S community/avr-gcc community/avr-binutils community/avr-libc`

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
