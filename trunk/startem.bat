rem temporary bootstrap script
rem check http://bugtraq.ireon.org/ticket/49

@echo off

echo rs
start /Direon_rs\bin /MIN ireon_rs-vs80-md-d.exe
echo .
ping 127.0.0.1 -n 2 -w 1000 > nul
echo ws
start /Direon_ws\bin /MIN ireon_ws-vs80-md-d.exe
echo .
ping 127.0.0.1 -n 2 -w 1000 > nul
echo client
start /Direon_client\bin ireon_client-vs80-md-d.exe
echo .
