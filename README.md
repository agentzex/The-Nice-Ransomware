# The-Nice-Ransomware
Please use this repo for educational purposes only!

This is a PoC made to mimic a ransomware running on Windows platforms, communicating with a python CnC server which will generate a unique AES key to encrypt a system files, and save this key with a unique id to later decryption (keys will be found in a file called "victims.json" on the server directory).

* The agent will search the entire path given (including subdirectories) and will encrypt each file that his extensions is included in the "file_types" vector variable.
* The project needs to be compiled with CPP 17 on Visual Studio
* Please use this code for good purposes only! I will not hold accountable for any misuse of this project
* Please run this code on safe environments only (preferably on a VM with a snapshot you can revert back to)
