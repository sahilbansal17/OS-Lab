# Reading Master Boot Record file

To read one block of MBR from **/dev/sdb**:

		$ make
		$ sudo ./read_mbr

# /proc/tty directory

- **/proc/tty**: contains information about the current terminals 
- **/proc/tty/driver**: contains a list of the different types of tty available - each of which is a list of those of each type
- This directory contains information about the available and currently used tty devices on the system. Originally called *teletype devices*, any character-based data terminals are called tty devices.
- Virtual terminals opened using **Alt+ F-key** at the system console.
- The **tty core** takes data from a user that is to be sent to a tty device. It then passes it to a tty **line discipline driver**, which then passes it to the **tty driver**.
- The tty driver converts the data into a format that can be sent to the hardware.
- To control the flow of the data, there are a number of **line disciplines** that can be virtually plugged into any tty device.
	
![Image](https://www.oreilly.com/library/view/linux-device-drivers/0596005903/httpatomoreillycomsourceoreillyimages138427.png)

## driver directory
- contains individual files for some of the tty drivers, if they implement that functionality.
- The default **serial driver** creates a file in this directory that shows a lot of serial-port-specific information about the hardware.

## drivers file

- There are three different types of tty drivers: 
	- console, 
	- serial port, and 
	- pty (psuedo-terminal driver).
- list of current tty devices in use
    
		Name of the driver, Default node name, Major driver no., Range of minors used, Type of the tty driver
		/dev/tty
		/dev/console
		/dev/ptmx
		/dev/vc/0
		rfcomm
		dbc_serial
		ttyprintk
		max310x
		serial
		pty_slave
		pty_master
		unknown

## driver/serial file

- lists the usage statistics and status of each of the serial tty lines.

## ldiscs file

- contains the registered line disciplines
- more detailed information in the **ldisc/** folder.