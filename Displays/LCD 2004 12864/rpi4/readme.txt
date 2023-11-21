# Download LCD library
wget https://github.com/dbrgn/RPLCD/archive/refs/heads/master.zip

# Unzip it and install
unzip master.zip
python RPLCD-master/setup.py install
rm -rf master.zip
rm -rf RPLCD-master

# Turn on I2C interface
sudo raspi-config

# Connect display
SDA and SCL portsна on Raspberry Pi
Check diagram

# Run the program
python3 ./hello-world.py