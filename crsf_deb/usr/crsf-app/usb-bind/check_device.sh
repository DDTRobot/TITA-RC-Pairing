#!/bin/bash  
  
# 标记是否已经执行过crsf-app -bind  
bound=false  
  
while true; do  
    # 查询ls /dev/serial/by-id  
    DEVICE=$(ls /dev/serial/by-id | grep -i "LR3PRO")  
  
    # 检查是否找到了设备  
    if [[ -n "$DEVICE" ]]; then  
        # 如果还没有执行过crsf-app -bind，则执行它  
        if [[ $bound == false ]]; then  
            crsf-app -bind  
            bound=true  # 标记为已执行  
        fi  
        # 可能还需要一些延时，以便不要过于频繁地调用crsf-app -bind  
        sleep 5  # 例如，等待5秒再次检查  
    else  
        # 如果没有找到设备且之前已经执行过crsf-app -bind，则退出循环  
        if [[ $bound == true ]]; then  
            break  
        fi  
        # 如果没有找到设备且之前也没有执行过crsf-app -bind，则继续检查  
        sleep 2  # 例如，等待5秒再次检查  
    fi  
done  
  
# 退出脚本  
exit 0
