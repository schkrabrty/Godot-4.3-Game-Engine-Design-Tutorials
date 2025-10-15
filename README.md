# Godot-4.3-Game-Engine-Design-Tutorials

## Steps to setup the project on your computer
First, clone this project in your computer. Before opening up this project using Visual Studio Code, make sure to install Python, Scons, Cmake, LLVM in your computer. For Windows operating system, install Clang along with the previous packages. The installation procedure for Windows and Mac/Linux will be slightly different. I will go through them below.

## Setup for Windows
At first download and install the latest version of "Python" on your computer. **_When the setup file will load, do not hit install immediately! Rather, check the box called "Add Python.exe to PATH" (shown in the picture below)._**

![Python installation setup](/Images%20for%20Readme/Python%20Installation.PNG)

Once Python is installed, open Command Prompt, and install the packages one by one, as shown below.

`pip install cmake`

`pip install scons`

`pip install clang`

`pip install make`

`pip install ninja`

Once these packages are installed, go to [this link](https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8) and find the "LLVM-18.1.8-win64.exe". LLVM stands for Low Level Virtual Machines which is a collection of tools and libraries for building compilers, debuggers, and other software development tools. The 18.1.8 version of LLVM is the latest version now. In future, this version can change. So, feel free to tweak the hyperlink to get the latest version if needed. Once you find the exe file, click on it, and it will be downloaded to your computer. Feel free to install it. **_Make sure that you choose the PATH before you install the package (shown in the picture below)._**

![LLVM Installation](/Images%20for%20Readme/LLVM.PNG)

Now, most of the required packages are installed. Now, go to Visual Studio Code and follow the steps in the **Common** section below.

***NOTE:*** If you still find some error while executing the "scons" command (mentioned below), that may say that the compiler cannot find some ".o" files, then it is most likely that Visual Studio Code requires a g++ compiler. So, in that case, you need to download and install [the MSYS2 package](https://www.msys2.org/) on your computer. Please check the "Run MSYS2" checkbox when the installation is finished, and then finish the installation procedure. Once you do that, then run the command below in the newly opened command prompt - 

`pacman -S mingw-w64-x86_64-gcc`

Finally, you need to add the MinGW bin dir to your system PATH, and then you will have a working g++ compiler. So, go to the "Settings" app of your computer. Search for "Path" in the search bar, and choose the option that says "Environment Variables" (shown in the picture below). 

![Environment variable selection setting from the Settings App](/Images%20for%20Readme/Environment%20Variables%20Finder.png)

This will open a new window (shown in the picture below). From that window, choose the "Environment Variables" button. 

![Environment variable button selection from the window](/Images%20for%20Readme/Environment%20Variables.PNG)

It will open another window (shown below). From there, select the "Path" variable, and then click on the "Edit" button. 

![Path variable selection from the window](/Images%20for%20Readme/Path%20setup%201.PNG)

This will open another window (shown below). In this window, click on "New" which will create an empty entry at the bottom of the list. Now click on "Browse" and go to the "mingw64/bin/" folder in your computer. Normally it should be located at "C:/msys64/mingw64/bin/". Once you select that, press "OK" and then the path will be shown up at the end of the list. Then click on "OK" to close the window. Then also, click on "OK" in the other opened windows to register the path properly in your system. 

![Path variable final setup from the window](/Images%20for%20Readme/Path%20Setup%202.PNG)

## Setup for MacOS or Linux
I will suggest you to open this project using Visual Studio Code because it comes with a terminal window internally regardless of the OS that you are using. So, first you need to click on "Terminal" in the Menu Bar, and click on "New Terminal". Then in the terminal window, you need to install "Homebrew" if you have not installed already. The command is -

`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)”`

Next, you need to install Python -

`brew install python`

Then you need to install Scons -

`brew install scons`

After that you need to install Cmake -

`brew install cmake`

Please also check if the command line tools are installed or not if you are using macOS. Simply type -

`xcode-select --install`

Based on the output print statement, you can understand if command line tools is installed in your computer or not.

Theoretically, you should be done at this point. However, there can still be problems with the C++ compilers. So, to remain on the safe side, please install LLVM and GCC on your computer as well. The commands are -

`brew install llvm`

and

`brew install gcc`

Once you install all these packages through the Visual Studio Code Terminal, then you need to follow the steps mentioned in the **Common** section below.

## Common for all Operating Systems

Once you install all the packages, now you need to install some Extensions in Visual Studio Code. Click on the Extensions icon on the left side bar of your Visual Studio Code, and download these extensions one by one -

`C#`,

`C# Dev Kit`,

`.NET Install Tool`,

`.NET Extension Pack`,

`C/C++`,

`C/C++ Extension Pack`,

`CMake Tools`,

`Intellicode`,

`Intellicode API Usage Examples`, and

`Intellicode for C# Dev Kit`.

Feel free to sign in with your Microsoft or GitHub Id in your Visual Studio Code and "Turn on Syncing", so that you do not have to do the Extension setups in your other computers. They will automatically be synchronized on your Visual Studio Code Editor once you sign in.

Once all of your Extensions are downloaded, you will see that there will be multiple tabs opened up in the editor. Find the tab that shows the C# Dev Kit installation procedure. If you cannot find it, you can also click on File > New Window, and from there you can select the setup procedure for C# Dev Kit which will show up as a link on the right hand side of the window (It will be shown something like - "Get Started with C# Dev Kit"), shown in the picture below.

![C# Dev Kit Set Up Link in Visual Studio Code](/Images%20for%20Readme/NET%20First%20Page.PNG)

If you cannot see it, please click on the "More" link below (shown in the picture above), and you will be able to see all the links in the newly opened dropdown menu. From there, you can select the "Get Started with C# Dev Kit" link. Once, you select that link, click on "Set up your environment" option (or the option where it will ask you to install .NET SDK), shown in the picture below.

![Link to Install .NET SDK from the C# Dev Kit in Visual Studio Code](/Images%20for%20Readme/Install%20NET.PNG)

Then click on the button to install .NET SDK. It will open another new tab on the side where you will see another button saying install .NET SDK (shown below).

![Installing .NET SDK Finally](/Images%20for%20Readme/Install%20NET%202nd.PNG)

Click on it, to install the .NET SDK in your computer. It will install the sdk partially through terminal and partially through setup executable. Once, it is installed, then close the editor. Theoretically, it should setup everything by now. However, I found in one of my computer, turning off my editor was not enough. I had to restart my computer. So, you can restart your computer to install everything properly.

***NOTE:***
If you see any pop up shows up in Visual Studio Code at the bottom right corner that asks you to setup C++ library, or setup Git, or setup Cmake, etc., then click on the right buttons to setup Visual Studio Code with the packages and PATH variables properly. For example, if it asks you to setup C/C++ with your Visual Studio Code, and give you two choices, i.e., Yes and No, then click on Yes to set it up properly with the IDE. 

## Procedure for Setting Up GDExtension with any Godot Project
Once your computer is restarted, at first, create a blank project on GitHub and clone it on your computer. Let's say this cloned folder's name is "Soumya-Class-Demo". Once you do that, go to that cloned blank folder and either create a blank Godot project or you can simply copy and paste your already working Godot project folder (which contains all the files in it). Let's say that the Godot project's name is "classDemo". So, inside the "Soumya-Class-Demo" folder, you will see the "classDemo" Godot folder now. Next, you need to create another blank folder under the "Soumya-Class-Demo" folder called "src". In this "src" folder, all of your .cpp files and .h files will be stored. Finally, you can go to [the official tutorial website](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html) and download the hard coded SConstruct file on your computer. You can also use the SConstruct file that comes with the current GitHub repo. You then need to place the downloaded SConstruct file in the "Soumya-Class-Demo" folder. This Sconstruct file contains hard coded information to generate the dynamic linking between your C++ scripts and Godot with the GDExtension. So, the folder structure of the "Soumya-Class-Demo" should look like this - 


```
Soumya-Class-Demo          # The parent folder cloned from GitHub
    |
    |__ classDemo           # The Godot Project Folder
    |
    |__ src                 # A Blank folder where all of your custom .cpp and .h files will go
    |
    |__ SConstruct          # This file can be downloaded from the official tutorial or from the current GitHub tutorial repository
```


Now, open Visual Studio Code and click on File -> Open Folder and Open the "Soumya-Class-Demo" folder where you can see all these new file and folder structures. 

***NOTE:***
As soon as you open this project, it may or may not ask you to select a Cmake file. If you do get any pop up like that, just do not do anything with it now, and let it remain as it is. We need to do some more setups first, and then we will return back to it in a few moments later. 

The first thing that you need to do is to change the content of the SConstruct file. Remember, it is a hard coded file, which means it can only understand a specific project. On line numbers 21, 29, 34, and 39, it shows where will the dynamic link libraries will be generated, i.e., under the Godot project's folder, this file will create a separate folder called "bin" and inside that folder, it will store the dynamic link libraries. However, we need to make sure that our Godot project folder's name is correctly mentioned in those 4 lines. In the current example, the name of our Godot project folder is "classDemo". So, we need to copy that name -> go to the SConstruct file-> go to each of the 4 lines mentioned before -> and replace the folder name before the "bin" folder if it is something else other than "classDemo" for the current project. Here is an image below regarding how should the SConstruct file look like after the edit - 

![An image of the SConstruct file after the editing the Godot Project's name](/Images%20for%20Readme/Sconstruct%20file.png)

After this step, open the terminal once again in Visual Studio Code by going to the "Terminal" option in the Menu Bar and then click on "New Terminal". Then execute this command -

`git clone -b 4.3 https://github.com/godotengine/godot-cpp`

I am assuming that the terminal is considering the current folder where the project is located at (i.e., the current work directory should be the main parent folder, which is "Soumya-Class-Demo" in our case). If not, please make sure that the path should points to the current folder. This command will clone a new folder called "godot-cpp" under the "Soumya-Class-Demo" folder. This is one of the most vital folder because it consists of all the required header and C++ files to write Godot functions in your .cpp files. 

***NOTE:*** 
While writing the above command in the terminal, make sure that you used "4.3" in the command. This depicts the version of the Godot game engine that we are using in our current project. If there is a version mismatch, it can give you trouble later on.
 
Once, the repo is cloned, then execute this command -

`git submodule update --init`

This will create a .gitmodules file in the project directory, which let GitHub know that you are rightfully using the godot-cpp repository from its "4.3" branch (because we are using Godot 4.3). Now, execute this command -

`scons platform=<platform>`

where `<platform>` is either **windows** or **macos** or **linux**. Please replace `<platform>` with your os. For example, if you are using macos, then the command will be -

`scons platform=macos`

This command will generate all the C++ bindings for the project in the "classDemo/bin/" folder as mentioned in the SConstruct file. This will take a long time depending on your computer's hardware. Once this step is done, then close the editor once again, and reopen it. This time, it should ask you to select a path for the cmake file through a dialog box on the right corner or directly through a drop down menu on the top search box. It should look like this -

![A picture showing the Selection of CMakeLists.txt](/Images%20for%20Readme/CmakeLists.png)

If you see the dialog box on the right corner, click on the "Configure Cmake Option Visibility" button (or similar to it). It will automatically suggest you some paths in the drop down menu. Please choose the one that says -

`${workspaceFolder}/godot-cpp/CMakeLists.txt`

It can then ask you to reselct the path once again (It may or may not happen). Please select the same path once again. Next it will ask you to choose a C++ compiler. Choose the option that starts with "Clang". It should look like this -

![A picture showing the Selection of Clang Compiler for compiling Cmake](/Images%20for%20Readme/Clang.png)

***NOTE:***
On Windows, there can be two Clang compilers. One says Clang x86... and another may say Clang cl... Please choose the Clang x86 one. The x86 can be x64, which is fine also.

Then the editor will compile the project and will create a "build" folder in the project directory. If it fails to create the folder, then again close the editor and reopen it, and it should create the "build" folder automatically this time. That's how, the project is almost setup properly with the GDExtension.

Now, you need to create a blank file under the "classDemo/bin/" folder and give it a name something like "gdexample.gdextension". Inside this file, you need to mention the path where the generated dynamic link libraries are located so that Godot can look into it. You can definitely copy paste the code provided in the gdextension file in [the official tutorial website](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html), or you can simply use the gdextension that you can find in the current repository under the current project directory. Just simply copy paste the entire content of the gdextension file in your newly created gdextension file. Again make sure that the "compatibility_minimum" variable in this file should say "4.3" to indicate that we are using Godot 4.3, and the "reloadable" variable should be "true" so that when you compile your cpp codes later, Godot should get the updates immediately. 

Because the "bin/" folder is locally generated based on your OS, so naturally gitignore ignores that folder. However, the gdextension file is very essential so that Godot can talk to the C++ files. So, make sure that the gdextension stays in the "classDemo/bin/" folder. 

We have done setting up our GDextension. Now, we need to do some coding to talk to Godot from the cpp files. So, go to the "src" folder and create two files called "register_types.cpp" and "register_types.h". As you can understand, the .h file is the header file, and the .cpp file is the actual C++ script. This files will help you to register your custom .cpp and .h files so that they can be dynamically linked to Godot. I have basically copy pasted the contents of these two files from [the official tutorial website](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html). So, feel free to do that or feel free to use the ones in the current repository. 

***NOTE:***
A very important to note here is that, whatever custome C++ script and header files you will write for your project, you need to ***(i) import those header files, and (ii) register the classes in the register_types.cpp file.*** In the current GitHub project, I have two custom .cpp and .h files called "keyinput" and "modifyspeed". So, I need to register them in the register_types.cpp file. Here is a picture below showing how can you do that - 

![Registering custom .cpp and .h files in register_types.cpp](/Images%20for%20Readme/Register_Types.png)

Now, once all these setup is done, you are completely free to write your custom .cpp file and .h files. So, if you want to write any script in C++, you need to create a header file, and then you need to include that header file in your cpp file. Then you can write your own C++ code in the .cpp file. Make sure to register these two files with the register_types.cpp file. 

***NOTE:***
Make sure that the names of both the .h file and the .cpp file are exactly same. Also, make sure that the class names inside these two files follows the exact same naming conventions. Otherwise, you will get errors during compilation. 

Once you do that, then you need to compile the code so that the dynamic linking will be made for you. To do that, go to the Terminal window and execute the command - 

`scons platform=<platform>`

where again `<platform>` means you OS. If there is no error in your code, you will be able to see a new file generated in the "src" folder that has the same name of your custom .cpp and .h file name. However, its extension will be ".os". If you see this file, then you can assure yourself that the dynamic linking is successful now. 

Now, open Godot. Click on "Import", and then find your main parent folder (which is "Soumya-Class-Demo" in this case). Then from there go to your Godot project (which is "classDemo" in our case). Finally open the "project.godot" file. Now, you can click on "Other Node" to import your custom .cpp script as a node in your Godot project. In the dialog box, search for the name of your custom .cpp file (in our case, it is "KeyInput"). Once you find that, select it, and click on "Create". And there you go! You have got your custom written code as a node in Godot. 

You can also, call any public functions that you wrote in your .cpp file from a GDScript. So, in this GitHub tutorial, you can see that the KeyInput class is attached to the "Main" node. Now, let's say that I want to access the "move" function and the speed variable from the KeyInput class from another GDScript to do some other things. How can we do that? Well, we can create a child node under the Main" node, and then we can add a GDScript to it. The name of the GDScript is "node.gd" which you can find in the "classDemo/Scripts/" folder. In that script, I simply accessed the parent node by saying "$..". Because the move function requires a Vector2 value. So, I randomly created a Vector2 value, and I simply called the move function by passing this Vector2 value as a parameter of the function. I also changed the speed variable's value. Note that, I was able to access the function and the variable of the .cpp file because they were public variables. In Godot, if you write any functions without putting an "_" in front of the function, it will be treated as a public function (which follows in your custom .cpp file also). However, you might think that the speed variable is "private" in the header file of the KeyInput class. So, why could we access it? It is because, we are using the Getter and setter functions of C++ which will allow you to access and update a private variable publicly. 

Note that I have written this GDScript in the _ready function because, I just wanted to execute this code once. If you want, you can write anything in the _process function also, based on your own need. Once you complete writing the script, you can play the game. Now, you can see that the "Main" node will show up at (100, 100) position because that's what we said in the GDScript and as it talked to the .cpp file, it translated the node to the (100, 100) position. Also, now if you press the up, down, right, left arrows, you can still be able to move the "Main" node in the screen because you are utilizing your .cpp script now. 

You can also call your custom method from your GDScript to one of your C++ script. How can you do that? Well, for that, you can consider the "Secondary.tscn" scene in the Godot project. If you open that scene, you will see that a GDscript called "secondary.gd" is attached with the parent node. This script looks very similar to the "keyinput.cpp" class that we wrote earlier, and in fact it does exactly the same thing. This GDscript will check if the WASD keys or the arrow keys are pressed or not and based on the input, it will move the node in the scene. Now, what we want is to access the "move" function and the "speed" variable from the GDscript from another C++ script. So, you can see another C++ script in the "src" folder called "modifyspeed.cpp" where we wrote the logic for this task. The main method in that script is the "process_secondary_script()" method where all the magic happens. You can take a look at the script to understand how can you talk to the GDscript from the C++ script. Also, I called this method from the _ready() method in the C++ script. However, I used a "call_deferred()" method. This method will ensure that all the children are initialized properly, and then the "process_secondary_script()" method will be executed. If you do not want this feature, feel free to call the "process_secondary_script()" method alone instead. 

## The main workflow of GDExtension in short

Okay, in short, whenever you want to write C++ scripts for Godot, I would suggest you to check - 

1. Whether you followed the folder structure in your project (i.e., inside the parent folder, there will be the Godot project, the Godot-cpp folder downloaded from GitHub repo, src folder, and the SConstruct file).
2. Run the command scons platform=`<platform>` command.
3. After the binding compilation, put the “.gdextension” file in the Godot project/bin folder. 
4. Then write all of your cpp and header files in the “src” folder.
5. Once you do that, then run the command again - scons platform=`<platform>`.
6. If everything works properly, then you can see the “.os” file generated in the “src” folder.
7. Once you do that, then you can come to Godot, and then you should be able to find the Cpp node in Godot.
8. If you cannot find it, then close Godot, and restart it. Then you should be able to find it.
9. Once you can see it, you can drag it to the Node window and you can access the node in Godot. 
10. For Gdscript, you can create another child node, and add a gdscript with it. Then you can write your code over there to access the cpp file from there if you want. 

This is the workflow sequence that you need to follow to write C++ scripts for Godot Game Engine. 

## Conclusion

I hope, this tutorial makes sense to you. For further details, please consider checking [the official tutorial website](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html) about how to setup the gdextension with Godot. Also, feel free to contact me if you face any problem. Enjoy playing with C++ and Godot!
