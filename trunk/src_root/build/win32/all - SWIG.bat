@echo off
set include_path=..\..\include\
set src_dest_path=..\..\src\common\python_modules\
set inc_dest_path=..\..\include\common\python_modules\
set py_dest_dir=..\..\..\ireon_client\data\interface\scripts\

set src_client_dest_path=..\..\src\ireon_client\python_modules\
set inc_client_dest_path=..\..\include\ireon_client\python_modules\

set ogre_include_path=..\..\include\extlib\Ogre\main\

@rem Client
rem 
@echo ============== APP =================
tools\swig.exe %1 -debug_typemap -dump_module -dump_classes -dump_top -dump_typedef -Wall  -outdir %py_dest_dir% -c++ -python -I%include_path% -I%inc_client_dest_path% -I%ogre_include_path% -o %src_client_dest_path%py_ireon_client.cpp -oh %inc_client_dest_path%py_ireon_client.h %src_client_dest_path%ireon_client.i > out.txt


pause

