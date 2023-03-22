data = "abcdefghdsadasdada;00001881"
soucet = 0
vycet = 1
ck = ""
ckedit = 0

delka = (len(data))
a = delka - 8
b = delka - 8

for znak in data:
    if a > 0:
        print (znak)
        soucet += (ord(znak))
    if a < 0:
        print (znak)
        ck += znak
    a = a - 1

print (soucet)
print (ck)
ckedit = int(ck)
print (ckedit)

if ckedit == soucet:
    print("true")
else:
    print("false")
    
