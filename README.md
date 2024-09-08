# ESP32-CAM-Webserver 

* Headers are taken from 1. in chapter README.md

Arduino ESP32-camwebserver example setup in PlatformIO 

## Description

- Acquisition of the various analog meter values using the **ESP32-Cam with built-in USB board**, like: [ESP32-Cam with built-in USB board](https://mikroelectron.com/Product/ESP32-CAM-WITH-BUILT-IN-USB-BOARD/) 
- Digitization of the meter values recorded with ESP32-Cam using AI
- Display of digitized meter readings in the web browser

### README.md

README.md is based on
1. [A Simple README.md template](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)

2. [Basic writing and formatting syntax](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)

#### README.md. more detailed info for creating

3. [How to write a good README for your GitHub project?](https://bulldogjob.com/readme/how-to-write-a-good-readme-for-your-github-project)

4. [Best-README-Template](https://github.com/othneildrew/Best-README-Template)

## Getting Started

- Create new PlatformIO project **`esp32-cam-webserver`** on Local Development Computer (DevCP) with following files
        
    -   #### **`platform.io`**
        ```
        [env:esp32cam]
        platform = espressif32
        board = esp32cam
        framework = arduino
        ```
    - #### **`README.md`**
    - #### **`LICENSE.md`**
    - #### **`.gitignore`** taken from former PlatformIO projects

- Open Folder **`esp32-cam-webserver`** in **`git bash`**
    -  initialize GIT project on local PC
        - [renaming Local and Remote *master* Branch to *main*](https://www.git-tower.com/learn/git/faq/git-rename-master-to-main/) according to GitHub convention
- add primary remote GIT repository (in my case on private GIT server)
- add second GIT repository on GITHUB
- push to multiple remotes
- fetch from multiple remotes. 'git pull' is not possible from multiple repos. 

### Dependencies

### Installing

### Executing 

## Help

## Author

Bernd Furnier
[@Bernd Furnier](https://www.fu-net.de/)

## Version history

- 0.1
    - Initial Release

## License

This project is licensed under the [MIT License](LICENSE.md)
 - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
- [awesome-readme](https://github.com/matiassingers/awesome-readme)

- [ How To Write a USEFUL README On Github](https://www.youtube.com/watch?v=E6NO0rgFub4)

- [Licensing a repository](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/licensing-a-repository
)
    - [Choosing the right license](https://choosealicense.com/)

