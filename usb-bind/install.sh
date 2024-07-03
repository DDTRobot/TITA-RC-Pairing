cp check-device.service /etc/systemd/system/
cp check_device.sh /usr/bin
systemctl daemon-reload
systemctl enable check-device.service
systemctl restart check-device.service
