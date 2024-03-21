This is a project to show a bug that specifically happens in Ableton Live (11 and 12) on VST3 - Windows AND Mac. On the latest develop tip of JUCE.

The bug is as follows:
1) A meta parameter (we will call it MasterParam) moves a slave paremeter (or several) we will call it "SlaveParam" - but does not report the change to the host.
2) The slave param sends a beging gesture change to the host
3) the host then sets the SlaveParam to the previously reported value - overriding its un-reported parameter

This only happens when there are 65 parameters or more in the plug-in. not on vst2 or au.
