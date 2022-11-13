#ifndef SIM_APP_H
#define SIM_APP_H

#include <memory>
#include <vector>

#include "phm_window.h"
#include "phm_renderer.h"
#include "phm_object.h"


namespace evo
{
	class Application
	{

	public:
		static constexpr size_t WIDTH = 800;
		static constexpr size_t HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void run();

	private:
		phm::Window window_{ WIDTH, HEIGHT, "Triangle" };
		phm::Device device_{ window_ };
		phm::Renderer renderer_{ window_, device_ };

		std::vector<phm::Object> objects_; // TEMP

		void loadObjects(); // TEMP
	};
}



#endif /* SIM_APP_H */