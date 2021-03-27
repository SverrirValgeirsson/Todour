# Todour
The todo.txt application Todour

You can always download the latest release on [nerdur.com](https://nerdur.com/todour). Or just build your own. 

The program uses QtAwesome for the icons. 

# Downloading
```
git clone https://github.com/SverrirValgeirsson/Todour.git
cd Todour
git submodule init 
git submodule update 
```

# Building
```
qmake Todour.pro
make
```
## Dependencies 
On Ubuntu 20.04 the following has been reported to be needed (see https://github.com/SverrirValgeirsson/Todour/issues/28)
```
sudo apt install qtchooser
sudo apt install qt5-default
sudo apt install qtbase5-private-dev
sudo apt install cmake
sudo apt install libxcb-keysyms1-dev
sudo apt install clang
``` 
 
## Dependency to nerdur.com 
Please note that there is a small part of the code that checks on nerdur.com if there is an update available. This should not be an issue, 
but if you are making your own copy you may want to disable this. 
This is done in aboutbox.cpp, and can be turned off by simply commenting out the line in the constructor that says "requestPage.."

