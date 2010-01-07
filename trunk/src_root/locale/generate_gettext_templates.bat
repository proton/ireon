@echo off

echo generating pot files...

set PACKAGE=common
call do_pot_generation.bat

set PACKAGE=ireon_rs
call do_pot_generation.bat

set PACKAGE=ireon_ws
call do_pot_generation.bat

echo done.
