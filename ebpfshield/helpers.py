
from functools import reduce

class TaggedIpList:
    def __init__(self, tag, handle):
        self.addresses = []
        self.tag = tag
        for line in handle:
            line = line.strip()

            if line and line[0] == "#":
                continue

            self.addresses.append(self.ip2int(line))

        self.addresses = sorted(self.addresses)

    def check_membership(self, ip_address):
        ip_address = self.ip2int(ip_address)

        low = 0
        high = len(self.addresses)-1

        while high >= low:
            midpoint = (low + high)//2
            if self.addresses[midpoint] == ip_address:
                return True
            elif self.addresses[midpoint] > ip_address:
                high = midpoint-1
            elif self.addresses[midpoint] < ip_address:
                low = midpoint+1

        return False

    def ip2int(self, ip):
        return reduce(lambda out, x: (out << 8) + int(x), ip.split('.'), 0)

