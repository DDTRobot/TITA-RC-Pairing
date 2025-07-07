tita library for reading CRSF data from a Serial Port (V1.1)

### To clone crsf-app:
    git clone https://github.com/DDTRobot/TITA-RC-Pairing.git
    cd crsf-app
    
### To install deb:
    sudo dpkg -i crsf-app_1.1-1_arm64.deb

### To read channel data:
    crsf-app
    
### To enter pairing:
    crsf-app -bind
    
### To enter wifi mode:
    crsf-app -wifi

### To restart:
    crsf-app -restart
    
### To make crsf_deb.deb:
    chmod 0755 crsf_deb/DEBIAN/postinst
    chmod 0755 crsf_deb/DEBIAN/prerm
    fakeroot dpkg-deb -b crsf_deb
