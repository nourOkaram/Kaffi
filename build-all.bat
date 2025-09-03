@ECHO OFF
REM Build Everything

ECHO Building everything...

SET assemblies=engine testbed
SETLOCAL ENABLEDELAYEDEXPANSION

FOR %%F IN (%assemblies%) DO (
    PUSHD %%F
    CALL build.bat
    POPD

    IF !ERRORLEVEL! NEQ 0 (
        ECHO [ERROR]: !ERRORLEVEL!
        EXIT /B !ERRORLEVEL!
    )
)

ECHO ALL assemblies built successfully
