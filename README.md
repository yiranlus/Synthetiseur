# Synthetiseur

Visualization of sound signal in time and frequency domain.

## Code Structure

The repository only contains source code files under `/src` directory.

## Build and Run

In order to make the compilable project, you need to use the Project Generate that comes with OpenFramework. To do this, follow the step below:

1. use the Project Generator to create a project structure. Depends on which platform you are on, you can add one or more platforms. `VS Code` is the most versatile option.
2. if you have added `VS Code`, open the `.code-workspace` file using VS Code. You will be able to see the source code directory is loaded with OpenFramework.
3. run tasks like `Run RELEASE`. This will automatically build the program.

Using `VS Code`, on Windows, `msbuild` will be used by default. So you may need to modify or remove some lines in the `tasks.json` if you want to use MinGW.

## Program

A screenshot of the program is shown below:

<img width="1028" height="815" alt="image" src="https://github.com/user-attachments/assets/796b1333-dcb5-414f-8be5-a32c06759e20" />

The usage is written on the screen, so there is no need for other documentation.
