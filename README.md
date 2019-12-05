# CISC440 Fall 2019 Hw5

AdvancedBNB

Letian Zhang, Yixiong Wu
 
In this final project, we need to write a 3-D game using OpenGl and GLSL. The requirments that our game must had are following.

// requirments

An environment which consists primarily of 3-D objects, rather than 2-D sprites (although these are allowed). The view of the environment should not be purely plan or elevation, but rather something closer to isometric. Your camera may be perspective or orthographic.
User control through keyboard or mouse input
Objects must move smoothly, camera views (if they change) must change smoothly. Your game can still be turn-based: a recent student had a very nice chess implementation in which the pieces "flew" to their next square
At least one texture-mapped element
At least one 3-D element which is "lit" with diffuse and/or specular lighting.

Also, since we are doing group work, we also need to include two following additional elements in the game. 

Incorporate more features of Bullet physics (i.e., something beyond gravity and/or collision with a plane)
3-D or 4-D value/Perlin noise (this does not count as the texture-mapped element above)
Environment/reflection mapping
Shadow mapping
Billboards/impostors
A scoreboard or other text-based overlay about what is going on (this also does not count as your texture-mapped element)
"Complicated" 3-D objects (i.e., NOT platonic solids)

For addtional elements, we choosed "Complicated" 3-D and Billboard.

//what is our game 

Our game is a maze and strategy game which inspired by a famous game called Crazy Arcade. We implement some mechanisms to fulfil the requirments of the project. There are four characters (include the player) in the game. Due to financial reason, we only have one map, but maybe in the future, there will be more DLCs come out. The player need to kill other characters (controlled by PC) using bombs (only weapon) and stay alive until he/she is the only remaining character in the game. If the player get chased up other characters, the player will lose some HP.Once the player set a bomb, after sevearl seconds, the bomb will explode and create a cruciform shaped blast. If anyone who has been impacted by that blast, they will lose some HP. If anyone loss all thier HP, they will die. The blast can also destory some obstacles on the map. In order to make our game become a ESRB E game, we use water bomb and water current to represent the bomb and the blast. 

//how to play our game

The player can move up, down, right and left, and also they can set up a bomb. 
  UP: ↑
  Down: ↓
  RIGHT: →
  LEFT: ←
  SET BOMB: SPACE
  
Moreover, the player can use WASD to change the camera view.

//how we made the game

  
