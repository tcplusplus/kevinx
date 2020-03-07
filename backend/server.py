from serial import Serial
stream = Serial(port='/dev/ttyS0')
data = ''
while True:
    c = stream.read(1)
    if c != '|':
        data += c
    else:
        print(c)

'''
with open('/dev/ttyS0') as fptr:
    while True:
        data = fptr.read()
        print(data)
'''
