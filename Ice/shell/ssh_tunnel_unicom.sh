echo "connect to unicom 192.168.99.71"
ssh -N -C -f -L 192.168.99.96:10071:192.168.99.71:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.72"
ssh -N -C -f -L 192.168.99.96:10072:192.168.99.72:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.73"
ssh -N -C -f -L 192.168.99.96:10073:192.168.99.73:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.74"
ssh -N -C -f -L 192.168.99.96:10074:192.168.99.74:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.75"
ssh -N -C -f -L 192.168.99.96:10075:192.168.99.75:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.76"
ssh -N -C -f -L 192.168.99.96:10076:192.168.99.76:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.77"
ssh -N -C -f -L 192.168.99.96:10077:192.168.99.77:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.78"
ssh -N -C -f -L 192.168.99.96:10078:192.168.99.78:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.79"
ssh -N -C -f -L 192.168.99.96:10079:192.168.99.79:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.80"
ssh -N -C -f -L 192.168.99.96:10080:192.168.99.80:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.81"
ssh -N -C -f -L 192.168.99.96:10081:192.168.99.81:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.82"
ssh -N -C -f -L 192.168.99.96:10082:192.168.99.82:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.83"
ssh -N -C -f -L 192.168.99.96:10083:192.168.99.83:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.84"
ssh -N -C -f -L 192.168.99.96:10084:192.168.99.84:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.85"
ssh -N -C -f -L 192.168.99.96:10085:192.168.99.85:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.86"
ssh -N -C -f -L 192.168.99.96:10086:192.168.99.86:8822 prod@58.242.2.55


echo "connect to unicom 192.168.99.171"
ssh -N -C -f -L 192.168.99.96:10171:192.168.99.171:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.172"
ssh -N -C -f -L 192.168.99.96:10172:192.168.99.172:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.173"
ssh -N -C -f -L 192.168.99.96:10173:192.168.99.173:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.174"
ssh -N -C -f -L 192.168.99.96:10174:192.168.99.174:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.175"
ssh -N -C -f -L 192.168.99.96:10175:192.168.99.175:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.176"
ssh -N -C -f -L 192.168.99.96:10176:192.168.99.176:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.177"
ssh -N -C -f -L 192.168.99.96:10177:192.168.99.177:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.178"
ssh -N -C -f -L 192.168.99.96:10178:192.168.99.178:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.179"
ssh -N -C -f -L 192.168.99.96:10179:192.168.99.179:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.180"
ssh -N -C -f -L 192.168.99.96:10180:192.168.99.180:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.181"
ssh -N -C -f -L 192.168.99.96:10181:192.168.99.181:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.182"
ssh -N -C -f -L 192.168.99.96:10182:192.168.99.182:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.183"
ssh -N -C -f -L 192.168.99.96:10183:192.168.99.183:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.184"
ssh -N -C -f -L 192.168.99.96:10184:192.168.99.184:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.185"
ssh -N -C -f -L 192.168.99.96:10185:192.168.99.185:8822 prod@58.242.2.55

echo "connect to unicom 192.168.99.186"
ssh -N -C -f -L 192.168.99.96:10186:192.168.99.186:8822 prod@58.242.2.55


netstat -tnpl 2>/null | grep -E 'tcp .*:100.*ssh'
