# hack_embedded
Embedded runtime for the [Hack assembly lifter](https://github.com/antoshre/hack_asm_lifter) project

Keyboard handling is in but untested due to lack of Hack assembly programs that only 
touch the keyboard and not the screen.

Screen handling is theoretically in but not yet implemented.  Still need a good way to
 convert the black/white bitmap into an RGBA image that SFML can handle.
 
 ## Requirements
 
 SFML 2.5 or another version that is source-compatible.
 
 Boost Lockfree.  Any vaguely modern version of Boost should work fine.
 
 Pthreads, for C++ `<thread>` support.
 
 ## Usage
 
 This is, at best, alpha-quality proof of concept.  Good luck!
 
 Provide an object file from hack_asm_lifter and link it in CMakeLists.txt.
 
 A screen will appear to (eventually) hold the image output, refreshes at 10FPS.  
 
 Keyboard input directed at the screen will be buffered and fed to the recompiled Hack code.
 The buffer is only 1024 characters deep and will throw if that is exceeded.  