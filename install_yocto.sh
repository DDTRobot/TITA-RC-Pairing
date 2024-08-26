make UART_SOURCE=0 
cp crsf-app /usr/bin
cp check-device.service /etc/systemd/system/
cp check_device.sh /usr/bin
systemctl daemon-reload
systemctl enable check-device.service
systemctl restart check-device.service
