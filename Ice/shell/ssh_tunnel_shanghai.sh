echo "connect to 192.168.99.111"
ssh -N -C -f -L 192.168.99.96:10111:192.168.99.111:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.113"
ssh -N -C -f -L 192.168.99.96:10113:192.168.99.113:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.115"
ssh -N -C -f -L 192.168.99.96:10115:192.168.99.115:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.117"
ssh -N -C -f -L 192.168.99.96:10117:192.168.99.117:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.119"
ssh -N -C -f -L 192.168.99.96:10119:192.168.99.119:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.121"
ssh -N -C -f -L 192.168.99.96:10121:192.168.99.121:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.123"
ssh -N -C -f -L 192.168.99.96:10123:192.168.99.123:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.125"
ssh -N -C -f -L 192.168.99.96:10125:192.168.99.125:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.127"
ssh -N -C -f -L 192.168.99.96:10127:192.168.99.127:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.129"
ssh -N -C -f -L 192.168.99.96:10129:192.168.99.129:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.130"
ssh -N -C -f -L 192.168.99.96:10130:192.168.99.130:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.135"
ssh -N -C -f -L 192.168.99.96:10135:192.168.99.135:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.136"
ssh -N -C -f -L 192.168.99.96:10136:192.168.99.136:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.137"
ssh -N -C -f -L 192.168.99.96:10137:192.168.99.137:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.138"
ssh -N -C -f -L 192.168.99.96:10138:192.168.99.138:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.166"
ssh -N -C -f -L 192.168.99.96:10166:192.168.99.166:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.101"
ssh -N -C -f -L 192.168.99.96:10101:192.168.99.101:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.145"
ssh -N -C -f -L 192.168.99.96:10145:192.168.99.145:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.147"
ssh -N -C -f -L 192.168.99.96:10147:192.168.99.147:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.156"
ssh -N -C -f -L 192.168.99.96:10156:192.168.99.156:8822 m8000t@183.195.244.250 

echo "connect to 192.168.99.159"
ssh -N -C -f -L 192.168.99.96:10159:192.168.99.159:8822 m8000t@183.195.244.250 

netstat -tnpl 2>/null | grep -E 'tcp .*:101[1-3][0-9].*ssh'
