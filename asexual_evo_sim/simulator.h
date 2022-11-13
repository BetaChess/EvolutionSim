#ifndef SIMULATOR_H
#define SIMULATOR_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace evo
{

class Simulator
{
public:
	struct Creature
	{
		glm::vec2 position{0, 0.0};

		inline glm::mat4 mat4() const
		{
			glm::mat4 translateMat
			{
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{position.x, position.y, 1.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f}
			};

			return translateMat * rotationMatrix_ * scaleMat_;
		};
		
	private:
		float rotation_ = 0.6;
		float scale_ = 0.5f;

		glm::mat4 rotationMatrix_ =
		{
			{glm::cos(rotation_), glm::sin(rotation_), 0.0f, 0.0f},		// First column (left to right is top to bottom).
			{-glm::sin(rotation_), glm::cos(rotation_), 0.0f, 0.0f},	// Second column
			{0.0f, 0.0f, 1.0f, 0.0f},									// Third column
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
		glm::mat4 scaleMat_ =
		{
			{scale_, 0.0f, 0.0f, 0.0f},
			{0.0f, scale_, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
	};

public:

	Simulator();


private:
	std::vector<Creature> creatures_;

	// Friends
	friend class Application;
};

}


#endif // SIMULATOR_H