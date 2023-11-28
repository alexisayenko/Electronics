from rpi_rf import RFDevice
import time

# Настройка GPIO пина 27 для приёмника
rfdevice = RFDevice(27)
rfdevice.enable_rx()
timestamp = None

print('Приёмник RF 433MHz на GPIO 27 запущен')

try:
    while True:
        if rfdevice.rx_code_timestamp != timestamp:
            timestamp = rfdevice.rx_code_timestamp
            print('Received:', rfdevice.rx_code)
        time.sleep(0.01)
except KeyboardInterrupt:
    pass
finally:
    rfdevice.cleanup()