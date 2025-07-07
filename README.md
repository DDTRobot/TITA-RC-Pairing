tita library for read CRSF data from a Serial Port (UART)


### To clone crsf-app:
    git clone https://github.com/DDTRobot/TITA-RC-Pairing.git
    cd crsf-app
    
### To install crsf_deb.deb:
    sudo dpkg -i crsf_deb.deb

### To install in ubuntu:
    chmod 777 install_ubuntu.sh
    sudo ./install_ubuntu.sh

### To install in yocto:
    chmod 777 install_ubuntu.sh
    ./install_yocto.sh

### To make crsf_deb.deb:
    chmod 0755 crsf_deb/DEBIAN/postinst
    chmod 0755 crsf_deb/DEBIAN/prerm
    fakeroot dpkg-deb -b crsf_deb
