from pyzbar.pyzbar import decode


def detect_barcode(img):
    barcode = decode(img)
    for code in barcode:
        res = code.data.decode("utf-8")
        print(res)


