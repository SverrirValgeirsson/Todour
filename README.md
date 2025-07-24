# Todour
The todo.txt application Todour

You can always download the latest release on [nerdur.com](https://nerdur.com/todour). Or just build your own. 

The program uses QtAwesome for the icons. 

# Documentation
https://sverrirvalgeirsson.github.io/Todour/


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

# Documentation Generation
The project uses [MkDocs](https://www.mkdocs.org/) to generate static documentation from Markdown files. The documentation source is located in the `documentation/` directory and is automatically deployed to GitHub Pages.

## Documentation Structure
```
documentation/
├── mkdocs.yml          # MkDocs configuration file
└── docs/
    ├── index.md        # Homepage/Introduction
    ├── generic usage.md # General usage guide
    ├── commands.md     # Command reference
    ├── settings.md     # Settings documentation
    └── License.md      # License information
```

## Prerequisites
Install MkDocs using pip:
```bash
pip install mkdocs
```

For additional themes and plugins (optional):
```bash
pip install mkdocs-material
pip install mkdocs-awesome-pages-plugin
```

## Working with Documentation

### Local Development
```bash
cd documentation

# Start local development server
mkdocs serve

# The documentation will be available at: http://127.0.0.1:8000
# Changes to Markdown files will auto-reload in the browser
```

### Adding New Pages
1. Create a new `.md` file in the `docs/` directory
2. Add the page to the navigation in `mkdocs.yml`:
   ```yaml
   nav:
       - 'index.md'
       - 'new-page.md'  # Add your new page here
       - 'settings.md'
   ```
3. Preview your changes with `mkdocs serve`

### Deployment to GitHub Pages
Deploy the documentation to the live site:
```bash
cd documentation
mkdocs gh-deploy
```

This command will:
- Build the documentation into static HTML
- Push the generated files to the `gh-pages` branch
- Update the live documentation automatically

### Manual Build (Alternative)
If you need to build without deploying:
```bash
cd documentation
mkdocs build
# Generated files will be in the site/ directory
```

## Live Documentation
The documentation is automatically published to: https://sverrirvalgeirsson.github.io/Todour/

## Troubleshooting
- **Permission errors**: Ensure you have write access to the repository
- **Build failures**: Check that all Markdown files have valid syntax
- **Missing dependencies**: Install MkDocs with `pip install mkdocs`
- **Navigation issues**: Verify that all pages listed in `mkdocs.yml` exist in the `docs/` directory

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

