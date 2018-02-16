# MIVCOTS_SOFTWARE
Only Software (Sean, we're looking at you...)

## Installing wxWidgets:
1. Download and run the Windows installer for 3.0.3 (latest stable release)

   https://www.wxwidgets.org/downloads/
   
2. Navigate to the install directory (hopefully C:\wxWidgets-3.0.3) then \build\msw
3. Open wx_vc10.sln in Visual Studios 2017. Let it convert everything to v141.
4. Build the solution.
5. Set the WXWIN environment variable to the install directory if the installer didn't. 
   
   See https://wiki.wxwidgets.org/Adding_an_Environment_Variable_under_Windows
   
   Make sure to include a backslash on the end: "C:\wxWidgets-3.0.3\\"
