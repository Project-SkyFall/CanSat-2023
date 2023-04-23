def kroky2smer1(var, Yhra0, Yhra1, XHod0, XHod1, XHod2, XHod3):

    YHranice = [Yhra0, Yhra1]
    XHodnoty = [XHod0, XHod1, XHod2, XHod3]

    b1 = (YHranice[0]*XHodnoty[0] /
             (XHodnoty[0]*XHodnoty[2] + (XHodnoty[1]**2)/2 -
              XHodnoty[1]*XHodnoty[2] - (XHodnoty[0]**2)/2))
    a1 = -b1/(2*XHodnoty[0])
    c1 = b1**2/(4*a1)

    k1 = 2*a1*XHodnoty[1] + b1
    q1 = YHranice[0] - k1*XHodnoty[2]

    a2 = ((YHranice[1] - YHranice[0] - k1*(XHodnoty[3]-XHodnoty[2]))
          / ((XHodnoty[3]-XHodnoty[2])**2))
    b2 = k1 - 2*a2*XHodnoty[2]
    c2 = YHranice[1] - a2*XHodnoty[3]**2 - b2*XHodnoty[3]

    k2 = 2*a2*XHodnoty[3] + b2
    q2 = YHranice[1] - k2*XHodnoty[3]

    if var <= XHodnoty[0]:
        odecti = 0
          
    elif (var > XHodnoty[0]) and (var <= XHodnoty[1]):
        odecti = a1*var**2 + b1*var + c1
            
    elif (var > XHodnoty[1]) and (var < XHodnoty[2]):
        odecti = k1*var + q1

    elif var == XHodnoty[2]:
        odecti = YHranice[0]

    elif (var > XHodnoty[2]) and (var < XHodnoty[3]):
        odecti = a2*var**2 + b2*var + c2

    elif var == XHodnoty[3]:
        odecti = YHranice[1]

    else:
        odecti = k2*var + q2

    return odecti

def kroky2smer2(var, Yhra0, Yhra1, XHod0, XHod1, XHod2, XHod3, XHod4, XHod5, XHod6, XHod7):
    YHranice = [Yhra0, Yhra1]
    XHodnoty = [XHod0, XHod1, XHod2, XHod3, XHod4, XHod5, XHod6, XHod7]

    if XHodnoty[-1] == 0:
        cMinus1 = 0
        bMinus1 = 0
        aMinus1 = YHranice[0]/(XHodnoty[-2]*(2*XHodnoty[-3] - XHodnoty[-2]))

    else:
        bMinus1 = (YHranice[0]*XHodnoty[-1] /
             (XHodnoty[-1]*XHodnoty[-3] + (XHodnoty[-2]**2)/2 -
              XHodnoty[-2]*XHodnoty[-3] - (XHodnoty[-1]**2)/2))
        aMinus1 = -bMinus1/(2*XHodnoty[-1])
        cMinus1 = bMinus1**2/(4*aMinus1)

    kMinus1 = 2*aMinus1*XHodnoty[-2] + bMinus1
    qMinus1 = YHranice[0] - kMinus1*XHodnoty[-3]

    aMinus2 = ((YHranice[1] - YHranice[0] - kMinus1*(XHodnoty[-4]-XHodnoty[-3]))
          / ((XHodnoty[-4]-XHodnoty[-3])**2))
    bMinus2 = kMinus1 - 2*aMinus2*XHodnoty[-3]
    cMinus2 = YHranice[1] - aMinus2*XHodnoty[-4]**2 - bMinus2*XHodnoty[-4]

    kMinus2 = 2*aMinus2*XHodnoty[-4] + bMinus2
    qMinus2 = YHranice[1] - kMinus2*XHodnoty[-4]


    b1 = (YHranice[0]*XHodnoty[0] /
             (XHodnoty[0]*XHodnoty[2] + (XHodnoty[1]**2)/2 -
              XHodnoty[1]*XHodnoty[2] - (XHodnoty[0]**2)/2))
    a1 = -b1/(2*XHodnoty[0])
    c1 = b1**2/(4*a1)

    k1 = 2*a1*XHodnoty[1] + b1
    q1 = YHranice[0] - k1*XHodnoty[2]

    a2 = ((YHranice[1] - YHranice[0] - k1*(XHodnoty[3]-XHodnoty[2]))
          / ((XHodnoty[3]-XHodnoty[2])**2))
    b2 = k1 - 2*a2*XHodnoty[2]
    c2 = YHranice[1] - a2*XHodnoty[3]**2 - b2*XHodnoty[3]

    k2 = 2*a2*XHodnoty[3] + b2
    q2 = YHranice[1] - k2*XHodnoty[3]

    x = list()
    y = list()

    if (var >= XHodnoty[-1]) and (var <= XHodnoty[0]):
        odecti = 0
            
    elif (var >= XHodnoty[-2]) and (var < XHodnoty[-1]):
        odecti = aMinus1*var**2 + bMinus1*var + cMinus1
            
    elif (var < XHodnoty[-2]) and (var > XHodnoty[-3]):
        odecti = kMinus1*var + qMinus1
            
    elif var == XHodnoty[-3]:
        odecti = YHranice[0]
            
    elif (var < XHodnoty[-3]) and (var > XHodnoty[-4]):
        odecti = aMinus2*var**2 + bMinus2*var + cMinus2

    elif var == XHodnoty[-4]:
        odecti = YHranice[1]

    elif var < XHodnoty[-4]:
        odecti = kMinus2*var + qMinus2

    elif (var > XHodnoty[0]) and (var <= XHodnoty[1]):
        odecti = a1*var**2 + b1*var + c1
            
    elif (var > XHodnoty[1]) and (var < XHodnoty[2]):
        odecti = k1*var + q1

    elif var == XHodnoty[2]:
        odecti = YHranice[0]

    elif (var > XHodnoty[2]) and (var < XHodnoty[3]):
        odecti = a2*var**2 + b2*var + c2

    elif var == XHodnoty[3]:
        odecti = YHranice[1]

    else:
        odecti = k2*var + q2

    return odecti
