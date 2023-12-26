User's can add their own huds to install.
 - How to do this?
 - A button which will create another button - Dymanic buttons.
 - User clicks on button which opens a dialog to add a hud
 - User clicks on HUDs location in storage
 - Save this path to a file
 - Get user to name the button/HUD.
 - Add button with name, plus a variable storing where the path is held somehow.
 - When user clicks on install. It will copy the file from location in storage to custom folder.
 - and then do the writeHudTxt function(also using readHuxTxt and deleting). Making sure to copy and not to move the HUD folder's so they can be reused.
 - Where to add? Perhaps at the bottom add like 6 buttons which can be modified to save huds, and  have the normal design to show case new huds.





In conclusion:
	New class - customhud:
	- name QString, path QString.
	- constructor
	- destructor
	- 
- Function to add:

- plus(layout/anything else needed to create button).
- addbutton() function to dynamically add a button or smth.
- removebutton() // will call destructor on the hud/button to delete or smth like this.
- getname() // box to enter hud button name
- setname() // will set button name to this^.
- getpath() //dialog box, to get path of where stored.
- setpath() //saves path to txt file for use
- copyhud() //method to copy hud from one place to another. (downloads->custom).
- tofile() some function to save hud name and path to a file. 
- readhudfile() // some function to read the txt file.