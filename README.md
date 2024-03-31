# Overview
Docker image to build the Tribes source using the original build chain.

Dev tools included in the image:
- Borland C++ compiler
- Turbo Assembler (TASM)
- Opus Make
- TODO: Visual Studio (only used for the source of some tools)

# Building the image
Requirements:
- Docker Desktop on Windows [configured to use Windows containers](https://learn.microsoft.com/en-us/virtualization/windowscontainers/quick-start/set-up-environment?tabs=dockerce#windows-10-and-11-1)
```
cd tribes-build-image
docker build -t tribes-build-image .
```

# Using the image
Once the image is built, you can use it as follows:
```
docker run --rm -ti -v C:\host\path\to\code:C:\code tribes-build-image cmd.exe
```
This will start the container with an interactive command prompt, mounting the folder `C:\host\path\to\code` to `C:\code` in the container.