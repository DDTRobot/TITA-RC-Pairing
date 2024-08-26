make UART_SOURCE=0 
cp crsf-app /usr/bin
cp usb-bind/check-device.service /etc/systemd/system/
cp usb-bind/check_device.sh /usr/bin
systemctl daemon-reload
systemctl enable check-device.service
systemctl restart check-device.service
