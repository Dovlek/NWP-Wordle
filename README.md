# 🎮 NWP-Wordle

A desktop implementation of the popular **Wordle** word-guessing game built with **wxWidgets** and **C++**.

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![wxWidgets](https://img.shields.io/badge/wxWidgets-1B365D?style=for-the-badge&logo=&logoColor=white)

## 📖 About

This project was made for a college assignment in Advanced Windows Programming.

## ✨ Features

- 🎯 **Classic Wordle Gameplay** - Guess the 5-letter word in 6 attempts
- 🎨 **Visual Feedback** - Color-coded tiles show correct letters, positions, and invalid guesses
- ⌨️ **Dual Input Methods** - Use on-screen keyboard or physical keyboard
- 📊 **Game Statistics** - Track wins, losses, current streak, and best streak
- 💾 **Save/Load Progress** - Resume your game anytime
- 🔤 **Keyboard Shortcuts** - Enter to submit, Backspace to delete, Escape for menu
- 🌙 **Dark Theme** - Comfortable gaming experience

## 📸 Screenshots

<table border="0" cellpadding="10" cellspacing="0" style="border: none;">
  <tr>
    <th align="center" style="border: none;">🏠 Main Menu</th>
    <th align="center" style="border: none;">🎮 Gameplay</th>
  </tr>
  <tr>
    <td align="center" style="border: none;">
      <img src="Screenshots/main-menu.png" alt="Main Menu" width="400"/>
    </td>
    <td align="center" style="border: none;">
      <img src="Screenshots/gameplay.png" alt="Gameplay" width="400"/>
    </td>
  </tr>
</table>

## 🎮 How to Play

1. **Start the game** and you'll see a 6x5 grid
2. **Type a 5-letter word** using your keyboard or click the on-screen keys
3. **Press Enter** to submit your guess
4. **Read the feedback:**
   - 🟩 **Green** - Correct letter in the right position
   - 🟨 **Yellow** - Correct letter in the wrong position
   - ⬜ **Gray** - Letter not in the word
5. **Use the clues** to make your next guess
6. **Win** by guessing the word in 6 attempts or fewer!

## 🚀 Getting Started

### Prerequisites

- Windows operating system
- Executable must be run in the same directory as the Resources folder

### Installation

1. **Download the latest release** from the [Releases](https://github.com/Dovlek/NWP-Wordle/releases) page
2. **Extract the files** to your desired location
3. **Run `NWP-Wordle.exe`** to start playing

## 🔧 Building from Source

### Prerequisites for Building
- **Windows operating system**
- **Visual Studio 2022** (Desktop development with C++)
- **C++ standard: C++14**
- **Git**
-	**vcpkg** (for dependencies)
- **wxWidgets** library (3.1.6 or later)

### Build Instructions

1. **Clone the repository**
```
git clone https://github.com/Dovlek/NWP-Wordle.git cd NWP-Wordle
```
2. **Install wxWidgets**
#### Installing wxWidgets manually:
- [Download the wxWidgets source code ZIP or 7z file](https://wxwidgets.org/downloads/) for Windows
- Extract the folder (preferably to path without spaces. Example: "C:\Libraries\wxWidgets-x.x.x")
- Add the extracted path to your Environment Variables under the name **WXWIN**.
- Navigate to "\build\msw" inside your wxWidgets folder
- Open the latest solution for VS 2022 (wx_vc17.sln = VS 2022)
- Set Platform to x64 and Configuration to Debug or Release.
- Build via Build > Build Solution (Ctrl+Shift+B).
- Do the same for the other Configuration
- Repeat proccess for x32 Platform (optional)

#### Installing wxWidgets via vcpkg:
- If you don’t have vcpkg:
```
git clone https://github.com/microsoft/vcpkg %HOMEPATH%\vcpkg
%HOMEPATH%\vcpkg\bootstrap-vcpkg.bat
```
- Install wxWidgets (x64 recommended):
```
%HOMEPATH%\vcpkg\vcpkg.exe install wxwidgets:x64-windows
```
- Optional: integrate MSBuild user-wide (helps VS discover vcpkg libs):
```
%HOMEPATH%\vcpkg\vcpkg.exe integrate install
```

3. **Setup Project**
- If wxWidgets was installed manually, the project should already be configured for static linking
- If wxWidgets was installed through vcpkg, and MSBuild wasn't integrated user-wide:
  - Create a `WXWIN_VCPKG` Environment Variable pointing to: `%HOMEPATH%\vcpkg\installed\x64-windows`
  - In your project settings, use:
    - Include directories: `$(WXWIN_VCPKG)\include`
    - Library directories: `$(WXWIN_VCPKG)\lib` (Release) or `$(WXWIN_VCPKG)\debug\lib` (Debug)

4. **Compile the project!**

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by the original [Wordle](https://www.nytimes.com/games/wordle/index.html) by Josh Wardle
- Built with the [wxWidgets](https://www.wxwidgets.org/) cross-platform GUI library
- Word lists sourced from common English dictionaries
