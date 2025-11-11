# C++ DevContainer Template

A ready-to-use **C++ development environment** with *VS Code Devcontainers*, pre-configured for rapid development using the *cmake* build system. This is the structure of the template:

```
.
├── .github/workflows/compile-ci.yml # GitHub Actions workflow
├── .devcontainer/devcontainer.json  # Devcontainer JSON definition
├── cmake/                           # (Optional) Custom CMake modules
├── src/
│   ├── main.cpp                     # Application entry point
│   ├── app.cpp                      # Core application source
│   └── app.hpp                      # Header file
├── CMakeLists.txt                   # Main CMake configuration
├── Dockerfile                       # Docker image creation file
└── README.md
```

The main `CMakeLists` file is already included with automatic testing capabilities using `ctest`. Just create the `test` folder.

## ⚡ Getting Started

### 1. Clone the repo

```bash
git clone https://github.com/lmriccardo/cpp-template.git
mv cpp-template <ur-app-name>
```

### 2. Open in VS Code

- Install the [*Remote - Containers*](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension
- Open the folder in VS Code
- *Reopen in the container* when prompted

### 3. Build and Run

From the command line inside the container:

```bash
mkdir build
cmake -S . -B build [-DCMAKE_BUILD_TYPE=Release]
cmake --build build [--target TARGET-NAME] # to build target(s)
./bin/APP-NAME # replace APP-NAME with the name of the executable
```

Otherwise, since the devcontainer has already installed all C++ development VS Code extensions you can just use the buttons of the status bar, selecting which targets to build, run or debug.

### 4. Outputs

- Libraries (created with `add_library`) are placed in the `lib` folder
- Executables (created with `add_executable`) are placed in the `bin` folder

Both folders are in the `.gitignore` file along with `log` and other folders.

## 🌟 Using This Template

You can create a new repository from this template directly on GitHub:

1. Go to the template repository page.
2. Click the green “Use this template” button.
3. Fill in your repository name, description, and choose Public or Private.
4. Click “Create repository from template”.
5. Clone your new repo locally
6. Open in VS Code and optionally reopen in the DevContainer