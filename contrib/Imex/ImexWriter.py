#
#   Konnex-Tool-Chain.
#
#   EIBA EX-IM-Fileformat-/XML-Converter.
#
#   Copyright (C) 2006 Christoph Schueler.
#           christoph_schueler@web.de
#

import sys
import xml.parsers.expat

###
### Commons.
###

SIGNATURE="EX-IM"
EOF_SIG="XXX\n"    #EOF_SIG="XXX\r\n"
SEPERATOR="-------------------------------------"
CONTINUATION="\\"

BUF_SIZE=4096

tokInvalid=0        # Token-Defs.
tokFilename=1
tokComment=2
tokDate=3
tokVersion=4
tokHeader=5
tokTable=6
tokColumn=7
tokRow=8

###
###
###


ETE_IMEX="ete_imex"
HEADER="header"
TABLES="tables"
FILE_NAME="filename"
COMMENT="comment"
DATETIME="timestamp"
VERSION="version"
ROOT="root"
DESCRIPTION="description"
ROWS="rows"
TABLE="table"
COLUMN="c"

secInvalid=0    # Section-States.
secHeader=1
secTables=2

sec2Invalid=0
secDescription=1
secRow=2

class ImexWriter:
	uDepth=0
	section1=secInvalid
	section2=sec2Invalid
	uNumChars=0
	uCharsWritten=0
	uTableID=0
	uColumnIdx=0
	TableName=""
	ColumnName=""
	column=[]
	bInContext=False
	token=tokInvalid
	inf=None
	outf=None
	p=None	

	def PrintToken(self,tok):
		str="%c " % (tok)
		self.outf.write(str.encode("utf-8"))

	def PrintNewLine(self):		
		self.outf.write("\n".encode("utf-8"))

	def PrintSeperator(self):
		self.outf.write(("%s\n" % (SEPERATOR)).encode("utf-8"))

	def GetSectionLevel1(self,sec):
		if sec==HEADER:
			return secHeader
		elif sec==TABLES:
			return secTables
		else:
			return secInvalid

	def GetSectionLevel2(self,sec):
		if sec==DESCRIPTION:
			return secDescription
		elif sec==ROWS:
			return secRow
		else:
			return sec2Invalid

	def MapHeaderTag(self,tag):
		tok=0
		if tag==FILE_NAME:
			tok=tokFilename
		elif tag==COMMENT:
			tok=tokComment
		elif tag==DATETIME:
			tok=tokDate
		elif tag==VERSION:
			tok=tokVersion
		elif tag==ROOT:
			tok=tokHeader
		else:
			tok=tokInvalid
		return tok
		

	def start(self,name, attrs):
		self.bInContext=True
		self.token=tokInvalid
		name=name.lower()
		if self.uDepth==0:
			if name!=ETE_IMEX:
				print "unexpected token \'%s\'" % (name)
				sys.exit(1)
			else:
				self.outf.write((SIGNATURE+"\n").encode("utf-8"))
		elif self.uDepth==1:
			self.section1=self.GetSectionLevel1(name)
			if self.section1==secInvalid:
				print "unexpected token \'%s\'" % (name)
				sys.exit(1)
		elif self.uDepth==2:
			if self.section1==secHeader:
				self.token=self.MapHeaderTag(name)
				if (self.token==tokInvalid):
					print "unexpected token \'%s\'" % (name)
					sys.exit(1)
				else:
					if self.token==tokFilename:
						self.PrintToken('N')
					elif self.token==tokComment:
						self.PrintToken('K')
					elif self.token==tokDate:
						self.PrintToken('D')
					elif self.token==tokVersion:
						self.PrintToken('V')
					elif self.token==tokHeader:
						self.PrintToken('H')
			elif self.section1==secTables:
				if (not attrs.has_key("id")) or (not attrs.has_key("name")):
					print "wrong attributes in table-tag"
					sys.exit(1)
				self.column=[]
				self.uColumnIdx=0
				self.uTableID=int(attrs["id"])
				self.TableName=attrs["name"]
				self.PrintSeperator()
				self.outf.write(("T %u %s\n" % (self.uTableID,self.TableName)).encode("utf-8") )
			else:
				print "unexpected token \'%s\'" % (name)
				sys.exit(1)
		elif self.uDepth==3:
			self.section2=self.GetSectionLevel2(name)
			if self.section2==sec2Invalid:
				print "unexpected token \'%s\'" % (name)
				sys.exit(1)				
		elif self.uDepth==4:
			if (self.section2==secDescription):
				if (not attrs.has_key("name")) or (not attrs.has_key("id")) \
				or (not attrs.has_key("type")) or (not attrs.has_key("len")) \
				or (not attrs.has_key("null")):
					print "wrong attributes in column-description"
					sys.exit(1)
				else:
					self.outf.write(("C%s T%u %s %s %s %s\n" % (attrs["id"], \
						self.uTableID,attrs["type"],attrs["len"],attrs["null"],attrs["name"])).encode("utf-8"))
					self.column.append(attrs["type"])
			elif (self.section2==secRow):
				self.outf.write(("R %s T %u %s\n" % (attrs["id"],self.uTableID,self.TableName)).encode("utf-8"))
				self.uColumnIdx=0
		elif self.uDepth==5:
			if name!=COLUMN:
				print "unexpected token \'%s\'" % (name)
				sys.exit(1)
			else:
				self.uNumChars=0
				self.uCharsWritten=0
				self.uColumnIdx=self.uColumnIdx+1
		self.uDepth=self.uDepth+1


	def end(self,name):
		self.bInContext=False
		self.token=tokInvalid
		self.uDepth=self.uDepth-1

		if self.uDepth==0:
			self.outf.write(EOF_SIG.encode("utf-8"))
		elif (self.uDepth==2):
			if (self.section1==secHeader):
				self.PrintNewLine()
		elif (self.uDepth==3):
#			if (self.section2==secDescription):
#				self.tables[self.TableName]=self.columns
			section2=sec2Invalid
		elif (self.uDepth==5):
			self.uNumChars=0
			self.uCharsWritten=0
			self.PrintNewLine()			

#        
#		if ((strncmp(szTableName,"catalog_entry",LINE_LEN)==0) && (uNumChars==0)) {
#//			if (uNumChars==0) {
#//				fprintf(out,"<NULL>");	

	def char_data(self,data):
		data=data.encode("utf-8")
		
		if (self.bInContext==True):
			if ((self.section1==secHeader) and (self.token!=tokInvalid)):				
				self.outf.write(data.encode("utf-8"))

		if (self.uDepth==6):
			type=int(self.column[self.uColumnIdx-1])
			
			for c in data:
				c=ord(c)
				c=chr(c)
#				self.outf.write(c.encode("utf-8"))				
#				self.outf.write(c)
				self.uNumChars=self.uNumChars+1

				if 	((self.uNumChars % 40)==0) and (type==3):
					self.outf.write("\n\\\\".encode("utf-8"))
					pass
				elif ((self.uNumChars % 80)==0) and (type==8):
					self.outf.write("\n\\\\".encode("utf-8"))
					pass

	def Write(self):
		pass

	def __init__(self):
		self.inf=open("c:\projekte\ETE_import\ete_imex\ete.xml","r")
		self.outf=open("c:\projekte\ETE_import\ete_imex\ets2.vdx","w")

		p=xml.parsers.expat.ParserCreate()
		p.StartElementHandler=self.start
		p.EndElementHandler=self.end
		p.CharacterDataHandler=self.char_data

		p.ParseFile(self.inf)

		self.outf.close()
		self.inf.close()		
		

if __name__=="__main__":
	writer=ImexWriter()
#    ETE_XMLToIMEX()
