# ImageService
**Image Service** - program package which consist of server part and some utilities for client which allows to do next:
- Make and send screenshots after recieving request from the server;
- Send information about all pressed buttons on current PC to the server;
- Launch the screen video streaming on client's PC;

This soft created only for improving my program skills. All this works as follows:
- At first through utility ["**ServicePackerSetup.exe**"](https://github.com/dreamhostage/Image-Service/tree/master/Release) (which may be installed on the PC) we can generate "Setup.exe" for installing complex of utilities on another PC. During generating with using ["**ServicePackerSetup.exe**"](https://github.com/dreamhostage/Image-Service/tree/master/Release), user should set only IP and port where all information from current PC will be send.
- Using ["**ImageServerSetup.exe**"](https://github.com/dreamhostage/Image-Service/tree/master/Release) allow to install server on your PC which make monitoring clients who installed utilites from the first punkt in real time.
# How Client Utilities Work
After launching "Setup.exe" binary file which was generated using "**Packer.exe**" on client PC creates Windows service which names "ImageService". This service automatically starts in the context of the system after turning on the PC. Then service  runs app "bReader.exe" which runs in the context of the current PC user and make monitoring of all pressed buttons on keyboard and then sends this information to the server. Also ImageService after receiving concretical command from server runs app "loop.exe" in the context of the current PC user. "loop.exe" makes screenshot and sends it to server. Taking screenshots engine was written by using GDI+. "VideoStreamer.exe" utility is able to launch the screan video streaming on client's PC after pressing appropriate button on the server app. 
# Important information
After generating Setup file and setting information about IP and port for correct networking it's necessary customize server app ports correctly if you need to use port forwarding on server PC. Every client's utility listens on appropriate separete port. For example if you setting up port **7777** during generating setup file via "**Packer.exe**" all clients utilities will be listen on next ports:
- Screenshots getter (**loop.exe**)      - 7777 port;
- Buttons reader (**bReader.exe**)       - 7776 port;
- Video streamer (**VideoStreamer.exe**) - 7775 port;
# UI review
**Main menu**

![alt text](https://github.com/dreamhostage/Image-Service/blob/master/ImageServerGui/mainMenuEN.png?raw=true)

**File menu**

![alt text](https://github.com/dreamhostage/Image-Service/blob/master/ImageServerGui/FileMenuEN.png?raw=true)

**Settings**

![alt text](https://github.com/dreamhostage/Image-Service/blob/master/ImageServerGui/SettingsMenu.png?raw=true)

**Text Viewer**

![alt text](https://github.com/dreamhostage/Image-Service/blob/master/ImageServerGui/TextViewer.png?raw=true)

# Used libraries & technologies
- WinAPI;
- SFML;
- GDI+;
