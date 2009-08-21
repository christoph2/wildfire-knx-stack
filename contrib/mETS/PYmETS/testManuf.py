import MySQLdb


def test():
    result=[]
    dict={}
    db=MySQLdb.Connect(host="localhost",user="root",db="ets_2_12")
    cursor=db.cursor()
    cursor.execute("select * from manufacturer")
    result=cursor.fetchall()

    for row in result:
        dict[row[0]]=(row[1:])
        print row

#    print dict.keys()
    print dict.get(105)    
    
if __name__=="__main__":
    test()
