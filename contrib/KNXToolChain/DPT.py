#
#   todo: aus Kompatibilitäts-Gründen das Modul 'EIS5' implementieren...
#

import string
import exceptions
"""
                    Datapoint-Type
            Data-Type               Dimension
        Format      Encoding    Range       Unit    

"""


DPT1Def={"001": ("DPT_Switch",("Off","On")),    # EIS1
"002":("DPT_Bool",("False","True")),
"003":("DPT_Enable",("Disable","Enable")),
"004": ("DPT_Ramp",("No ramp","Ramp")),
"005": ("DPT_Alarm",("No alarm","Alarm")),
"006": ("DPT_BinaryValue",("Low","High")),
"007": ("DPT_Step",("Decrease","Increase")),
"008": ("DPT_UpDown",("Up","Down")),
"009": ("DPT_OpenClose",("Open","Close")),
"010": ("DPT_Start",("Stop","Start")),
"011": ("DPT_State",("Inactive","Active")),
"012": ("DPT_Invert",("Not inverted","Inverted")),
"013": ("DPT_DimSendStyle",("Start/stop","Cyclically")),
"014": ("DPT_InputSource",("Fixed","Calculated"))
}

DPT2Def={                                       # EIS8
"001": ("DPT_Switch_Control",("Off","On")),
"002": ("DPT_Bool_Control",("False","True")),
"003": ("DPT_Enable_Control",("Disable","Enable")),
"004": ("DPT_Ramp_Control",("No ramp","Ramp")),
"005": ("DPT_Alarm_Control",("No alarm","Alarm")),
"006": ("DPT_BinaryValue_Control",("Low","High")),
"007": ("DPT_Step_Control",("Decrease","Increase")),
"008": ("DPT_Direction1_Control",("Up","Down")),
"009": ("DPT_Direction2_Control",("Open","Close")),
"010": ("DPT_Start_Control",("Stop","Start")),
"011": ("DPT_State_Control",("Inactive","Active")),
"012": ("DPT_Invert_Control",("Not inverted","Inverted"))
}    

DPT3Def={                                      # EIS2
"007": ("DPT_Control_Dimming",("Decrease","Increase")), #   See 1.007
"008": ("DPT_Control_Blinds",("Up","Down")),            #   See 1.008
"009": ("DPT_Mode_Boiler",("Fixed","Calculated"))      #   See 1.014
}

DPT4Def={                                       # EIS13
"001": "DPT_Char_ASCII",
"002": "DPT_Char_8859_1"
}

DPT5Def={
"001": ("DPT_Scaling",(0,100),"%"),
"003": ("DPT_Angle",(0,360),"°"),
"004": ("DPT_RelPos_Valve",(0,255),"%"),
"010": ("DPT_Value_1_Ucount6",(0,255),"cnter pulses")
}

DPT6Def={                                       # EIS14 / signed.
"010": ("DPT_Value_1_Count","counter value"),
"020": ("DPT_Status_Mode3",None)
}

DPT7Def={                                       # EIS10 / unsigned.
"001": ("DPT_Value_2_Ucount","counter pulses"), # (16-bit unsigned value)
"010": ("DPT_PropDataType",None)
}    

DPT8Def={                                       # EIS10 / signed.
"001": ("DPT_Value_2_Count","counter pulses")
}    

DPT9Def={                                       # EIS5
"001": ("DPT_Value_Temp",(-273,+670760),"°C"),
"002": ("DPT_Value_Tempd",(-670760,+670760),"K"),
"003": ("DPT_Value_Tempa",(-670760,+670760),"K/h"),
"004": ("DPT_Value_Lux",(0,670760),"Lux"),
"005": ("DPT_Value_Wsp",(0,670760),"m/s"),
"006": ("DPT_Value_Pres",(0,670760),"Pa"),
"010": ("DPT_Value_Time1",(-670760,+670760),"s"),
"011": ("DPT_Value_Time2",(670760,+670760),"ms"),
"020": ("DPT_Value_Volt",(-670760,+670760),"mV"),
"021": ("DPT_Value_Curr",(-670760,+670760),"mA")
}    

DPTDefs={"Boolean":("1",1,DPT1Def)} # DPT-Number,Bit-Len,PTR


def DPT9ToFloat(value):
    Mantisse=0
    Exponent=0
    Res=0.0
    Sign=False

    Sign=(value & 0x8000)==0x8000
    Mantisse=value & 0x7ff

    if Sign==True:
        Mantisse=(-Mantisse) & 0x7ff
    
    Exponent=(value & 0x7800)>>11
    Res=(float)((Mantisse<<Exponent)/100.0)

    if Sign==True:
        Res=Res*-1.0  
            
    return Res

def FloatToDPT9(value):
    Exponent=0
    Mantisse=0
    Res=0
    Sign=(value<0.0)

    if Sign==True:
        value*=-1.0

    while value>20.47:
        value/=2.0
        Exponent+=1

    Mantisse=int(value*100.0)

    if Sign==True:
        Mantisse=(-Mantisse) & 0x7ff
        Res|=0x8000

    Res|=((((Exponent<<3) & 0x78) * 0x100) | Mantisse)
        
    return Res    

class DPTBase:
    DataType=""
    BitLen=0
    #EISDataType=None
    # Hinweis: die folgenden Klassen-Variablen sind
    #          eigentlich Instanz-Variablen!!!
    Format=None
    Encoding=None
    Range=None    
    
    def __init__(self):        
        pass
    def getUnit(self):
        return None
    def getRange(self):        
        return None
        

class DPT1(DPTBase):
    BitLen=DPTDefs["Boolean"][1]
    DataType="Boolean"
    
    def __init__(self,subtype="001",value=0):
        self.subtype=subtype
        if not DPT1Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT1: Invalid Subtype: %s" % self.subtype)
        self.value=bool(value)
    def __str__(self):
        # check: DPT-Name getrennt verwalten???
        return DPT1Def[self.subtype][0]+":"+DPT1Def[self.subtype][1][self.value]
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)
    def __invert__(self):
        self.value=not self.value
        return self.value    

class DPT2(DPTBase):
    BitLen=2
    DataType="1-Bit Controlled"
    
    def __init__(self,subtype="001",value=0):
        self.subtype=subtype
        if not DPT2Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT2: Invalid Subtype: %s" % self.subtype)
        self.value=value & 0x03
    def __str__(self):
        print DPT2Def[self.subtype][0]
        if (self.value & 0x02)==0x02:
            return "Control:"+DPT2Def[self.subtype][1][self.value & 0x01]
        else:
            return "No control"        
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)        

class DPT3(DPTBase):
    BitLen=4
    DataType="3-Bit Controlled"
    
    def __init__(self,subtype="007",value=0):
        self.subtype=subtype
        if not DPT3Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT3: Invalid Subtype: %s" % self.subtype)
        self.value=value & 0x0f
    def __str__(self):        
        print DPT3Def[self.subtype][0]

        c=DPT3Def[self.subtype][1][(self.value & 0x08)>>3]        

        if self.subtype=="007" or self.subtype=="008":            
            if self.value & 0x07==0x00:
                v="Break"
            else:
                v="Step:%u" % (self.value & 0x07)                    
        elif self.subtype=="009":
            if self.value in [1,2,4]:
                t={1:"0",2:"1",4:"2"}
                v="Mode %s" % (t[self.value])
            else:
                v="Invalid Mode"            
        else:
            v="???"
        return c+":"+v
    
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)

class DPT4(DPTBase):
    BitLen=8
    DataType="Character Set"

    def __init__(self,subtype="001",value=0):    
        self.subtype=subtype
        if not DPT4Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT4: Invalid Subtype: %s" % self.subtype)
        if self.subtype=="001":
            self.value=value & 0x7f
        elif self.subtype=="002":
            self.value=value & 0xff
    
    def __str__(self):        
        print DPT4Def[self.subtype]
        
        return chr(self.value)
    
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)


class DPT5(DPTBase):
    BitLen=8
    DataType="8-Bit Unsigned Value"

    def __init__(self,subtype="001",value=0):
        self.subtype=subtype
        if not DPT5Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT5: Invalid Subtype: %s" % self.subtype)
        self.value=value & 0xff
        self.Unit=DPT5Def[self.subtype][2]
        self.Range=DPT5Def[self.subtype][1]
        
    def getUnit(self):
        return self.Unit

    def getRange(self):
        return self.Range

    def __str__(self):        
        print DPT5Def[self.subtype][0]
        offset=self.Range[0]
        scaling=self.Range[1]-offset        
        value=float((self.value*(scaling/255.0))+offset)
        return str(value)

    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)

class DPT6(DPTBase):
    BitLen=8
    DataType="8-Bit Signed Value"

    def __init__(self,subtype="010",value=0):
        self.subtype=subtype
        if not DPT6Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT6: Invalid Subtype: %s" % self.subtype)

        if subtype=="010":
            if (value & 0x80)==0x80:
                self.value=-((0xff-(value & 0xff))+1)
            else:
                self.value=(value & 0xff)
        elif subtype=="020":
            self.value=(value & 0xff)
        
        self.Unit=DPT6Def[self.subtype][1]

    def __str__(self):        
        print DPT6Def[self.subtype][0]
        if self.subtype=="010":
            return str(self.value)
        elif self.subtype=="020":
            abcde=(self.value & 0xf8)>>3
            xxx=(self.value & 0x07)
            st=""
            for i in [4,3,2,1,0]:
                if (abcde & (1<<i)):
                    st=st+chr(69-i)
                else:
                    st=st+"_"
            if xxx in [1,2,4]:
                t={1:"0",2:"1",4:"2"}
                v="Mode %s" % (t[xxx])
            else:
                v="Invalid Mode"            
            return st+":"+v

    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)        

class DPT7(DPTBase):
    BitLen=16
    DataType="2-Octet Unsigned Value"

    def __init__(self,subtype="001",value=0):
        self.subtype=subtype
        if not DPT7Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT7: Invalid Subtype: %s" % self.subtype)
        self.Unit=DPT7Def[self.subtype][1]
        self.value=(value & 0xffff)
    def __str__(self):        
        print DPT8Def[self.subtype][0]
        return str(self.value)    
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)

class DPT8(DPTBase):
    BitLen=16
    DataType="2-Octet Signed Value"

    def __init__(self,subtype="001",value=0):
        self.subtype=subtype
        if not DPT8Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT8: Invalid Subtype: %s" % self.subtype)
        self.Unit=DPT8Def[self.subtype][1]

        if (value & 0x8000)==0x8000:
            self.value=-((0xffff-(value & 0xffff))+1)
        else:
            self.value=(value & 0xffff)
            
    def __str__(self):        
        print DPT8Def[self.subtype][0]
        return str(self.value)
    def __int__(self):
        return int(self.value)
    def __long__(self):
        return long(self.value)            


class DPT9(DPTBase):
    BitLen=16
    DataType="2-Octet Float Value"
    def __init__(self,subtype="001",value=0):        
        self.subtype=subtype
        if not DPT9Def.has_key(self.subtype):
            raise exceptions.ValueError("DPT9: Invalid Subtype: %s" % self.subtype)
        self.value=DPT9ToFloat(value)
        self.Unit=DPT9Def[self.subtype][2]
        self.Range=DPT9Def[self.subtype][1]

    def __str__(self):        
        print DPT9Def[self.subtype][0]
        return str(self.value)
# __float__(self) ???
    def getUnit(self):
        return self.Unit

    def getRange(self):
        return self.Range        
    
        
def test():
#    r=DPTDefs["Boolean"]
#    print r
#    print r[2]["012"]

    d=DPT1("013",True)    
    #print string.split(str(d),":")
    d2=DPT9("001",0x14e2)
#    print str(d2)    
    
    print "Range ",d2.getRange()
    print string.split(str(d2),":"),
    print d2.getUnit()
    
if __name__=="__main__":
    test()
    