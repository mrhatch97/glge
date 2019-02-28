#include "glge/Support/ProcGen/Tree.h"

namespace eng
{
	namespace lsystem
	{
		Tree::Tree() {
			turtle = LTurtle();
		}

		void Tree::generate(float length, char rule, int curIter, int numIter, bool colorLeaf) {
			mat4 init_mat = glm::translate(vec3(1.0f, 0.0f, 0.0f));
			mat4 rot_mat;
			mat4 trans_mat;
			// Base
			if (curIter >= numIter) {
				if (rule == 'X' || rule == 'L') { return; }

				// turn model 
				rot_mat = turtle.getOrientation();
				rot_mat = rot_mat * init_mat;
				// translate to turtle position
				trans_mat = glm::translate(turtle.getPosition());
				rot_mat = trans_mat * rot_mat;
				transforms.emplace_front(std::make_tuple(rot_mat, colorLeaf));

				// move turtle forward
				turtle.moveForward(length);
				return;
			}
			switch (rule) {
				// Sierpinski Triangle
				
			case 'A':
				generate(length, 'B', curIter + 1, numIter, true);
				turtle.turn(-60);
				generate(length, 'A', curIter + 1, numIter, true);
				turtle.turn(-60);
				generate(length, 'B', curIter + 1, numIter, true);
				break;

			case 'B':
				generate(length, 'A', curIter + 1, numIter, true);
				turtle.turn(60);
				generate(length, 'B', curIter + 1, numIter, true);
				turtle.turn(60);
				generate(length, 'A', curIter + 1, numIter, true);
				break;


				
				// Fern
			case 'F':
				generate(length, 'F', curIter + 1, numIter, false);
				generate(length, 'F', curIter + 1, numIter, colorLeaf);
				break;

			case 'X':
				generate(length, 'F', curIter + 1, numIter, colorLeaf);
				turtle.turn(-22);
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.pitch(75);
				generate(length, 'X', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();
				turtle.turn(22);
				generate(length, 'X', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();

				turtle.turn(22);

				generate(length, 'F', curIter + 1, numIter, true);
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(22);
				turtle.pitch(5);
				generate(length, 'F', curIter + 1, numIter, true);
				generate(length, 'X', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();
				turtle.turn(-22);
				generate(length, 'X', curIter + 1, numIter, true);
				break;

				// Bush
			case 'G':
				generate(length, 'G', curIter + 1, numIter, false);
				generate(length, 'G', curIter + 1, numIter, false);
				turtle.turn(25);
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(25);
				turtle.pitch(15);
				turtle.roll(12);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.turn(-25);
				turtle.roll(-21);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.turn(-25);
				turtle.pitch(-20);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();

				turtle.turn(-25);

				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(-25);
				turtle.pitch(-30);
				turtle.roll(-12);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.turn(25);
				turtle.pitch(4);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.turn(25);
				generate(length, 'G', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();
				break;

				// Tree1
			case 'J':
				generate(length, 'J', curIter + 1, numIter, false);
				generate(length, 'J', curIter + 1, numIter, false);
				break;

			case 'K':
				generate(length, 'J', curIter + 1, numIter, false);
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(20);
				turtle.pitch(20);
				generate(length, 'K', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();

				generate(length, 'J', curIter + 1, numIter, colorLeaf);

				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(-20);
				turtle.pitch(-30);
				generate(length, 'K', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();
				turtle.turn(20);
				turtle.pitch(-20);
				generate(length, 'K', curIter + 1, numIter, true);
				break;

				// Tree2 - can turn into bush?
			case 'L':
				loc_stack.push(turtle.getPosition());
				orientation_stack.push(turtle.getOrientation());
				turtle.turn(-40);
				turtle.pitch(-40);
				generate(length, 'M', curIter + 1, numIter, true);
				generate(length, 'L', curIter + 1, numIter, true);
				turtle.setPosition(loc_stack.top());
				loc_stack.pop();
				turtle.setOrientation(orientation_stack.top());
				orientation_stack.pop();

				turtle.turn(40);
				turtle.pitch(-40);

				generate(length, 'M', curIter + 1, numIter, true);
				generate(length, 'L', curIter + 1, numIter, true);
				break;

			case 'M':
				generate(length, 'M', curIter + 1, numIter, true);
				break;

			}
		}
	}
}