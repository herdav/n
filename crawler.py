# CRAWLER

from urllib.request import urlopen
import socket

HOST = 'localhost'
PORT = 5204
ADDR = (HOST, PORT)
BUFSIZE = 4096

url_gas = ' '
id_gas_current = ' '
url_wheat = ' '
id_wheat_current = ' '


def crawler(url, id_value):
    global pos_start, pos_end
    site = urlopen(url)
    site_read = site.read().decode()
    pos_id = site_read.find(id_value)
    i = pos_id
    while i < len(site_read):
        i += 1
        if site_read[i] == '>':
            pos_start = i + 1
        if site_read[i] == '<':
            pos_end = i
            break

    value = site_read[pos_start:pos_end]
    value = round(float(value.replace(',', '.')), 2)
    return value


j = 0
while j < 20:
    gas_current_price = crawler(url_gas, id_gas_current)
    print('GAS:  ', gas_current_price, 'USD/Btu')
    wheat_current_price = crawler(url_wheat, id_wheat_current)
    print('WHEAT:', wheat_current_price, 'USD/bu')
    j += 1

    data = ('a' + str(int(100 * gas_current_price)) + 'b' + str(int(100 * wheat_current_price)) + '#')
    print('Port Stream: ' + data)
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    data = data.encode()
    client.send(data)
    client.close()
