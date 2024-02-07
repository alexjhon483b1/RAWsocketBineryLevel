import binascii

hex_string = "e8x65xd4x6axa9x30xe0xd5x5exb7xa2x03x08x00x45x00x00x34x76x0dx40x00x40x06xedxddxc0xa8x00x68x22x6bxf3x5dx8bxaax01xbbx28xd1x66x68xdfx7dxe6xb3x80x10x01xf5xd6xffx00x00x01x01x08x0axf4x50xdbx05x1fxcex0cxb7"

# Remove 'x' characters
hex_string = hex_string.replace('x', '')

# Convert hexadecimal to binary
binary_output = binascii.unhexlify(hex_string)

print(binary_output)

