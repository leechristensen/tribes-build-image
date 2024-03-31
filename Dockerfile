FROM mcr.microsoft.com/windows/servercore:ltsc2022

COPY ./BC5/ /BC5/
COPY ./OPUS/ /OPUS/
COPY ./TASM/ /TASM/

# Note that I had to set the path manually because Opus Make fails for some reason due to it being too long
RUN powershell.exe -C \
    [Environment]::SetEnvironmentVariable('PATH', 'C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Users\ContainerAdministrator\AppData\Local\Microsoft\WindowsApps;C:\OPUS\;C:\BC5\BIN\;C:\TASM\BIN\;', [EnvironmentVariableTarget]::Machine);\
    [Environment]::SetEnvironmentVariable('COMPILER', 'b', [EnvironmentVariableTarget]::Machine);\
    [Environment]::SetEnvironmentVariable('TASM', 'C:\TASM\BIN\', [EnvironmentVariableTarget]::Machine)

WORKDIR /code/