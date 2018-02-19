# Todour
The todo.txt application Todour

You can always download the latest release on [nerdur.com](https://nerdur.com/todour). Or just build your own. 

The program uses QtAwesome for the icons. 


## Dependency to nerdur.com 
Please note that there is a small part of the code that checks on nerdur.com if there is an update available. This should not be an issue, 
but if you are making your own copy you may want to disable this. 
This is done in aboutbox.cpp, and can be turned off by simply commenting out the line in the constructor that says "requestPage.."

