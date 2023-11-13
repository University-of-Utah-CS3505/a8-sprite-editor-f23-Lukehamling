```
Team Name   : Coders For Christ
Team Members: August O'Rourke
              Luke Hamling
              Ryan Dalrymple
              Vincentio Dane
Project Name: A8 - Sprite Editor Implementation
Course      : CS 3505, University of Utah, School of Computing
Repo        : https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f23-Lukehamling
Copyright   : CS 3505, Coders For Christ - This work may not be copied for use in Academic Coursework.
```

# Sprite Editor Overview

Our team developed a Qt-based SpriteEditor. A sprite is a small image used in gaming that is rendered at different locations on the screen.
Most sprites have animation cycles associated with them, so that a sprite is really a vector of small images. The sequence of images 
might show the sprite walking, or exploding, or powering-up, and so on. The sprites data can be saved into a file with a .ssp extenstion

# How-to

A user can easily create new/load an .ssp file to begin, when creating a new file, a user will need to select the sprite size they want from
an array of commonly used sizes (i.e 32x32 and 64x64). Once chosen, a user can now proceed to the editor screen, here there are various common
tools to choose from (i.e. fill, pen, erase, undo, add circle, add frame, pan, etc.) Further, a if a user wish to do add animation to their sprite,
they can click the add a frame button and utilize the preview window in the top right to see how their sprite would look when the animation is played.
Last, once content with the result, the user should save the file by clicking the save icon and choosing the directory they wish the file to be saved on.

## Comments to Evaluators

The extra features we decided to implement are:
    1. Undo-Redo
    2. Fill
    3. Adding a circle and rectangle
    4. hotkeys (Ctrl-S to save, Ctrl-Z to undo, Ctrl-Y to redo)

### Branches

We utilized branches whenever we are writing code, each team member made a branch for each stuff they do. Most of the work 
were done in branches so that conflicts could be avoided and each member can modify and test the functionality they worked
on without having to worry about breaking the other person's code.
