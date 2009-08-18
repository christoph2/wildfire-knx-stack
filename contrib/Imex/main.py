import os
import os.path
import win32ui

TEMP="\\tmp"
UNZIP="\\unzip.exe"

def UnzipTo(filename,path):
    unzip=cwd+UNZIP
    print unzip,filename,path
    os.system("%s -POrleander %s -d %s" % (unzip,filename,path))    

def ZipFrom(path,filename):
    pass

def Remove(path):
    os.system("del /S/Q/F %s\\*.*>nul" % (path))
    os.system("rmdir /S/Q %s>nul" % (path))

def ChooseVDFile():
    fd=win32ui.CreateFileDialog(True,None,"*.vdx;*.vd1;*.vd2;*.vd3",0,"*.vdx;*.vd1;*.vd2;*.vd3||",None)
    #fd=win32ui.CreateFileDialog(True,None,"*.vdx;*.vd1;*.vd2;*.vd3",0,"\"(VDX-Dateien)\",\"*.vdx\"\"||\"",None)
    if fd.DoModal()==1:
        return fd.GetPathName()
    else:
        return None

if __name__=="__main__":
    global cwd,tmppath
    cwd=os.getcwd()
    tmppath=cwd+TEMP

    file=ChooseVDFile()
    print file
    if file!=None:    
        Remove(tmppath)
        UnzipTo(file,tmppath)
