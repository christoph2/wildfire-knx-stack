import httplib
from xml.dom.minidom import parseString

DEBUG=0

NODE_NAMES={
1  : "ELEMENT_NODE",
2  : "ATTRIBUTE_NODE",
3  : "TEXT_NODE",
4  : "CDATA_SECTION_NODE",
5  : "ENTITY_REFERENCE_NODE",
6  : "ENTITY_NODE",
7  : "PROCESSING_INSTRUCTION_NODE",
8  : "COMMENT_NODE",
9  : "DOCUMENT_NODE",
10 : "DOCUMENT_TYPE_NODE",
11 : "DOCUMENT_FRAGMENT_NODE",
12 : "NOTATION_NODE"
}    



def RequestXML(host,path):
    global DEBUG
    data=None
    if host=="":
        host="localhost:80"

    conn=httplib.HTTPConnection(host)
    conn.request("GET", "/"+path)
    conn.set_debuglevel(DEBUG)
    resp=conn.getresponse()
    status=resp.status
    reason=resp.reason

    if status==200:
        data=resp.read()
    else:
        print "***",status,reason,"***"
    conn.close()
    return data

depth=0
    
def PrintChildNodes(node):
    global depth

    depth=depth+1    
    for n in node.childNodes:
        print '\t'*depth,n  #,NODE_NAMES[n.nodeType],n.nodeName
        if n.hasChildNodes():
            PrintChildNodes(n)
    depth=depth-1

if __name__=="__main__":    
    xmldata=RequestXML("","desc.xml") # "serv1.xml"  "serv2.xml" "serv3.xml"
    if xmldata!=None:
        dom=parseString(xmldata)
        root=dom.documentElement
        root.normalize()
        PrintChildNodes(root)
        dom.unlink()
    
"""    
    h=httplib.HTTP()
    if DEBUG==1:
        debuglevel=1
    else:
        debuglevel=0
    h.set_debuglevel(debuglevel)
    h.connect(host)
    h.putrequest('GET',path)
    h.endheaders()
    status, reason, headers = h.getreply()

    if status==200:
#        if headers:
#            for header in headers.headers:
#                if DEBUG==1:
#                    print header.strip()
#                else:
#                    pass                
        data=h.getfile().read()
        if DEBUG==1:
            print data
    h.close()
"""