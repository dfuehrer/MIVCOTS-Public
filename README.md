# MIVCOTS_SOFTWARE
Only Software (Sean, we're looking at you...)

## Cloning
Use the command "git clone --recurse-submodules -j8 https://github.com/pdxfrog/MIVCOTS_SOFTWARE.git" to also clone the submodules.

## To pull submodules into an already cloned repository, run:
#git submodule init
#git submodule update

## Installing wxWidgets:
1. Download and run the Windows installer for 3.0.4 (latest stable release)

   https://www.wxwidgets.org/downloads/
   
2. Navigate to the install directory (hopefully C:\wxWidgets-3.0.4) then \build\msw
3. Open wx_vc10.sln in Visual Studios 2017. Let it convert everything to v141.
4. Build the solution.
5. Set the WXWIN environment variable to the install directory if the installer didn't. 
   
   See https://wiki.wxwidgets.org/Adding_an_Environment_Variable_under_Windows
   
   Make sure to include a backslash on the end: "C:\wxWidgets-3.0.3\\"

## Other Requirements
1. Add another environemnt variable "C:\Users\Sean\Documents\MIVCOTS_SOFTWARE\projects\mivcots\Resources\" with the variable name "MivcotsResources"
## Testing with com0com

Install com0com https://sourceforge.net/projects/com0com/

* After running com0com add a pair and then rename the ports to COM42 and COM43 and then hit apply

Install python 3 https://www.python.org/downloads/ (make sure to include it in the PATH)

Open a command prompt and use pip to install serial and pyserial

``` pip install serial```

```pip install pyserial```

Double click the test_data_source.py file to run the com testing
