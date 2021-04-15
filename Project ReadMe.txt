Physics
Final Project: Something cool!
Kinematic Character Controller!

Submission by Dylan Kirkby, Brian Cowan, Ivan Parkhomenko

Created in Visual Studio 2019 Community Edition
Solution runs on Platforms and configurations.

Simply open in visual studio and build, it should all be configured with the correct dependencies.

Animations are not yet blended and dispatch in a one at a time queue system, so the controls are clunky as yet.
Relevant Controls:
Keyboard:
	W		Walk Forward
	S		Walk Backward
	Q		Strafe Left
	E		Strafe Right
	Space		Jump - just fires the animation, bullet jump not yet implemented.
			Running Strafe then space triggers a dodge animation.
	+Shift		Modifies above keys to do things faster
	A		Turn Left
	D		Turn Right
	R		Reset to regular idle (from combat idle)
	1		Disable/Enable Physics debug rendering.

Mouse:
	Left		Combat Mode / Punch
	Right		Face character camera's direction
	Move		Rotate Camera
	Wheel Up	Zoom Out (Camera starts at max distance)
	Wheel Down	Zoom In

External files can be found in:
	Entities	$(SolutionDir)AmethystEngine\data\config\complex\Entities.json

Interface/Bullet library formatted quite similarly to yours.

Shapes
Capsules - Primitive shape, always useful
Boxes - The same!
btBvhTriangleMeshShape - My forum readings said this was the optimized version of triangle
meshes and not to bother with the other. So I converted the mesh passed in to place it properly
in the world relative to 0 by applying the transform to each vertice on load. This should be
'relatively' efficient in runtime, and not too bad for loading as long as I don't use too many / a big one.


Constraints
None implemented, sorry!


Character controller
I set this up fairly similar to my other shapes, with a bit of a learning curve primarily in the constructor.
It handles deleting its own shapes - though I could have added a vector for those to the world.
Aside from initial setup, I hooked it into my animation system to dispatch turns and linear velocities
to it, or in the case of non in place animations, to directly update its position instead of allowing bullet to.


Improvements


FeedBack
This required some tweaking of the collision listener to prevent my ghost objects from reporting a collision with every
collision body every frame. This fix is done by checking the Contact Manifolds for actual contact points.

See AmethystEngine/cCollisionListener for the below.
Death Plane:	If the character controller falls too far down in dynamics world it hits a death plane.
This collision triggers a wilhelm scream, and the death animation. On completon, the death animation
respawns the player at its original point.

Health Potion:	A static box shape that when collided with, is removed from the physics world. A short sound 
is played and the players health increases 10000 up to a cap of 25000 (arbitrary numbers). It respawns after
roughly 15 seconds.

Bonfire:	While not the greatest graphically, the bonfire has two collision shapes on it as a bit
of an experiement. A box encompasses the hot rocks, and a capsule goes over the center making it uncrossable
'terrain.' Contact with either of these shapes inflicts damage. I'd play a sound too, but haven't fiddled
with it in too long, and would likely crash the program!

The death animation prevents additional collision events.