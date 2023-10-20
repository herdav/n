# N/Crawler
# Created 2018 by David Herren
# Version 20.10.2023
# https://github.com/herdav/n
# Licensed under the MIT License.
# -------------------------------

from urllib.request import urlopen, URLError
from datetime import datetime as dt
import socket
import time

class WebCrawler:
    def __init__(self):
        self.HOST = 'localhost'
        self.PORT = 5204
        self.ADDR = (self.HOST, self.PORT)
        self.BUFSIZE = 4096
        self.count = 0
        self.first_data = 0

    def crawler(self, url, id_value):
        try:
            site = urlopen(url)
            site_read = site.read().decode()
        except URLError as e:
            print(f"URL Error: {e}")
            return None
        except Exception as e:
            print(f"General Error: {e}")
            return None

        pos_id = site_read.find(id_value)
        if pos_id == -1:
            print(f"ID {id_value} not found in the site")
            return None

        i = pos_id
        pos_start, pos_end = None, None
        while i < len(site_read):
            i += 1
            if site_read[i] == '>':
                pos_start = i + 1
            if site_read[i] == '<':
                pos_end = i
                break

        if pos_start is None or pos_end is None:
            print("Could not find data boundaries in the site")
            return None

        value = site_read[pos_start:pos_end]
        try:
            value = round(float(value.replace(',', '.')), 3)
        except ValueError:
            print(f"Value Error: Could not convert {value} to float")
            return None

        return value

    def conv(self, float_value):
        if float_value is None:
            return 'N/A'
        conversation = str(int(1000 * float_value))
        return conversation

    def run(self):
        while True:
            interval = 1 if self.first_data < 10 else 900

            if dt.now().minute % 15 == 0 and dt.now().second % 59 == 0 or self.first_data < 10:
                if self.first_data < 10:
                    self.first_data += 1

                gas_current_price = self.crawler(url_gas, id_gas_last_trade)
                gas_52high_price = self.crawler(url_gas, id_gas_52w_high)
                gas_52low_price = self.crawler(url_gas, id_gas_52w_low)

                data_gas = ('aa' + self.conv(gas_current_price) + 'ab' + self.conv(gas_52high_price) + 'ac' + self.conv(gas_52low_price))

                wheat_current_price = self.crawler(url_wheat, id_wheat_last_trade)
                wheat_52high_price = self.crawler(url_wheat, id_wheat_52w_high)
                wheat_52low_price = self.crawler(url_wheat, id_wheat_52w_low)

                data_wheat = ('ba' + self.conv(wheat_current_price) + 'bb' + self.conv(wheat_52high_price) + 'bc' + self.conv(wheat_52low_price))

                usd_chf = self.crawler(url_usd_chf, id_usd_chf)
                data_usd_chf = ('ca' + self.conv(usd_chf))

                gbp_chf = self.crawler(url_gbp_chf, id_gbp_chf)
                data_gbp_chf = ('cb' + self.conv(gbp_chf))

                data = ("Crawler: " + data_gas + data_wheat + data_usd_chf + data_gbp_chf + '#')

                print('[' + str(self.count) + '] Port Stream ' + data)
                print('GAS:  ', gas_current_price, gas_52high_price, gas_52low_price, 'GBP/mmBtu')
                print('WHEAT:', wheat_current_price, wheat_52high_price, wheat_52low_price, 'USD/bu')
                print('RATE: ', usd_chf, 'USD/CHF', gbp_chf, 'GBP/CHF')

                self.count += 1

                try:
                    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    client.connect(self.ADDR)
                    data = data.encode()
                    client.send(data)
                    client.close()
                except socket.error as e:
                    print(f"Socket Error: {e}")
            time.sleep(interval)

if __name__ == "__main__":
    url_gas = ''
    id_gas_last_trade = 'data-field-key="lval'
    id_gas_52w_high = 'data-field-key="cash_52w_hi'
    id_gas_52w_low = 'data-field-key="cash_52w_lo'

    url_wheat = ''
    id_wheat_last_trade = 'data-field-key="lval'
    id_wheat_52w_high = 'data-field-key="cash_52w_hi'
    id_wheat_52w_low = 'data-field-key="cash_52w_lo'

    url_usd_chf = ''
    id_usd_chf = 'data-field-key="lval'

    url_gbp_chf = ''
    id_gbp_chf = 'data-field-key="lval'

    crawler = WebCrawler()
    crawler.run()
