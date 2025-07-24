

# Introduction

Todour is a simple and powerful task manager application that uses the [todo.txt](https://github.com/todotxt/todo.txt) file format (with some variations).
This means that all the tasks are in a file, and this file is stored where you want it to be. 
With Todour, you can easily organize and prioritize your tasks without being locked into a tool or sharing any data. 

This manual will guide you through the basics of using Todour, including how to add and edit tasks, set recurrence, and use the different features of the application. With Todour, you'll be able to increase your productivity and achieve your goals in no time.

The application always operates on the todo.txt file in every operation. There is no cache and changes are directly reflected in the todo.txt file. 

## Key Features

- **todo.txt Format**: Full compatibility with the todo.txt standard
- **Advanced Date Handling**: Support for relative dates (`due:+3d`, `t:+1w`)
- **Recurrence**: Automatic task recreation with `rec:` tags
- **Threshold Dates**: Hide tasks until a specific date with `t:` tags
- **Priority Management**: Standard A-Z priority support
- **Project & Context**: Full `+project` and `@context` support
- **System Tray**: Run minimized to system tray
- **Global Hotkeys**: Quick access from anywhere
- **Undo/Redo**: Full operation history
- **Live File Watching**: Automatic refresh when file changes
- **Cross-Platform**: Windows, Linux, and macOS support

## Installation

### Windows
1. Download the latest release from [nerdur.com](https://nerdur.com/todour)
2. Extract the archive to your preferred location
3. Run `Todour.exe`
4. (Optional) Create a desktop shortcut for easy access

### Linux
#### Ubuntu/Debian

Either

1. Download the latest release from [nerdur.com](https://nerdur.com/todour) (either tar.gz or the deb file)
2. Install whatever you chose to download. 

Or build it yourself

```bash
# Install dependencies
sudo apt install qtchooser qt5-default qtbase5-private-dev cmake libxcb-keysyms1-dev

# Download and build
git clone https://github.com/SverrirValgeirsson/Todour.git
cd Todour
git submodule init && git submodule update
qmake Todour.pro
make

# Run the application
./Todour
```

#### Other Distributions
Install the Qt5 development packages for your distribution, then follow the build steps above.

### macOS
1. Download the latest macOS release from [nerdur.com](https://nerdur.com/todour)
2. Mount the DMG file
3. Drag Todour to your Applications folder
4. Launch from Applications or Launchpad

Alternatively, build from source using the same steps as Linux (requires Qt5 development tools).

## Getting Started

### First Launch
1. **Choose Your todo.txt directory**: On first launch, Todour will be empty. Go to settings and set it up. Most important is to point to where it should store (or find your current) todo.txt file
2. **Location Matters**: Choose a location that's easy to sync (Dropbox, Google Drive, etc.) for cross-device access
3. **Start Adding Tasks**: Begin with simple tasks to get familiar with the interface

### Basic Workflow
1. **Add a task**: Type your task and press Enter
2. **Set priorities**: Use `(A)`, `(B)`, `(C)` at the beginning of tasks
3. **Add context**: Use `@context` tags (e.g., `@home`, `@work`)
4. **Add projects**: Use `+project` tags (e.g., `+website`, `+vacation`)
5. **Set due dates**: Use `due:2024-12-31` or `due:+3d` for relative dates
6. **Mark complete**: Check the box 

### Example Tasks
```
(A) Call dentist for appointment @phone
Buy groceries @errands +personal
(B) Review quarterly reports due:2024-03-15 @work +quarterly-review
Plan vacation activities +vacation t:+30d
Daily standup meeting rec:1d due:2024-03-16 @work +daily
```

## Quick Tips

- **Global Hotkey**: Press `Ctrl+Alt+T` to quickly add a task from anywhere (needs to be enabled in settings)
- **Filtering**: Use the search box to filter tasks by text, project, or context
- **System Tray**: Enable tray icon to keep Todour running in background
- **Settings!**: There are quite a few settings. Tinker with them and find the best combination for you