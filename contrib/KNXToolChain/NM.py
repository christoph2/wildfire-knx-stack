#
#   KNX-Network-Management-Procedures
#

# RCo, RCoV     Point-to-Point, Connection-oriented
# RCl           Point-to-Point, Connection-less

import MG

"""

;
;	AccD ==> Operand-A
;	2,sp ==> Operand-B
;	Ret: AccD ==> Y
;
Implication:	; Implikation-/Subjunktion-A (hier Akzeptanz-Maske).
	coma
	comb
	oraa	2,sp
	orab	3,sp
	rts	

	
;
;	AccD ==> Operand-A
;	2,sp ==> Operand-B
;	Ret: AccD ==> Y
;
XNor:	; Equivalenz.
	eora	2,sp
	eorb	3,sp
	coma
	comb
	rts
	
MatchingIDs:
;
;	AccD	==> Message-ID
;	4,sp	==> Empfangs-ID
;	6,sp	==> Akzeptanz-Filter.
;
	pshd
	ldd		4,sp
	bsr		XNor
	ldd		6,sp
	bsr		Implication
	leas	2,sp
; AccD==0xffff ==> Filter-Match.
	rts
"""

def expect(apci_bits,apci_mask,timeout):
    #
    #   todo: statt 'func' APCI(-Filter); Sinnvoll wäre eine
    #         Flags-/Masks-Kombination um auch Gruppen von APCIs
    #         (==> Wildcard) filtern zu können.
    #   'timeout' ist in Millisekunden.
    #
    pass

def NM_IndividualAddress_Read():
    responses=[]
    A_IndividualAddress_Read_Req()
    # Wait 3 Seconds, collect all resonses ('A_IndividualAddress_Res') during this time.    
    return responses

def NM_IndividualAddress_Write():
    pass

def NM_IndividualAddress_SerialNumber_Read():
    pass

def NM_IndividualAddress_SerialNumber_Write():
    pass

def NM_DomainAddress_Read():
    pass

def NM_DomainAndIndividualAddress_Write():
    pass

def NM_DomainAddress_Scan():
    pass

def NM_Router_Scan():
    pass

def NM_SubnetworkDevices_Scan():
    pass

def NM_SubnetworkAddress_Read():
    pass

def NM_IndividualAddress_Reset():
    pass

def NM_IndividualAddress_Scan():
    pass

def test():
    NM_IndividualAddress_Read()

if __name__=="__main__":
    test()
    