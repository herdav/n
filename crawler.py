# CRAWLER

from urllib.request import urlopen
from datetime import datetime as dt
import socket

HOST = 'localhost'
PORT = 5204
ADDR = (HOST, PORT)
BUFSIZE = 4096

url_gas = ''
id_gas_last_trade = ''
id_gas_52w_high = ''
id_gas_52w_low = ''

url_wheat = ''
id_wheat_last_trade = ''
id_wheat_52w_high = ''
id_wheat_52w_low = ''

url_usd_chf = ''
id_usd_chf = ''

url_gbp_chf = ''
id_gbp_chf = ''

count = 0


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
    value = round(float(value.replace(',', '.')), 3)
    return value


def conv(float_value):
    conversation = str(int(1000 * float_value))
    return conversation


while True:
    dt.now()
    if dt.now().second % 10 == 0:
        gas_current_price = crawler(url_gas, id_gas_last_trade)
        gas_52high_price = crawler(url_gas, id_gas_52w_high)
        gas_52low_price = crawler(url_gas, id_gas_52w_low)
        data_gas = ('aa' + conv(gas_current_price) + 'ab' + conv(gas_52high_price) + 'ac' + conv(gas_52low_price))

        wheat_current_price = crawler(url_wheat, id_wheat_last_trade)
        wheat_52high_price = crawler(url_wheat, id_wheat_52w_high)
        wheat_52low_price = crawler(url_wheat, id_wheat_52w_low)
        data_wheat = ('ba' + conv(wheat_current_price) + 'bb' + conv(wheat_52high_price) + 'bc' + conv(wheat_52low_price))

        usd_chf = crawler(url_usd_chf, id_usd_chf)
        data_usd_chf = ('ca' + conv(usd_chf))

        gbp_chf = crawler(url_gbp_chf, id_gbp_chf)
        data_gbp_chf = ('cb' + conv(gbp_chf))

        data = ("Crawler: " + data_gas + data_wheat + data_usd_chf + data_gbp_chf + '#')

        print('[' + str(count) + '] Port Stream ' + data)
        print('GAS:  ', gas_current_price, gas_52high_price, gas_52low_price, 'GBP/mmBtu')
        print('WHEAT:', wheat_current_price, wheat_52high_price, wheat_52low_price, 'USD/bu')
        print('RATE: ', usd_chf, 'USD/CHF', gbp_chf, 'GBP/CHF')

        count += 1

        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(ADDR)
        data = data.encode()
        client.send(data)
        client.close()
