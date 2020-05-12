#pragma once

#ifdef GG_PLATFORM_WINDOWS

extern GitGud::Application* GitGud::CreateApplication();

int main(int argc, char** argv)
{
	GitGud::Log::Init();

	auto app = GitGud::CreateApplication();
	app->Run();
	delete app;
}

#endif // GG_PLATFORM_WINDOWS
