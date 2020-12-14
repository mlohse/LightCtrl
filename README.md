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
  This can be accomplished by either opening/modifying the throttle controller (see instructions below) or creating an adapter cable which can be installed between the throttle and the motor controller (in the battery compartment).

* The front light's plastic case needs some mechanical modification to fit a cable through the housing (drill a hole) and fit PCB into the battery compartment.

* You'll need to remove the front light's on/off switch and solder wire between the voltage regulators enable pin (again SMD) and ground, see description below...

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


# Front Light PCB Modification
1. Remove the on/off switch and undervoltage indicator led (no longer needed)
2. Connect the voltage regulator's enable pin (outer pin) to GND, as shown:
![front light pcb modification](https://github.com/mlohse/LightCtrl/blob/master/img/led1.jpg?raw=true)


# Throttle Controller Modification
1. Open the throttle controller (it is glued with silicone and requieres some force - use a knife or sharp piece of metal). Once open remove the USB socket which is also glued in with silicone and clean it as best as you can:
![open throttle controller](https://github.com/mlohse/LightCtrl/blob/master/img/throttle1.jpg?raw=true)

2. Unsolder the TX data wire (white, labels as T6):
![tx data wire](https://github.com/mlohse/LightCtrl/blob/master/img/throttle2.jpg?raw=true)

3. Open the solder bridge between the USB data lines:
![usb solder bridge](https://github.com/mlohse/LightCtrl/blob/master/img/throttle3.jpg?raw=true)

4. Conntect the white cabel (which was soldered to throttle T6 previously) to the USB data line and add a new cable from the other usb data line to the throttle's T6 pin. Note: the 4k7 resistor is optional. If it is installed the scooter can be started without the LightCtrl pcb connected. If it is not fitted, the throttle will show an error if powered on without it. This may be usefull as a weak theft-protection (if you take the front-light with you when you leave the scooter alone).
![usb data lines connected](https://github.com/mlohse/LightCtrl/blob/master/img/throttle4.jpg?raw=true)

5. Use some hot glue to put the USB socket back in
Important: After modification the USB socket can no longer be used to charge usb devices! Do not plug in you cell phone!
