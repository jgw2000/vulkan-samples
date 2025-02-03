#include "application.h"

namespace vkb
{
	Application::Application() : name{ "Sample Name" }
	{
	}

	void Application::run(const Window::Properties& properties)
	{
		prepare(properties);

		while (!window->should_close()) {
			update(0.1f);	// TODO
			window->process_events();
		}

		finish();
	}

	bool Application::prepare(const Window::Properties& properties)
	{
		window = std::make_unique<Window>(properties);
		return true;
	}

	void Application::update(float delta_time)
	{
		fps = 1.0f / delta_time;
		frame_time = delta_time * 1000.0f;
	}

	void Application::finish()
	{
	}

	const std::string& Application::get_name() const
	{
		return name;
	}

	void Application::set_name(const std::string& name)
	{
		this->name = name;
	}
}