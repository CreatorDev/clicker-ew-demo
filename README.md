![logo](https://static.creatordev.io/logo-md-s.svg)

# contiki-ew-demo

This project is part of the Creator demo built for the Embedded World 2017 show. The demo consists of a pair of conveyor belts moving coloured blocks in a loop, a board detects the colour of blocks and the data is pushed to a cloud application to store the colour and update a multicolour LED bluetooth blub with the last scanned colour.  The colour of the block is detected using a colour sensor fitted to a Clicker board running this contiki application which is made available over UART so that the rest of the system can process the colour blocked.

Other components of the EW17 Creator demo are

* [Ci40 application](https://github.com/CreatorDev/ci40-ew17-uart-to-ds) to receive the detected colour over UART and update an IPSO object with the new colour
* [A javascript application](https://github.com/CreatorDev/webapp-ew17) that receives notifications when new colours are detected, stores them to a database, and pushes bulb-colour value changes
* [Ci40 application](https://github.com/CreatorDev/Ci40-ew17-bulb) that receives bulb-colour value changes and issues the corresponding colour change instruction to the bulb over bluetooth
* A python application on a Mediatek 7688 Duo that drives the motors for the conveyor belt when a user presses the stop/start button, or when an inactivity timeout triggers

---

## Help

If you have any problems installing or utilising this project, please look into 
our [Creator Forum](https://forum.creatordev.io). 

Otherwise, If you have come across a nasty bug or would like to suggest new 
features to be added then please go ahead and open an issue or a pull request 
against this repo.

## License

Please see the [license](LICENSE) file for a detailed explanation.

## Contributing

Any Bug fixes and/or feature enhancements are welcome. Please see the 
[contributing](CONTRIBUTING.md) file for a detailed explanation.

--- 