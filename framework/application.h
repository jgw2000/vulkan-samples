#pragma once

#include "window.h"
#include <memory>

namespace vkb
{
	class Window;

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void run(const Window::Properties& properties);

		/**
		* @brief Prepares the application for execution
		*/
		virtual bool prepare(const Window::Properties& properties);

		/**
		* @brief Updates the application
		* @param delta_time The time since the last update
		*/
		virtual void update(float delta_time);

		/**
		* @brief Handles cleaning up the application
		*/
		virtual void finish();

		const std::string& get_name() const;

		void set_name(const std::string& name);

	protected:
		float fps{ 0.0f };

		float frame_time{ 0.0f };

		uint32_t frame_count{ 0 };

		uint32_t last_frame_count{ 0 };

		std::unique_ptr<Window> window;

	private:
		std::string name{};

		bool requested_close{ false };
	};
}