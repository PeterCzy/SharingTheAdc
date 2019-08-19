@echo off
REM This script runs by  MDK, so is in <Project>
set PATH=%PATH;..\..\Tools\GetRegions
@echo on

GetRegions.exe %1 -z -oObjects\function_sizes.txt
REM sort /R ..\Objects\function_sizes.txt /O Objects\sorted_function_sizes.txt
