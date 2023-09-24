void MessageHandler(SFSE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case SFSE::MessagingInterface::kPostLoad:
		{
			logger::info("{:*^50}", "POST LOAD"sv);
		}
		break;
	default:
		break;
	}
}

#undef USE_ADDR_LIB

DLLEXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Plugin::VERSION);
	data.PluginName(Plugin::NAME);
	data.AuthorName(Plugin::AUTHOR);
	data.HasNoStructUse(true);

#ifdef USE_ADDR_LIB
	data.UsesAddressLibrary(true);
	data.MinimumRequiredXSEVersion({ SFSE::RUNTIME_SF_1_7_23 });
#else
	data.UsesSigScanning(true);
	data.CompatibleVersions({ SFSE::RUNTIME_SF_1_7_29 });
#endif  // USE_ADDR_LIB

	return data;
}();

DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
#ifndef NDEBUG
	while (!SFSE::WinAPI::IsDebuggerPresent()) {};
#endif
	SFSE::Init(a_sfse);

	auto version = Plugin::VERSION.string();
	logger::info("Loaded {} {}", Plugin::NAME, version);

	const auto messaging = SFSE::GetMessagingInterface();
	messaging->RegisterListener(MessageHandler);

	return true;
}
