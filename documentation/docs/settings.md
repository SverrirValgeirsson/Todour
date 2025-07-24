# Settings

Access settings through **File → Settings**.

## General Settings

- **Directory**: Set the default location for todo.txt files
- **Inactive**: A semicolon separated list of keywords you want to use for marking something inactive
- **Active Font**: How to display active items
- **Inactive Font**: How to display inactive items
- **Enable due:**: Enabling recognizing the `due:` keyword and when to color lines with warning color as well as how to color warning and late items
- **Font size**: The default font size to use
- **Refresh automatically when files has changed**: Tries to monitor changes on disk and refresh
- **Show dates**: If this is not selected, creation dates are not shown (they can still exist)
- **Add dates to lines**: Automatically add todays date to new item created
- **Treat threshold as inactive**: Treat items that haven't reached their threshold the same as inactive ones
- **Sort all inactive last**: All inactive items last in the list (else they are shown in default sorting)
- **Live search**: Search the list directly as you type. This is generally what you want. _HOWEVER_ if you select **Window → Show all** in the menus the todo.txt and done.txt will both be shown, allowing you to do searches over both new and archived items. For a large done.txt it may be a good idea to turn off live search first.
- **Ctrl-Alt-t as Todo hotkey**: As it says
- **Track delete**: Put deleted items into deleted.txt
- **Respect threshold dates**: Enable the `t:` keyword
- **Respect threshold contexts and projects**: Allow the `t:` keyword to not only have dates as keys, but also context or projects
- **Treat due as threshold**: Sometimes the deadline and the threshold are the same thing
- **Enable tray**: As it says
- **Check for updates**: Checks ca once per week if there is a new version of Todour and notifies you if there is
- **Remove duplicate lines**: If two lines are identical, remove one (this is from a onenote integration problem that happened a few years ago)
- **Default deadline/threshold to add when missing**: When executing a `rec:` there is need for a `t:` or a `due:` to update. If there is only `rec:` in the item the application needs to add either a `t:` or `due:`
- **Symbol for NOT in search**: The negation symbol
- **Business days**: What days of the week to be considered as business days. When for example having a `rec:+3b` this helps the application to understand what days to count.
- **Priorities at checked**: When an item is checked/finished the priority can be kept (moved to after the dates), changed to `pri:X` or removed from the item.  


### Settings File Location
Settings are stored in platform-specific locations:
- **Windows**: `%APPDATA%/Todour/` or registry
- **Linux**: `~/.config/Todour/` or `~/.todour/`
- **macOS**: `~/Library/Preferences/` or `~/Library/Application Support/Todour/`
