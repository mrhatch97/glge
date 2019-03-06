#include "LTurtle.h"

namespace eng::procgen
{
	LTurtle::LTurtle() {
		position = vec3(0.0f, 0.0f, 0.0f);
		// face up
		orientation_mat = mat4(0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			- 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		mat4 I = glm::translate(mat4(1.0f), vec3(4.0f, 2.0f, 3.0f));
	}


	vec3 LTurtle::getPosition() {
		return position;
	}

	mat4 LTurtle::getOrientation() {
		return orientation_mat;
	}

	void LTurtle::setPosition(vec3 newPos) {
		position = newPos;
	}

	void LTurtle::setOrientation(mat4 newOrientation) {
		orientation_mat = newOrientation;
	}

	void LTurtle::turn(int degrees) {
		//auto up_vec = vec3(orientation_mat[2][0], orientation_mat[2][1], orientation_mat[2][2]);
		auto rot_mat = glm::rotate(mat4(1.0f), glm::radians(float(degrees)), vec3(0.0f, 0.0f, 1.0f));
		orientation_mat = rot_mat * orientation_mat;
	}

	void LTurtle::pitch(int degrees) {
		//auto left_vec = vec3(orientation_mat[1][0], orientation_mat[1][1], orientation_mat[1][2]);
		auto rot_mat = glm::rotate(mat4(1.0f), glm::radians(float(degrees)), vec3(0.0f, 1.0f, 0.0f));
		orientation_mat = rot_mat * orientation_mat;
	}

	void LTurtle::roll(int degrees) {
		//auto head_vec = vec3(orientation_mat[0][0], orientation_mat[0][1], orientation_mat[0][2]);
		//auto rot_mat = glm::rotate(mat4(1.0f), glm::radians(float(degrees)), head_vec);
		//orientation_mat = rot_mat * orientation_mat;
	}

	void LTurtle::moveForward(float units) {
		auto heading_vec = vec3(orientation_mat[0][0], orientation_mat[0][1], orientation_mat[0][2]);
		position += units * heading_vec;
	}
}
