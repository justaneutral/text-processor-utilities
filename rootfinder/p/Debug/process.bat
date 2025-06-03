REM Instructions: 
REM Program p.exe
REM In input file (parameter # 1)
REM finds dictionary entries contained in dictionary file (parameter # 3)
REM and places them to output file (parameter # 2)
REM Starting format:
REM p.exe p1 p2 p3 [p4] [p5]
REM where: 
REM p1 = <input text file name>
REM p2 = <output text file name>
REM p3 = <dictionary text file name>
REM p4 = <optional output log flag>
REM p5 = <optional verbose output log flag>
REM To stop - "<CTRL>" & "C"
p.exe if.txt of.txt rot.txt
REM p.exe if.txt of.txt rot.txt f
REM p.exe if.txt of.txt rot.txt f v
REM p.exe if.txt of.txt rot.txt f > logf.txt
REM p.exe if.txt of.txt rot.txt f v > logfv.txt


