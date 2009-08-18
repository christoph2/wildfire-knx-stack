#
#   Konnex-Tool-Chain.
#
#   EIBA EX-IM-Fileformat/XML Converter.
#
#   Copyright (C) 2006 Christoph Schueler.
#           christoph_schueler@web.de
#

import sys

XML_VERSION="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n" 

SIGNATURE="EX-IM"
EOF_SIG="XXX"    #EOF_SIG="XXX\r\n"
SEPERATOR="-------------------------------------"
CONTINUATION="\\"
LINE_LEN=1024

#
#   Header-Token
#
FILE_NAME=	'N'	# Datei-Name.
COMMENT=	'K'	# Kommentar.
DATE=		'D'	# Datum.
VERSION=	'V'	# Version.
HEADER=		'H'	# Header.

#
#	Token in der Tabellen-Beschreibung.
#
TABLE=		'T'	# Tabellen-Name	(nummer - name).
COLUMN=		'C'	# Tabellen-Spalte	(nummer - table - type -length - null? -name).
ROW=		'R'	# Tabellen-Zeile	().

_HEADER=0           # Section-Defs.
_TABLES=1

_NONE=0             # State-Defs.
_TABLE_DESC_START=1
_TABLE_DESC_END=2
_TABLE_DESC_CONT=3
_TABLE_ROW_DESC=4
_TABLE_DATA=5

tokInvalid=0        # Token-Defs.
tokFilename=1
tokComment=2
tokDate=3
tokVersion=4
tokHeader=5
tokTable=6
tokColumn=7
tokRow=8

TokenMapping={FILE_NAME:tokFilename,COMMENT:tokComment,DATE:tokDate,
              VERSION:tokVersion,HEADER:tokHeader,TABLE:tokTable,
              COLUMN:tokColumn,ROW:tokRow}

TRANSCODE_TAB=(
	['&',"&#x26;"],
	['ä',"&#xe4;"],
	['ö',"&#xf6;"],	
	['ü',"&#xfc;"],	
	['Ä',"&#xc4;"],	
	['Ö',"&#xd6;"],	
	['Ü',"&#xdc;"],	
	['ß',"&#xdf;"],
	['<',"&#x3c;"],
	['>',"&#x3e;"],		
	['°',"&#xb0;"],
	['\"',"&#x22;"],
#	'\'':"&apos;"},	
	['ˆ',"&#x2c6;"],
	['˜',"&#x2dc;"],
	['€',"&#x20ac;"],
	['©',"&#xa9;"],
	['®',"&#xae;"],
	['±',"&#xb1;"],
	['²',"&#xb2;"],
	['³',"&#xb3;"],
	['µ',"&#xb5;"],
    ['„',"&#x84;"]
)

def TranscodeToXML(line):
#    line=line.encode("utf-8")
#   line=line.__repr__()
    for p in TRANSCODE_TAB:
        line=line.replace(p[0],p[1])
    return line

def ExtractLine(line):
    line=line[2:].rstrip()
    return line

def StripContinuation(line):
    if (line[:1]=="\\"):
        line=line[2:]
    return line

def IsSeperator(line):
    if line.startswith(SEPERATOR):
        return True
    else:
        return False

def GetToken(l):
    if TokenMapping.has_key(l):
        return TokenMapping[l]
    else:
        return tokInvalid

def ETE_IMEXToXML():
    bReachedEOF=False
    section=_HEADER
    state=_NONE
    inf=None
    outf=None
    uLine=0
    uColumnCount=0
    bThisContinued=False
    bNextContinued=False
    fEOF=False
    uRowID=0
    uColumnID=0
    ColumnName=""
    uColumnType=0
    uColumnLen=0
    chNulls=''
    uRowCount=0
    uTableID=0
    uTmp=0
    uCount=0
    TableName=""
    Advance=""
    szTmp=""
    Tables={}
    column={}
    inf=open("C:\projekte\ETE_Import\ete_imex\ets2.vd_","rb")
    outf=open("C:\projekte\ETE_Import\ete_imex\ete.xml","w")

    print

    filetype="vd2"  # z.Z. dummy.

    if not (inf.readline(LINE_LEN).startswith(SIGNATURE)):
        print "*** Invalid Signature ***"
        exit(1)

    outf.write(XML_VERSION)
    outf.write("<ete_imex type=\"%s\">\n" % filetype)
    outf.write("<header>\n")
    
    while not fEOF:
        line=inf.readline(LINE_LEN)

        if line=="":
            fEOF=True
            print "*** EOF ***"
            break
        uLine=uLine+1;

        if (section==_HEADER):              # Header parsen.
            if (IsSeperator(line)):
                section=_TABLES         # Header zu Ende.
                state=_TABLE_DESC_START
                outf.write("</header>\n")
                outf.write("<tables>\n")
            else:
                if line[1]!=' ':        # einem Token muss ein Leerzeichen folgen.
                    print "*** Invalid Token-Format ***"
                    sys.exit(1)
                tok=GetToken(line[0])
                line=ExtractLine(line)
                if tok==tokInvalid:
                    print "*** Invalid Token ***"
                    sys.exit(1)
                elif tok==tokFilename:
                    outf.write("\t<filename>%s</filename>\n" % line)
                elif tok==tokComment:
                    outf.write("\t<comment>%s</comment>\n" % line)
                elif tok==tokDate:      # Todo: XML-kodiertes Datum.
                    outf.write("\t<timestamp>%s</timestamp>\n" % line)
                elif tok==tokVersion:
                    outf.write("\t<version>%s</version>\n" % line)
                elif tok==tokHeader:
                    outf.write("\t<root>%s</root>\n" % line)
                else:
                    print "*** Invalid Token ***"
                    sys.exit(1)                                    
        elif (section==_TABLES):
            if state==_TABLE_DESC_START:
                if line[1]!=' ':        # einem Token muss ein Leerzeichen folgen.
                    print "*** Invalid Token-Format ***"
                    sys.exit(1)                
                tok=GetToken(line[0])
                if (tok==tokTable):
#
#	Tabellen-Definition verarbeiten.
#                    
                    line=ExtractLine(line)
                    tab_desc=line.split(" ")                    
                    if len(tab_desc)==2:
                        uTableID=int(tab_desc[0])
                        TableName=tab_desc[1]
                        outf.write("\t<table id=\"%u\" name=\"%s\">\n"
                                   % (uTableID,TableName))

                        outf.write("\t\t<description>\n")
                        state=_TABLE_DESC_CONT
                        uColumnCount=0
                        uRowCount=0
                    else:
                        print "*** wrong number of Table-Attributes ***"
                        sys.exit(1)
                else:
                    print("\n*** Token <>TABLE ***\n")
                    sys.exit(1)

            elif state==_TABLE_DESC_CONT:
                tok=GetToken(line[0])
                if tok==tokColumn:
                    col_desc=line.split(" ")
                    if len(col_desc)!=6:
                        print "*** wrong number of attributes in column-description !!!"
                        sys.exit(1)
                    else:                        
                        uColumnID=int(col_desc[0][1:])                        
                        uTmp=int(col_desc[1][1:])

                        if uTmp!=uTableID:
                            print "*** wrong Table-ID in column-description ***"
                            sys.exit(1)
                        
                        uColumnType=int(col_desc[2])
                        uColumnLen=int(col_desc[3])
                        chNulls=col_desc[4][0]
                        ColumnName=col_desc[5].rstrip()                        
                        column[ColumnName]=(uColumnID,uColumnType,
                                            uColumnLen,chNulls)

                        outf.write("\t\t\t<column id=\"%u\" name=\"%s\" type=\"%u\" " \
                                   "len=\"%u\" null=\"%s\" />\n" % (uColumnID,ColumnName,uColumnType,uColumnLen,chNulls))
                        uColumnCount=uColumnCount+1
                elif tok==tokRow:
#
# Row-Definition.
#
                    if line[1]!=' ':
                        print "*** invalid token-format ***"
                        sys.exit(1)
                        
                    row_desc=line.split(" ")                    
                    if len(row_desc)!=5:
                        print "*** invalid number of attributes in Row-Description ***"
                        sys.exit(1)
                    
                    uRowID=int(row_desc[1])
                    uTmp=int(row_desc[3])
                    szTmp=row_desc[4].rstrip()

                    if (uTmp!=uTableID) or (szTmp!=TableName):
                        print "*** invalid row-description ***"
                        sys.exit(1)
                    state=_TABLE_DATA

                    outf.write("\t\t</description>\n")
                    outf.write("\t\t<rows>\n")
                    outf.write("\t\t\t<row id=\"%u\">\n" % (uRowID))
                    bThisContinued=False
                    bNextContinued=False
                    uCount=0
                    uRowCount=1                    
                    Tables[TableName]=column
                    del column
                    column={}
                elif IsSeperator(line): # Tabellen-Definition ohne Daten.
                    outf.write("\t\t</description>\n")
                    outf.write("\t</table>\n")
                    state=_TABLE_DESC_START                    
                else:
                    print "*** invalid Token ***"
                    sys.exit(1)
            elif state==_TABLE_ROW_DESC:
                if line[:3]==EOF_SIG:
                    break;

                if (IsSeperator(line)):                    
                    uRowCount=0
                    outf.write("\t\t</rows>\n")
                    outf.write("\t</table>\n")
                    state=_TABLE_DESC_START
                else:
                    row_desc=line.split(" ")                    

                    if len(row_desc)!=5:
                        print "*** invalid number of attributes in Row-Description ***"
                        print row_desc
                        sys.exit(1)
                    
                    uRowID=int(row_desc[1])
                    uTmp=int(row_desc[3])
                    szTmp=row_desc[4].rstrip()

                    if (uTmp!=uTableID) or (szTmp!=TableName):
                        print "*** invalid row-description ***"
                        sys.exit(1)                    

                    state=_TABLE_DATA
                    bThisContinued=False
                    bNextContinued=False
                    outf.write("\t\t\t<row id=\"%u\">\n" % (uRowID))
                    uCount=0
                    uRowCount=uRowCount+1                
            elif state==_TABLE_DATA:                
                if IsSeperator(line):
                    outf.write("\t\t</rows>\n")
                    outf.write("\t</table>\n")
                    state=_TABLE_DESC_START
                else:
#
#   Tabellen-Daten verarbeiten.
#

                    line=line.rstrip()
                    line=TranscodeToXML(line)   # todo: Nur bei Strings!!!

                    Advance=inf.read(2) 
                    if ((Advance==None) or (Advance=="")):
                        print "*** Reached EOF ***"
                        bReachedEOF=True
                    else:
                        if ((Advance[0]=='\\') and (Advance[1]=='\\')):                            
                            bNextContinued=True
                        else:
                            bNextContinued=False                        
                    inf.seek(-2,1)
                    					
                    if (bThisContinued==True):
                        if (bNextContinued==True):  # JJ
                            line=StripContinuation(line)
#                            outf.write("%s" % (line))
                            outf.write(line)
                        else:                       # JN
                            uCount=uCount+1
                            line=StripContinuation(line)
                            outf.write("%s</c>\n" % (line))
                    else:
                        if (bNextContinued==True):  # NJ                            
                            outf.write("\t\t\t\t<c id=\"%u\">%s" % (uCount,line))
                        else:                       # NN
                            uCount=uCount+1                            
                            outf.write("\t\t\t\t<c id=\"%u\">%s</c>\n" % (uCount,line))

                    bThisContinued=bNextContinued

                    if ((uCount % uColumnCount)==0):
                        uCount=0
                        outf.write("\t\t\t</row>\n")
                        state=_TABLE_ROW_DESC

#
#   OK, Fertig.
#
    outf.write("\t\t</rows>\n")
    outf.write("\t</table>\n")
    outf.write("</tables>\n")
    outf.write("</ete_imex>")

    outf.close()
    inf.close()    

#    print("*** FERTIG ***")

if __name__=="__main__":
    ETE_IMEXToXML()
    