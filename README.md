# The-Nice-Ransomware
Please use this repo for educational purposes only!

This is a PoC made to mimic a ransomware running on Windows platforms, communicating with a python CnC server which will generate a unique AES key to encrypt files with chosen file extensions, and save this key with a unique id for later decryption (keys will be found in a file called "victims.json" on the server's directory).

Notes:
* The agent will search the entire path given (including subdirectories) and will encrypt each file that his extensions is included in the "file_types" vector variable.
* The executables will search for the server on the same machine TheNiceRansomware is running, via loopback IP address, so make sure the server is running before executing them
* The project needs to be compiled with CPP 17 on Visual Studio
* Python libs to install before execution: Flask & pycrypto
* Tested on Windows 10-64bit and Windows 7-32bit
* Please use this code for good purposes only! I will not hold accountable for any misuse of this project
* Please run this code on safe environments only (preferably on a VM with a snapshot you can revert back to)

For more technical details about the development process and a video demo - https://whiterosezex.blogspot.com/2018/12/the-nice-ransomware.html
