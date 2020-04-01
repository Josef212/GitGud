#pragma once

#ifdef GG_PLATFORM_WINDOWS

extern GitGud::Application* GitGud::CreateApplication();

int main(int argc, char** argv)
{
	GitGud::Log::Init();
	GG_CORE_WARN("Init log!");
	GG_INFO("dafuq {0}", 1);

	auto app = GitGud::CreateApplication();
	app->Run();
	delete app;
}

#endif // GG_PLATFORM_WINDOWS
